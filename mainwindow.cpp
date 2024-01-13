//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################
#include "mainwindow.h"
#include "qmediaplayer.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
#include <QSoundEffect>
#include <unistd.h>
#include <sys/utsname.h>
#include <QRegularExpression>
#include <QCompleter>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString mainDir = QDir::homePath() + "/.config/kLaus/";
QString filePath = mainDir + "settings.ini";
QString currentVersion = "11.3";
QString packagesArchiveAUR = "steam";
QSettings settings(filePath, QSettings::IniFormat);
int nvidia = 0; // nvidia
int pkg = 0; //пакетный менеджер 0-yay / 1-paru
int page = 0; // какая страница используется
int animloadpage = 0;
int trayon = 0; // закрывать без трея
int autostart = 0; //автостарт
int repair = 0; // создавать бэкап при удалении или нет
int animload = 0; // анимация загрузки
int updinst = 0; //проверять систему перед установкой пакетов или нет
int clearinstall = 2; //удалить пакет перед новой установкой
int volumenotify = 0; // громкость уведомлений
int mainpage = 0; // главная страница
int helpercache = 0; // кэш
int benchlist = 0; // бенчлист
int numPackages = 0;
int list = 0;
int cacheremove = 0; // удаление кэша при выходе
int auth = 0;
int animation = 0;
int saveurl = 2;

//---#####################################################################################################################################################
//--############################################################## ОПРЕДЕЛЕНИЕ ТЕРМИНАЛА ################################################################
//-#####################################################################################################################################################

QMap<int, QMap<QString, QStringList>> packageCommands = {
    {0,
        {
            {"install", {"yay", "-S"}},
            {"remove", {"yay", "-R"}},
            {"info", {"yay", "-Qi"}},
            {"update", {"yay", "-Syu"}},
            {"list_files", {"yay", "-Ql"}},
            {"query", {"yay", "-Q"}},
            {"show_info", {"yay", "-Si"}},
            {"query_explicit", {"yay", "-Qe"}},
            {"search", {"yay", "-Ss"}},
            {"query_q", {"yay", "-Qq"}},
            {"remove_explicit", {"yay", "-Rs"}},
            {"query_depends", {"yay", "-Qdtq"}},
            {"clean_cache", {"yay", "-Sc"}},
            {"clean_all_cache", {"yay", "-Scc"}},
            {"localinstall", {"yay", "-Udd"}},
            {"like", {"yay", "-Wv"}},
            {"dislike", {"yay", "-Wu"}}
        }
    },
    {1,
        {
            {"install", {"paru", "-S", "--skipreview"}},
            {"remove", {"paru", "-R"}},
            {"info", {"paru", "-Qi"}},
            {"update", {"paru", "-Syu", "--skipreview"}},
            {"list_files", {"paru", "-Ql"}},
            {"query", {"paru", "-Q"}},
            {"show_info", {"paru", "-Si"}},
            {"query_explicit", {"paru", "-Qe"}},
            {"search", {"paru", "-Ss"}},
            {"query_q", {"paru", "-Qq"}},
            {"remove_explicit", {"paru", "-Rs"}},
            {"query_depends", {"paru", "-Qdtq"}},
            {"clean_cache", {"paru", "-Sc"}},
            {"clean_all_cache", {"paru", "-Scc"}},
            {"localinstall", {"paru", "-Udd"}}
        }
    }
};

Terminal getTerminal()
{
    QVector<Terminal> terminalList = {
        {"/usr/bin/konsole", "-e"},
        {"/usr/bin/gnome-terminal", "--"},
        {"/usr/bin/xfce4-terminal", "-x"},
        {"/usr/bin/lxterminal", "-e"},
        {"/usr/bin/alacritty", "-e"},
        {"/usr/bin/xterm", "-e"}
    };
    for (const Terminal& terminal : terminalList) {
        if (QFile::exists(terminal.binary))
            return terminal;
    }
    return Terminal();
}
//---#####################################################################################################################################################
//--################################################################# ОСНОВНЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_action_2_triggered()
{
    mrpropper(2);
    ui->label1->setText(tr("Каталог пакетов"));
    ui->searchLineEdit->setPlaceholderText(tr("Поиск по пакетам..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(1);
    ui->action_updatelist->setVisible(true);
    ui->action_4->setVisible(true);
    ui->action_30->setVisible(true);
    ui->action_34->setVisible(true);
    ui->action_imgpkg->setVisible(true);
    ui->action_like->setVisible(true);
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_7_triggered()
{
    mrpropper(4);
    ui->label1->setText(tr("Каталог установленных пакетов"));
    ui->searchLineEdit->setPlaceholderText(tr("Поиск установленных пакетов..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(2);
    ui->action_updatelist->setVisible(true);
    ui->action_11->setVisible(true);
    ui->action_5->setVisible(true);
    ui->action_6->setVisible(true);
    ui->action_34->setVisible(true);
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_17_triggered()
{
    mrpropper(3);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск скриптов..."));
    ui->searchLineEdit->setVisible(true);
    ui->label1->setText(tr("Полезные скрипты"));
    ui->action_addsh->setVisible(true);
    ui->action_editsh->setVisible(true);
    ui->action_rmsh->setVisible(true);
    ui->tabWidget->setCurrentIndex(4);
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_9_triggered()
{
    mrpropper(5);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск журналов и конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->action_27->setVisible(true);
    ui->action_bench->setVisible(true);
    ui->action_repair->setVisible(true);
    ui->action_system->setVisible(true);
    on_action_27_triggered();
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_3_triggered()
{
    if (page == 6) return;
    showLoadingAnimationMini(false);
    mrpropper(6);

    QString aurUrl = ui->webEngineView_aur->url().toString();
    bool isBlankPage = aurUrl == "about:blank";

    if (isBlankPage) {
        showLoadingAnimation(true,ui->webEngineView_aur);

        QString wikiUrl = (QUrl(*aururl).isValid() && saveurl) ?
                              *aururl :
                              ((*lang == "en_US") ?
                                   "https://wiki.archlinux.org/title/General_recommendations" :
                                   "https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)");

        ui->webEngineView_aur->setUrl(QUrl(wikiUrl));
        ui->searchLineEdit->setText(wikiUrl);
        *aururl = ui->webEngineView_aur->url().toString();
        ui->action_35->setEnabled(false);
    } else {
        if (animation >= 1) {
            opacityEffect->setOpacity(1.0);
            ui->centralwidget->setGraphicsEffect(opacityEffect);
        }
        ui->searchLineEdit->setText(aurUrl);
    }

    ui->label1->setText(tr("Arch Linux - Wiki"));
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(15);
}

void MainWindow::on_action_8_triggered()
{
    if (page == 7) return;
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();

    process->start(packageCommands.value(pkg).value("query").at(0), QStringList() << packageCommands.value(pkg).value("query").at(1) << "ocs-url");
    if (process->waitForFinished()) {
        QString output = QString::fromUtf8(process->readAllStandardOutput());
        if (!output.contains("ocs-url")) {

            ui->action_2->trigger();

            ui->searchLineEdit->setText("ocs-url");
            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            QCoreApplication::postEvent(ui->searchLineEdit, event);
            sendNotification(tr("Ошибка"), tr("Установите пакет ocs-url для установки тем!"));
            return;
        }
    }

    mrpropper(7);

    bool isBlankPage = ui->webEngineView_custom->url().toString() == "about:blank";

    if (isBlankPage)
        showLoadingAnimation(true,ui->webEngineView_custom);

    ui->label1->setText(tr("Кастомизация"));
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);

    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(14);

    if (isBlankPage) {

        if (QUrl(*customurl).isValid() && saveurl)
            ui->webEngineView_custom->setUrl(QUrl(*customurl));
        else {
            if (*currentDesktop == "KDE")
                ui->webEngineView_custom->setUrl(QUrl("https://store.kde.org/browse/"));
            else if (*currentDesktop == "GNOME")
                ui->webEngineView_custom->setUrl(QUrl("https://www.pling.com/s/Gnome/browse/"));
            else if (*currentDesktop == "XFCE")
                ui->webEngineView_custom->setUrl(QUrl("https://www.pling.com/s/XFCE/browse/"));
            else if (*currentDesktop == "LXQt")
                ui->webEngineView_custom->setUrl(QUrl("https://store.kde.org/browse?cat=446"));
            else if (*currentDesktop == "Cinnamon")
                ui->webEngineView_custom->setUrl(QUrl("https://www.pling.com/s/Cinnamon/browse/"));
            else if (*currentDesktop == "MATE")
                ui->webEngineView_custom->setUrl(QUrl("https://www.pling.com/s/Mate/browse/"));
            else if (*currentDesktop == "Enlightenment")
                ui->webEngineView_custom->setUrl(QUrl("https://www.pling.com/s/Enlightenment/browse/"));
            else {
                sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
                return;
            }
            *customurl = ui->webEngineView_custom->url().toString();
            ui->action_35->setEnabled(false);
        }
    } else {
        if (animation >= 1) {
            opacityEffect->setOpacity(1.0);
            ui->centralwidget->setGraphicsEffect(opacityEffect);
        }
        ui->searchLineEdit->setText(ui->webEngineView_custom->url().toString());
    }
    showLoadingAnimationMini(false);
}

void MainWindow::on_push_vk_clicked()
{
    showLoadingAnimationMini(false);
    showLoadingAnimation(true,ui->webEngineView);
    mrpropper(8);
    ui->label1->setText(tr("Наше сообщество"));
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    if (animation >= 1) {
        opacityEffect->setOpacity(1.0);
        ui->centralwidget->setGraphicsEffect(opacityEffect);
    }
    ui->webEngineView->setUrl(QUrl("https://vk.com/linux2"));
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_action_12_triggered()
{
    mrpropper(9);
    ui->label1->setText(tr("Настройки приложения"));
    ui->action_28->setVisible(true);
    ui->action_timer->setVisible(true);
    ui->tabWidget->setCurrentIndex(7);
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_host_triggered()
{
    if (page == 10) return;
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->start(packageCommands.value(pkg).value("query").at(0), QStringList() << packageCommands.value(pkg).value("query").at(1) << "apache");

    if (process->waitForFinished()) {
        QString output = QString::fromUtf8(process->readAllStandardOutput());
        if (!output.contains("apache")) {

            ui->action_2->trigger();
            ui->searchLineEdit->setText("apache");

            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            QCoreApplication::postEvent(ui->searchLineEdit, event);

            sendNotification(tr("Ошибка"), tr("Установите пакет apache для управления веб-сервером!"));
            return;
        }
    }
    showLoadingAnimationMini(false);
    mrpropper(10);

    QString hostUrl = ui->webEngineView_host->url().toString();
    bool isBlankPage = hostUrl == "about:blank";

    if (isBlankPage)
        showLoadingAnimation(true,ui->webEngineView_host);

    ui->label1->setText(tr("Веб-сервер"));
    ui->action_5->setVisible(true);
    ui->action_restart->setVisible(true);
    ui->action_stop->setVisible(true);
    ui->action_catalog->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);

    ui->tabWidget->setCurrentIndex(13);

    if (isBlankPage) {
        ui->webEngineView_host->setUrl(QUrl("http://localhost"));
        ui->searchLineEdit->setText("http://localhost");
    } else {
        if (animation >= 1) {
            opacityEffect->setOpacity(1.0);
            ui->centralwidget->setGraphicsEffect(opacityEffect);
        }
        ui->searchLineEdit->setText(ui->webEngineView_host->url().toString());
    }
}

void MainWindow::on_action_game_triggered()
{
    if (page == 11) return;
    showLoadingAnimationMini(false);
    mrpropper(11);

    QString protonDBUrl = ui->webEngineView_game->url().toString();
    bool isBlankPage = protonDBUrl == "about:blank";

    if (isBlankPage)
        showLoadingAnimation(true,ui->webEngineView_game);

    ui->label1->setText(tr("Игровая совместимость - ProtonDB"));
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(0);

    if (isBlankPage) {
        if (QUrl(*gameurl).isValid() && saveurl)
            ui->webEngineView_game->setUrl(QUrl(*gameurl));
        else
        {
            ui->webEngineView_game->setUrl(QUrl("https://www.protondb.com/explore"));
            ui->searchLineEdit->setText("https://www.protondb.com/explore");
            *gameurl = ui->webEngineView_game->url().toString();
            ui->action_35->setEnabled(false);
        }
    } else {
        if (animation >= 1) {
            opacityEffect->setOpacity(1.0);
            ui->centralwidget->setGraphicsEffect(opacityEffect);
        }
        ui->searchLineEdit->setText(ui->webEngineView_game->url().toString());
    }
}

//12-13 заняты

void MainWindow::on_action_downgrade_triggered()
{
    mrpropper(14);
    ui->label1->setText(tr("Откат пакетов"));
    ui->action_nvidia->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск по архиву..."));
    ui->searchLineEdit->setVisible(true);
    if (nvidia >= 1)
        sendNotification(tr("Внимание"), tr("Откат NVIDIA пакетов отменен!"));
    nvidia = 0;
    ui->tabWidget->setCurrentIndex(12);
    showLoadingAnimationMini(false);
}

void MainWindow::on_action_nvidia_triggered()
{
    QString driverNames[] = {"nvidia-dkms", "nvidia-utils", "nvidia-settings", "libxnvctrl", "opencl-nvidia", "lib32-nvidia-utils", "lib32-opencl-nvidia"};
    int driverIndex = nvidia;

    if (driverIndex >= 1 && driverIndex <= 7)
    {
        checkForDowngrades(driverNames[driverIndex - 1]);
        ui->label1->setText(tr("Откат пакетов NVIDIA [%1/7]").arg(driverIndex));
    }
    else if (driverIndex == 8)
    {
        ui->label1->setText(tr("Откат пакетов NVIDIA"));
        ui->tabWidget->setCurrentIndex(6);
        ui->details_driver->setHtml(QString(tr("<b>Вы выбрали следующие пакеты:</b><br>"
                                               "- <b>%1</b><br>"
                                               "- <b>%2</b><br>"
                                               "- <b>%3</b><br>"
                                               "- <b>%4</b><br>"
                                               "- <b>%5</b><br>"
                                               "- <b>%6</b><br>"
                                               "- <b>%7</b><br>"
                                               "<br><span style=\"font-weight:700; color:#ad2429;\">Все версии должны совпадать, для избежания конфликтов зависимостей!</span><br><br>"
                                               "<b>Внимание! Если у вас не загрузится компьютер после этих манипуляций:</b><br>"
                                               "- Войдите в tty (CTRL+ALT+F2)<br>"
                                               "- Запустите консольную утилиту downgrade и установите свои предыдущие версии драйверов<br>"
                                               "- Перезагрузитесь")).arg(nvidiaDkmsName, nvidiaUtilsName, nvidiaSettingsName, libxnvctrlName, openclNvidiaName, lib32NvidiaUtilsName, lib32OpenclNvidiaName));
    }
    else
    {
        checkForDowngrades(driverNames[0]);
        ui->label1->setText(tr("Откат пакетов NVIDIA [1/7]"));
        nvidia = 1;
    }
}

void MainWindow::on_push_install_clicked()
{
    Terminal terminal = getTerminal();
    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        sendNotification(tr("Внимание"), tr("После отката пакетов NVIDIA, рекомендуется перезагрузка!"));
        on_push_back_clicked();
    });
    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("localinstall") << nvidiaDkms << nvidiaUtils << nvidiaSettings << libxnvctrl << openclNvidia << lib32NvidiaUtils << lib32OpenclNvidia);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->start();
}

void MainWindow::on_push_back_clicked()
{
    nvidia = 0;
    checkForDowngrades("steam");
    ui->label1->setText(tr("Откат пакетов"));
    ui->tabWidget->setCurrentIndex(12);
}
//---#####################################################################################################################################################
//--################################################################## БЫСТРЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################
void MainWindow::on_action_restart_triggered()
{
    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        sendNotification(tr("Веб-сервер"), tr("Веб-сервер перезапущен!"));
        showLoadingAnimation(false,ui->webEngineView_host);
    });
    currentProcess->setProgram("pkexec");
    currentProcess->setArguments(QStringList() << "sudo" << "systemctl" << "restart" << "httpd");
    currentProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentProcess->start();
}

void MainWindow::on_action_stop_triggered()
{
    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        sendNotification(tr("Веб-сервер"), tr("Веб-сервер остановлен!"));
        showLoadingAnimation(true,ui->webEngineView_host);
    });
    currentProcess->setProgram("pkexec");
    currentProcess->setArguments(QStringList() << "sudo" << "systemctl" << "stop" << "httpd");
    currentProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentProcess->start();
}

void MainWindow::on_action_catalog_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("/srv/http/"));
}

