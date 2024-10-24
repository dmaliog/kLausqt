#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qcompleter.h"
#include "qdir.h"
#include "qgraphicseffect.h"
#include "qgraphicsvideoitem.h"
#include "qmediaplayer.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "qstandarditemmodel.h"
#include "qsyntaxhighlighter.h"
#include "qtextbrowser.h"
#include "qtimer.h"
#include "qtreewidget.h"
#include <QLabel>
#include <QWebEngineView>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QProcess>
#include <QGraphicsView>
#include <QRandomGenerator>
#include <QRegularExpression>


struct Terminal {
    QString binary;
    QString args;
};
Terminal getTerminal();

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    QSystemTrayIcon trayIcon;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sendNotification(const QString& title, const QString& message)
    {
        QStringList arguments{
            title,
            message,
            "-i", QDir::homePath() + "/.config/kLaus/other/notify.png",
            "-a", "kLaus",
            "-t", "10000"
        };

        QProcess::startDetached("notify-send", arguments);
        loadSound(1);
    }

    void fadeIn(QWidget *widget);
    void fadeOut(QWidget *widget);


private:
    QColor generateRandomColor(const int &colorlist)
    {
        if (colorlist == 2)
        {
            QColor baseColor("#F3B235");
            int hue = baseColor.hue();
            int saturation = QRandomGenerator::global()->bounded(256); // случайная насыщенность от 0 до 255
            int value = QRandomGenerator::global()->bounded(150, 256); // случайная яркость от 130 до 255

            QColor color = QColor::fromHsv(hue, saturation, value);
            return color;
        }
        else
        {
            return QColor(Qt::gray);
        }
    }

    bool runPkexecCommand(const QString& command)
    {
        QProcess process;
        process.start("pkexec", {"--disable-internal-agent", "sh", "-c", command});
        process.waitForFinished();

        return process.exitCode() == 0;
    }

    QMap<QString, QString> commandDescriptions;


    QString currentPackageName;
    QString currentLoadingUrl;

    QGraphicsScene* imageScene;
    QStandardItemModel* thumbnailModel;
    int currentIndex;
    QList<QPixmap> pixmaps;

    QProcessEnvironment env;
    QProcessEnvironment enveng;

    QGraphicsVideoItem *videoItem;
    QGraphicsView *graphicsView;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QGraphicsScene *scene;

    QString detailsAURdefault;
    QString initialDetailsDowngradeText;

    QString originalLabelText;

    QString currentCategory;      // Текущая категория
    QString currentSubcategory;   // Текущая подкатегория

    // Умные указатели
    QCompleter* completer;
    QStandardItemModel* completerModel;
    QSharedPointer<QProcess> snapProcess;

    QSharedPointer<QProcess> currentTerminalProcess;
    QSharedPointer<QProcess> currentProcessDetails;
    QSharedPointer<QProcess> fileListProcess;
    QSharedPointer<QProcess> currentProcess;
    QScopedPointer<QLabel> loadingLabel;
    QSharedPointer<QLabel> loadingAnimationLabel;
    QSharedPointer<QUrlQuery> newParams;
    QSharedPointer<QString> packageURL;
    QSharedPointer<QString> teatext;
    QSharedPointer<QString> worktext;
    QSharedPointer<QString> currentDesktop;
    QSharedPointer<QString> repo;
    QSharedPointer<QString> lang;
    QSharedPointer<QString> animationname;

    QSharedPointer<QString> gameurl;
    QSharedPointer<QString> customurl;
    QSharedPointer<QString> aururl;

    QSharedPointer<QTimer> updateIconTimer;

    // Простые типы данных и переменные
    QString helper;
    QListWidgetItem* orphanButton;
    QListWidgetItem* cacheButtonHelper;
    QListWidgetItem* cacheButtonPacman;
    QStringList snapPackageNames;
    QStringList helperPackageNames;

    // Статические переменные
    QTimer *timer;

    QStringList imageUrls;
    QNetworkAccessManager *manager;
    QNetworkAccessManager *rss;
    QNetworkAccessManager networkManager;
    QSet<QString> addedLinks;
    QMap<QString, QString> appIcons;
    QTime timeupdate;
    QTime timetea;
    QTime timework;
    QTime timeout;

    QTimer *searchTimer;

    bool errorShown = false;
    bool hasUpdates = false;
    bool hasUpdatesSnap = false;
    bool loadpage = true;
    bool isFirstLoad = true;
    bool stopProcessing = false;
    bool allItemsAdded = true;

    bool actionInfopkgPressed = false;
    bool actionInfopkgPkgPressed = false;

    int lastSelectedRow = -1;
    QString lastSelectedPackage;

    // QAction
    QAction* actionLoad = nullptr;
    QAction* previousAction = nullptr;

    // QMap
    QMap<QString, QString> iconMap;

    QStringList shResourcePaths = {":/sh/1c/1c.sh",
                                   ":/sh/imgneofetch/imgneofetch.sh",
                                   ":/sh/root/root.sh",
                                   ":/sh/PKGBUILD/PKGBUILD.sh",
                                   ":/sh/save/save.sh",
                                   ":/sh/load/load.sh",
                                   ":/sh/zen/zen.sh",
                                   ":/sh/snap/snap.sh",
                                   ":/sh/amd/amd.sh",
                                   ":/sh/nvidia/nvidia.sh",
                                   ":/sh/intel/intel.sh",
                                   ":/sh/pacman-lock/pacman-lock.sh"};

    QStringList clearResourcePaths = {":/clear/clear_trash/clear_trash.sh"};

    QStringList journalsResourcePaths = {":/journals/neofetch/neofetch.sh",
                                         ":/journals/neofetch/demo.gif",
                                         ":/journals/systemd-analyze/systemd-analyze.sh",
                                         ":/journals/systemd-analyze/demo.gif",
                                         ":/journals/lspci/lspci.sh",
                                         ":/journals/lspci/demo.gif",
                                         ":/journals/lsusb/lsusb.sh",
                                         ":/journals/lsusb/demo.gif",
                                         ":/journals/inxi/inxi.sh",
                                         ":/journals/inxi/demo.gif",
                                         ":/journals/hwinfo/hwinfo.sh",
                                         ":/journals/hwinfo/demo.gif",
                                         ":/journals/lsblk/lsblk.sh",
                                         ":/journals/lsblk/demo.gif",
                                         ":/journals/fdisk/fdisk.sh",
                                         ":/journals/fdisk/demo.gif",
                                         ":/journals/xorg-log/xorg-log.sh",
                                         ":/journals/xorg-log/demo.gif",
                                         ":/journals/top-pkg/top-pkg.sh",
                                         ":/journals/top-pkg/demo.gif"};

    QStringList benchResourcePaths = {":/bench/unixbench/unixbench.sh",
                                      ":/bench/interbench/interbench.sh",
                                      ":/bench/ttcp/ttcp.sh",
                                      ":/bench/iperf/iperf.sh",
                                      ":/bench/time/time.sh",
                                      ":/bench/hdparm/hdparm.sh",
                                      ":/bench/gnome-disks/gnome-disks.sh",
                                      ":/bench/kdiskmark/kdiskmark.sh",
                                      ":/bench/systemd-analyze/systemd-analyze.sh",
                                      ":/bench/dd/dd.sh",
                                      ":/bench/7z/7z.sh",
                                      ":/bench/peakperf/peakperf.sh",
                                      ":/bench/basemark/basemark.sh",
                                      ":/bench/blender-benchmark/blender-benchmark.sh",
                                      ":/bench/GFXBench/GFXBench.sh",
                                      ":/bench/glmark2/glmark2.sh",
                                      ":/bench/glxgears/glxgears.sh",
                                      ":/bench/gputest/gputest.sh",
                                      ":/bench/intel-gpu-tools/intel-gpu-tools.sh",
                                      ":/bench/unigine-sanctuary/unigine-sanctuary.sh",
                                      ":/bench/unigine-tropics/unigine-tropics.sh",
                                      ":/bench/unigine-heaven/unigine-heaven.sh",
                                      ":/bench/unigine-valley/unigine-valley.sh",
                                      ":/bench/unigine-superposition/unigine-superposition.sh",
                                      ":/bench/vkmark/vkmark.sh",
                                      ":/bench/bonnie/bonnie.sh",
                                      ":/bench/s-tui/s-tui.sh",
                                      ":/bench/phoronix-test-suite/phoronix-test-suite.sh",
                                      ":/bench/hardinfo/hardinfo.sh",
                                      ":/bench/iozone/iozone.sh"};

    QStringList menuResourcePaths = {":/menu/icon.ini",
                                      ":/menu/ru_RU.ini",
                                      ":/menu/en_US.ini",
                                      ":/menu/Core_utilities/Core_utilities.txt",
                                      ":/menu/Documents/Documents.txt",
                                      ":/menu/Internet/Internet.txt",
                                      ":/menu/List_of_games/List_of_games.txt",
                                      ":/menu/Multimedia/Multimedia.txt",
                                      ":/menu/Other/Other.txt",
                                      ":/menu/Science/Science.txt",
                                      ":/menu/Security/Security.txt",
                                      ":/menu/Utilities/Utilities.txt",
                                      ":/menu/Favorite/Favorite.txt",
                                      ":/menu/Populaty/Populaty.txt"};

    QStringList otherResourcePaths = {":/other/notify.png",
                                     ":/other/main.sh",
                                     ":/other/grub/en_US.ini",
                                     ":/other/grub/ru_RU.ini"};


protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();
    void on_action_5_triggered();
    void on_action_6_triggered();
    void on_action_7_triggered();
    void on_action_8_triggered();
    void on_action_9_triggered();
    void on_action_11_triggered();
    void on_action_12_triggered();
    void on_action_17_triggered();
    void on_action_27_triggered();
    void on_action_30_triggered();
    void updateCommandDescription(const QString& word);

private slots:
    void loadCommandDescriptions(const QString& filePath);

    QColor getDominantColor(const QImage& image);

    void on_list_journal_itemSelectionChanged();

    QString formatWikiContent(const QString& content);
    void loadWikiContent(const QString& url);
    void loadGifToGraphicsView(const QString& gifPath);
    void updateFavoritePackages(const QStringList& favorites);

    QStringList readFavoritePackages();
    QListWidgetItem* getCurrentListItem();

    bool checkIfPackageIsInstalled(const QString& packageName);

    void prepareDetails(QListWidget* listWidget, QTextBrowser* detailsWidget, const QString& packageName);
    void prepareFiles(QTreeWidget* treeWidget, const QString& packageName);

    void openFolder(const QString& path);

    void loadPackageList(const QStringList& packages, QListWidget* listWidget);

    QString getCurrentPathFromList(const QString& itemName);
    QString getCurrentPathFromTree(QTreeWidgetItem* item);

    QString findIconPapirus(const QString& iconName);

    void loadPackages(const QString& category, const QString& subcategory);
    void loadSubcategories(const QString& category);
    void loadMainMenu();

    void loadDowngrades(const QString& packagesArchiveCatalog);

    void writeToFile(const QString& filePath, const QString& content);
    void loadConfigFile(const QString &filePath, QTextEdit *targetTextEdit, int tabIndex);

    void readProgramsFromFile(QFile& file, QVector<QString>& programs);
    void processPackageName(const QString& packageName, bool valuepage);

    void onListItemClicked(const QString &packageName, int row, QListWidgetItem *item);

    void setupConnections();
    void handleActionHovered();

    void mrpropper(int value, QAction *action);

    void onCurrentProcessReadyReadSearch();
    void onCurrentProcessReadyRead();
    void onReplyFinished(QNetworkReply *reply);

    void handleServerResponseSearch(const QString& reply);
    void onSearchTimeout();
    void createAndAddListItemSearch(const QString& packageName);
    void updateCompleterModel();
    void checkForDowngrades(const QString& packagesArchiveAUR);
    void onListDowngradeItemClicked(QListWidgetItem *item);
    void addLinkToList(const QString &link);
    void handleListItemClicked(QListWidgetItem *item, const QString& scriptDir);
    void processListItem(int row, QListWidget *listWidget, QTextBrowser *detailsWidget, const QString &package);
    void search(const QString& searchText);
    void searchAndScroll(QAbstractItemView* view, const QString& text);

    void createSearchBar();
    void setupListContextMenu();
    void showListContextMenu(const QPoint& pos);
    void miniAnimation(bool visible, QWidget *targetWidget);
    QIcon getPackageIcon(const QString& packageName);
    QString getScriptContent(const QString& filePath);
    QString processPackageInfo(const QString& packageInfo);
    QStringList executeCommand(const QStringList& command);

    void onTimeChanged(const QTime& time);
    void loadSound(int soundIndex);

    void loadSettings();
    void loadContentInstall();
    void loadingListWidget();
    void loadFolders();
    void loadScripts(const QString& basedDir, QListWidget* listWidget);
    void saveScripts(const QStringList& resourcePaths);
    void removeToolButtonTooltips(QToolBar* toolbar);
    void removeScripts(const QStringList &resourcePaths,
                       const QString &baseDir);
    void removeDirectory(const QString& dirPath);
    void restoreArchive(const QString& archivePath);
    void createArchive(const QString& folderPath, const QString& folderName);

    void UpdateIcon();
    void UpdateSnap();

    void setHasUpdates(bool updates);
    void showLoadingAnimation(bool show, QWebEngineView* webView);

    void checkVersionAndClear();
    void handleServerResponse(const QString &reply);
    void handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void searchTextChanged(const QString& searchText);
    void on_combo_mainpage_currentIndexChanged(int index);
    void on_combo_cache_currentIndexChanged(int index);
    void on_combo_lang_currentIndexChanged(int index);
    void on_combo_helper_currentIndexChanged(int index);
    void on_combo_bench_currentIndexChanged(int index);
    void on_check_repair_stateChanged(int arg1);
    void on_check_updateinstall_stateChanged(int arg1);
    void on_check_clearinstall_stateChanged(int arg1);
    void on_check_trayon_stateChanged(int arg1);
    void on_check_animload_stateChanged(int arg1);
    void on_check_autostart_stateChanged(int arg1);
    void on_check_trans_stateChanged(int arg1);
    void on_check_colorlist_stateChanged(int arg1);
    void on_dial_volnotify_valueChanged(int value);
    void on_list_repair_itemDoubleClicked(QListWidgetItem *item);
    void on_list_bench_itemDoubleClicked(QListWidgetItem *item);
    void on_list_sh_itemDoubleClicked(QListWidgetItem *item);
    void on_list_journal_itemDoubleClicked(QListWidgetItem *item);
    void on_list_clear_itemDoubleClicked(QListWidgetItem *item);
    void on_time_update_timeChanged(const QTime &time);
    void on_time_timeout_timeChanged(const QTime &time);
    void on_action_bench_triggered();
    void on_action_grub_triggered();
    void on_action_pacman_triggered();
    void on_action_repair_triggered();
    void on_action_addsh_triggered();
    void on_action_rmsh_triggered();
    void on_action_editsh_triggered();
    void on_action_31_triggered();
    void on_action_32_triggered();
    void on_action_33_triggered();
    void on_action_34_triggered();
    void on_action_35_triggered();
    void on_action_host_triggered();
    void on_action_restart_triggered();
    void on_action_stop_triggered();
    void on_action_catalog_triggered();
    void on_action_downgrade_triggered();
    void on_action_game_triggered();
    void on_list_bench_itemClicked(QListWidgetItem *item);
    void on_list_journal_itemClicked(QListWidgetItem *item);
    void on_list_sh_itemClicked(QListWidgetItem *item);
    void on_push_kde_clicked();
    void on_push_repair_clicked();
    void on_push_vk_clicked();
    void on_action_updatelist_triggered();
    void on_check_saveurl_stateChanged(int arg1);
    void on_push_pacman_clicked();
    void on_push_fstab_clicked();
    void on_action_fstab_triggered();
    void on_push_grub_clicked();
    void on_https_proxy_textChanged(const QString &arg1);
    void on_http_proxy_textChanged(const QString &arg1);
    void on_check_proxy_stateChanged(int arg1);
    void on_line_ignored_textChanged(const QString &arg1);
    void parseRSS(const QString &rssContent);
    void onNetworkReply(QNetworkReply *reply);
    void fetchData();
    void on_spin_timerupdpkg_valueChanged(int arg1);
    void on_action_infopkg_triggered(bool checked);
    void on_action_infopkg_pkg_triggered(bool checked);
    void on_list_aur_itemSelectionChanged();
    void on_list_aurpkg_itemSelectionChanged();
    void on_action_favorite_triggered();
    void on_action_favorite_del_triggered();
    void on_action_searchpkg_triggered();
    void on_action_allpkg_triggered();
};

