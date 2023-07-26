#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QWebEngineView>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QProcess>

#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>

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
    QSharedPointer<QProcess> snapProcess;
    QSharedPointer<QProcess> currentProcess;

    QTime timeupdate;
    QTime timetea;
    QTime timework;

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

    //умные QLineEdit
    QSharedPointer<QLineEdit> searchLineEdit;

    //умные QWebEngineView
    QSharedPointer<QWebEngineView> webView;

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
                                   ":/sh/snap.sh"};

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
                                         ":/journals/grub-cfg.sh",
                                         ":/journals/fstab.sh",
                                         ":/journals/pacman.sh",
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
                                                << "qt-" << "qt4-" << "qt5-" << "qt6-" << "-gtk"
                                                << "-gtk2" << "-gtk3";
protected: // события сворачивания окна
    void closeEvent(QCloseEvent *event) override; // объявление метода closeEvent()

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
    void on_action_10_triggered();
    void on_action_11_triggered();
    void on_action_12_triggered();
    void on_action_17_triggered();
    void on_action_27_triggered();
    void on_action_28_triggered();
    void on_action_30_triggered();
    void sendNotification(const QString& title, const QString& message);

private slots:
    void onSnapProcessFinished();
    void onCurrentProcessReadyRead();

    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    bool isSnapInstalled();

    void createSearchBar();
    void searchTextChanged(const QString& searchText);
    void search();

    void setupTableContextMenu();
    void showTableContextMenu(const QPoint& pos);

    void miniAnimation(int x, int y, bool visible);

    QIcon getPackageIcon(const QString& packageName);
    QString getScriptContent(const QString& filePath);

    void mrpropper(int value);
    void showLoadingAnimation(bool show);
    void showLoadingAnimationMini(bool show);
    void checkVersionAndClear();

    void loadSound(int soundIndex);
    void loadSettings();
    void loadContentInstall();
    void loadContent();
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
    void searchAndScroll(QListWidget* listWidget, const QString& text);

    void on_combo_mainpage_currentIndexChanged(int index);
    void on_combo_animload_currentIndexChanged(int index);
    void on_combo_cache_currentIndexChanged(int index);
    void on_combo_host_currentIndexChanged(int index);
    void on_combo_lang_currentIndexChanged(int index);
    void on_combo_repo_currentIndexChanged(int index);
    void on_combo_helper_currentIndexChanged(int index);
    void on_combo_bench_currentIndexChanged(int index);

    void on_check_repair_stateChanged();
    void on_check_updateinstall_stateChanged();
    void on_check_trayon_stateChanged();
    void on_check_animload_stateChanged();

    void onTableAurCellClicked(int row);

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

    void on_action_stop_triggered();
    void on_action_restart_triggered();
    void on_action_catalog_triggered();
    void on_action_bench_triggered();
    void on_action_system_triggered();
    void on_action_repair_triggered();
    void on_action_timer_triggered();
    void on_action_host_triggered();
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

    void on_push_server_clicked();
    void on_push_conf_clicked();
    void on_push_php_clicked();
    void on_push_grub_clicked();
    void on_push_site_clicked();
    void on_push_repair_clicked();
    void on_action_snap_triggered();
    void on_push_pacman_clicked();
};

#endif // MAINWINDOW_H