void MainWindow::on_push_repair_clicked()
{
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->start("zenity", QStringList() << "--file-selection" << tr("--title=Выберите архив") << "--file-filter=*.zip");
    if (process->waitForFinished() && process->exitCode() == 0) {
        QString archivePath = process->readAllStandardOutput().trimmed();
        restoreArchive(archivePath);
    }
}

void MainWindow::on_action_27_triggered()
{
    page = 111;
    ui->label1->setText(tr("Информация о системе"));
    ui->push_pacman->setVisible(false);
    ui->push_kde->setVisible(false);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск журналов и конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(false);
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::on_action_bench_triggered()
{
    page = 112;
    ui->label1->setText(tr("Бенчмарки"));
    ui->push_pacman->setVisible(false);
    ui->push_kde->setVisible(false);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск бенчмарков..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(true);
    ui->tabWidget->setCurrentIndex(10);
}

void MainWindow::on_action_system_triggered()
{
    ui->label1->setText(tr("Настройки системы"));
    ui->push_pacman->setVisible(true);
    ui->push_kde->setVisible(true);
    ui->searchLineEdit->setVisible(false);
    if (*currentDesktop == "KDE")
        ui->push_kde->setDisabled(false);

    ui->tabWidget->setCurrentIndex(8);
}

void MainWindow::on_action_repair_triggered()
{
    page = 113;
    ui->label1->setText(tr("Оптимизация"));
    ui->push_pacman->setVisible(false);
    ui->push_kde->setVisible(false);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(false);
    ui->tabWidget->setCurrentIndex(11);
}

void MainWindow::on_action_28_triggered()
{
    ui->label1->setText(tr("Настройки приложения"));
    ui->tabWidget->setCurrentIndex(7);
}

void MainWindow::on_action_timer_triggered()
{
    ui->label1->setText(tr("Настройки таймеров"));
    ui->tabWidget->setCurrentIndex(9);
}

void MainWindow::on_action_31_triggered()
{
    switch (page)
    {
        case 6:
            ui->webEngineView_aur->setUrl(QUrl((*lang == "en_US") ? "https://wiki.archlinux.org/title/General_recommendations" :
                                            "https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));
            break;
        case 7:
        {
            const QMap<QString, QString> desktopUrls{
                                                     {"KDE", "https://store.kde.org/browse/"},
                                                     {"GNOME", "https://www.pling.com/s/Gnome/browse/"},
                                                     {"XFCE", "https://www.pling.com/s/XFCE/browse/"},
                                                     {"LXQt", "https://store.kde.org/browse?cat=446"},
                                                     {"Cinnamon", "https://www.pling.com/s/Cinnamon/browse/"},
                                                     {"MATE", "https://www.pling.com/s/Mate/browse/"},
                                                     {"Enlightenment", "https://www.pling.com/s/Enlightenment/browse/"}};

            if (desktopUrls.contains(*currentDesktop))
                ui->webEngineView_custom->setUrl(QUrl(desktopUrls.value(*currentDesktop)));
            else
            {
                sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
                return;
            }
            break;
        }
        case 11:
            ui->webEngineView_game->setUrl(QUrl("https://www.protondb.com/explore"));
            break;
    }
}

void MainWindow::on_action_34_triggered()
{
    QListWidget* listWidget = nullptr;

    if (page == 2)
            listWidget = ui->list_aur;
    else if (page == 4)
            listWidget = ui->list_app;

    if (!listWidget || !listWidget->currentItem()) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для просмотра информации!"));
            return;
    }

    int currentRow = listWidget->currentRow();
    QString packageName = listWidget->item(currentRow)->text();

    QStringList command;

    command = packageCommands.value(pkg).value("show_info");
    command << packageName;

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->start(command.at(0), command.mid(1));
    process->waitForFinished();

    QString packageInfo = QString::fromUtf8(process->readAllStandardOutput());
    QRegularExpression urlRegex("URL\\s+:\\s+(\\S+)");
    QRegularExpressionMatch match = urlRegex.match(packageInfo);

    if (match.hasMatch()) {
            ui->action_35->setEnabled(true);
            ui->tabWidget->setCurrentIndex(3);
            QString url = match.captured(1);
            showLoadingAnimation(true,ui->webEngineView);
            ui->webEngineView->setUrl(QUrl(url));
    }
    else
            sendNotification(tr("Внимание"), tr("URL недоступен!"));
}

void MainWindow::on_action_35_triggered()
{
    if (page == 6)
    {
        ui->webEngineView_aur->back();
        return;
    }
    else if (page == 7)
    {
        ui->webEngineView_custom->back();
        return;
    }
    else if (page == 11)
    {
        ui->webEngineView_game->back();
        return;
    }
    else if (page == 8)
    {
        on_action_12_triggered();
        return;
    }

    if (animation >= 1)
    {
        opacityEffect->setOpacity(0.8);
        ui->centralwidget->setGraphicsEffect(opacityEffect);
    }

    ui->action_34->setVisible(true);
    ui->action_11->setVisible(page == 4);
    ui->action_35->setVisible(false);
    ui->action_updatelist->setVisible(true);

    if (page == 2) {
        ui->tabWidget->setCurrentIndex(1);
        ui->action_imgpkg->setVisible(true);
    }
    else if (page == 4)
        ui->tabWidget->setCurrentIndex(2);

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}

void MainWindow::on_action_32_triggered()
{
    if (page == 6)
        ui->webEngineView_aur->reload();
    else if (page == 7)
        ui->webEngineView_custom->reload();
    else if (page == 11)
        ui->webEngineView_game->reload();
}

void MainWindow::on_action_33_triggered()
{
    QClipboard *clipboard = QGuiApplication::clipboard();

    if (page == 6)
        clipboard->setText(ui->webEngineView_aur->url().toDisplayString());
    else if (page == 7)
        clipboard->setText(ui->webEngineView_custom->url().toDisplayString());
    else if (page == 11)
        clipboard->setText(ui->webEngineView_game->url().toDisplayString());

    sendNotification(tr("Буфер обмена"), tr("Ссылка успешно скопирована в буфер обмена!"));
}

void MainWindow::on_action_11_triggered()
{
    UpdateIcon();
    if (hasUpdates) {
        Terminal terminal = getTerminal();
        currentTerminalProcess = QSharedPointer<QProcess>::create(this);
        connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
            UpdateIcon();
        });
        currentTerminalProcess->setProgram(terminal.binary);
        currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("update"));
        currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
        currentTerminalProcess->start();
    } else
        sendNotification(tr("Обновление"), tr("Система в актуальном состоянии!"));
}

void MainWindow::on_action_5_triggered()
{
    if (page == 10) {
        currentProcess = QSharedPointer<QProcess>::create(this);
        connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
            sendNotification(tr("Веб-сервер"), tr("Веб-сервер запущен!"));
            showLoadingAnimation(false,ui->webEngineView_host);
        });
        currentProcess->setProgram("pkexec");
        currentProcess->setArguments(QStringList() << "sudo" << "systemctl" << "start" << "httpd");
        currentProcess->setProcessChannelMode(QProcess::MergedChannels);
        currentProcess->start();
    } else {
        QListWidget* listWidget = ui->list_app;

        if (listWidget->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
            return;
        }

        QString packageName = listWidget->currentItem()->text().split(" ").first();

        QString desktopFilePath;

        QProcess process;
        process.start(packageCommands.value(pkg).value("list_files").at(0), {packageCommands.value(pkg).value("list_files").at(1), packageName});
        process.waitForFinished(-1);

        QString output = process.readAllStandardOutput();
        for (const QString& line : output.split('\n')) {
            if (line.contains(packageName) && line.contains(".desktop")) {
                desktopFilePath = line.split(' ').last();
                break;
            }
        }

        if (!desktopFilePath.isEmpty()) {
            QString desktopFileName = QFileInfo(desktopFilePath).fileName();
            QProcess::startDetached("gtk-launch", {desktopFileName});
        } else
            sendNotification(tr("Ошибка"), tr("Файл .desktop не найден для пакета ") + packageName);
    }
}

void MainWindow::on_action_6_triggered()
{
    QListWidget* listWidget = nullptr;

    if (page == 2)
        listWidget = ui->list_aur;
    else
        listWidget = ui->list_app;

    if (listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
        return;
    }

    QString packageName = listWidget->item(listWidget->currentRow())->text();
    Terminal terminal = getTerminal();

    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        loadContentInstall();
    });
    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("remove") << packageName);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->start();
}

void MainWindow::on_action_4_triggered()
{
    if (hasUpdates && updinst == 2 && page == 2) {
        sendNotification(tr("Внимание"), tr("Вначале требуется обновить систему до актуального состояния! Это поможет предотвратить конфликт зависимостей и избежать кучи других проблем!"));
        return;
    }

    QListWidget* listWidget = nullptr;

    if (page == 2)
        listWidget = ui->list_aur;
    else
        listWidget = ui->list_app;

    if (listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
        return;
    }

    QString packageName = listWidget->item(listWidget->currentRow())->text();
    Terminal terminal = getTerminal();

    if (clearinstall && (pkg == 0 || pkg == 1))
        removeDirectory(QDir::homePath() + "/.cache/" + ((pkg == 0) ? "yay/" : "paru/clone/") + packageName);

    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        loadContentInstall();
    });
    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("install") << packageName);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->start();
}

void MainWindow::on_action_30_triggered()
{
    if (hasUpdates && updinst == 2 && page == 2) {
        sendNotification(tr("Внимание"), tr("Вначале требуется обновить систему до актуального состояния! Это поможет предотвратить конфликт зависимостей и избежать кучи других проблем!"));
        return;
    }

    if (ui->list_aur->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
        return;
    }

    QString packageName = ui->list_aur->item(ui->list_aur->currentRow())->text();
    Terminal terminal = getTerminal();
    QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "bash" << mainDir + "sh/PKGBUILD.sh" << *lang << helper << packageName);
}

void MainWindow::on_push_grub_clicked()
{
    QString filename = "/etc/default/grub";
    QString grubContent = ui->line_grub->text().trimmed();
    QString timeout = ui->spin_grub->value() > 0 ? QString::number(ui->spin_grub->value()) : "5";

    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        sendNotification(tr("GRUB изменен"), tr("Изменения GRUB вступят в силу после перезагрузки."));
    });
    currentProcess->setProgram("pkexec");
    QStringList arguments;
    arguments << "bash" << "-c" << "sed -i 's/^GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT=\"" + grubContent + "\"/; s/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=\"" + timeout + "\"/' " + filename + " && sudo grub-mkconfig -o /boot/grub/grub.cfg";
    currentProcess->setArguments(arguments);
    currentProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentProcess->start();
}

void MainWindow::on_action_addsh_triggered()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Добавить скрипт"));

    QLabel* nameLabel = new QLabel(tr("Имя файла:"), dialog);
    QLineEdit* nameLineEdit = new QLineEdit(dialog);
    QLabel* nameRuLabel = new QLabel(tr("Название скрипта:"), dialog);
    QLineEdit* nameRuLineEdit = new QLineEdit(dialog);
    QLabel* msgRuLabel = new QLabel(tr("Описание скрипта:"), dialog);
    QLineEdit* msgRuLineEdit = new QLineEdit(dialog);
    QLabel* scriptLabel = new QLabel(tr("Тело скрипта:"), dialog);
    QTextEdit* scriptTextEdit = new QTextEdit(dialog);
    QPushButton* saveButton = new QPushButton(tr("Сохранить"), dialog);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    dialog->setStyleSheet("QWidget{background-color:#2d2b79;} QLineEdit,QTextEdit{background-color:#21205b;padding:10px;border-radius:10px;} QLabel{color:#fff;font-size:10pt;}QPushButton{border-radius:10px;padding:5px 20px;background-color:#916ee4;color:#fff;}");

    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(nameRuLabel);
    layout->addWidget(nameRuLineEdit);
    layout->addWidget(msgRuLabel);
    layout->addWidget(msgRuLineEdit);
    layout->addWidget(scriptLabel);
    layout->addWidget(scriptTextEdit);
    layout->addWidget(saveButton);
    dialog->setLayout(layout);

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QString fileName = nameLineEdit->text() + ".sh";
        QString filePath = mainDir + "sh/" + fileName;

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << "#name_" + *lang + " " << nameRuLineEdit->text() << "\n";
            stream << "#msg_ " + *lang + " " << msgRuLineEdit->text() << "\n";
            stream << "#icon 31" << "\n";
            stream << "#!/bin/bash" << "\n";
            stream << scriptTextEdit->toPlainText();
            file.close();

            loadScripts(mainDir + "sh/", ui->list_sh);
            sendNotification(tr("Добавление"), tr("Скрипт успешно добавлен!"));
        }

        dialog->close();
        dialog->deleteLater();
    });
    dialog->exec();
}

void MainWindow::on_action_rmsh_triggered()
{
    if (ui->list_sh->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите скрипт из списка для удаления!"));
        return;
    }

    QListWidgetItem* selectedItem = ui->list_sh->currentItem();
    QString itemContent = selectedItem->text();
    QMap<QString, QString> scriptMap;
    QDir dir(mainDir + "sh/");
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);
    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();
        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang))
                {
                    QString itemName = line.mid(12).trimmed();
                    scriptMap[itemName] = filePath;
                    break;
                }
            }
            scriptFile.close();
        }
    }

    QString filePath = scriptMap.value(itemContent);
    if (!filePath.isEmpty() && QFile::remove(filePath))
    {
        delete ui->list_sh->takeItem(ui->list_sh->row(selectedItem));
        sendNotification(tr("Удаление"), tr("Скрипт успешно удален!"));
    }
}

QString MainWindow::getScriptContent(const QString& filePath)
{
    QFile scriptFile(filePath);
    if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream scriptStream(&scriptFile);
        QString content = scriptStream.readAll();
        scriptFile.close();
        return content;
    }
    return QString();
}

