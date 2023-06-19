#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QString>
#include <QVector>

struct Terminal {
    QString binary;
    QString args;
};

const QVector<Terminal> m_terminalList = {
    {"/usr/bin/konsole", "-e"},
    {"/usr/bin/gnome-terminal", "--"},
    {"/usr/bin/xfce4-terminal", "-x"},
    {"/usr/bin/lxterminal", "-e"},
    {"/usr/bin/alacritty", "-e"},
    {"/usr/bin/xterm", "-e"}
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
    QString packageURL;
    int page; // какая страница используется
    int trayon; // закрывать без трея
    int repair; // создавать бэкап при удалении или нет
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
    QString lang;
    QString currentDesktop;
    QString teatext;
    QString worktext;

    QTime timeupdate;
    QTime timetea;
    QTime timework;

    bool errorShown = false;
    bool hasUpdates;

    QTimer *updateIconTimer;
    QTimer* teaTimer;
    QTimer* workTimer;

    QStringList shResourcePaths = {":/sh/1c.sh",
                                   ":/sh/wayland.sh",
                                   ":/sh/imgneofetch.sh",
                                   ":/sh/root.sh",
                                   ":/sh/PKGBUILD.sh",
                                   ":/sh/save.sh",
                                   ":/sh/load.sh",
                                   ":/sh/zen.sh"};

    QStringList clearResourcePaths = {":/clear/clear_trash.sh",
                                      ":/clear/yay.sh",
                                      ":/clear/pacman.sh"};

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
    void on_action_18_triggered();
    void on_action_24_triggered();
    void on_action_25_triggered();
    void on_action_26_triggered();
    void on_action_27_triggered();
    void on_action_28_triggered();
    void on_action_29_triggered();
    void on_action_30_triggered();
    void on_action_sh_triggered();

    void sendNotification(const QString& title, const QString& message);

private slots:
    void UpdateIcon();
    void setHasUpdates(bool updates);

    void createArchive(const QString& folderPath, const QString& folderName);
    void loadFolders();
    void restoreArchive(const QString& archivePath);

    void checkVersionAndClear();

    void loadScripts(const QString& baseDir, QListWidget* listWidget);
    void saveScripts(const QStringList& resourcePaths, const QString& baseDir);
    void removeScripts(const QStringList& resourcePaths, const QString& baseDir);
    QString getScriptContent(const QString& filePath);

    void loadSound(int soundIndex);
    void loadSettings();
    void loadContent();
    void loadSystemInfo();
    void loadingListWidget();
    void mrpropper();
    void showLoadingAnimation(bool show);

    void onTimeChanged(const QTime& time);
    void TeaTimer();
    void WorkTimer();

    void handleServerResponse(QNetworkReply* reply);
    void openDirectory(const QString &dirPath);
    void removeToolButtonTooltips(QToolBar* toolbar);
    void on_list_itemDoubleClicked(QListWidgetItem *item, const QString& scriptDir);
    void on_list_sh_itemDoubleClicked(QListWidgetItem *item);
    void on_list_grub_itemDoubleClicked(QListWidgetItem *item);
    void on_list_clear_itemDoubleClicked(QListWidgetItem *item);
    void on_spin_rating_valueChanged(int arg1);
    void on_time_update_timeChanged(const QTime &time);
    void on_check_trayon_stateChanged();
    void on_combo_mainpage_currentIndexChanged();
    void on_combo_cache_currentIndexChanged();
    void on_check_description_stateChanged();
    void on_check_version_stateChanged();
    void on_check_voices_stateChanged();
    void on_check_popularity_stateChanged();
    void on_check_lastupdate_stateChanged();
    void on_action_31_triggered();
    void on_action_32_triggered();
    void on_action_33_triggered();
    void on_combo_lang_currentIndexChanged(int index);
    void on_action_34_triggered();
    void on_action_35_triggered();
    void on_action_13_triggered();
    void on_dial_volmenu_valueChanged(int value);
    void on_dial_volnotify_valueChanged(int value);
    void on_action_addsh_triggered();
    void on_action_rmsh_triggered();
    void on_action_editsh_triggered();
    void on_list_repair_itemDoubleClicked(QListWidgetItem *item);
    void on_push_repair_clicked();
    void on_check_repair_stateChanged();
    void on_action_timer_triggered();
    void on_action_host_triggered();
    void on_action_19_triggered();

    void on_line_tea_textChanged(const QString &arg1);
    void on_line_work_textChanged(const QString &arg1);
    void on_time_tea_timeChanged(const QTime &time);
    void on_time_work_timeChanged(const QTime &time);
};

#endif // MAINWINDOW_H
