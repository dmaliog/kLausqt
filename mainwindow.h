#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qwebengineview.h"
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QNetworkAccessManager>

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
    QMap<QString, QString> iconMap; // Статический словарь для кэширования информации об иконках

    QListWidgetItem* orphanButton;
    QListWidgetItem* cacheButtonYay;
    QListWidgetItem* cacheButtonPacman;

    QAction* previousAction; // Предыдущий QAction

    int page; // какая страница используется
    int animloadpage;
    int trayon; // закрывать без трея
    int repair; // создавать бэкап при удалении или нет
    int animload; // анимация загрузки
    int updinst; //проверять систему перед установкой пакетов или нет
    int volumenotify; // громкость уведомлений
    int volumemenu; // громкость меню
    int mainpage; // главная страница
    int yaycache; // кэш
    int table1; // описание
    int table2; // версия
    int table3; // голоса
    int table4; // популярность
    int table5; // последнее обновление
    int fav; // последнее обновление
    int host;

    QString packageURL;
    QString lang;
    QString currentDesktop;
    QString teatext;
    QString worktext;

    QTime timeupdate;
    QTime timetea;
    QTime timework;

    QTimer *updateIconTimer;
    QTimer* teaTimer;
    QTimer* workTimer;

    bool errorShown = false;
    bool hasUpdates;

    QWebEngineView* webView;

    QStringList shResourcePaths = {":/sh/1c.sh",
                                   ":/sh/wayland.sh",
                                   ":/sh/imgneofetch.sh",
                                   ":/sh/root.sh",
                                   ":/sh/PKGBUILD.sh",
                                   ":/sh/save.sh",
                                   ":/sh/load.sh",
                                   ":/sh/zen.sh"};

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
                                      ":/bench/dd.sh"};

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
    void on_action_16_triggered();
    void on_action_17_triggered();
    void on_action_27_triggered();
    void on_action_28_triggered();
    void on_action_29_triggered();
    void on_action_30_triggered();

    void sendNotification(const QString& title, const QString& message);

private slots:
    QIcon getPackageIcon(const QString& packageName);
    QString getScriptContent(const QString& filePath);

    void mrpropper(int value);
    void showLoadingAnimation(bool show);
    void checkVersionAndClear();

    void loadSound(int soundIndex);
    void loadSettings();
    void loadContent();
    void loadSystemInfo();
    void loadingListWidget();
    void loadFolders();
    void loadScripts(const QString& baseDir, QListWidget* listWidget);
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

    void TeaTimer();
    void WorkTimer();
    void handleServerResponse(QNetworkReply* reply);
    void handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir);

    void on_spin_rating_valueChanged(int arg1);

    void on_combo_mainpage_currentIndexChanged();
    void on_combo_animload_currentIndexChanged();
    void on_combo_cache_currentIndexChanged();
    void on_combo_host_currentIndexChanged(int index);
    void on_combo_lang_currentIndexChanged(int index);

    void on_check_description_stateChanged();
    void on_check_version_stateChanged();
    void on_check_voices_stateChanged();
    void on_check_popularity_stateChanged();
    void on_check_lastupdate_stateChanged();
    void on_check_repair_stateChanged();
    void on_check_updateinstall_stateChanged();
    void on_check_trayon_stateChanged();
    void on_check_animload_stateChanged();

    void on_dial_volmenu_valueChanged(int value);
    void on_dial_volnotify_valueChanged(int value);

    void on_list_repair_itemDoubleClicked(QListWidgetItem *item);
    void on_list_bench_itemDoubleClicked(QListWidgetItem *item);
    void on_list_sh_itemDoubleClicked(QListWidgetItem *item);
    void on_list_grub_itemDoubleClicked(QListWidgetItem *item);
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
};

#endif // MAINWINDOW_H