void MainWindow::on_action_editsh_triggered()
{
    if (ui->list_sh->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите скрипт из списка для изменения!"));
        return;
    }
    QListWidgetItem* selectedItem = ui->list_sh->currentItem();
    QString itemContent = selectedItem->text();
    QDir dir(mainDir + "sh/");
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);
    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();
        QString fileName = fileInfo.fileName();
        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang))
                {
                    QString itemName = line.mid(12).trimmed();
                    if (itemName == itemContent)
                    {
                        scriptFile.close();
                        QDialog* editDialog = new QDialog(this);
                        editDialog->setWindowTitle(tr("Редактировать скрипт"));
                        editDialog->resize(500, 300);
                        QLabel* nameLabel = new QLabel(tr("Имя файла:"));
                        QLineEdit* nameEdit = new QLineEdit(fileName);
                        QLabel* scriptLabel = new QLabel(tr("Тело скрипта:"));
                        QTextEdit* scriptEdit = new QTextEdit();
                        QPushButton* saveButton = new QPushButton(tr("Сохранить"));
                        scriptEdit->setPlainText(getScriptContent(filePath));
                        QVBoxLayout* layout = new QVBoxLayout(editDialog);
                        editDialog->setStyleSheet("QWidget{background-color:#2d2b79;} QLineEdit,QTextEdit{background-color:#21205b;padding:10px;border-radius:10px;} QLabel{color:#fff;font-size:10pt;}QPushButton{border-radius:10px;padding:5px 20px;background-color:#916ee4;color:#fff;}");
                        layout->addWidget(nameLabel);
                        layout->addWidget(nameEdit);
                        layout->addWidget(scriptLabel);
                        layout->addWidget(scriptEdit);
                        layout->addWidget(saveButton);
                        editDialog->setLayout(layout);
                        connect(saveButton, &QPushButton::clicked, this, [=]() mutable {
                            QString newFileName = nameEdit->text();
                            QString newScriptContent = scriptEdit->toPlainText();
                            QString newFilePath = mainDir + "sh/" + newFileName;
                            QFile newScriptFile(newFilePath);
                            if (newScriptFile.open(QIODevice::WriteOnly | QIODevice::Text))
                            {
                                QTextStream newScriptStream(&newScriptFile);
                                newScriptStream << newScriptContent;
                                newScriptFile.close();
                            }
                            selectedItem->setText(newFileName);
                            editDialog->close();
                            loadScripts(mainDir + "sh/", ui->list_sh);
                            sendNotification(tr("Сохранение"), tr("Скрипт успешно изменен!"));
                        });
                        editDialog->exec();
                        return;
                    }
                    break;
                }
            }
            scriptFile.close();
        }
    }
}
//---#####################################################################################################################################################
//--################################################################# ОСНОВНЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################
void MainWindow::setupListContextMenu()
{
    connect(ui->list_aur, &QListWidget::customContextMenuRequested, this, &MainWindow::showListContextMenu);
    connect(ui->list_app, &QListWidget::customContextMenuRequested, this, &MainWindow::showListContextMenu);
}

void MainWindow::showListContextMenu(const QPoint& pos)
{
    QListWidget* listWidget = nullptr;

    if (page == 2) {
        listWidget = ui->list_aur;
    } else if (page == 4) {
        listWidget = ui->list_app;
    }

    if (!listWidget)
        return;

    QListWidgetItem* selectedItem = listWidget->itemAt(pos);
    if (!selectedItem)
        return;

    QMenu contextMenu(this);
    QAction action1(QIcon(":/img/15.png"), tr("Установить"), this);
    QAction action2(QIcon(":/img/27.png"), tr("Изменить PKGBUILD и установить"), this);
    QAction action3(QIcon(":/img/14.png"), tr("Удалить"), this);
    QAction action4(QIcon(":/img/34.png"), tr("Информация о пакете"), this);
    QAction action5(QIcon(":/img/13.png"), tr("Запустить"), this);
    QAction action6(QIcon(":/img/15.png"), tr("Обновить версию"), this);

    connect(&action1, &QAction::triggered, this, &MainWindow::on_action_4_triggered);
    connect(&action2, &QAction::triggered, this, &MainWindow::on_action_30_triggered);
    connect(&action3, &QAction::triggered, this, &MainWindow::on_action_6_triggered);
    connect(&action4, &QAction::triggered, this, &MainWindow::on_action_34_triggered);
    connect(&action5, &QAction::triggered, this, &MainWindow::on_action_5_triggered);
    connect(&action6, &QAction::triggered, this, &MainWindow::on_action_4_triggered);


    if (page == 2) {
        contextMenu.addAction(&action1);
        contextMenu.addAction(&action2);
    } else if (page == 4) {
        contextMenu.addAction(&action5);
        contextMenu.addAction(&action6);
        contextMenu.addAction(&action3);
    }
    contextMenu.addAction(&action4);
    contextMenu.exec(listWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::createSearchBar()
{
    // Комплитер
    completerModel = new QStandardItemModel(this);
    completer = new QCompleter(completerModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->searchLineEdit->setCompleter(completer);

    if (QAbstractItemView* popup = completer->popup()) {
        popup->setStyleSheet("QAbstractItemView {background-color:#1f1f1f;padding: 3px 5px; border: 0;border-radius: 5px;}"
                             "QAbstractItemView::item {background-color: #1e1e1e; padding:5px; margin:3px 0; border-radius: 5px; color: white; outline: none;}"
                             "QAbstractItemView::item:hover {background-color: #242424; border-radius: 5px;}"
                             "QScrollBar:vertical, QScrollBar:horizontal { width: 8px; height: 8px; background-color: #656766; border-radius: 4px; }"
                             "QScrollBar::handle { padding: 60px 0; background-color: #916ee4; border-radius: 4px; }"
                             "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"
                             "QScrollBar::add-line, QScrollBar::sub-line { width: 0px; }");
    }

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [=](const QString& searchText) {
        searchTextChanged(searchText);
        updateCompleterModel();
    });

    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, [this] {
        search(ui->searchLineEdit->text());
    });

    ui->searchLineEdit->installEventFilter(this);
 }

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if ((page == 2 || page == 4) && obj == ui->searchLineEdit && event->type() == QEvent::KeyPress) {
       QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
       if (keyEvent->key() == Qt::Key_Tab) {
            completerModel->clear();
            handleServerResponseSearch(ui->searchLineEdit->text());
            return true;
       }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::handleServerResponseSearch(const QString& reply)
{
    if (currentProcess && currentProcess->state() == QProcess::Running) {
        currentProcess->disconnect();
        currentProcess->kill();
        currentProcess->waitForFinished();
    }
    completerModel->clear();

    QString searchCommand = packageCommands.value(pkg).value("search").at(0);
    QStringList arguments = {packageCommands.value(pkg).value("search").at(1), reply};

    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), &QProcess::readyReadStandardOutput, this, &MainWindow::onCurrentProcessReadyReadSearch);
    currentProcess->start(searchCommand, arguments);
}

void MainWindow::onCurrentProcessReadyReadSearch()
{
    completerModel->clear();

    while (currentProcess->canReadLine()) {
        QByteArray line = currentProcess->readLine();
        QString lineString = QString::fromUtf8(line).trimmed();

        QRegularExpressionMatch match = QRegularExpression(*repo).match(lineString);
        if (match.hasMatch()) {
            createAndAddListItemSearch(match.captured(1));
        }
    }
    completer->complete();
}

void MainWindow::createAndAddListItemSearch(const QString& packageName)
{
    static const QRegularExpression regex("(\\w+)/\\S+");
    QRegularExpressionMatch match = regex.match(packageName);

    if (match.hasMatch()) {
        QString repoName = match.captured(1);
        QString iconPath = QFile::exists(":/img/" + repoName + ".png") ? ":/img/" + repoName + ".png" : ":/img/pacman.png";

        QString packageNameWithoutPrefix = packageName;
        QString prefixToRemove = repoName + "/";

        if (!prefixToRemove.isEmpty() && packageNameWithoutPrefix.startsWith(prefixToRemove)) {
            packageNameWithoutPrefix.remove(0, prefixToRemove.length());
        }

        QStandardItem* item = new QStandardItem(QIcon(iconPath), packageNameWithoutPrefix);
        completerModel->appendRow(item);
    }
}

void MainWindow::search(const QString& searchText)
{
    if (page == 2)
        handleServerResponse(searchText);
    else if (page == 6)
        ui->webEngineView_aur->setUrl(QUrl(searchText));
    else if (page == 7)
        ui->webEngineView_custom->setUrl(QUrl(searchText));
    else if (page == 10)
        ui->webEngineView_host->setUrl(QUrl(searchText));
    else if (page == 11)
        ui->webEngineView_game->setUrl(QUrl(searchText));
    else if (page == 14) {
        if (nvidia == 0)
            checkForDowngrades(searchText);
        else
            sendNotification(tr("Ошибка"), tr("При установке драйверов, поиск не доступен!"));
    }
}

void MainWindow::searchTextChanged(const QString& searchText)
{
    switch (page) {
        case 2: searchAndScroll(ui->list_aur, searchText); break;
        case 4: searchAndScroll(ui->list_app, searchText); break;
        case 3: searchAndScroll(ui->list_sh, searchText); break;
        case 14: searchAndScroll(ui->list_downgrade, searchText); break;
        case 111: searchAndScroll(ui->list_journal, searchText); searchAndScroll(ui->list_cfg, searchText); break;
        case 112: searchAndScroll(ui->list_bench, searchText); break;
        case 113: searchAndScroll(ui->list_repair, searchText); break;
        default: break;
    }
}

void MainWindow::updateCompleterModel()
{
    completerModel->clear();
    if (page == 2 || page == 4)
        completer->complete();
}

//not
void MainWindow::searchAndScroll(QAbstractItemView* view, const QString& text)
{
    if (QListWidget* listWidget = qobject_cast<QListWidget*>(view))
    {
        QList<QListWidgetItem*> matchingItems = listWidget->findItems(text, Qt::MatchContains);
        if (!matchingItems.isEmpty()) {
            listWidget->setCurrentItem(matchingItems.first());
            listWidget->scrollToItem(matchingItems.first(), QAbstractItemView::PositionAtCenter);
        }
    }
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //-##################################################################################
    //-######################### ПОДКЛЮЧЕНИЕ КОНФИГУРАЦИЙ ###############################
    //-##################################################################################
    setupListContextMenu();
    createSearchBar();

    AnimationBackground();
    loadSettings();

    checkVersionAndClear();
    UpdateIcon();

    list = 0;
    loadContent(0, true);

    loadContentInstall();
    loadFolders();
    loadingListWidget();

    detailsAURdefault = ui->details_aur->toHtml();

    ui->check_trayon->setChecked(trayon);
    ui->check_autostart->setChecked(autostart);
    ui->check_repair->setChecked(repair);
    ui->check_animload->setChecked(animload);
    ui->check_updateinstall->setChecked(updinst);
    ui->check_clearinstall->setChecked(clearinstall);
    ui->combo_mainpage->setCurrentIndex(mainpage);
    ui->combo_helper->setCurrentIndex(pkg);
    ui->combo_animload->setCurrentIndex(animloadpage);
    ui->combo_theme->setCurrentIndex(animation);
    ui->check_cacheremove->setChecked(cacheremove);
    ui->check_saveurl->setChecked(saveurl);

    ui->time_update->setTime(timeupdate);
    ui->time_timeout->setTime(timeout);

    ui->time_tea->setTime(timetea);
    ui->time_work->setTime(timework);
    ui->line_tea->setText(*teatext);
    ui->line_work->setText(*worktext);

    ui->dial_volnotify->setValue(volumenotify);

    if (*lang == "ru_RU")
        ui->combo_lang->setCurrentIndex(0);
    else if (*lang == "en_US")
        ui->combo_lang->setCurrentIndex(1);

    helper = (pkg == 0) ? "yay" : "paru";

    // Создание QNetworkAccessManager
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onReplyFinished);

    checkForDowngrades("steam");
}

//not
void MainWindow::checkVersionAndClear() {
    QSettings settings(filePath, QSettings::IniFormat);
    QString storedVersion = settings.value("Version").toString();
    QString storedLanguage = settings.value("Language").toString();

    if (storedVersion.isEmpty() || storedVersion != currentVersion) {
        settings.setValue("Version", currentVersion);
        settings.setValue("Language", storedLanguage);
        settings.sync();

        QStringList excludedFolders = {"clear", "journals", "cfg", "bench", "other", "sh"};
        QDir baseDir(mainDir);

        QStringList subDirs = baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : subDirs) {
            if (!excludedFolders.contains(subDir)) {
                removeDirectory(baseDir.absoluteFilePath(subDir));
            }
        }

        removeScripts(shResourcePaths, mainDir + "sh/");
        removeDirectory(mainDir + "clear/");
        removeDirectory(mainDir + "journals/");
        removeDirectory(mainDir + "cfg/");
        removeDirectory(mainDir + "bench/");
        removeDirectory(mainDir + "other/");
        sendNotification(tr("Обновление kLaus"), tr("Версия kLaus поменялась, конфигурация сброшена!"));
    }
}

void MainWindow::removeScripts(const QStringList& resourcePaths, const QString& baseDir)
{
    QDir().mkpath(baseDir);
    for (const QString& path : resourcePaths)
    {
        QFile::remove(baseDir + QFileInfo(path).fileName());
    }
}

void MainWindow::removeDirectory(const QString& dirPath)
{
    QDir(dirPath).removeRecursively();
}

void MainWindow::saveScripts(const QStringList& resourcePaths, const QString& baseDir)
{
    QDir().mkpath(baseDir);
    for (const QString& path : resourcePaths)
    {
        QFile::copy(path, baseDir + QFileInfo(path).fileName());
    }
}

MainWindow::~MainWindow()
{
    if (cacheremove == 2)
        removeDirectory(mainDir + "cache/");

    iconMap.clear();

    //delete videoItem;
    //delete graphicsView;
    //delete player;
    //delete videoWidget;
    delete scene;
    delete opacityEffect;

    delete previousAction;
    //delete actionLoad;
    delete orphanButton;
    delete cacheButtonHelper;
    delete cacheButtonPacman;

    snapPackageNames.clear();
    shResourcePaths.clear();
    clearResourcePaths.clear();
    journalsResourcePaths.clear();
    cfgResourcePaths.clear();
    benchResourcePaths.clear();
    endingsToRemove.clear();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayon == 2) {
        qApp->quit();
    } else {
        hide();
        event->ignore();
    }
}

void MainWindow::loadSound(int soundIndex)
{
    QSoundEffect* beep = new QSoundEffect(this);
    const QString soundPath = (soundIndex == 1) ? "qrc:/media/message.wav" : "qrc:/media/sound.wav";
    beep->setSource(QUrl(soundPath));

    if (soundIndex == 1) {
        float volnotify = static_cast<float>(volumenotify) / 100.0f;
        volnotify = static_cast<float>(static_cast<int>(volnotify * 10.0f)) / 10.0f;
        beep->setVolume(volnotify);
    }
    beep->play();
}

