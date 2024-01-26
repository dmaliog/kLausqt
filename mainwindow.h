#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcompleter.h"
#include "qgraphicseffect.h"
#include "qgraphicsvideoitem.h"
#include "qmediaplayer.h"
#include "qnetworkaccessmanager.h"
#include "qstandarditemmodel.h"
#include "qtextbrowser.h"
#include <QLabel>
#include <QWebEngineView>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QProcess>
#include <QGraphicsView>

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

    QColor generateRandomColor();

private:
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
    QList<QPixmap> pixmaps;
    int currentIndex;
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

    QStringList cfgResourcePaths = {":/cfg/grub-cfg.sh",
                                         ":/cfg/fstab.sh",
                                         ":/cfg/pacman.sh"};

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
    void processListItem(int row, QListWidget* listWidget, QTextBrowser* detailsWidget);
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
    void onListAurItemClicked(QListWidgetItem *item);
    void on_dial_volnotify_valueChanged(int value);
    void on_list_repair_itemDoubleClicked(QListWidgetItem *item);
    void on_list_bench_itemDoubleClicked(QListWidgetItem *item);
    void on_list_sh_itemDoubleClicked(QListWidgetItem *item);
    void on_list_journal_itemDoubleClicked(QListWidgetItem *item);
    void on_list_cfg_itemDoubleClicked(QListWidgetItem *item);
    void on_list_clear_itemDoubleClicked(QListWidgetItem *item);
    void on_line_tea_textChanged(const QString &arg1);
    void on_line_work_textChanged(const QString &arg1);
    void on_time_tea_timeChanged(const QTime &time);
    void on_time_work_timeChanged(const QTime &time);
    void on_time_update_timeChanged(const QTime &time);
    void on_time_timeout_timeChanged(const QTime &time);
    void on_action_bench_triggered();
    void on_action_system_triggered();
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
    void on_back_slider_clicked();
    void on_list_bench_itemClicked(QListWidgetItem *item);
    void on_list_cfg_itemClicked(QListWidgetItem *item);
    void on_list_journal_itemClicked(QListWidgetItem *item);
    void on_list_sh_itemClicked(QListWidgetItem *item);
    void on_next_slider_clicked();
    void on_push_back_clicked();
    void on_push_grub_clicked();
    void on_push_install_clicked();
    void on_push_kde_clicked();
    void on_push_pacman_clicked();
    void on_push_repair_clicked();
    void on_push_vk_clicked();
    void on_action_like_triggered();
    void on_action_imgpkg_triggered(bool checked);
    void on_action_updatelist_triggered();
    void on_combo_theme_currentIndexChanged(int index);
    void on_check_saveurl_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
