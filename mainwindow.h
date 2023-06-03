#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    {"/usr/bin/mate-terminal", "-e"},
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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QColor generateRandomColor();

private:
    QString packageURL;

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
    void on_action_13_triggered();
    void on_action_16_triggered();
    void on_action_17_triggered();
    void on_action_18_triggered();
    void on_action_19_triggered();
    void on_action_20_triggered();
    void on_action_21_triggered();
    void on_action_22_triggered();
    void on_action_23_triggered();
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
    void checkUpdates();
    void checkVersionAndClear();
    void loadScripts(const QStringList& resourcePaths, const QString& baseDir, QListWidget* listWidgete);
    void loadSound(int soundIndex);
    void loadSettings();
    void loadContent();
    void loadingListWidget();
    void mrpropper();
    void showLoadingAnimation(bool show);

    void onTimeChanged(const QTime& time);
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
    void on_check_soundon_stateChanged();
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
};

#endif // MAINWINDOW_H