//not
void MainWindow::loadSettings()
{
    // Создание эффекта прозрачности
    opacityEffect = new QGraphicsOpacityEffect;
    //-##################################################################################
    //-########################## НАСТРОЕННЫЕ ПЕРЕМЕННЫЕ ################################
    //-##################################################################################
    previousAction = ui->action_2; //[заглушка]

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
    //-##################################################################################
    //-############################## ОСНОВНАЯ ЧАСТЬ ####################################
    //-##################################################################################
    pkg = settings.value("PackageManager", 0).toInt();
    mainpage = settings.value("MainPage", 0).toInt();
    animloadpage = settings.value("AnimLoadPage", 0).toInt();
    helpercache = settings.value("HelperCache", 0).toInt();
    trayon = settings.value("TrayOn", 0).toInt();
    autostart = settings.value("Autostart", 0).toInt();
    repair = settings.value("RepairBackup", 2).toInt();
    animload = settings.value("AnimLoad", 2).toInt();
    updinst = settings.value("UpdateInstall", 2).toInt();
    clearinstall = settings.value("ClearInstall", 2).toInt();
    cacheremove = settings.value("CacheRemove", 2).toInt();
    volumenotify = settings.value("VolumeNotify", 30).toInt();
    animation = settings.value("Animation", 0).toInt();
    saveurl = settings.value("SaveURL", 2).toInt();

    lang = QSharedPointer<QString>::create(settings.value("Language").toString());
    teatext = QSharedPointer<QString>::create(settings.value("TeaText").toString());
    worktext = QSharedPointer<QString>::create(settings.value("WorkText").toString());
    animationname = QSharedPointer<QString>::create(settings.value("AnimationName").toString());

    gameurl = QSharedPointer<QString>::create(settings.value("GameURL").toString());
    customurl = QSharedPointer<QString>::create(settings.value("CustomURL").toString());
    aururl = QSharedPointer<QString>::create(settings.value("AurURL").toString());

    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");
    timeout = QTime::fromString(settings.value("TimeOut", "00:00:10").toString(), "HH:mm:ss");
    timetea = QTime::fromString(settings.value("TimeTea").toString(), "HH:mm");
    timework = QTime::fromString(settings.value("TimeWork").toString(), "HH:mm");

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    currentDesktop = QSharedPointer<QString>::create(environment.value("XDG_CURRENT_DESKTOP"));

    ui->toolBar_2->setFixedWidth(50);

    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBar_2->setContextMenuPolicy(Qt::PreventContextMenu);

    switch(mainpage) {
        case 0:
            on_action_2_triggered();
            ui->action_2->setChecked(true);
            previousAction = ui->action_2;
            break;
        case 1:
            on_action_7_triggered();
            ui->action_7->setChecked(true);
            previousAction = ui->action_7;
            break;
        case 2:
            on_action_downgrade_triggered();
            ui->action_downgrade->setChecked(true);
            previousAction = ui->action_downgrade;
            break;
        case 3:
            on_action_9_triggered();
            ui->action_9->setChecked(true);
            previousAction = ui->action_9;
            break;
        case 4:
            on_action_17_triggered();
            ui->action_17->setChecked(true);
            previousAction = ui->action_17;
            break;
        case 5:
            on_action_12_triggered();
            ui->action_12->setChecked(true);
            previousAction = ui->action_12;
            break;
    }
    //-##################################################################################
    //-############################## СИГНАЛЫ И СЛОТЫ ###################################
    //-##################################################################################
    connect(ui->time_update, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(ui->list_aur, &QListWidget::itemClicked, this, &MainWindow::onListAurItemClicked);
    connect(ui->list_app, &QListWidget::itemClicked, this, &MainWindow::onListAurItemClicked);
    connect(ui->list_downgrade, &QListWidget::itemDoubleClicked, this, &MainWindow::onListDowngradeItemDoubleClicked);

    connect(ui->webEngineView, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        QString urlString = url.toString();
        ui->searchLineEdit->setText(urlString);
    });
    connect(ui->webEngineView_aur, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        if(ui->webEngineView_aur->url().toString() == *aururl)
            ui->action_35->setEnabled(false);
        else
            ui->action_35->setEnabled(true);

        QString urlString = url.toString();
        ui->searchLineEdit->setText(urlString);
        settings.setValue("AurURL", urlString);
    });
    connect(ui->webEngineView_custom, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        if(ui->webEngineView_custom->url().toString() == *customurl)
            ui->action_35->setEnabled(false);
        else
            ui->action_35->setEnabled(true);

        QString urlString = url.toString();
        ui->searchLineEdit->setText(urlString);
        settings.setValue("CustomURL", urlString);
    });
    connect(ui->webEngineView_game, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        if(ui->webEngineView_game->url().toString() == *gameurl)
            ui->action_35->setEnabled(false);
        else
            ui->action_35->setEnabled(true);

        QString urlString = url.toString();
        ui->searchLineEdit->setText(urlString);
        settings.setValue("GameURL", urlString);
    });
    connect(ui->webEngineView_host, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        QString urlString = url.toString();
        ui->searchLineEdit->setText(urlString);
    });

    //-##################################################################################
    //-############################### ЗАНЯТОЕ МЕСТО ####################################
    //-##################################################################################
    QProgressBar* progressBar = new QProgressBar();
    progressBar->setFixedSize(50, 30);
    QStorageInfo storageInfo = QStorageInfo::root();
    qint64 freeBytes = storageInfo.bytesAvailable();
    int usedPercentage = 100 - static_cast<int>((freeBytes * 100) / storageInfo.bytesTotal());
    progressBar->setValue(usedPercentage);
    progressBar->setFormat("");
    QLabel* label = new QLabel(tr("свободно %1 ГиБ").arg(QString::number(freeBytes / (1024.0 * 1024.0 * 1024.0), 'f', 2)));

    QWidget* containerWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(containerWidget);
    layout->addStretch();
    layout->addWidget(progressBar);
    layout->addWidget(label);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->statusBar->addPermanentWidget(containerWidget, 1);

    QColor customColor(42, 40, 112);
    QPalette palette = progressBar->palette();
    palette.setColor(QPalette::Highlight, customColor);
    progressBar->setPalette(palette);

    QHBoxLayout* statusBarLayout = qobject_cast<QHBoxLayout*>(ui->statusBar->layout());
    if (statusBarLayout)
        statusBarLayout->setAlignment(Qt::AlignRight);

    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    //-##################################################################################
    //-############################# ВЕБ ЧАСТЬ / СКРИПТЫ ################################
    //-##################################################################################
    QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
    profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);

    connect(ui->webEngineView_custom->page(), &QWebEnginePage::loadStarted, this, [=]() {
        QFile scriptFile(":/loading.browser.js");
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&scriptFile);
            QString script = stream.readAll();
            ui->webEngineView_custom->page()->runJavaScript(script);
        }
    });
    connect(ui->webEngineView_aur->page(), &QWebEnginePage::loadStarted, this, [=]() {
        QFile scriptFile(":/loading.browser.js");
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&scriptFile);
            QString script = stream.readAll();
            ui->webEngineView_aur->page()->runJavaScript(script);
        }
    });

    connect(ui->webEngineView_game->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success)
            showLoadingAnimation(false,ui->webEngineView_game);
    });
    connect(ui->webEngineView_host->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success)
            showLoadingAnimation(false,ui->webEngineView_host);
        else {
            if (page == 10) {
                ui->webEngineView_host->reload();
                errorShown = true;
                return;
            }
        }
    });
    connect(ui->webEngineView_custom->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success)
            showLoadingAnimation(false,ui->webEngineView_custom);
    });
    connect(ui->webEngineView_aur->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success)
            showLoadingAnimation(false,ui->webEngineView_aur);
    });
    connect(ui->webEngineView->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (isFirstLoad) {
            isFirstLoad = false;
            return;
        }
        if (success)
            showLoadingAnimation(false,ui->webEngineView);
        else {
            if ((!errorShown && page == 2) || (!errorShown && page == 4)) {
                sendNotification(tr("Ошибка"), tr("Страница не найдена (ошибка 404)"));
                errorShown = true;  // Устанавливаем флаг, что уведомление было показано
                showLoadingAnimation(false,ui->webEngineView);
            }
        }
    });

    ui->list_aur->setSelectionBehavior(QAbstractItemView::SelectRows);
    //-##################################################################################
    //-########################### ГРОМКОСТЬ УВЕДОМЛЕНИЙ ################################
    //-##################################################################################
    QString labelvolnotify = QString(tr("Звук уведомлений: %1/100")).arg(volumenotify);
    ui->label_volnotify->setText(labelvolnotify);
    //-##################################################################################
    //-############################# ТАЙМЕР ИКОНКИ ТРЕЯ #################################
    //-##################################################################################
    updateIconTimer = QSharedPointer<QTimer>::create(this);
    updateIconTimer->setInterval(3600000); // 1 час
    updateIconTimer->setSingleShot(false);
    connect(updateIconTimer.data(), &QTimer::timeout, this, &MainWindow::UpdateIcon);
    updateIconTimer->start();
    //-##################################################################################
    //-############################## ТАЙМЕР COFFETIME ##################################
    //-##################################################################################
    teaTimer = QSharedPointer<QTimer>::create(this);
    connect(teaTimer.data(), &QTimer::timeout, this, &MainWindow::TeaTimer);
    workTimer = QSharedPointer<QTimer>::create(this);
    connect(workTimer.data(), &QTimer::timeout, this, &MainWindow::WorkTimer);
    //-##################################################################################
    //-################################# ИКОНКИ ТРЕЯ ####################################
    //-##################################################################################
    trayIcon.setIcon(QIcon(":/img/2.png"));
    trayIcon.setToolTip("kLaus ;)");
    trayIcon.show();

    QMenu *trayMenu = new QMenu();

    QAction *action_11 = new QAction(tr("Обновить систему"), trayMenu);
    action_11->setIcon(QIcon(":/img/16.png"));
    trayMenu->addAction(action_11);

    QAction *action_2 = new QAction(tr("Каталог пакетов"), trayMenu);
    action_2->setIcon(QIcon(":/img/2.png"));
    trayMenu->addAction(action_2);

    QAction *action_7 = new QAction(tr("Установленные пакеты"), trayMenu);
    action_7->setIcon(QIcon(":/img/5.png"));
    trayMenu->addAction(action_7);

    QAction *action_downgrade = new QAction(tr("Откат пакетов"), trayMenu);
    action_downgrade->setIcon(QIcon(":/img/55.png"));
    trayMenu->addAction(action_downgrade);

    QAction *action_9 = new QAction(tr("Информация о системе"), trayMenu);
    action_9->setIcon(QIcon(":/img/7.png"));
    trayMenu->addAction(action_9);

    QAction *action_17 = new QAction(tr("Полезные скрипты"), trayMenu);
    action_17->setIcon(QIcon(":/img/19.png"));
    trayMenu->addAction(action_17);

    QAction *action_12 = new QAction(tr("Настройки"), trayMenu);
    action_12->setIcon(QIcon(":/img/9.png"));
    trayMenu->addAction(action_12);

    QAction *exitAction = new QAction(tr("Выход"), trayMenu);
    exitAction->setIcon(QIcon(":/img/18.png"));
    trayMenu->addAction(exitAction);

    trayIcon.setContextMenu(trayMenu);
    connect(action_11, &QAction::triggered, this, &MainWindow::on_action_11_triggered);
    connect(action_2, &QAction::triggered, this, &MainWindow::on_action_2_triggered);
    connect(action_7, &QAction::triggered, this, &MainWindow::on_action_7_triggered);
    connect(action_downgrade, &QAction::triggered, this, &MainWindow::on_action_downgrade_triggered);
    connect(action_9, &QAction::triggered, this, &MainWindow::on_action_9_triggered);
    connect(action_17, &QAction::triggered, this, &MainWindow::on_action_17_triggered);
    connect(action_12, &QAction::triggered, this, &MainWindow::on_action_12_triggered);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    connect(&trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    //-##################################################################################
    //-############################## ПЕРЕКЛЮЧЕНИЕ МЕНЮ #################################
    //-##################################################################################
    connect(ui->toolBar, &QToolBar::actionTriggered, this, [=](QAction* action) {
        if (previousAction)
            previousAction->setChecked(false);

        action->setChecked(true);
        previousAction = action;
    });
    //-##################################################################################
    //-##################################### GRUB #######################################
    //-##################################################################################
    QFile grub("/etc/default/grub");

    if (!grub.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->push_grub->setDisabled(true);
        ui->line_grub->setDisabled(true);
        ui->spin_grub->setDisabled(true);
        ui->line_grub->setText(tr("GRUB не установлен"));
    } else {
        QTextStream in(&grub);
        QString grubContent;
        QString timeoutStr;

        static QRegularExpression timeoutRegex("^GRUB_TIMEOUT=['\"]?(\\d+)['\"]?$");
        static QRegularExpression grubCmdlineRegex("^GRUB_CMDLINE_LINUX_DEFAULT=['\"]?(.*)['\"]?$");

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("GRUB_TIMEOUT=")) {
                QRegularExpressionMatch match = timeoutRegex.match(line);
                if (match.hasMatch()) {
                    timeoutStr = match.captured(1).trimmed();
                    static const QRegularExpression quotationRegex("[\"']");
                    timeoutStr.remove(quotationRegex);
                }
                continue;
            }
            if (line.startsWith("GRUB_CMDLINE_LINUX_DEFAULT=")) {
                QRegularExpressionMatch match = grubCmdlineRegex.match(line);
                if (match.hasMatch()) {
                    grubContent = match.captured(1).trimmed();
                    static const QRegularExpression quotationRegex("[\"']");
                    grubContent.remove(quotationRegex);
                }
                else {
                    grubCmdlineRegex.setPattern("^GRUB_CMDLINE_LINUX_DEFAULT=['\"]?(.*)['\"]?$");
                    match = grubCmdlineRegex.match(line);
                    if (match.hasMatch()) {
                        grubContent = match.captured(1).trimmed();
                        static const QRegularExpression quotationRegex("[\"']");
                        grubContent.remove(quotationRegex);
                    }
                }
                break;
            }
        }
        grub.close();

        int timeout = timeoutStr.toInt();
        ui->spin_grub->setValue(timeout);
        ui->line_grub->setText(grubContent);
    }
    //-##################################################################################
    //-########################## СПИСОК РЕПОЗИТОРИЕВ ###################################
    //-##################################################################################
    QFile pacman("/etc/pacman.conf");
    if (pacman.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pacman);
        QStringList sections;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.startsWith('[') && line.endsWith(']')) {
                QString section = line.mid(1, line.length() - 2);
                if (section != "options") { // Исключение секции [options]
                    sections.append(section);
                }
            }
        }
        pacman.close();

        QString regexPattern = "^(" + sections.join("/\\S+|") + "/\\S+|aur/\\S+)"; // Формирование регулярного выражения
        settings.setValue("Repository", regexPattern);
        repo = QSharedPointer<QString>::create(regexPattern);
    }
    //-##################################################################################
    //-########################### MINI LODING ANIMATION ################################
    //-##################################################################################
    loadingLabel.reset(new QLabel(this));
    loadingLabel->setVisible(false);
    loadingLabel->setFixedSize(58, 53);

    QMovie* loadingAnimation = new QMovie(":/img/miniload.gif");
    loadingAnimation->setScaledSize(QSize(50, 50));

    loadingLabel->setMovie(loadingAnimation);
    loadingAnimation->start();
    loadingLabel->setStyleSheet("margin-left:4px;padding-left:2px;border:0;");
}

