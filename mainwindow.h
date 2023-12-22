#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcompleter.h"
#include "qnetworkaccessmanager.h"
#include "qsettings.h"
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

struct PreviousState {
    int value;
    bool valuepage;
    int list;
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
    QSharedPointer<QLineEdit> searchLineEdit;
    QCompleter* completer;
    QStandardItemModel* completerModel;

    QStringList imageUrls;
    QList<QPixmap> pixmaps;
    int currentIndex;

    QNetworkAccessManager *manager;
    QNetworkAccessManager networkManager;

    QSet<QString> addedLinks;

    QMap<QString, QString> appIcons;

    QWebEngineView *webEngineView2;


    QSharedPointer<QProcess> snapProcess;
    QSharedPointer<QProcess> currentProcess;

    QTime timeupdate;
    QTime timetea;
    QTime timework;
    QTime timeout;

    bool errorShown = false;
    bool hasUpdates = false;
    bool hasUpdatesSnap = false;

    //тупые QAction
    QAction* actionLoad = nullptr;
    QAction* previousAction = nullptr; // Предыдущий QAction

    //тупые QMAP
    QMap<QString, QString> iconMap; // Статический словарь для кэширования информации об иконках

    //умные Qlabel
    QSharedPointer<QLabel> loadingAnimationLabel;
    QScopedPointer<QLabel> loadingLabel;

    //умные QUrlQuery
    QSharedPointer<QUrlQuery> newParams;

    //умные QString
    QSharedPointer<QString> packageURL;
    QSharedPointer<QString> teatext;
    QSharedPointer<QString> worktext;
    QSharedPointer<QString> currentDesktop;
    QSharedPointer<QString> repo;
    QSharedPointer<QString> lang;
    QString helper;

    //умные QTimer
    QSharedPointer<QTimer> updateIconTimer;
    QSharedPointer<QTimer> teaTimer;
    QSharedPointer<QTimer> workTimer;

    //не требуют управления памятью
    QListWidgetItem* orphanButton;
    QListWidgetItem* cacheButtonHelper;
    QListWidgetItem* cacheButtonPacman;

    QStringList snapPackageNames;
    QStringList helperPackageNames;

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
                                   ":/sh/intel.sh"};

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
                                                << "qt-" << "qt4-" << "qt5-" << "qt6-" << "-gtk"
                                                << "-gtk2" << "-gtk3";
protected:
    void closeEvent(QCloseEvent *event) override; // объявление метода closeEvent()
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void on_action_1_triggered();
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
    QString findIconPath(const QString &iconNumber);

    QStringList executeCommand(const QStringList& command);

    void connectProcessSignals(QSharedPointer<QProcess>& process, QTextBrowser* outputWidget);
    QString processPackageInfo(const QString& packageInfo);

    void handleServerResponseSearch(const QString& reply);
    void onCurrentProcessReadyReadSearch();
    void createAndAddListItemSearch(const QString& packageName);

    void updateCompleterModel();

    void setCursorAndScrollToItem(const QString& itemName);
    void onSearchTimeout();

    void downloadAndSaveImages(const QString& packageName, const QStringList& imageUrls, const QString& folder);
    void updateImageView();

    void checkForDowngrades(const QString& packagesArchiveAUR);
    void onListDowngradeItemDoubleClicked(QListWidgetItem *item);

    void addLinkToList(const QString &link);

    void onReplyFinished(QNetworkReply *reply);

    void handleListItemClicked(QListWidgetItem *item, const QString& scriptDir);

    void processListItem(int row, QListWidget* listWidget, QTextBrowser* detailsWidget);

    void onSnapProcessFinished();
    void onCurrentProcessReadyRead();

    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    bool isSnapInstalled();

    void createSearchBar();
    void searchTextChanged(const QString& searchText);
    void search(const QString& searchText);

    void setupListContextMenu();
    void showListContextMenu(const QPoint& pos);

    void miniAnimation(bool visible, QWidget *targetWidget);

    QIcon getPackageIcon(const QString& packageName);
    QString getScriptContent(const QString& filePath);

    void mrpropper(int value);
    void showLoadingAnimation(bool show);
    void showLoadingAnimationMini(bool show);
    void checkVersionAndClear();

    void loadSound(int soundIndex);
    void loadSettings();
    void loadContentInstall();
    void loadContent(int value, bool valuepage);
    void loadSystemInfo();
    void loadingListWidget();
    void loadFolders();
    void loadScripts(const QString& basedDir, QListWidget* listWidget);
    void openDirectory(const QString &dirPath);
    void openUrl(const QString& url);

    void setHasUpdates(bool updates);

    void createArchive(const QString& folderPath, const QString& folderName);
    void saveScripts(const QStringList& resourcePaths, const QString& baseDir);

    void removeToolButtonTooltips(QToolBar* toolbar);
    void removeScripts(const QStringList& resourcePaths, const QString& baseDir);
    void removeDirectory(const QString& dirPath);

    void restoreArchive(const QString& archivePath);
    void onTimeChanged(const QTime& time);

    void UpdateIcon();
    void UpdateSnap();

    void TeaTimer();
    void WorkTimer();
    void handleServerResponse(const QString &reply);
    void handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir);
    void searchAndScroll(QAbstractItemView* view, const QString& text);

    void on_combo_mainpage_currentIndexChanged(int index);
    void on_combo_animload_currentIndexChanged(int index);
    void on_combo_cache_currentIndexChanged(int index);
    void on_combo_lang_currentIndexChanged(int index);
    void on_combo_repo_currentIndexChanged(int index);
    void on_combo_helper_currentIndexChanged(int index);
    void on_combo_bench_currentIndexChanged(int index);

    void on_check_repair_stateChanged(int arg1);
    void on_check_updateinstall_stateChanged(int arg1);
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
    void on_action_19_triggered();
    void on_action_addsh_triggered();
    void on_action_rmsh_triggered();
    void on_action_editsh_triggered();
    void on_action_31_triggered();
    void on_action_32_triggered();
    void on_action_33_triggered();
    void on_action_34_triggered();
    void on_action_35_triggered();
    void on_action_13_triggered();
    void on_action_host_triggered();
    void on_action_restart_triggered();
    void on_action_stop_triggered();
    void on_action_catalog_triggered();
    void on_action_downgrade_triggered();
    void on_action_snap_triggered();
    void on_action_game_triggered();

    void on_push_grub_clicked();
    void on_push_repair_clicked();
    void on_push_pacman_clicked();
    void on_list_journal_itemClicked(QListWidgetItem *item);
    void on_list_cfg_itemClicked(QListWidgetItem *item);
    void on_list_sh_itemClicked(QListWidgetItem *item);
    void on_list_bench_itemClicked(QListWidgetItem *item);
    void on_push_vk_clicked();
    void on_push_kde_clicked();
    void on_reload_aur_clicked();
    void on_reload_aurpkg_clicked();
    void on_back_slider_clicked();
    void on_next_slider_clicked();
    void on_img_aur_toggled(bool checked);
    void on_action_nvidia_triggered();
    void on_push_back_clicked();
    void on_push_install_clicked();
};

#endif // MAINWINDOW_H
