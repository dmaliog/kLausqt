#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qcompleter.h"
#include "qgraphicseffect.h"
#include "qgraphicsvideoitem.h"
#include "qmediaplayer.h"
#include "qnetworkaccessmanager.h"
#include "qstandarditemmodel.h"
#include "qsyntaxhighlighter.h"
#include "qtextbrowser.h"
#include <QLabel>
#include <QWebEngineView>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QProcess>
#include <QGraphicsView>
#include <QRandomGenerator>

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
    Q_OBJECT // Добавляем макрос Q_OBJECT

public:
    Ui::MainWindow *ui;
    QSystemTrayIcon trayIcon;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QColor generateRandomColor(const int &colorlist)
    {
        if (colorlist == 2)
        {
            QColor color;
            do {
                color = QColor::fromHsv(QRandomGenerator::global()->bounded(360),
                                        QRandomGenerator::global()->bounded(200),
                                        QRandomGenerator::global()->bounded(150, 256));
            } while (color.blue() > 200 || !color.isValid());
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
    QGraphicsOpacityEffect *opacityEffect;

    QString detailsAURdefault;

    QString originalLabelText;

    QString nvidiaVersion;
    QString nvidiaDkms;
    QString nvidiaUtils;
    QString nvidiaSettings;
    QString libxnvctrl;
    QString openclNvidia;
    QString lib32NvidiaUtils;
    QString lib32OpenclNvidia;
    QString nvidiaDkmsName;
    QString nvidiaUtilsName;
    QString nvidiaSettingsName;
    QString libxnvctrlName;
    QString openclNvidiaName;
    QString lib32NvidiaUtilsName;
    QString lib32OpenclNvidiaName;

    // Умные указатели
    QCompleter* completer;
    QStandardItemModel* completerModel;
    QSharedPointer<QProcess> snapProcess;

    QSharedPointer<QProcess> currentTerminalProcess;
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
    QSharedPointer<QTimer> teaTimer;
    QSharedPointer<QTimer> workTimer;

    // Простые типы данных и переменные
    QString helper;
    QListWidgetItem* orphanButton;
    QListWidgetItem* cacheButtonHelper;
    QListWidgetItem* cacheButtonPacman;
    QStringList snapPackageNames;
    QStringList helperPackageNames;

    // Статические переменные
    QStringList imageUrls;
    QNetworkAccessManager *manager;
    QNetworkAccessManager networkManager;
    QSet<QString> addedLinks;
    QMap<QString, QString> appIcons;
    QTime timeupdate;
    QTime timetea;
    QTime timework;
    QTime timeout;
    bool errorShown = false;
    bool hasUpdates = false;
    bool hasUpdatesSnap = false;
    bool loadpage = true;
    bool isFirstLoad = true;

    // QAction
    QAction* actionLoad = nullptr;
    QAction* previousAction = nullptr;

    // QMap
    QMap<QString, QString> iconMap;

    QStringList shResourcePaths = {":/sh/1c.sh",
                                   ":/sh/wayland.sh",
                                   ":/sh/imgneofetch.sh",
                                   ":/sh/root.sh",
                                   ":/sh/PKGBUILD.sh",
                                   ":/sh/save.sh",
                                   ":/sh/load.sh",
                                   ":/sh/zen.sh",
                                   ":/sh/snap.sh",
                                   ":/sh/amd.sh",
                                   ":/sh/nvidia.sh",
                                   ":/sh/intel.sh",
                                   ":/sh/pacman-lock.sh"};

    QStringList clearResourcePaths = {":/clear/clear_trash.sh"};

    QStringList journalsResourcePaths = {":/journals/neofetch.sh",
                                         ":/journals/systemd-analyze.sh",
                                         ":/journals/lspci.sh",
                                         ":/journals/lsusb.sh",
                                         ":/journals/inxi.sh",
                                         ":/journals/hwinfo.sh",
                                         ":/journals/lsblk.sh",
                                         ":/journals/fdisk.sh",
                                         ":/journals/native-pkg.sh",
                                         ":/journals/foreign-pkg.sh",
                                         ":/journals/xorg-log.sh",
                                         ":/journals/top-pkg.sh"};

    QStringList benchResourcePaths = {":/bench/unixbench.sh",
                                      ":/bench/interbench.sh",
                                      ":/bench/ttcp.sh",
                                      ":/bench/iperf.sh",
                                      ":/bench/time.sh",
                                      ":/bench/hdparm.sh",
                                      ":/bench/gnome-disks.sh",
                                      ":/bench/kdiskmark.sh",
                                      ":/bench/systemd-analyze.sh",
                                      ":/bench/dd.sh",
                                      ":/bench/7z.sh",
                                      ":/bench/peakperf.sh",
                                      ":/bench/basemark.sh",
                                      ":/bench/blender-benchmark.sh",
                                      ":/bench/GFXBench.sh",
                                      ":/bench/glmark2.sh",
                                      ":/bench/glxgears.sh",
                                      ":/bench/gputest.sh",
                                      ":/bench/intel-gpu-tools.sh",
                                      ":/bench/unigine-sanctuary.sh",
                                      ":/bench/unigine-tropics.sh",
                                      ":/bench/unigine-heaven.sh",
                                      ":/bench/unigine-valley.sh",
                                      ":/bench/unigine-superposition.sh",
                                      ":/bench/vkmark.sh",
                                      ":/bench/bonnie.sh",
                                      ":/bench/s-tui.sh",
                                      ":/bench/phoronix-test-suite.sh",
                                      ":/bench/hardinfo.sh",
                                      ":/bench/iozone.sh"};


    QStringList endingsToRemove = QStringList() << "-bin" << "-git" << "-qt" << "-qt4" << "-qt5" << "-qt6"
                                                << "qt-" << "qt4-" << "qt5-" << "qt6-" << "-gtk" << "-gtk2"
                                                << "-gtk3" << "-cvs" << "-svn" << "-hg" << "-darcs" << "-bzr";
protected:
    void closeEvent(QCloseEvent *event) override; // объявление метода closeEvent()
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

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
    void on_action_28_triggered();
    void on_action_30_triggered();
    void sendNotification(const QString& title, const QString& message);

private slots:
    void onThumbnailClicked(const QModelIndex& index);
    void onListItemClicked(const QString &packageName, int row, QListWidgetItem *item);

    void setupConnections();
    void handleActionHovered();

    void AnimationBackground();
    void mrpropper(int value, QAction *action);

    void connectProcessSignals(QSharedPointer<QProcess>& process, QTextBrowser* outputWidget);
    void onCurrentProcessReadyReadSearch();
    void onCurrentProcessReadyRead();
    void onReplyFinished(QNetworkReply *reply);

    void handleServerResponseSearch(const QString& reply);
    void onSearchTimeout();
    void createAndAddListItemSearch(const QString& packageName);
    void updateCompleterModel();
    void setCursorAndScrollToItem(const QString& itemName);
    void downloadAndSaveImages(const QString& packageName, const QStringList& imageUrls, const QString& folder);
    void updateImageView();
    void checkForDowngrades(const QString& packagesArchiveAUR);
    void onListDowngradeItemDoubleClicked(QListWidgetItem *item);
    void addLinkToList(const QString &link);
    void handleListItemClicked(QListWidgetItem *item, const QString& scriptDir);
    void processListItem(int row, QListWidget* listWidget, QTextBrowser* detailsWidget, QString package);
    void search(const QString& searchText);
    void searchAndScroll(QAbstractItemView* view, const QString& text);

    void createSearchBar();
    void setupListContextMenu();
    void showListContextMenu(const QPoint& pos);
    void miniAnimation(bool visible, QWidget *targetWidget);
    QIcon getPackageIcon(const QString& packageName);
    QString getScriptContent(const QString& filePath);
    QString processPackageInfo(const QString& packageInfo);
    QString findIconPath(const QString &iconNumber);
    QStringList executeCommand(const QStringList& command);

    void onTimeChanged(const QTime& time);
    void TeaTimer();
    void WorkTimer();
    void loadSound(int soundIndex);

    void loadSettings();
    void loadContentInstall();
    void loadContent(int value, bool valuepage);
    void loadingListWidget();
    void loadFolders();
    void loadScripts(const QString& basedDir, QListWidget* listWidget);
    void saveScripts(const QStringList& resourcePaths, const QString& baseDir);
    void removeToolButtonTooltips(QToolBar* toolbar);
    void removeScripts(const QStringList& resourcePaths, const QString& baseDir);
    void removeDirectory(const QString& dirPath);
    void restoreArchive(const QString& archivePath);
    void createArchive(const QString& folderPath, const QString& folderName);

    void UpdateIcon();
    void UpdateSnap();

    void setHasUpdates(bool updates);
    void showLoadingAnimation(bool show, QWebEngineView* webView);
    void showLoadingAnimationMini(bool show);

    void checkVersionAndClear();
    void handleServerResponse(const QString &reply);
    void handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void searchTextChanged(const QString& searchText);
    void on_combo_mainpage_currentIndexChanged(int index);
    void on_combo_animload_currentIndexChanged(int index);
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
    void on_check_cacheremove_stateChanged(int arg1);
    void on_check_trans_stateChanged(int arg1);
    void on_check_colorlist_stateChanged(int arg1);
    void on_dial_volnotify_valueChanged(int value);
    void on_list_repair_itemDoubleClicked(QListWidgetItem *item);
    void on_list_bench_itemDoubleClicked(QListWidgetItem *item);
    void on_list_sh_itemDoubleClicked(QListWidgetItem *item);
    void on_list_journal_itemDoubleClicked(QListWidgetItem *item);
    void on_list_clear_itemDoubleClicked(QListWidgetItem *item);
    void on_line_tea_textChanged(const QString &arg1);
    void on_line_work_textChanged(const QString &arg1);
    void on_time_tea_timeChanged(const QTime &time);
    void on_time_work_timeChanged(const QTime &time);
    void on_time_update_timeChanged(const QTime &time);
    void on_time_timeout_timeChanged(const QTime &time);
    void on_action_bench_triggered();
    void on_action_grub_triggered();
    void on_action_pacman_triggered();
    void on_action_repair_triggered();
    void on_action_timer_triggered();
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
    void on_action_nvidia_triggered();
    void on_list_bench_itemClicked(QListWidgetItem *item);
    void on_list_journal_itemClicked(QListWidgetItem *item);
    void on_list_sh_itemClicked(QListWidgetItem *item);
    void on_push_back_clicked();
    void on_push_grub_fast_clicked();
    void on_push_install_clicked();
    void on_push_kde_clicked();
    void on_push_repair_clicked();
    void on_push_vk_clicked();
    void on_action_like_triggered();
    void on_action_imgpkg_triggered(bool checked);
    void on_action_updatelist_triggered();
    void on_combo_theme_currentIndexChanged(int index);
    void on_check_saveurl_stateChanged(int arg1);
    void on_push_pacman_clicked();
    void on_push_fstab_clicked();
    void writeToFile(const QString& filePath, const QString& content);
    void on_action_fstab_triggered();
    void loadConfigFile(const QString &filePath, QTextEdit *targetTextEdit, int tabIndex);
    void on_push_grub_clicked();
};


class MySyntaxHighlighter : public QSyntaxHighlighter {
public:
    MySyntaxHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {
        HighlightingRule rule;

        // Ключевые слова
        keywordFormat.setForeground(QColor(86, 156, 214)); // #569cd6
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns;
        keywordPatterns << "\\bif\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bwhile\\b";
        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        // Комментарии
        singleLineCommentFormat.setForeground(QColor(128, 128, 128)); // #808080
        rule.pattern = QRegularExpression("#[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        // Строки в одинарных кавычках
        quotationFormat.setForeground(QColor(215, 186, 125)); // #d7ba7d
        rule.pattern = QRegularExpression("\'([^\'\\n]*)\'");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // Строки в двойных кавычках
        quotationFormat.setForeground(QColor(215, 186, 125)); // #d7ba7d
        rule.pattern = QRegularExpression("\"([^\"]*)\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // Числа
        numberFormat.setForeground(QColor(181, 206, 168)); // #b5cea8
        rule.pattern = QRegularExpression("\\b\\d+\\b");
        rule.format = numberFormat;
        highlightingRules.append(rule);

        // Квадратные скобки
        bracketFormat.setForeground(QColor(0, 149, 200)); // #0095c8
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
    CustomListItemWidget(const QString &repo, const QString &text, const int &installed, const int &orphaned, const int &old, const int &rating, const QString &sizeInstallation, const QColor &color, QWidget *parent = nullptr)
        : QWidget(parent), packageName(text)
    {

        QHBoxLayout *layout = new QHBoxLayout(this);


        QLabel *repoLabel = createIconLabel(":/img/" + repo + ".png");
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

    QLabel *createIconLabel(const QString &iconPath)
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setFixedSize(26, 24);
        label->setPixmap(QPixmap(iconPath).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
        painter.setBrush(QColor(137, 123, 170));
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
        painter.setBrush(QColor(137, 123, 170));
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