void MainWindow::AnimationBackground()
{
    ui->centralwidget->setStyleSheet("");
    videoItem = new QGraphicsVideoItem;

    player = new QMediaPlayer(this);
    player->setVideoOutput(videoItem);
    player->setLoops(QMediaPlayer::Infinite);

    scene = new QGraphicsScene(this);
    scene->addItem(videoItem);

    // Создаем виджет для сцены
    graphicsView = new QGraphicsView(scene, this);
    graphicsView->setGeometry(0, 0, width(), height());

    graphicsView->setFrameShape(QFrame::NoFrame);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    videoItem->setSize(graphicsView->size());
    videoItem->setAspectRatioMode(Qt::IgnoreAspectRatio);

    ui->centralwidget->raise();
    ui->toolBar->raise();
    ui->toolBar_2->raise();
    ui->statusBar->raise();

    if (animation >= 1) {
        player->setSource(QUrl("qrc:/theme/" + *animationname + ".mp4"));
        videoItem->show();
        player->play();
        ui->centralwidget->setStyleSheet("");

    } else {
        videoItem->hide();
        player->stop();
        ui->centralwidget->setStyleSheet("#centralwidget{background: url(:/img/bg.png) no-repeat bottom right;background-color:#322F85;}");
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (animation >= 1) {
        QMainWindow::resizeEvent(event);
        graphicsView->setGeometry(0, 0, width(), height());
        videoItem->setSize(graphicsView->size());
    }
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        isHidden() ? show() : hide();
}

void MainWindow::removeToolButtonTooltips(QToolBar* toolbar) {
    for (QAction* action : toolbar->actions()) {
        if (QWidget* widget = toolbar->widgetForAction(action)) {
            widget->setToolTip("");
            widget->setToolTipDuration(0);
        }
    }
}

void MainWindow::mrpropper(int value) {
    if (animation >= 1 && !(value == 6 || value == 7 || value == 10 || value == 11))
    {
        opacityEffect->setOpacity(0.8);
        ui->centralwidget->setGraphicsEffect(opacityEffect);
    }

    miniAnimation(false, ui->details_aur);
    miniAnimation(false, ui->list_aur);
    showLoadingAnimationMini(true);
    page = value;

    errorShown = true;

    for (QAction* action : ui->toolBar_2->actions()) {
        action->setVisible(false);
    }

    ui->combo_bench->setVisible(false);
    ui->push_pacman->setVisible(false);
    ui->push_kde->setVisible(false);
    ui->searchLineEdit->setVisible(false);
    ui->searchLineEdit->clear();
}

void MainWindow::TeaTimer()
{
    sendNotification(tr("Отдохни!"), teatext->isEmpty() ? tr("Пора пить чай!") : *teatext);
}

void MainWindow::WorkTimer()
{
    sendNotification(tr("Отдохни!"), teatext->isEmpty() ? tr("Пора сделать зарядку!") : *worktext);
}

void MainWindow::setHasUpdates(bool updates)
{
    trayIcon.setIcon(QIcon(updates ? ":/img/tray.png" : ":/img/2.png"));
    hasUpdates = updates;
}

void MainWindow::UpdateIcon()
{
    QProcess process;
    process.start("sh", QStringList() << "-c" << "checkupdates | wc -l");
    process.waitForFinished();

    if (process.exitCode() == QProcess::NormalExit) {
        int numUpdates = process.readAll().trimmed().toInt();
        setHasUpdates(numUpdates > 0);
        updateIconTimer->start();
    } else
        setHasUpdates(false);
}

void MainWindow::UpdateSnap()
{
    QProcess process;
    process.start("sh", QStringList() << "-c" << "snap refresh --list | wc -l");
    process.waitForFinished();
    hasUpdatesSnap = (process.exitCode() == QProcess::NormalExit) && (process.readAll().trimmed().toInt() > 0);
}

QColor MainWindow::generateRandomColor()
{
    QColor color;
    do {
        color = QColor::fromHsv(QRandomGenerator::global()->bounded(360),
                                QRandomGenerator::global()->bounded(200),
                                QRandomGenerator::global()->bounded(150, 256));
    } while (color.blue() > 200 || !color.isValid());
    return color;
}

void MainWindow::onTimeChanged(const QTime& time)
{
    if (time == QTime(0, 0))
        return;

    QTime selectedTime = time;
    int interval = QTime(0, 0).msecsTo(selectedTime);

    if (interval < 0)
        interval += 24 * 60 * 60 * 1000;

    QTimer::singleShot(interval, this, &MainWindow::on_action_11_triggered);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_action_11_triggered);
    timer->start(24 * 60 * 60 * 1000);
}

void MainWindow::showLoadingAnimationMini(bool show)
{
    if (show)
    {
        for (QAction* action : ui->toolBar->actions())
        {
            if (action->isCheckable() && action->isChecked())
            {
                loadingLabel->setVisible(true);
                ui->toolBar->insertWidget(action, loadingLabel.data());
                action->setVisible(false);
                actionLoad = action;
                ui->toolBar->setEnabled(!show);
                break;
            }
        }
    }
    else
    {
        QTimer::singleShot(500, this, [=]() {
            loadingLabel->setVisible(false);
            if (QLayout *toolBarLayout = ui->toolBar->layout()) {
                toolBarLayout->removeWidget(loadingLabel.data());
            }

            if (actionLoad)
                actionLoad->setVisible(true);

            ui->toolBar->setEnabled(true);
            removeToolButtonTooltips(ui->toolBar);
            removeToolButtonTooltips(ui->toolBar_2);
        });
    }

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}

void MainWindow::showLoadingAnimation(bool show, QWebEngineView* webView)
{
    if (animload == 0 || animloadpage > 1)
        return;

    QWidget* overlayWidget = webView->property("OverlayWidget").value<QWidget*>();
    QLabel* loadingLabel = webView->property("LoadingLabel").value<QLabel*>();

    if (show) {
        if (animation >= 1)
        {
            opacityEffect->setOpacity(1.0);
            ui->centralwidget->setGraphicsEffect(opacityEffect);
        }

        int leftShift = -100;

        if (!overlayWidget) {
            overlayWidget = new QWidget(webView);
            overlayWidget->setObjectName("OverlayWidget");
            overlayWidget->setGeometry(0, 0, width(), height());
            overlayWidget->raise();

            loadingLabel = new QLabel(overlayWidget);
            loadingLabel->setObjectName("LoadingLabel");
            loadingLabel->setAlignment(Qt::AlignCenter);
            loadingLabel->setFixedSize(500, 500);

            webView->setProperty("OverlayWidget", QVariant::fromValue(overlayWidget));
            webView->setProperty("LoadingLabel", QVariant::fromValue(loadingLabel));
        }

        QMovie* loadingMovie = nullptr;
        QString backgroundColor;

        if (page == 10) {
            loadingMovie = new QMovie(":/img/server.gif");
            backgroundColor = "#0072ce";
        }
        else
        {
            if (page == 2 || page == 4 || page == 8)
            {
                ui->action_35->setVisible(true);
                ui->action_11->setVisible(false);
                ui->action_34->setVisible(false);
                ui->action_updatelist->setVisible(false);
                ui->action_imgpkg->setVisible(false);
            }

            if (animloadpage == 0) {
                loadingMovie = new QMovie(":/img/loading.gif");
                backgroundColor = "#472e91";
            }
            else if (animloadpage == 1) {
                loadingMovie = new QMovie(":/img/loading2.gif");
                backgroundColor = "#2d2b79";
            }
        }

        loadingLabel->setMovie(loadingMovie);
        loadingMovie->start();

        loadingLabel->move((overlayWidget->width() - loadingLabel->width()) / 2 + leftShift,
                           (overlayWidget->height() - loadingLabel->height()) / 2);

        overlayWidget->setStyleSheet(QString("QWidget#OverlayWidget { background-color: %1; }").arg(backgroundColor));
        overlayWidget->show();
        loadingLabel->show();
    } else {

        if (overlayWidget) {
            overlayWidget->hide();
            overlayWidget = nullptr;
            overlayWidget->deleteLater();
        }

        if (loadingLabel) {
            loadingLabel->hide();
            loadingLabel = nullptr;
            loadingLabel->deleteLater();
        }
    }
    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}


//not
void MainWindow::downloadAndSaveImages(const QString& packageName, const QStringList& urls, const QString& folder)
{
    miniAnimation(true, ui->image_aur);
    if (ui->image_aur->scene())
        ui->image_aur->scene()->clear();

    imageUrls = urls;
    pixmaps.clear();
    currentIndex = 0;

    QString cacheFolder = folder + packageName + "/";
    QDir().mkpath(cacheFolder);

    for (int i = 0; i < imageUrls.size(); ++i) {
        const QString& imageUrl = imageUrls.at(i);
        QString fileName = QString("%1_%2.png").arg(packageName).arg(i);
        QString cacheFilePath = cacheFolder + fileName;

        if (QFile::exists(cacheFilePath)) {
            QPixmap pixmap(cacheFilePath);
            pixmaps.append(pixmap);
        } else {
            QNetworkRequest imageRequest((QUrl(imageUrl)));
            QNetworkReply* imageReply = networkManager.get(imageRequest);
            QEventLoop loop;
            connect(imageReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (imageReply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(imageReply->readAll());
                int targetWidth = 680;
                int targetHeight = pixmap.height() * targetWidth / pixmap.width();
                QSize newSize(targetWidth, targetHeight);
                pixmap = pixmap.scaled(newSize, Qt::KeepAspectRatio);

                pixmaps.append(pixmap);
                pixmap.save(cacheFilePath);
            }

            imageReply->deleteLater();
        }
    }
    updateImageView();
    miniAnimation(false, ui->image_aur);
}

//not
void MainWindow::updateImageView()
{
    if (!pixmaps.isEmpty() && currentIndex >= 0 && currentIndex < pixmaps.size()) {
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->addPixmap(pixmaps[currentIndex]);
        ui->image_aur->setScene(scene);
    }
}

void MainWindow::on_back_slider_clicked()
{
    currentIndex = (currentIndex - 1 + pixmaps.size()) % pixmaps.size();
    updateImageView();
}

void MainWindow::on_next_slider_clicked()
{
    currentIndex = (currentIndex + 1) % pixmaps.size();
    updateImageView();
}

//not
void MainWindow::processListItem(int row, QListWidget* listWidget, QTextBrowser* detailsWidget) {
    QListWidgetItem* nameItem = listWidget->item(row);
    QString packageName = nameItem->text();

    if (page == 2)
    {
        QString appName = packageName.split(' ')[0];

        for (const QString& ending : endingsToRemove) {
            if (appName.endsWith(ending)) {
                appName.chop(ending.length());
                break;
            }
        }

        QString snapcraftUrl = "https://snapcraft.io/" + packageName;
        QNetworkRequest request((QUrl(snapcraftUrl)));

        QNetworkReply* reply = networkManager.get(request);

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray htmlData = reply->readAll();

                static const QRegularExpression screenshotRegex("<img[^>]*data-original=\"([^\"]*)\"");
                QRegularExpressionMatchIterator matchIterator = screenshotRegex.globalMatch(htmlData);

                QStringList imageUrls;

                while (matchIterator.hasNext()) {
                    QRegularExpressionMatch match = matchIterator.next();
                    QString imageUrl = match.captured(1);

                    imageUrls.append(imageUrl);
                }

                if (imageUrls.isEmpty()){
                    ui->tabWidget_details->setCurrentIndex(0);
                    ui->action_imgpkg->setEnabled(false);
                    ui->action_imgpkg->setChecked(false);

                } else {
                    downloadAndSaveImages(packageName, imageUrls, mainDir + "cache/");
                    ui->action_imgpkg->setEnabled(true);
                }

            } else {
                ui->tabWidget_details->setCurrentIndex(0);
                ui->action_imgpkg->setEnabled(false);
                ui->action_imgpkg->setChecked(false);
            }

            reply->deleteLater();
        });
    }


    QSharedPointer<QProcess> currentProcess = QSharedPointer<QProcess>::create();
    int scrollBarValue = detailsWidget->verticalScrollBar()->value();

    connect(currentProcess.data(), &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = currentProcess->readAllStandardOutput();
        QString packageInfo = QString::fromUtf8(output);
        QStringList lines = packageInfo.split("\n");
        QString processedInfo;

        for (const QString& line : lines) {
            if (!line.isEmpty()) {
                int colonIndex = line.indexOf(':');
                if (colonIndex != -1) {
                    QString header = line.left(colonIndex).trimmed();
                    QString content = line.mid(colonIndex + 1).trimmed();
                    header = "<b>" + header + ":</b> ";
                    processedInfo += "<p><span>" + header + "</span>" + content + "</p>";
                }
            }
        }

        detailsWidget->append(processedInfo);
        detailsWidget->verticalScrollBar()->setValue(scrollBarValue);

        if (page == 2)
            ui->action_like->setEnabled(true);

        miniAnimation(false,detailsWidget);
    });

    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode != 0 || exitStatus == QProcess::CrashExit) {
            QByteArray errorOutput = currentProcess->readAllStandardError();
            QString errorMessage = QString::fromUtf8(errorOutput);

            if (!errorMessage.trimmed().isEmpty())
                detailsWidget->setText(errorMessage);
            else
                detailsWidget->setText(tr("Пакет не найден!\nВозможно, он поменял свое название..."));

            ui->searchLineEdit->setText(packageName);

            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            QCoreApplication::postEvent(ui->searchLineEdit, event);

            if (page == 2)
                ui->action_like->setEnabled(false);

            miniAnimation(false,detailsWidget);
        }
    });

    detailsWidget->clear();

    miniAnimation(true,detailsWidget);

    QStringList command;

    if (page == 2)
        command = packageCommands.value(pkg).value("show_info");
    else if (page == 4)
        command = packageCommands.value(pkg).value("info");

    currentProcess->start(command[0], QStringList() << command[1] << packageName);
}

//not
void MainWindow::onListAurItemClicked(QListWidgetItem *item)
{
    if (page == 2) {
        int row = ui->list_aur->row(item);
        bool iconFound = false;
        if (list != 8) {
            for (auto it = appIcons.constBegin(); it != appIcons.constEnd(); ++it) {
                if (appIcons.contains(item->text()) && appIcons[item->text()] == it.value()) {
                    loadContent(row + 1, loadpage);
                    iconFound = true;
                    break;
                }
            }
        }
        if (!iconFound)
            processListItem(row, ui->list_aur, ui->details_aur);
    } else if (page == 4) {
        int row = ui->list_app->row(item);
        processListItem(row, ui->list_app, ui->details_aurpkg);
    }
}

//not
void MainWindow::miniAnimation(bool visible, QWidget* targetWidget)
{
    static QLabel* miniLoadLabel = nullptr;

    if (visible) {
        if (miniLoadLabel) {
            delete miniLoadLabel;
            miniLoadLabel = nullptr;
        }

        miniLoadLabel = new QLabel(targetWidget);
        miniLoadLabel->setObjectName("MiniLoadLabel");
        miniLoadLabel->setFixedSize(221, 31);

        QMovie* movie = new QMovie(":/img/load.gif", QByteArray(), miniLoadLabel);
        miniLoadLabel->setMovie(movie);
        movie->start();

        int x = (targetWidget->width() - miniLoadLabel->width()) / 2;
        int y = (targetWidget->height() - miniLoadLabel->height()) / 2;

        miniLoadLabel->setGeometry(x, y, 221, 31);
        miniLoadLabel->setVisible(true);
        targetWidget->setStyleSheet("background-color: #222222;");
    } else {
        if (miniLoadLabel) {
            miniLoadLabel->setVisible(false);
            delete miniLoadLabel;
            miniLoadLabel = nullptr;
        }
        if (targetWidget == ui->image_aur)
            targetWidget->setStyleSheet("background-color: #191919;");
        else
            targetWidget->setStyleSheet("background-color: #272727;");

        removeToolButtonTooltips(ui->toolBar);
        removeToolButtonTooltips(ui->toolBar_2);
    }
}

