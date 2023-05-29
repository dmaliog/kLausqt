#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QNetworkAccessManager>

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
    void loadScripts(const QStringList& resourcePaths, const QString& baseDir, QListWidget* listWidget);
    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_grub_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_clear_itemDoubleClicked(QListWidgetItem *item);
    void on_timeEdit_update_timeChanged(const QTime &time);
    void showLoadingAnimation(bool show);
    void onTimeChanged(const QTime& time);
    void handleServerResponse(QNetworkReply* reply);
    void openDirectory(const QString &dirPath);
    void on_spinBox_valueChanged(int arg1);
    void removeToolButtonTooltips(QToolBar* toolbar);
    void loadSound(int soundIndex);
    void loadSettings();
    void loadContent();
    void loadingListWidget();
    void mrpropper();
    void on_checkBox_trayon_stateChanged();
    void on_checkBox_soundon_stateChanged();
    void on_comboBox_mainpage_currentIndexChanged();
    void on_comboBox_yaycache_currentIndexChanged();
    void on_checkBox_1_stateChanged();
    void on_checkBox_2_stateChanged();
    void on_checkBox_3_stateChanged();
    void on_checkBox_4_stateChanged();
    void on_checkBox_5_stateChanged();
    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