class MySyntaxHighlighter : public QSyntaxHighlighter {
public:
    MySyntaxHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {
        HighlightingRule rule;

        // Ключевые слова
        keywordFormat.setForeground(QColor(86, 156, 214));
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns;
        keywordPatterns << "\\bif\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bwhile\\b";
        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        // Комментарии
        singleLineCommentFormat.setForeground(QColor(128, 128, 128));
        rule.pattern = QRegularExpression("#[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        // Строки в одинарных кавычках
        quotationFormat.setForeground(QColor(215, 186, 125));
        rule.pattern = QRegularExpression("\'([^\'\\n]*)\'");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // Строки в двойных кавычках
        quotationFormat.setForeground(QColor(215, 186, 125));
        rule.pattern = QRegularExpression("\"([^\"]*)\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // Числа
        numberFormat.setForeground(QColor(181, 206, 168));
        rule.pattern = QRegularExpression("\\b\\d+\\b");
        rule.format = numberFormat;
        highlightingRules.append(rule);

        // Квадратные скобки
        bracketFormat.setForeground(QColor(0, 149, 200));
        rule.pattern = QRegularExpression("\\[([^\\]]+)\\]");
        rule.format = bracketFormat;
        highlightingRules.append(rule);
    }

protected:
    void highlightBlock(const QString& text) override {
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat bracketFormat;
};

class CustomListItemWidget : public QWidget
{
    Q_OBJECT

public:
    CustomListItemWidget(const QString &repo, const QString &text, const int &installed, const int &orphaned, const int &old, const int &rating, const QString &sizeInstallation, const QColor &color, const bool &isFavorite, QWidget *parent = nullptr)
        : QWidget(parent), packageName(repo + "/" + text)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);

        QString repoName = repo;
        // Убираем префиксы вида "cachyos-v3", "cachyos-core-v3" и т.д.
        static const QRegularExpression prefixPattern("^(cachyos)(?:-(core|extra))?-v\\d+$");
        QRegularExpressionMatch match = prefixPattern.match(repo);

        if (match.hasMatch())
            repoName = match.captured(1);

        QLabel *repoLabel = createIconLabel(QFile::exists(":/img/" + repoName + ".png") ? ":/img/" + repoName + ".png" : ":/img/community.png");
        layout->addWidget(repoLabel);

        QLabel *textLabel = new QLabel(text, this);
        textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        textLabel->setStyleSheet(QString("color:%1;font-size:12pt;").arg(color.name()));
        layout->addWidget(textLabel);

        if (orphaned == 1)
        {
            QLabel *orphanedLabel = createIconLabel(":/img/orphaned.png");
            layout->addWidget(orphanedLabel);
        }

        if (old == 1)
        {
            QLabel *oldLabel = createIconLabel(":/img/old.png");
            layout->addWidget(oldLabel);
        }

        if (installed == 1)
        {
            QLabel *installedLabel = createIconLabel(":/img/installed.png");
            layout->addWidget(installedLabel);
        }

        favoriteLabel = createIconLabel(":/img/p_1.png");
        favoriteLabel->setVisible(isFavorite);
        layout->addWidget(favoriteLabel);

        if (repo == "aur")
        {
            QLabel *ratingLabel = createRatingLabel(rating);
            layout->addWidget(ratingLabel);
        }
        else
        {
            QLabel *instLabel = new QLabel(sizeInstallation, this);
            instLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
            instLabel->setFixedSize(80, 24);
            instLabel->setStyleSheet("color:#474747;font-size:12pt;");
            layout->addWidget(instLabel);
        }

        setLayout(layout);
        layout->setContentsMargins(4, 0, 4, 0);

        connect(this, &CustomListItemWidget::clicked, this, &CustomListItemWidget::handleClicked);
    }

    void updateFavoriteIcon(bool isFavorite) {
        if (favoriteLabel) {
            favoriteLabel->setVisible(isFavorite);
        }
    }

    QString getPackageNameWithoutRepo() const {
        return packageName.section('/', -1);  // Возвращает часть после последнего "/"
    }

    QString getPackageName() const {
        return packageName;
    }
signals:
    void clicked(const QString &packageName);

private slots:
    void handleClicked() {
        emit clicked(packageName);
    }

private:
    QString packageName;
    QLabel *favoriteLabel;

    QLabel *createIconLabel(const QString &iconPath)
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setFixedSize(20, 20);
        label->setPixmap(QPixmap(iconPath).scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        return label;
    }

    QLabel *createRatingLabel(int rating)
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setFixedSize(50, 8);

        QPixmap pixmap(50, 8);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.setPen(QPen(QColor(31, 31, 31)));
        painter.setBrush(QColor(47, 47, 47));
        painter.drawRoundedRect(0, 0, 50, 8, 4, 4);

        if (rating > 100)
            rating = 100;

        int fillWidth = static_cast<int>(50 * static_cast<double>(rating) / 100.0);
        painter.setBrush(QColor(72, 140, 135));
        painter.drawRoundedRect(0, 0, fillWidth, 8, 4, 4);

        label->setPixmap(pixmap);
        return label;
    }
};

class CustomProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomProgressBar(QWidget *parent = nullptr)
        : QWidget(parent), value(0), maxValue(100)
    {
        setFixedSize(60, 8);
    }

    void setValue(int val)
    {
        value = val;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);
        QPixmap pixmap(size());
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.setPen(QPen(QColor(31, 31, 31)));  // Устанавливаем цвет рамки
        painter.setBrush(QColor(47, 47, 47)); // Прозрачный цвет
        painter.drawRoundedRect(0, 0, width(), height(), 4, 4);

        int fillWidth = static_cast<int>((static_cast<double>(value) / maxValue) * width());
        painter.setBrush(QColor(70, 134, 130));
        painter.drawRoundedRect(0, 0, fillWidth, height(), 4, 4);

        painter.end();

        QPainter widgetPainter(this);
        widgetPainter.drawPixmap(0, 0, pixmap);
    }

private:
    int value;
    int maxValue;
};

#endif // MAINWINDOW_H