QIcon MainWindow::getPackageIcon(const QString& packageName) {

    QString appName = packageName.split(' ').first();
    for (const QString& ending : endingsToRemove) {
        if (appName.endsWith(ending)) {
            appName.chop(ending.length());
            break;
        }
    }

    QString additionalIconPath = "/usr/share/icons/Papirus/48x48/apps/" + appName + ".svg";
    if (QFileInfo(additionalIconPath).isFile()) {
        return QIcon(additionalIconPath);
    }

    static QMap<QString, QString> iconMap;
    if (iconMap.isEmpty()) {
        QStringList searchPaths = {"/usr/share/applications/",
                                   QDir::homePath() + "/.local/share/applications",
                                   "/usr/local/share/applications",
                                   "/var/lib/snapd/desktop/applications"};

        for (const QString& searchPath : searchPaths) {
            QDir desktopFilesDir(searchPath);
            QStringList desktopFiles = desktopFilesDir.entryList({"*.desktop"}, QDir::Files);

            for (const QString& desktopFileName : desktopFiles) {
                QFile desktopFile(desktopFilesDir.filePath(desktopFileName));
                if (desktopFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream stream(&desktopFile);

                    QString iconName;
                    while (!stream.atEnd()) {
                        QString line = stream.readLine().trimmed();
                        if (line.startsWith("Icon=")) {
                            iconName = line.mid(5).trimmed();
                        }
                        if (!iconName.isEmpty()) {
                            break;
                        }
                    }
                    desktopFile.close();

                    if (!iconName.isEmpty()) {
                        iconMap[desktopFileName.split('.').first()] = iconName;
                    }
                }
            }
        }
    }

    if (iconMap.contains(appName)) {
        QString iconName = iconMap[appName];

        QIcon themeIcon = QIcon::fromTheme(iconName);
        if (!themeIcon.isNull()) {
            return themeIcon;
        } else {
            QStringList searchPaths = {"/usr/share/icons", mainDir + "/share/icons"};

            for (const QString& searchPath : searchPaths) {
                QDirIterator it(searchPath, QDirIterator::Subdirectories);
                while (it.hasNext()) {
                    QString filePath = it.next();
                    if (QFileInfo(filePath).isFile() && filePath.endsWith(iconName + ".png")) {
                        return QIcon(filePath);
                    }
                }
            }
        }
    }

    return QIcon(":/img/pacman.png");
}

//not
void MainWindow::loadContent(int value, bool valuepage)
{
    miniAnimation(true,ui->list_aur);
    ui->list_aur->clear();
    QString sourceFilePath;
    QString targetFilePath;

    // 0 - лист

    if (value == 0) {
        sourceFilePath = ":/other/" + *lang + "/list.txt";
        targetFilePath = mainDir + "other/" + *lang + "/list.txt";
        valuepage = true;
        list = 0;
    }
    else if (value == 1)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/populaty.txt";
            targetFilePath = mainDir + "other/populaty.txt";
            list = 1;
            valuepage = false;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/network.txt";
            targetFilePath = mainDir + "other/network.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/codecs.txt";
            targetFilePath = mainDir + "other/codecs.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/terminals.txt";
            targetFilePath = mainDir + "other/terminals.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/texteditors.txt";
            targetFilePath = mainDir + "other/texteditors.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/network-security.txt";
            targetFilePath = mainDir + "other/network-security.txt";
            valuepage = false;
        }
    }
    else if (value == 2)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/" + *lang + "/net.txt";
            targetFilePath = mainDir + "other/" + *lang + "/net.txt";
            list = 2;
            valuepage = true;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/browsers.txt";
            targetFilePath = mainDir + "other/browsers.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/images.txt";
            targetFilePath = mainDir + "other/images.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/files.txt";
            targetFilePath = mainDir + "other/files.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/office.txt";
            targetFilePath = mainDir + "other/office.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/firewall-management.txt";
            targetFilePath = mainDir + "other/firewall-management.txt";
            valuepage = false;
        }
    }
    else if (value == 3)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/" + *lang + "/multimedia.txt";
            targetFilePath = mainDir + "other/" + *lang + "/multimedia.txt";
            list = 3;
            valuepage = true;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/servers.txt";
            targetFilePath = mainDir + "other/servers.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/audio.txt";
            targetFilePath = mainDir + "other/audio.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/development.txt";
            targetFilePath = mainDir + "other/development.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/markup-languages.txt";
            targetFilePath = mainDir + "other/markup-languages.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/threat-and-vulnerability-detection.txt";
            targetFilePath = mainDir + "other/threat-and-vulnerability-detection.txt";
            valuepage = false;
        }
    }
    else if (value == 4)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/" + *lang + "/utility.txt";
            targetFilePath = mainDir + "other/" + *lang + "/utility.txt";
            list = 4;
            valuepage = true;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/file-sharing.txt";
            targetFilePath = mainDir + "other/file-sharing.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/video.txt";
            targetFilePath = mainDir + "other/video.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/text-input.txt";
            targetFilePath = mainDir + "other/text-input.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/document-converters.txt";
            targetFilePath = mainDir + "other/document-converters.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/file-security.txt";
            targetFilePath = mainDir + "other/file-security.txt";
            valuepage = false;
        }
    }
    else if (value == 5)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/" + *lang + "/office.txt";
            targetFilePath = mainDir + "other/" + *lang + "/office.txt";
            list = 5;
            valuepage = true;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/communication.txt";
            targetFilePath = mainDir + "other/communication.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/coll-managers.txt";
            targetFilePath = mainDir + "other/coll-managers.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/disks.txt";
            targetFilePath = mainDir + "other/disks.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/bibliographic-reference-manager.txt";
            targetFilePath = mainDir + "other/bibliographic-reference-manager.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/anti-malware.txt";
            targetFilePath = mainDir + "other/anti-malware.txt";
            valuepage = false;
        }
    }
    else if (value == 6)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/" + *lang + "/security.txt";
            targetFilePath = mainDir + "other/" + *lang + "/security.txt";
            list = 6;
            valuepage = true;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/news-and-rss.txt";
            targetFilePath = mainDir + "other/news-and-rss.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/media-servers.txt";
            targetFilePath = mainDir + "other/media-servers.txt";
            valuepage = false;
        }
        else if (list == 4)
        {
            sourceFilePath = ":/other/system.txt";
            targetFilePath = mainDir + "other/system.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/reader-sand-viewers.txt";
            targetFilePath = mainDir + "other/reader-sand-viewers.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/screen-lockers.txt";
            targetFilePath = mainDir + "other/screen-lockers.txt";
            valuepage = false;
        }
    }
    else if (value == 7)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/game.txt";
            targetFilePath = mainDir + "other/game.txt";
            list = 7;
            valuepage = false;
        }
        else if (list == 2)
        {
            sourceFilePath = ":/other/remote-desktop.txt";
            targetFilePath = mainDir + "other/remote-desktop.txt";
            valuepage = false;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/metadata.txt";
            targetFilePath = mainDir + "other/metadata.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/document-managers.txt";
            targetFilePath = mainDir + "other/document-managers.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/password-auditing.txt";
            targetFilePath = mainDir + "other/password-auditing.txt";
            valuepage = false;
        }
    }
    else if (value == 8)
    {
        if (list == 0)
        {
            sourceFilePath = ":/other/phone.txt";
            targetFilePath = mainDir + "other/phone.txt";
            list = 8;
            valuepage = true;
        }
        else if (list == 3)
        {
            sourceFilePath = ":/other/mobile-device-managers.txt";
            targetFilePath = mainDir + "other/mobile-device-managers.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/scanning-software.txt";
            targetFilePath = mainDir + "other/scanning-software.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/password-managers.txt";
            targetFilePath = mainDir + "other/password-managers.txt";
            valuepage = false;
        }
    }
    else if (value == 9)
    {
        if (list == 3)
        {
            sourceFilePath = ":/other/optical-disc-burning.txt";
            targetFilePath = mainDir + "other/optical-disc-burning.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/osr-software.txt";
            targetFilePath = mainDir + "other/osr-software.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/cryptography.txt";
            targetFilePath = mainDir + "other/cryptography.txt";
            valuepage = false;
        }
    }
    else if (value == 10)
    {
        if (list == 3)
        {
            sourceFilePath = ":/other/personal-video-recorders.txt";
            targetFilePath = mainDir + "other/personal-video-recorders.txt";
            valuepage = false;
        }
        else if (list == 5)
        {
            sourceFilePath = ":/other/notes.txt";
            targetFilePath = mainDir + "other/notes.txt";
            valuepage = false;
        }
        else if (list == 6)
        {
            sourceFilePath = ":/other/privilege-elevation.txt";
            targetFilePath = mainDir + "other/privilege-elevation.txt";
            valuepage = false;
        }
    }
    else if (value == 11)
    {
        if (list == 5)
        {
            sourceFilePath = ":/other/special-writing-environments.txt";
            targetFilePath = mainDir + "other/special-writing-environments.txt";
            valuepage = false;
        }
    }
    else if (value == 12)
    {
        if (list == 5)
        {
            sourceFilePath = ":/other/language.txt";
            targetFilePath = mainDir + "other/language.txt";
            valuepage = false;
        }
    }
    else if (value == 13)
    {
        if (list == 5)
        {
            sourceFilePath = ":/other/barcode-generators-and-readers.txt";
            targetFilePath = mainDir + "other/barcode-generators-and-readers.txt";
            valuepage = false;
        }
    }

    QFileInfo fileInfo(targetFilePath);
    if (!fileInfo.exists())
    {
        if (!QDir().mkpath(fileInfo.absoluteDir().path()))
        {
            sendNotification(tr("Ошибка"), tr("Не удалось создать каталог: ") + fileInfo.absoluteDir().path());
            return;
        }
        if (!QFile::copy(sourceFilePath, targetFilePath))
        {
            sendNotification(tr("Ошибка"), tr("Не удалось скопировать файл из ") + sourceFilePath + tr(" в ") + targetFilePath);
            return;
        }
    }

    QFile file(targetFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        sendNotification(tr("Ошибка"), tr("Не удалось открыть файл ресурсов"));
        return;
    }

    QTextStream in(&file);
    QVector<QString> programs;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (!line.isEmpty())
        {
            int index = line.indexOf('}');

            if (index != -1)
            {
                QString program = line.mid(2, index - 3);
                programs.append(program);
            }
        }
    }
    file.close();

    for (int i = 0; i < programs.size(); i++) {
        QString packageName = programs[i];
        QColor color = generateRandomColor();
        QListWidgetItem *item = new QListWidgetItem();

        QString iconPath;

        iconPath = ":/img/pacman.png";

        QString prefixToRemove = "";

        if(valuepage)
        {
            static const QRegularExpression regex("(\\S+)/\\S+");
            QRegularExpressionMatch match = regex.match(packageName);

            if (match.hasMatch()) {
                QString repoName = match.captured(1);
                iconPath = "/usr/share/icons/Papirus/48x48/apps/" + repoName + ".svg";

                QFileInfo fileInfo(iconPath);
                if (!fileInfo.exists())
                    iconPath = ":/img/" + repoName + ".png";

                prefixToRemove = repoName + "/";
            }

            if (!prefixToRemove.isEmpty())
                packageName.remove(0, prefixToRemove.length());
        }
        else
        {
            static const QRegularExpression regex("\\S+");
            QRegularExpressionMatch match = regex.match(packageName);

            QString packageNameIcon;
            packageNameIcon = packageName;

            for (const QString& ending : endingsToRemove) {
                if (packageNameIcon.endsWith(ending)) {
                    packageNameIcon.chop(ending.length());
                    break;
                }
            }
            iconPath = "/usr/share/icons/Papirus/48x48/apps/" + packageNameIcon + ".svg";

            QFileInfo fileInfo(iconPath);
            if (!fileInfo.exists()) {
                iconPath = ":/img/pacman.png";
            }
        }

        item->setIcon(QIcon(iconPath));

        item->setText(packageName);
        item->setForeground(color);

        ui->list_aur->addItem(item);

        if(valuepage)
            appIcons[packageName] = iconPath;
    }
    miniAnimation(false,ui->list_aur);
}

QString packageVersion(const QString& packageName) {
    static const QRegularExpression regex(R"-(\d+\.\d+(?:\.\d+)*)-");
    return regex.match(packageName).captured(0);
}

//not
void MainWindow::onListDowngradeItemDoubleClicked(QListWidgetItem *currentItem) {
    QString packageName;

    if (currentItem != nullptr) {
        packageName = currentItem->text();
    } else {
        sendNotification(tr("Внимание"), tr("Выберите пакет для установки!"));
        return;
    }

    Terminal terminal = getTerminal();

    QString installUrl = "https://archive.archlinux.org/packages/" + QString(packagesArchiveAUR.at(0)) + "/" + packagesArchiveAUR +  "/" + packageName;

    if (nvidia == 1)
    {
        nvidiaVersion = packageVersion(packageName); //узнаем версию пакета для сортировки по остальным пакетам
        nvidiaDkms = installUrl;
        nvidiaDkmsName = packageName;
        nvidia = 2;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 2)
    {
        nvidiaUtils = installUrl;
        nvidiaUtilsName = packageName;
        nvidia = 3;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 3)
    {
        nvidiaSettings = installUrl;
        nvidiaSettingsName = packageName;
        nvidia = 4;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 4)
    {
        libxnvctrl = installUrl;
        libxnvctrlName = packageName;
        nvidia = 5;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 5)
    {
        openclNvidia = installUrl;
        openclNvidiaName = packageName;
        nvidia = 6;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 6)
    {
        lib32NvidiaUtils = installUrl;
        lib32NvidiaUtilsName = packageName;
        nvidia = 7;
        on_action_nvidia_triggered();
        return;
    }
    else if (nvidia == 7)
    {
        lib32OpenclNvidia = installUrl;
        lib32OpenclNvidiaName = packageName;
        nvidia = 8;
        on_action_nvidia_triggered();
        return;
    }

    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        //ничего делать не надо
    });
    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("localinstall") << installUrl);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->start();
}

void MainWindow::checkForDowngrades(const QString& packagesArchiveAUR)
{
    if (packagesArchiveAUR.isEmpty())
        return;
    miniAnimation(true, ui->list_downgrade);
    addedLinks.clear();
    ui->list_downgrade->clear();
    QChar firstLetter = packagesArchiveAUR.at(0);
    QUrl url("https://archive.archlinux.org/packages/" + QString(firstLetter) + "/" + packagesArchiveAUR);
    QNetworkRequest request(url);
    manager->get(request);
    QSharedPointer<QProcess> currentProcess = QSharedPointer<QProcess>::create();
    connectProcessSignals(currentProcess, ui->details_downgrade);
    ui->details_downgrade->clear();
    QStringList command = packageCommands.value(pkg).value("info");
    currentProcess->start(command[0], QStringList() << command[1] << packagesArchiveAUR);
}

void MainWindow::connectProcessSignals(QSharedPointer<QProcess>& process, QTextBrowser* outputWidget)
{
    connect(process.data(), &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput();
        QString packageInfo = QString::fromUtf8(output);
        process->waitForFinished();

        QString processedInfo = processPackageInfo(packageInfo);
        outputWidget->append(processedInfo);
        outputWidget->moveCursor(QTextCursor::Start);
    });

    connect(process.data(), &QProcess::readyReadStandardError, this, [=]() {
        QByteArray errorOutput = process->readAllStandardError();
        QString errorMessage = QString::fromUtf8(errorOutput);
        process->waitForFinished();

        if (!errorMessage.trimmed().isEmpty()) {
            outputWidget->setText(errorMessage);
        }
    });
}

QString MainWindow::processPackageInfo(const QString& packageInfo)
{
    QStringList lines = packageInfo.split("\n");
    QString processedInfo;

    for (const QString& line : lines) {
        if (!line.isEmpty()) {
            int colonIndex = line.indexOf(':');
            if (colonIndex != -1) {
                QString header = line.left(colonIndex).trimmed();
                QString content = line.mid(colonIndex + 1).trimmed();

                header = "<b>" + header + ":</b> ";
                processedInfo += "<p><span>" + header + "</span>" + content + "</p>";
            }
        }
    }
    return processedInfo;
}

void MainWindow::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404)
            return;

        QByteArray htmlData = reply->readAll();
        QString htmlContent(htmlData);
        static QRegularExpression linkRegex("<a [^>]*href=\"([^\"]*)\"[^>]*>.*</a>");  // Статический объект

        int pos = 0;

        QRegularExpressionMatch match;

        while ((match = linkRegex.match(htmlContent, pos)).hasMatch())
        {
            QString link = match.captured(1);
            addLinkToList(link);
            pos += match.capturedLength();
        }
    }
    else {
        miniAnimation(false,ui->list_downgrade);
        ui->details_downgrade->setText(tr("Пакет не найден в архиве"));
    }
    reply->deleteLater();
}

void MainWindow::addLinkToList(const QString &link)
{
    QString cleanedLink = link;
    static const QRegularExpression dotDotExp("\\.\\./");
    cleanedLink.replace(dotDotExp, "");

    static const QRegularExpression newlineExp("^\n");
    cleanedLink.replace(newlineExp, "");

    miniAnimation(false, ui->list_downgrade);

    if (cleanedLink.isEmpty() || cleanedLink.contains(".sig") || addedLinks.contains(cleanedLink))
        return;

    QListWidgetItem *item = new QListWidgetItem(QIcon("/usr/share/icons/Papirus/48x48/mimetypes/application-x-xz-pkg.svg"), cleanedLink);
    item->setForeground(generateRandomColor());

    if (nvidia == 1 || nvidia < 1 || packageVersion(link) == nvidiaVersion)
    {
        ui->list_downgrade->insertItem(0, item);
        addedLinks.insert(cleanedLink);

        if (nvidia == 1)
        {
            while (ui->list_downgrade->count() > 5)
            {
                delete ui->list_downgrade->takeItem(ui->list_downgrade->count() - 1);
            }
        }
    }
}

void MainWindow::loadContentInstall()
{
    QStringList allPackages = executeCommand(packageCommands.value(pkg).value("query_explicit"));

    const QStringList& allPackagesCombined = allPackages;

    numPackages = allPackagesCombined.size();
    ui->list_app->clear();

    for (const QString& package : allPackagesCombined) {
        QStringList packageParts = package.split(' ');
        QString packageName = packageParts.at(0);
        QIcon packageIcon = getPackageIcon(packageName);

        QListWidgetItem* item = new QListWidgetItem(packageName);
        item->setIcon(packageIcon);
        item->setForeground(generateRandomColor());
        ui->list_app->addItem(item);
    }
}

void MainWindow::setCursorAndScrollToItem(const QString& itemName)
{
    for (int row = 0; row < ui->list_aur->count(); ++row) {
        QListWidgetItem* currentItem = ui->list_aur->item(row);
        if (currentItem && currentItem->text() == itemName) {
            onListAurItemClicked(currentItem);
            ui->list_aur->setCurrentItem(currentItem);
            ui->list_aur->selectionModel()->select(ui->list_aur->model()->index(ui->list_aur->row(currentItem), 0),
                                                   QItemSelectionModel::Select);
            ui->list_aur->scrollToItem(currentItem);
            break;
        }
    }
}

QStringList MainWindow::executeCommand(const QStringList& command)
{
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->start(command.at(0), command.mid(1));
    process->waitForFinished(-1);
    return QString::fromUtf8(process->readAllStandardOutput()).split("\n", Qt::SkipEmptyParts);
}

void MainWindow::handleServerResponse(const QString& reply)
{
    ui->list_aur->clear();
    miniAnimation(true, ui->list_aur);
    helperPackageNames.clear();

    const QStringList& searchCommand = packageCommands.value(pkg).value("search");

    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), &QProcess::readyReadStandardOutput, this, &MainWindow::onCurrentProcessReadyRead);
    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        miniAnimation(false, ui->list_aur);
    });
    currentProcess->start(searchCommand.at(0), QStringList() << searchCommand.at(1) << reply);
}

//not
void MainWindow::onCurrentProcessReadyRead()
{
    ui->list_aur->clear();

    while (currentProcess->canReadLine()) {
        QByteArray line = currentProcess->readLine();
        QString lineString = QString::fromUtf8(line).trimmed();

        QRegularExpression regex(*repo); // Добавлено условие для extra
        QRegularExpressionMatch match = regex.match(lineString);
        if (match.hasMatch()) {
            QString packageName = match.captured(1);
            helperPackageNames.append(packageName);

            QString iconPath = "";
            QString prefixToRemove = "";

            static const QRegularExpression regex("(\\w+)/\\S+");
            QRegularExpressionMatch match = regex.match(packageName);

            if (match.hasMatch()) {
                QString repoName = match.captured(1);

                if (QFile::exists(":/img/" + repoName + ".png"))
                    iconPath = ":/img/" + repoName + ".png";
                else
                    iconPath = ":/img/pacman.png";

                prefixToRemove = repoName + "/";
            }

            if (!prefixToRemove.isEmpty())
                packageName.remove(0, prefixToRemove.length());

            QListWidgetItem *item = new QListWidgetItem(QIcon(iconPath), packageName);
            QColor color = generateRandomColor();
            item->setForeground(color);
            ui->list_aur->addItem(item);
        }
    }

    miniAnimation(false, ui->list_aur);

    QString searchText = ui->searchLineEdit->text();
    setCursorAndScrollToItem(searchText);
    QTimer::singleShot(timeout.msecsSinceStartOfDay() / 1000, this, &MainWindow::onSearchTimeout);
}

void MainWindow::onSearchTimeout()
{
    currentProcess->terminate();
    miniAnimation(false, ui->list_aur);
}

//not
void MainWindow::loadingListWidget()
{
    saveScripts(shResourcePaths, mainDir + "sh/");
    saveScripts(clearResourcePaths, mainDir + "clear/");
    saveScripts(journalsResourcePaths, mainDir + "journals/");
    saveScripts(cfgResourcePaths, mainDir + "cfg/");
    saveScripts(benchResourcePaths, mainDir + "bench/");

    loadScripts(mainDir + "sh/", ui->list_sh);
    loadScripts(mainDir + "clear/", ui->list_clear);
    loadScripts(mainDir + "journals/", ui->list_journal);
    loadScripts(mainDir + "cfg/", ui->list_cfg);
    loadScripts(mainDir + "bench/", ui->list_bench);

    cacheButtonHelper = new QListWidgetItem((pkg == 0) ? tr("Кэш пакетов Yay") : tr("Кэш пакетов Paru"), ui->list_clear);
    cacheButtonPacman = new QListWidgetItem(tr("Кэш пакетов Pacman"), ui->list_clear);
    orphanButton = new QListWidgetItem(tr("Пакеты сироты"), ui->list_clear);

    cacheButtonHelper->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/pacman.svg"));
    cacheButtonPacman->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/pacman.svg"));
    orphanButton->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/ark.svg"));

    cacheButtonHelper->setForeground(generateRandomColor());
    cacheButtonPacman->setForeground(generateRandomColor());
    orphanButton->setForeground(generateRandomColor());

    QDir().mkpath(mainDir + "other/");

    QFile::copy(":/other/notify.png", mainDir + "other/notify.png");
    QFile::copy(":/other/en_US/translations.txt", mainDir + "other/en_US/translations.txt");
    QFile::copy(":/other/ru_RU/translations.txt", mainDir + "other/ru_RU/translations.txt");
    QFile::copy(":/other/main.sh", mainDir + "other/main.sh");
}

void MainWindow::loadScripts(const QString& baseDir, QListWidget* listWidget)
{
    listWidget->clear();
    QDir dir(baseDir);
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);

    for (const QFileInfo& fileInfo : fileList)
    {
        QString itemName;
        QString iconPath;

        QFile scriptFile(fileInfo.filePath());
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang))
                {
                    itemName = line.mid(12).trimmed();
                }
                else if (line.startsWith("#icon"))
                {
                    QString iconNumber = line.mid(6).trimmed();
                    iconPath = findIconPath(iconNumber);
                }
            }
            scriptFile.close();
        }

        QListWidgetItem* item = new QListWidgetItem(itemName.isEmpty() ? fileInfo.fileName() : itemName);
        item->setForeground(generateRandomColor());

        if (!iconPath.isEmpty())
        {
            QIcon icon(iconPath);
            item->setIcon(icon);
        }
        listWidget->addItem(item);
    }
    listWidget->sortItems(Qt::AscendingOrder);
}

QString MainWindow::findIconPath(const QString &iconNumber)
{
    QString iconPath = QString("/usr/share/icons/Papirus/48x48/apps/%1.svg").arg(iconNumber);
    QFileInfo fileInfo(iconPath);
    return fileInfo.exists() ? iconPath : QString(":/img/%1.png").arg(iconNumber);
}

void MainWindow::sendNotification(const QString& title, const QString& message)
{
    QStringList arguments{
        title,
        message,
        "-i", mainDir + "other/notify.png",
        "-a", "kLaus",
        "-t", "10000"
    };

    QProcess::startDetached("notify-send", arguments);
    loadSound(1);
}

void MainWindow::createArchive(const QString& folderPath, const QString& folderName)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите удалить каталог \"%1\" %2?").arg(folderPath + folderName, (repair == 0) ? tr("с конфигурацией") : tr("и создать резервную копию")), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    if (repair == 0) {
        removeDirectory(folderPath + folderName);
        loadFolders();
        sendNotification(tr("Конфигурация"), tr("Конфигурация успешно удалена!"));
    } else {
        QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
        process->start("zenity", QStringList() << "--file-selection" << "--directory" << tr("--title=Выберите путь сохранения архива"));
        process->waitForFinished();

        if (process->exitCode() == 0) {
            QString selectedPath = process->readAllStandardOutput().trimmed();
            if (!selectedPath.isEmpty()) {
                QDir folderDir(folderPath);
                if (folderDir.exists() && folderDir.cd(folderPath)) {
                    QString archivePath = selectedPath + "/" + folderName + ".zip";
                    QSharedPointer<QProcess> zipProcess = QSharedPointer<QProcess>::create();
                    zipProcess->setWorkingDirectory(folderDir.absolutePath());
                    zipProcess->start("zip", QStringList() << "-r" << archivePath << folderName);
                    zipProcess->waitForFinished();

                    if (zipProcess->exitCode() == 0) {
                        removeDirectory(folderPath + folderName);
                        loadFolders();
                        sendNotification(tr("Конфигурация"), tr("Резервная копия %1 успешно создана! Конфигурация удалена!").arg(archivePath));
                        return;
                    }
                }
            }
        }
        sendNotification(tr("Ошибка"), tr("Резервная копия не создана или отменена"));
    }
}

void MainWindow::loadFolders()
{
    ui->list_repair->clear();
    int itemCount = 0;

    auto addFolderItem = [&](const QString& folderName) {
        QString iconName = folderName;
        if (iconName.startsWith('.') && iconName.length() > 1) {
            iconName = iconName.mid(1); // Убираем точку из начала названия
        }

        QString iconPath = QString("/usr/share/icons/Papirus/48x48/apps/") + iconName + ".svg";
        if (!QFileInfo::exists(iconPath)) {
            iconPath = "/usr/share/icons/Papirus/48x48/apps/gnome-do.svg";
        }

        ui->list_repair->addItem(new QListWidgetItem(QIcon(iconPath), folderName));
        itemCount++;
    };

    QDir homeDir(QDir::homePath());
    QDir configDir(homeDir.absoluteFilePath(".config"));

    QStringList configFolderList = configDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& folderName : configFolderList)
        addFolderItem(folderName);

    QFileInfoList homeFolderList = homeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    for (const QFileInfo& fileInfo : homeFolderList) {
        QString folderName = fileInfo.fileName();
        if (folderName.startsWith('.') &&
            folderName != ".config" &&
            folderName != ".local" &&
            folderName != ".themes" &&
            folderName != ".var" &&
            folderName != ".icons" &&
            folderName != ".cache") {
            addFolderItem(folderName);
        }
    }

    ui->label_repair->setText(tr("Очистка конфигураций: %1").arg(itemCount));
}

void MainWindow::restoreArchive(const QString& archivePath)
{
    QString archiveName = QFileInfo(archivePath).fileName();
    QString targetDir = archiveName.startsWith(".") ? QDir::homePath() : QDir::homePath() + "/.config";
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->setWorkingDirectory(targetDir);
    process->start("unzip", QStringList() << "-q" << archivePath);
    process->waitForFinished();

    if (process->exitCode() == 0) {
        sendNotification(tr("Резервная копия"), tr("Резервная копия успешно восстановлена в папку: ") + targetDir);
        loadFolders();
    } else
        sendNotification(tr("Ошибка"), tr("Не удалось восстановить конфигурацию через архив"));
}
//---#####################################################################################################################################################
//--################################################################## СОБЫТИЯ ФУНКЦИЙ ##################################################################
//-#####################################################################################################################################################
void MainWindow::on_combo_mainpage_currentIndexChanged(int index)
{
    mainpage = index;
    settings.setValue("MainPage", mainpage);
}

void MainWindow::on_combo_helper_currentIndexChanged(int index)
{
    pkg = index;
    settings.setValue("PackageManager", pkg);
    cacheButtonHelper->setText((pkg == 0) ? tr("Кэш пакетов Yay") : tr("Кэш пакетов Paru"));
}

void MainWindow::on_combo_animload_currentIndexChanged(int index)
{
    animloadpage = index;
    settings.setValue("AnimLoadPage", animloadpage);
}

void MainWindow::on_combo_lang_currentIndexChanged(int index)
{
    const QString newLang = (index == 0) ? "ru_RU" : "en_US";
    if (settings.value("Language").toString() != newLang) {
        settings.setValue("Language", newLang);
        sendNotification(tr("Смена языка"), tr("Приложение будет перезагружено для смены языка"));

        QTranslator translator;
        if (translator.load(":/lang/kLaus_" + newLang + ".qm"))
            qApp->installTranslator(&translator);

        qApp->quit();
        QSharedPointer<QProcess>(new QProcess)->startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_combo_theme_currentIndexChanged(int index)
{
    settings.setValue("Animation", index);
    animation = index;

    QString animationName = ui->combo_theme->currentText();

    *animationname = animationName.toLower();

    if (animation == 0) {
        settings.setValue("AnimationName", "");
        videoItem->hide();
        player->stop();
        ui->centralwidget->setStyleSheet("#centralwidget{background: url(:/img/bg.png) no-repeat bottom right;background-color:#322F85;}");
        opacityEffect->setOpacity(1.0);
        ui->centralwidget->setGraphicsEffect(opacityEffect);
    } else {
        settings.setValue("AnimationName", animationName.toLower());
        player->setSource(QUrl("qrc:/theme/" + animationName.toLower() + ".mp4"));
        videoItem->show();
        player->play();
        ui->centralwidget->setStyleSheet("");
        opacityEffect->setOpacity(0.8);
        ui->centralwidget->setGraphicsEffect(opacityEffect);
    }
}


//not
void MainWindow::on_combo_bench_currentIndexChanged(int index)
{
    benchlist = index;
    ui->list_bench->clear();

    if (index == 0)
    {
        loadScripts(mainDir + "bench/", ui->list_bench);
        return;
    }

    QDir dir(mainDir + "/bench");
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);

    QList<QListWidgetItem*> comboBenchScripts;

    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();
        QString fileName = fileInfo.fileName();

        QString itemName = fileName;
        QString iconPath = "";

        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang))
                {
                    itemName = line.mid(12).trimmed();
                }
                else if (line.startsWith("#icon"))
                {
                    QString iconNumber = line.mid(6).trimmed();

                    iconPath = QString("/usr/share/icons/Papirus/48x48/apps/%1.svg").arg(iconNumber);

                    QFileInfo fileInfo(iconPath);
                    if (!fileInfo.exists()) {
                        iconPath = QString(":/img/%1.png").arg(iconNumber);
                    }
                }
            }
            scriptFile.close();
        }
        QListWidgetItem* item = new QListWidgetItem(itemName);
        item->setForeground(generateRandomColor());

        if (!iconPath.isEmpty())
        {
            QIcon icon(iconPath);
            item->setIcon(icon);
        }

        if ((index == 1 && !iconPath.isEmpty() && (iconPath == "/usr/share/icons/Papirus/48x48/apps/org.kde.plasma.systemmonitor.svg" || iconPath == "/usr/share/icons/Papirus/48x48/apps/org.kde.plasma.systemmonitor.cpucore.svg")) ||
            (index == 2 && !iconPath.isEmpty() && iconPath == "/usr/share/icons/Papirus/48x48/apps/io.elementary.monitor.svg") ||
            (index == 3 && !iconPath.isEmpty() && iconPath == "/usr/share/icons/Papirus/48x48/apps/multimedia-photo-viewer.svg"))
        {
            comboBenchScripts.append(item);
        }
    }

    for (QListWidgetItem* item : comboBenchScripts)
    {
        ui->list_bench->addItem(item);
    }
}

void MainWindow::on_combo_cache_currentIndexChanged(int index)
{
    helpercache = index;
    settings.setValue("HelperCache", helpercache);
}

void MainWindow::on_time_update_timeChanged(const QTime &time)
{
    if (time.isValid()) {
        timeupdate = time;
        settings.setValue("TimeUpdate", timeupdate.toString("HH:mm"));
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_time_timeout_timeChanged(const QTime &time)
{
    if (time.isValid()) {
        timeout = time;
        settings.setValue("TimeUpdate", timeout.toString("HH:mm:ss"));
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_time_tea_timeChanged(const QTime &time)
{
    if (time.isValid()) {
        timetea = time;
        settings.setValue("TimeTea", timetea.toString("HH:mm")); // Сохраняем значение времени в настройках

        int msecsToTea = QTime(0, 0).msecsTo(timetea);

        if (msecsToTea == 0) {
            teaTimer->stop();
            ui->action_tea->setText(tr("Заваривание чая (выкл.)"));
        } else {
            teaTimer->setInterval(msecsToTea);
            teaTimer->start();
            ui->action_tea->setText(tr("Заваривание чая (каждые %1)").arg(timetea.toString("HH:mm")));
        }
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_time_work_timeChanged(const QTime &time)
{
    if (time.isValid()) {
        timework = time;
        settings.setValue("TimeWork", timework.toString("HH:mm"));

        int msecsToTea = QTime(0, 0).msecsTo(timework);

        if (msecsToTea == 0) {
            workTimer->stop();
            ui->action_work->setText(tr("Зарядка (выкл.)"));
        } else {
            workTimer->setInterval(msecsToTea);
            workTimer->start();
            ui->action_work->setText(tr("Зарядка (каждые %1)").arg(timework.toString("HH:mm")));
        }
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_line_tea_textChanged(const QString &arg1)
{
    teatext.reset(new QString(arg1));
    settings.setValue("TeaText", arg1);
}

void MainWindow::on_line_work_textChanged(const QString &arg1)
{
    worktext.reset(new QString(arg1));
    settings.setValue("WorkText", arg1);
}

void MainWindow::on_check_trayon_stateChanged(int arg1)
{
    trayon = arg1;
    settings.setValue("TrayOn", arg1);
}

void MainWindow::on_check_autostart_stateChanged(int arg1)
{
    settings.setValue("Autostart", arg1);

    QString autostartFilePath = QDir::homePath() + "/.config/autostart/klaus.desktop";

    if (arg1 == 0) {
        if (QFile::exists(autostartFilePath)) {
            QFile::remove(autostartFilePath);
        }
    } else {
        if (!QFile::exists(autostartFilePath)) {
            QString sourceFilePath = "/usr/share/applications/klaus.desktop";
            QFile::link(sourceFilePath, autostartFilePath);
        }
    }
}

void MainWindow::on_check_cacheremove_stateChanged(int arg1)
{
    cacheremove = arg1;
    settings.setValue("CacheRemove",arg1);
}

void MainWindow::on_check_repair_stateChanged(int arg1)
{
    repair = arg1;
    settings.setValue("RepairBackup",arg1);
}

void MainWindow::on_check_animload_stateChanged(int arg1)
{
    animload = arg1;
    settings.setValue("AnimLoad", arg1);
}

void MainWindow::on_check_updateinstall_stateChanged(int arg1)
{
    updinst = arg1;
    settings.setValue("UpdateInstall", arg1);
}

void MainWindow::on_check_clearinstall_stateChanged(int arg1)
{
    clearinstall = arg1;
    settings.setValue("ClearInstall", arg1);
}

void MainWindow::on_dial_volnotify_valueChanged(int value)
{
    volumenotify = value;
    settings.setValue("VolumeNotify", value);
    QString labelvolnotify = QString(tr("Звук уведомлений: %1/100")).arg(volumenotify);
    ui->label_volnotify->setText(labelvolnotify);
}

void MainWindow::on_check_saveurl_stateChanged(int arg1)
{
    saveurl = arg1;
    settings.setValue("SaveURL",arg1);
}

void MainWindow::handleListItemClicked(QListWidgetItem *item, const QString& scriptDir)
{
    if (item) {
        QString msg = item->data(Qt::UserRole).toString();

        QString scriptPath = "";
        QString itemName = item->text();

        QDir dir(scriptDir);
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
        for (const auto& fileInfo : fileInfoList) {
            QFile scriptFile(fileInfo.absoluteFilePath());
            if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream scriptStream(&scriptFile);
                while (!scriptStream.atEnd()) {
                    QString line = scriptStream.readLine();
                    if (line.startsWith("#name_" + *lang)) {
                        QString name = line.mid(12).trimmed();
                        if (name == itemName)
                            scriptPath = fileInfo.absoluteFilePath();
                    }
                    else if (line.startsWith("#msg_" + *lang))
                        msg = line.mid(11).trimmed();
                }
                scriptFile.close();
            }
            if (!scriptPath.isEmpty())
                break;
        }

        if (scriptPath.isEmpty())
            scriptPath = scriptDir + itemName;

        if (scriptDir == mainDir + "cfg/" || scriptDir == mainDir + "journals/")
            ui->details_journal->setText(msg);
        else if (scriptDir == mainDir + "sh/")
            ui->details_sh->setText(msg);
        else if (scriptDir == mainDir + "bench/")
            ui->details_bench->setText(msg);
    }
}

void MainWindow::handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir) {
    Terminal terminal = getTerminal();

    if (item == orphanButton) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Обычно, когда пакет становится сиротой, это означает, что он был установлен в качестве зависимости другого пакета, но этот пакет был удален, и больше нет других пакетов, которые бы зависели от данного. Удаление сирот из системы помогает поддерживать систему более чистой и оптимизированной. Вы действительно хотите удалить пакеты сироты?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString remove = packageCommands.value(pkg).value("remove_explicit").join(" ");
            QString removelist = packageCommands.value(pkg).value("query_depends").join(" ");
            QString command = remove + " $(" + removelist + ")";
            QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "sh" << "-c" << command);
        }
        return;
    }

    if (item == cacheButtonHelper || item == cacheButtonPacman) {
        QString command;
        QMessageBox::StandardButton replymod = QMessageBox::No;
        switch (helpercache) {
            case 0: {
                replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш только неустановленных пакетов (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                command = (item == cacheButtonHelper) ? packageCommands.value(pkg).value("clean_cache").join(" ") : "sudo pacman -Sc";
                break;
            }
            case 1: {
                replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш пакетов всех пакетов (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                command = (item == cacheButtonHelper) ? packageCommands.value(pkg).value("clean_all_cache").join(" ") : "sudo pacman -Scc";
                break;
            }
            case 2: {
                if(item == cacheButtonHelper)
                    sendNotification(tr("Ошибка"), tr("Yay и Paru так не умеют, измените настройки удаления кэша"));
                else
                {
                    replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш всех пакетов кроме последних трех версий (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                    command = "paccache -rvk3";
                }
                break;
            }
        }

        if (replymod == QMessageBox::Yes)
            QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << command);

        return;
    }

    QString scriptPath;
    QString itemName = item->text();

    QDir dir(scriptDir);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    for (const auto& fileInfo : fileInfoList) {
        QFile scriptFile(fileInfo.absoluteFilePath());
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd()) {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang)) {
                    QString name = line.mid(12).trimmed();
                    if (name == itemName)
                        scriptPath = fileInfo.absoluteFilePath();
                }
            }
            scriptFile.close();
        }
        if (!scriptPath.isEmpty())
            break;
    }

    if (scriptPath.isEmpty())
        scriptPath = scriptDir + itemName;


    if (hasUpdates && updinst == 2 && page == 2) {
        sendNotification(tr("Внимание"), tr("Вначале требуется обновить систему до актуального состояния! Это поможет предотвратить конфликт зависимостей и избежать кучи других проблем!"));
        return;
    }
    QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "bash" << scriptPath << *lang << helper);
}

void MainWindow::on_list_sh_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = mainDir + "sh/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_journal_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = mainDir + "journals/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_cfg_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = mainDir + "cfg/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_bench_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = mainDir + "bench/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_clear_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = mainDir + "clear/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_repair_itemDoubleClicked(QListWidgetItem *item)
{
    QString folderName = item->text();
    QString folderPath;

    if (folderName.startsWith('.'))
        folderPath = QDir::homePath() + "/";
    else
        folderPath = QDir::homePath() + "/.config/";

    createArchive(folderPath, folderName);
}

void MainWindow::on_list_journal_itemClicked(QListWidgetItem *item)
{
    QString scriptDir = mainDir + "journals/";
    handleListItemClicked(item, scriptDir);
}

void MainWindow::on_list_cfg_itemClicked(QListWidgetItem *item)
{
    QString scriptDir = mainDir + "cfg/";
    handleListItemClicked(item, scriptDir);
}

void MainWindow::on_list_sh_itemClicked(QListWidgetItem *item)
{
    QString scriptDir = mainDir + "sh/";
    handleListItemClicked(item, scriptDir);
}

void MainWindow::on_list_bench_itemClicked(QListWidgetItem *item)
{
    QString scriptDir = mainDir + "bench/";
    handleListItemClicked(item, scriptDir);
}

bool runPkexecCommand(const QString& command)
{
    QProcess process;
    process.start("pkexec", QStringList() << "--disable-internal-agent" << "sh" << "-c" << command);
    process.waitForFinished();

    if (process.exitCode() == 0)
        return true;
    else
        return false;
}

void MainWindow::on_push_pacman_clicked()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Pacman Repository Editor"));
    dialog->setFixedSize(500, 600);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QPlainTextEdit* editor = new QPlainTextEdit(dialog);

    QFile pacmanConfFile("/etc/pacman.conf");
    if (pacmanConfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&pacmanConfFile);
        QString pacmanConfText = in.readAll();
        editor->setPlainText(pacmanConfText);
        pacmanConfFile.close();
    }
    else
    {
        sendNotification(tr("Ошибка"), tr("Не удалось открыть pacman.conf"));
        delete dialog;
        return;
    }

    layout->addWidget(editor);

    dialog->setLayout(layout);
    dialog->setStyleSheet("QWidget{background-color:#2d2b79;} QLineEdit,QTextEdit{background-color:#21205b;padding:10px;border-radius:10px;} QLabel{color:#fff;font-size:10pt;}QPushButton{border-radius:10px;padding:5px 20px;background-color:#916ee4;color:#fff;}");

    QPushButton* saveButton = new QPushButton(tr("Сохранить"), dialog);
    layout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QString command = "sh -c 'cat > /etc/pacman.conf << EOF\n" + editor->toPlainText() + "\nEOF\n'";

        if (runPkexecCommand(command))
            dialog->accept();
        else
            sendNotification(tr("Ошибка"), tr("Не удалось выполнить аутентификацию"));

    });

    editor->setParent(dialog);
    layout->setParent(dialog);
    dialog->show();
}

void MainWindow::on_push_kde_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите полностью сбросить конфигурацию DE? Вам придется заново все настроить."), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    Terminal terminal = getTerminal();
    QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "sudo" << "rm" << QDir::homePath() + "/.config/kdeglobals");
}

void MainWindow::on_action_like_triggered()
{
    if (auth == 0)
    {
        sendNotification(tr("Ошибка"), tr("Для участия в голосовании пройдите авторизацию в настройках!"));
        return;
    }
}

void MainWindow::on_action_imgpkg_triggered(bool checked)
{
    if (checked)
        ui->tabWidget_details->setCurrentIndex(1);
    else
        ui->tabWidget_details->setCurrentIndex(0);
}


void MainWindow::on_action_updatelist_triggered()
{
    if (page == 2)
    {
        miniAnimation(true, ui->list_aur);

        ui->list_aur->clear();
        ui->action_imgpkg->setEnabled(false);
        ui->action_imgpkg->setChecked(false);

        QTimer::singleShot(500, this, [=]() {
            list = 0;
            loadContent(0, true);

            ui->action_imgpkg->setEnabled(false);
            ui->action_imgpkg->setChecked(false);

            ui->action_like->setEnabled(false);
            ui->details_aur->setHtml(detailsAURdefault);

            miniAnimation(false, ui->list_aur);
        });
    }

    if (page == 4)
    {
        miniAnimation(true, ui->list_app);
        ui->list_app->clear();

        QTimer::singleShot(500, this, [=]() {
            ui->details_aurpkg->setText(tr("Ничего не выбрано"));
            miniAnimation(false, ui->list_app);
            loadContentInstall();
        });
    }
}
