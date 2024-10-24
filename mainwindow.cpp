//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AnimationHelper.h"
#include "customtextedit.h"
#include <QtWebEngineWidgets>
#include <QSoundEffect>
#include <QRegularExpression>
#include <QCompleter>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QtConcurrent>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString mainDir = QDir::homePath() + "/.config/kLaus/";
QString filePath = mainDir + "settings.ini";
QString currentVersion = "18.9";
QString packagesArchiveAUR = "steam";
QString packagesArchiveDefault = "packages";
QString packagesArchiveCat = packagesArchiveDefault;

QSettings settings(filePath, QSettings::IniFormat);

QString lockFilePath = "/var/lib/pacman/db.lck";
QString description = "no";

//proxy
QString https;
QString http;
int proxy = 0;

QString ignoredpkg;

int pkg = 0; //пакетный менеджер 0-yay
int page = 0; // какая страница используется
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
int auth = 0;
int saveurl = 2;
int trans = 2; //перевод описания
int colorlist = 2; //раскрашивать листы
int timerupdpkg = 2;

int currentTabIndex;

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
            {"query_list_files", {"yay", "-Fl"}},
            {"show_info", {"yay", "-Si"}},
            {"search", {"yay", "-Ss"}},
            {"query_q", {"yay", "-Qq"}},
            {"remove_explicit", {"yay", "-Rs"}},
            {"query_depends", {"yay", "-Qdtq"}},
            {"clean_cache", {"yay", "-Sc"}},
            {"clean_all_cache", {"yay", "-Scc"}},
            {"localinstall", {"yay", "-Udd"}},
            {"like", {"yay", "-Wv"}},
            {"dislike", {"yay", "-Wu"}},
            {"query_explicit", {"yay", "-Qeq"}},
            {"query", {"yay", "-Qq"}},
            {"query_dependencies", {"yay", "-Qdq"}},
            {"query_unused_dependencies", {"yay", "-Qdtq"}},
            {"query_aur_packages", {"yay", "-Qmq"}},
            {"query_official_packages", {"yay", "-Qnq"}}
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
    mrpropper(2, ui->action_2);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск по пакетам..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(1);
    ui->action_updatelist->setVisible(true);
    ui->action_4->setVisible(true);
    ui->action_30->setVisible(true);
    ui->action_34->setVisible(true);
    ui->action_infopkg->setVisible(true);
}

void MainWindow::on_action_7_triggered()
{
    mrpropper(4, ui->action_7);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск установленных пакетов..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(2);
    ui->action_allpkg->setVisible(true);
    ui->action_updatelist->setVisible(true);
    ui->action_11->setVisible(true);
    ui->action_5->setVisible(true);
    ui->action_6->setVisible(true);
    ui->action_34->setVisible(true);
    ui->action_infopkg_pkg->setVisible(true);
}

void MainWindow::on_action_17_triggered()
{
    mrpropper(3, ui->action_17);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск скриптов..."));
    ui->searchLineEdit->setVisible(true);
    ui->action_addsh->setVisible(true);
    ui->action_editsh->setVisible(true);
    ui->action_rmsh->setVisible(true);
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::on_action_9_triggered()
{
    mrpropper(5, ui->action_9);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск журналов и конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->action_27->setVisible(true);
    ui->action_bench->setVisible(true);
    ui->action_repair->setVisible(true);
    ui->action_grub->setVisible(true);
    ui->action_pacman->setVisible(true);
    ui->action_fstab->setVisible(true);
    on_list_journal_itemSelectionChanged();
    on_action_27_triggered();
}

void MainWindow::on_action_3_triggered()
{
    if (page == 6) return;
    mrpropper(6, ui->action_3);

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
    } else
        ui->searchLineEdit->setText(aurUrl);

    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(15);
}

void MainWindow::on_action_8_triggered() {
    if (page == 7) return;

    mrpropper(7, ui->action_8);
    auto process = QSharedPointer<QProcess>::create();
    process->start(packageCommands.value(pkg).value("query").at(0),
                   QStringList() << packageCommands.value(pkg).value("query").at(1) << "ocs-url");

    if (process->waitForFinished() && !process->readAllStandardOutput().contains("ocs-url")) {
        ui->action_2->trigger();
        ui->searchLineEdit->setText("ocs-url");
        QCoreApplication::postEvent(ui->searchLineEdit, new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
        sendNotification(tr("Ошибка"), tr("Установите пакет ocs-url для установки тем!"));
        return;
    }

    bool isBlankPage = ui->webEngineView_custom->url().toString() == "about:blank";
    showLoadingAnimation(isBlankPage, ui->webEngineView_custom);

    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(14);

    if (isBlankPage) {
        QString url;
        if (QUrl(*customurl).isValid() && saveurl)
            url = *customurl;
        else {
            const QMap<QString, QString> urls {
                {"KDE", "https://store.kde.org/browse/"},
                {"GNOME", "https://www.pling.com/s/Gnome/browse/"},
                {"XFCE", "https://www.pling.com/s/XFCE/browse/"},
                {"LXQt", "https://store.kde.org/browse?cat=446"},
                {"Cinnamon", "https://www.pling.com/s/Cinnamon/browse/"},
                {"MATE", "https://www.pling.com/s/Mate/browse/"},
                {"Enlightenment", "https://www.pling.com/s/Enlightenment/browse/"}
            };

            if (urls.contains(*currentDesktop))
                url = urls[*currentDesktop];
            else {
                sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
                return;
            }
            *customurl = url;
            ui->action_35->setEnabled(false);
        }
        ui->webEngineView_custom->setUrl(QUrl(url));
    } else
        ui->searchLineEdit->setText(ui->webEngineView_custom->url().toString());
}

void MainWindow::on_push_vk_clicked()
{
    mrpropper(8, ui->action_10);
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->webEngineView->setUrl(QUrl("https://vk.com/linux2"));
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_action_12_triggered()
{
    mrpropper(9, ui->action_12);
    ui->tabWidget->setCurrentIndex(7);
}

void MainWindow::on_action_host_triggered() {
    if (page == 10) return;

    mrpropper(10, ui->action_host);
    auto process = QSharedPointer<QProcess>::create();
    process->start(packageCommands.value(pkg).value("query").at(0),
                   QStringList() << packageCommands.value(pkg).value("query").at(1) << "apache");

    if (process->waitForFinished() && !process->readAllStandardOutput().contains("apache")) {
        ui->action_2->trigger();
        ui->searchLineEdit->setText("apache");
        QCoreApplication::postEvent(ui->searchLineEdit, new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
        sendNotification(tr("Ошибка"), tr("Установите пакет apache для управления веб-сервером!"));
        return;
    }

    bool isBlankPage = (ui->webEngineView_host->url().toString() == "about:blank");
    showLoadingAnimation(isBlankPage, ui->webEngineView_host);

    ui->action_5->setVisible(true);
    ui->action_restart->setVisible(true);
    ui->action_stop->setVisible(true);
    ui->action_catalog->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(13);

    ui->webEngineView_host->setUrl(isBlankPage ? QUrl("http://localhost") : ui->webEngineView_host->url());
    ui->searchLineEdit->setText(isBlankPage ? "http://localhost" : ui->webEngineView_host->url().toString());
}

void MainWindow::on_action_game_triggered() {
    if (page == 11) return;

    mrpropper(11, ui->action_game);
    bool isBlankPage = (ui->webEngineView_game->url().toString() == "about:blank");
    if (isBlankPage) showLoadingAnimation(true, ui->webEngineView_game);

    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);
    ui->searchLineEdit->setPlaceholderText(tr("Введите URL адрес..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(0);

    if (isBlankPage) {
        QUrl url = QUrl(*gameurl).isValid() && saveurl ? QUrl(*gameurl) : QUrl("https://www.protondb.com/explore");
        ui->webEngineView_game->setUrl(url);
        ui->searchLineEdit->setText(url.toString());
        *gameurl = url.toString();
        if (url.toString() == "https://www.protondb.com/explore") ui->action_35->setEnabled(false);
    } else
        ui->searchLineEdit->setText(ui->webEngineView_game->url().toString());
}

void MainWindow::on_action_downgrade_triggered()
{
    mrpropper(14, ui->action_downgrade);
    ui->searchLineEdit->setPlaceholderText(tr("Поиск по архиву..."));
    ui->searchLineEdit->setVisible(true);
    ui->tabWidget->setCurrentIndex(12);
    ui->action_updatelist->setVisible(true);
}

//---#####################################################################################################################################################
//--################################################################## БЫСТРЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################
void MainWindow::on_action_restart_triggered()
{
    if (currentProcess && currentProcess->state() == QProcess::Running) {
        currentProcess->kill();

        disconnect(currentProcess.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
        disconnect(currentProcess.data(), &QProcess::finished, nullptr, nullptr);
    }

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
    if (currentProcess && currentProcess->state() == QProcess::Running) {
        currentProcess->kill();

        disconnect(currentProcess.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
        disconnect(currentProcess.data(), &QProcess::finished, nullptr, nullptr);
    }

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
    originalLabelText = ui->label1->text();
    ui->searchLineEdit->setPlaceholderText(tr("Поиск журналов и конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(false);
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::on_action_bench_triggered()
{
    page = 112;
    ui->label1->setText(tr("Бенчмарки"));
    originalLabelText = ui->label1->text();
    ui->searchLineEdit->setPlaceholderText(tr("Поиск бенчмарков..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(true);
    ui->tabWidget->setCurrentIndex(10);
}

void MainWindow::loadConfigFile(const QString &filePath, QTextEdit *targetTextEdit, int tabIndex)
{
    ui->searchLineEdit->setVisible(false);

    auto highlighter = new MySyntaxHighlighter(targetTextEdit->document());
    highlighter->setParent(this);

    QFile configFile(filePath);
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&configFile);
        QString configText = in.readAll();
        targetTextEdit->setPlainText(configText);
        configFile.close();
    }
    else
        targetTextEdit->setPlainText(tr("Не удалось открыть файл: %1").arg(filePath));

    ui->tabWidget->setCurrentIndex(tabIndex);
}

void MainWindow::on_action_grub_triggered()
{
    ui->label1->setText(tr("Настройки GRUB"));
    originalLabelText = ui->label1->text();

    loadConfigFile("/etc/default/grub", ui->text_grub, 8);
}

void MainWindow::on_action_pacman_triggered()
{
    ui->label1->setText(tr("Настройки Pacman"));
    originalLabelText = ui->label1->text();

    loadConfigFile("/etc/pacman.conf", ui->text_pacman, 16);
}

void MainWindow::on_action_fstab_triggered()
{
    ui->label1->setText(tr("Настройки файловой системы"));
    originalLabelText = ui->label1->text();

    loadConfigFile("/etc/fstab", ui->text_fstab, 17);
}

void MainWindow::on_action_repair_triggered()
{
    page = 113;
    ui->label1->setText(tr("Оптимизация"));
    originalLabelText = ui->label1->text();
    ui->searchLineEdit->setPlaceholderText(tr("Поиск конфигураций..."));
    ui->searchLineEdit->setVisible(true);
    ui->combo_bench->setVisible(false);
    ui->tabWidget->setCurrentIndex(11);
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
        default:
            return;
    }
}

void MainWindow::on_action_34_triggered()
{
    QListWidget* listWidget = nullptr;

    if (page == 2)
        listWidget = ui->list_aur;
    else if (page == 4) {
        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }
    } else
        return;

    if (listWidget == nullptr || listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для просмотра информации!"));
        return;
    }

    QListWidgetItem *currentItem = listWidget->currentItem();
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));

    QString packageName;

    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    QStringList command;

    command = packageCommands.value(0).value("show_info");
    command << packageName;

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    process->start(command.at(0), command.mid(1));
    process->waitForFinished();

    QString packageInfo = QString::fromUtf8(process->readAllStandardOutput());
    static QRegularExpression urlRegex("URL\\s+:\\s+(\\S+)");
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

    ui->action_34->setVisible(true);
    ui->action_11->setVisible(page == 4);
    ui->action_allpkg->setVisible(page == 4);
    ui->action_35->setVisible(false);
    ui->action_updatelist->setVisible(true);

    if (page == 2) {
        ui->action_infopkg->setVisible(true);
        ui->tabWidget->setCurrentIndex(1);
    }
    else if (page == 4)
    {
        ui->action_infopkg_pkg->setVisible(true);
        ui->tabWidget->setCurrentIndex(2);
    }

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
        QFile lockFile(lockFilePath);
        if (lockFile.exists()) {
            sendNotification(tr("Внимание"), tr("Pacman уже используется! Завершите все операции в Pacman и попробуйте снова!"));
            return;
        }

        Terminal terminal = getTerminal();
        currentTerminalProcess = QSharedPointer<QProcess>::create(this);

        connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
            UpdateIcon();
        });

        QStringList arguments;
        arguments << terminal.args;
        arguments << packageCommands.value(pkg).value("update");

        if (!ignoredpkg.isEmpty())
            arguments << "--ignore=" + ignoredpkg;

        currentTerminalProcess->setProgram(terminal.binary);
        currentTerminalProcess->setArguments(arguments);
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
        QListWidget* listWidget = nullptr;

        if (page == 2)
            listWidget = ui->list_aur;
        else if (page == 4) {
            switch (ui->tabWidget_pkg->currentIndex()) {
                case 0:
                    listWidget = ui->list_app;
                    break;
                case 1:
                    listWidget = ui->list_sysapp;
                    break;
                case 2:
                    listWidget = ui->list_dependencies;
                    break;
                case 3:
                    listWidget = ui->list_unused_dependencies;
                    break;
                case 4:
                    listWidget = ui->list_aur_packages;
                    break;
                case 5:
                    listWidget = ui->list_official_packages;
                    break;
                default:
                    return;
            }
        } else
            return;

        if (listWidget == nullptr || listWidget->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
            return;
        }

        QListWidgetItem *currentItem = listWidget->currentItem();
        CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));

        QString basePackageName;

        if (itemWidget)
            basePackageName = itemWidget->getPackageName();
        else
            basePackageName = currentItem->text();

        QString packageName = basePackageName.section('/', -1);

        QString desktopFilePath;

        QProcess process;
        process.start(packageCommands.value(pkg).value("list_files").at(0), {packageCommands.value(pkg).value("list_files").at(1), packageName});
        process.waitForFinished(-1);

        QString output = process.readAllStandardOutput();
        const QStringList lines = output.split('\n');

        for (auto it = lines.cbegin(); it != lines.cend(); ++it) {
            const QString& line = *it;
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
    else if (page == 4) {
        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }
    } else
        return;

    if (listWidget == nullptr || listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
        return;
    }

    QFile lockFile(lockFilePath);
    if (lockFile.exists()) {
        sendNotification(tr("Внимание"), tr("Pacman уже используется! Завершите все операции в Pacman и попробуйте снова!"));
        return;
    }

    QListWidgetItem *currentItem = listWidget->currentItem();
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));

    QString packageName;

    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    QStringList parts = packageName.split('/');
    if (parts.size() > 1)
        packageName = parts.last();

    Terminal terminal = getTerminal();

    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        QListWidget* listWidget = nullptr;

        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }

        miniAnimation(true, listWidget);
        listWidget->clear();

        QTimer::singleShot(500, this, [=]() {
            miniAnimation(false, listWidget);
            loadContentInstall();
        });
    });
    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("remove") << packageName);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->start();
}

void MainWindow::on_action_4_triggered()
{
    QListWidget* listWidget = nullptr;

    if (page == 2)
        listWidget = ui->list_aur;
    else if (page == 4) {
        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }
    } else
        return;

    if (listWidget == nullptr || listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
        return;
    }

    QFile lockFile(lockFilePath);
    if (lockFile.exists()) {
        sendNotification(tr("Внимание"), tr("Pacman уже используется! Завершите все операции в Pacman и попробуйте снова!"));
        return;
    }

    if (hasUpdates && updinst == 2 && page == 2) {
        if (QMessageBox::question(this, tr("Внимание"), tr("Обновить систему до последней версии, чтобы предотвратить конфликты между зависимостями?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) on_action_11_triggered();
    }

    QListWidgetItem *currentItem = listWidget->currentItem();
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));

    QString packageName;

    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    Terminal terminal = getTerminal();

    if (clearinstall && (pkg == 0))
        removeDirectory(QDir::homePath() + "/.cache/yay/" + packageName);

    currentTerminalProcess = QSharedPointer<QProcess>::create(this);
    connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        QListWidget* listWidget = nullptr;

        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }

        miniAnimation(true, listWidget);
        listWidget->clear();

        QTimer::singleShot(500, this, [=]() {
            miniAnimation(false, listWidget);
            loadContentInstall();
        });
    });

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (proxy == 2) {
        env.insert("https_proxy", https);
        env.insert("http_proxy", http);
    }

    QStringList arguments;
    arguments << terminal.args;
    arguments << packageCommands.value(pkg).value("install");
    arguments << packageName;

    currentTerminalProcess->setProgram(terminal.binary);
    currentTerminalProcess->setArguments(arguments);
    currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
    currentTerminalProcess->setEnvironment(env.toStringList());
    currentTerminalProcess->start();
}

void MainWindow::on_action_30_triggered()
{
    if (ui->list_aur->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
        return;
    }

    QFile lockFile(lockFilePath);
    if (lockFile.exists()) {
        sendNotification(tr("Внимание"), tr("Pacman уже используется! Завершите все операции в Pacman и попробуйте снова!"));
        return;
    }

    if (hasUpdates && updinst == 2 && page == 2) {
        if (QMessageBox::question(this, tr("Внимание"), tr("Обновить систему до последней версии, чтобы предотвратить конфликты между зависимостями?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) on_action_11_triggered();
    }

    QListWidgetItem *currentItem = ui->list_aur->currentItem();
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(currentItem));

    QString packageName;

    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    Terminal terminal = getTerminal();
    QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "bash" << mainDir + "sh/PKGBUILD/PKGBUILD.sh" << *lang << helper << packageName);
}

void MainWindow::on_list_aur_itemSelectionChanged() {
    if (ui->list_aur->selectedItems().isEmpty()) {
        lastSelectedRow = -1;
        lastSelectedPackage.clear();

        ui->action_infopkg->setChecked(false);
        ui->tabWidget_details->setCurrentIndex(0);
        ui->details_aur->setHtml(detailsAURdefault);

        actionInfopkgPressed = false;
    }
}

void MainWindow::on_list_journal_itemSelectionChanged() {
    if (ui->list_journal->selectedItems().isEmpty()) {
        ui->details_wiki->setVisible(false);
        ui->label_wiki->setVisible(false);
        ui->graphics_screen->setVisible(false);
        ui->label_screen->setVisible(false);
    }
}

void MainWindow::on_list_aurpkg_itemSelectionChanged()
{
    QListWidget* listWidget = nullptr;

    switch (ui->tabWidget_pkg->currentIndex()) {
        case 0:
            listWidget = ui->list_app;
            break;
        case 1:
            listWidget = ui->list_sysapp;
            break;
        case 2:
            listWidget = ui->list_dependencies;
            break;
        case 3:
            listWidget = ui->list_unused_dependencies;
            break;
        case 4:
            listWidget = ui->list_aur_packages;
            break;
        case 5:
            listWidget = ui->list_official_packages;
            break;
        default:
            return;
    }

    if (listWidget->selectedItems().isEmpty()) {
        ui->action_infopkg_pkg->setChecked(false);
        ui->tabWidget_details_pkg->setCurrentIndex(0);
        ui->details_aurpkg->setHtml(tr("Ничего не выбрано"));
    }
}

void MainWindow::on_action_infopkg_triggered(bool checked)
{
    QListWidgetItem *currentItem = ui->list_aur->currentItem();
    if (currentItem == nullptr) {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
            action->setChecked(!checked);

        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для составления файлов пакета!"));
        return;
    }

    QString packageName;
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(currentItem));
    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    actionInfopkgPressed = checked;

    if (checked) {
        ui->tabWidget_details->setCurrentIndex(1);
        ui->action_infopkg->setText(tr("Описание пакета"));
        prepareFiles(ui->tree_aur, packageName);
    } else {
        ui->tabWidget_details->setCurrentIndex(0);
        ui->action_infopkg->setText(tr("Файлы пакета"));
        prepareDetails(ui->list_aur, ui->details_aur, packageName);
    }
}

void MainWindow::on_action_infopkg_pkg_triggered(bool checked)
{
    QListWidget* listWidget = nullptr;

    switch (ui->tabWidget_pkg->currentIndex()) {
        case 0:
            listWidget = ui->list_app;
            break;
        case 1:
            listWidget = ui->list_sysapp;
            break;
        case 2:
            listWidget = ui->list_dependencies;
            break;
        case 3:
            listWidget = ui->list_unused_dependencies;
            break;
        case 4:
            listWidget = ui->list_aur_packages;
            break;
        case 5:
            listWidget = ui->list_official_packages;
            break;
        default:
            return;
    }

    QListWidgetItem *currentItem = listWidget->currentItem();
    if (listWidget->currentItem() == nullptr) {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
            action->setChecked(!checked);

        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для составления файлов пакета!"));
        return;
    }

    QString packageName;
    CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));
    if (itemWidget)
        packageName = itemWidget->getPackageName();
    else
        packageName = currentItem->text();

    actionInfopkgPkgPressed = checked;

    if (checked) {
        ui->tabWidget_details_pkg->setCurrentIndex(1);
        ui->action_infopkg_pkg->setText(tr("Описание пакета"));

        prepareFiles(ui->tree_files_pkg, packageName);
    } else {
        ui->tabWidget_details_pkg->setCurrentIndex(0);
        ui->action_infopkg_pkg->setText(tr("Файлы пакета"));

        prepareDetails(listWidget, ui->details_aurpkg, packageName);
    }
}

void MainWindow::on_action_addsh_triggered()
{
    QDialog* dialog = new QDialog(this);

    dialog->setMinimumSize(530, 620);
    dialog->setWindowTitle(tr("Добавить скрипт"));
    dialog->setObjectName("addDialog");

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
    dialog->setStyleSheet("QWidget#addDialog{background:#322F85 url(:/img/system.jpeg) repeat;} QLineEdit,QTextEdit{background:#32554b;padding:10px;border-radius:10px;}");

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

    const QFileInfoList fileList = dir.entryInfoList(filter);
    for (auto it = fileList.cbegin(); it != fileList.cend(); ++it) {
        const QFileInfo& fileInfo = *it;
        QString filePath = fileInfo.filePath();
        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd()) {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + *lang)) {
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

    const QFileInfoList fileList = dir.entryInfoList(filter);

    for (const QFileInfo& fileInfo : fileList) {
        QString filePath = fileInfo.filePath();
        QString fileName = fileInfo.fileName();
        QFile scriptFile(filePath);

        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream scriptStream(&scriptFile);

            while (!scriptStream.atEnd()) {
                QString line = scriptStream.readLine();

                if (line.startsWith("#name_" + *lang)) {
                    QString itemName = line.mid(12).trimmed();

                    if (itemName == itemContent) {
                        scriptFile.close();

                        QDialog* editDialog = new QDialog(this);

                        editDialog->setMinimumSize(530, 620);
                        editDialog->setWindowTitle(tr("Редактировать скрипт"));
                        editDialog->setObjectName("editDialog");

                        QLabel* nameLabel = new QLabel(tr("Имя файла:"));
                        QLineEdit* nameEdit = new QLineEdit(fileName);
                        QLabel* scriptLabel = new QLabel(tr("Тело скрипта:"));
                        QTextEdit* scriptEdit = new QTextEdit();
                        QPushButton* saveButton = new QPushButton(tr("Сохранить"));

                        scriptEdit->setPlainText(getScriptContent(filePath));

                        QVBoxLayout* layout = new QVBoxLayout(editDialog);
                        editDialog->setStyleSheet("QWidget#editDialog{background:#322F85 url(:/img/system.jpeg) repeat;} QLineEdit,QTextEdit{background:#32554b;padding:10px;border-radius:10px;}");
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

                            if (newScriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
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

    QList<QListWidget*> listWidgets = {
        ui->list_app,
        ui->list_sysapp,
        ui->list_dependencies,
        ui->list_unused_dependencies,
        ui->list_aur_packages,
        ui->list_official_packages
    };

    for (QListWidget* listWidget : listWidgets) {
        connect(listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showListContextMenu);
    }

    connect(ui->tree_aur, &QListWidget::customContextMenuRequested, this, &MainWindow::showListContextMenu);
    connect(ui->tree_files_pkg, &QListWidget::customContextMenuRequested, this, &MainWindow::showListContextMenu);
}

bool MainWindow::checkIfPackageIsInstalled(const QString& packageName) {
    QString basePackageName = packageName.section('/', -1);

    for (int i = 0; i < ui->list_sysapp->count(); ++i) {
        QListWidgetItem* item = ui->list_sysapp->item(i);
        if (!item) continue;

        QString installedPackageName = item->text();
        QString installedBasePackageName = installedPackageName.section('/', -1);

        if (installedBasePackageName == basePackageName)
            return true;
    }
    return false;
}

QStringList MainWindow::readFavoritePackages() {
    QString favoriteFilePath = mainDir + "menu/Favorite/Favorite.txt";
    QFile file(favoriteFilePath);
    QStringList favorites;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool headerFound = false;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line == "[Favorite]")
                headerFound = true;
            else if (headerFound && !line.isEmpty())
                favorites.append(line);
        }
        file.close();
    }

    return favorites;
}

void MainWindow::updateFavoritePackages(const QStringList& favorites) {
    QString favoriteFilePath = mainDir + "menu/Favorite/Favorite.txt";
    QFile file(favoriteFilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        sendNotification(tr("Ошибка"), tr("Не удалось открыть файл Favorite.txt для записи"));
        return;
    }

    QTextStream out(&file);

    if (!favorites.isEmpty()) {
        out << "[Favorite]\n";
        out << favorites.join("\n") << "\n"; // Записываем пакеты
    }

    file.close();
}

void MainWindow::showListContextMenu(const QPoint& pos) {
    QWidget* senderWidget = qobject_cast<QWidget*>(sender());
    if (!senderWidget) return;

    QListWidget* listWidget = qobject_cast<QListWidget*>(senderWidget);
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(senderWidget);

    if (!listWidget && !treeWidget) return;

    QString basePackageName;
    QString packageName;
    QString currentPath;
    bool isFile = false;

    if (listWidget) {
        QListWidgetItem* item = listWidget->itemAt(pos);
        if (!item || item->data(Qt::UserRole + 1).toString().contains("category")) return;

        QListWidgetItem *currentItem = listWidget->currentItem();
        CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));

        if (itemWidget)
            basePackageName = itemWidget->getPackageName();
        else
            basePackageName = currentItem->text();

        packageName = basePackageName.section('/', -1);

        currentPath = getCurrentPathFromList(packageName);
    } else if (treeWidget) {
        QTreeWidgetItem* treeItem = treeWidget->itemAt(pos);
        if (!treeItem) return;
        packageName = treeItem->text(0);
        currentPath = getCurrentPathFromTree(treeItem);
        isFile = QFileInfo::exists(currentPath) && !QFileInfo(currentPath).isDir();
    }

    QStringList favorites = readFavoritePackages();
    bool isFavorite = favorites.contains(packageName);

    QMenu menu(this);
    auto addAction = [&](const QString& icon, const QString& text, auto slot) {
        QAction* action = new QAction(QIcon(icon), text, this);
        connect(action, &QAction::triggered, this, slot);
        menu.addAction(action);
    };

    bool isInstalled = listWidget ? checkIfPackageIsInstalled(packageName) : false;

    if (treeWidget) {
        QString pathToOpen = isFile ? QFileInfo(currentPath).absolutePath() : currentPath;

        QTreeWidgetItem* currentTreeItem = treeWidget->itemAt(pos);
        bool isExpanded = currentTreeItem && currentTreeItem->isExpanded();

        std::function<void(QTreeWidgetItem*, bool)> toggleExpansionRecursive = [&](QTreeWidgetItem* item, bool expand) {
            if (item) {
                item->setExpanded(expand);
                for (int i = 0; i < item->childCount(); ++i) {
                    toggleExpansionRecursive(item->child(i), expand);
                }
            }
        };

        addAction(isExpanded ? ":/img/minus.png" : ":/img/plus.png",
                  isExpanded ? tr("Свернуть ветку") : tr("Развернуть ветку"),
                  [=]() {
                      toggleExpansionRecursive(currentTreeItem, !isExpanded);
                  });

        menu.addSeparator();

        addAction(":/img/minus.png", tr("Свернуть все"), [=]() {
            treeWidget->collapseAll();
        });

        addAction(":/img/plus.png", tr("Развернуть все"), [=]() {
            treeWidget->expandAll();
        });

        menu.addSeparator();

        addAction(":/img/copy.png", tr("Копировать в буфер обмена"), [=]() {
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(currentPath);
            sendNotification(tr("Буфер обмена"), tr("Путь скопирован в буфер обмена: %1").arg(currentPath));
        });

        if(QDir(currentPath).exists() || isFile)
            addAction(":/img/48.png", tr("Перейти к расположению"), [=]() { openFolder(pathToOpen); });
    }

    if (listWidget) {
        if (!isInstalled) {
            addAction(":/img/25.png", tr("Найти в каталоге пакетов"), &MainWindow::on_action_searchpkg_triggered);
            menu.addSeparator();
            addAction(":/img/15.png", tr("Установить"), &MainWindow::on_action_4_triggered);
            addAction(":/img/27.png", tr("Изменить PKGBUILD и установить"), &MainWindow::on_action_30_triggered);
        } else if (isInstalled) {
            addAction(":/img/25.png", tr("Найти в каталоге пакетов"), &MainWindow::on_action_searchpkg_triggered);
            menu.addSeparator();
            addAction(":/img/13.png", tr("Запустить"), &MainWindow::on_action_5_triggered);
            addAction(":/img/15.png", tr("Обновить версию"), &MainWindow::on_action_4_triggered);
            addAction(":/img/14.png", tr("Удалить"), &MainWindow::on_action_6_triggered);
        }

        addAction(":/img/34.png", tr("Информация о пакете"), &MainWindow::on_action_34_triggered);

        menu.addSeparator();
        addAction(":/img/star.png", isFavorite ? tr("Удалить из избранного") : tr("Добавить в избранное"),
                  isFavorite ? &MainWindow::on_action_favorite_del_triggered : &MainWindow::on_action_favorite_triggered);
    }

    if (!menu.actions().isEmpty())
        menu.exec(senderWidget->mapToGlobal(pos));
}


void MainWindow::openFolder(const QString& path) {
    QUrl url = QUrl::fromLocalFile(path);
    QDesktopServices::openUrl(url);
}

QString MainWindow::getCurrentPathFromList(const QString& itemName) {
    QString path = mainDir + "/" + itemName;
    return path;
}

QString MainWindow::getCurrentPathFromTree(QTreeWidgetItem* item) {
    QString path;
    if (item) {
        QStringList pathParts;
        while (item) {
            pathParts.prepend(item->text(0));
            item = item->parent();
        }
        path = "/" + pathParts.join("/");
    }
    return path;
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


 void MainWindow::handleServerResponseSearch(const QString& reply)
 {
     if (currentProcess && currentProcess->state() == QProcess::Running) {
         currentProcess->kill();

         disconnect(currentProcess.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
         disconnect(currentProcess.data(), &QProcess::finished, nullptr, nullptr);
     }

     completerModel->clear();

     QString searchCommand = packageCommands.value(0).value("search").at(0);
     QStringList arguments = {packageCommands.value(0).value("search").at(1), reply};

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

        QString iconPath = ":/img/" + repoName + ".png";

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
    if (page == 2){
        ui->list_aur->clear();
        handleServerResponse(searchText);
    }
    else if (page == 6)
        ui->webEngineView_aur->setUrl(QUrl(searchText));
    else if (page == 7)
        ui->webEngineView_custom->setUrl(QUrl(searchText));
    else if (page == 10)
        ui->webEngineView_host->setUrl(QUrl(searchText));
    else if (page == 11)
        ui->webEngineView_game->setUrl(QUrl(searchText));
    else if (page == 14)
        checkForDowngrades(searchText);
}

void MainWindow::searchTextChanged(const QString& searchText)
{
    switch (page) {
        case 2:
            searchAndScroll(ui->list_aur, searchText);
            break;
        case 4: {
            QList<QListWidget*> listWidgets = {
                ui->list_app,
                ui->list_sysapp,
                ui->list_dependencies,
                ui->list_unused_dependencies,
                ui->list_aur_packages,
                ui->list_official_packages
            };

            int currentIndex = ui->tabWidget_pkg->currentIndex();
            if (currentIndex >= 0 && currentIndex < listWidgets.size())
                searchAndScroll(listWidgets[currentIndex], searchText);
            break;
        }
        case 3:
            searchAndScroll(ui->list_sh, searchText);
            break;
        case 14:
            searchAndScroll(ui->list_downgrade, searchText);
            break;
        case 111:
            searchAndScroll(ui->list_journal, searchText);
            break;
        case 112:
            searchAndScroll(ui->list_bench, searchText);
            break;
        case 113:
            searchAndScroll(ui->list_repair, searchText);
            break;
        default:
            return;
    }
}

void MainWindow::updateCompleterModel()
{
    completerModel->clear();
    if (page == 2 || page == 4)
        completer->complete();
}

void MainWindow::searchAndScroll(QAbstractItemView* view, const QString& text)
{
    if (QListWidget* listWidget = qobject_cast<QListWidget*>(view))
    {
        for (int row = 0; row < listWidget->count(); ++row) {
            QListWidgetItem *currentItem = listWidget->item(row);

            if (currentItem->text().contains(text, Qt::CaseInsensitive)) {
                listWidget->setCurrentItem(currentItem);
                listWidget->scrollToItem(currentItem, QAbstractItemView::PositionAtCenter);
                break;
            }

            CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));
            if (itemWidget && itemWidget->getPackageName().contains(text, Qt::CaseInsensitive)) {
                listWidget->setCurrentItem(currentItem);
                listWidget->scrollToItem(currentItem, QAbstractItemView::PositionAtCenter);
                break;
            }
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

    loadSettings();

    checkVersionAndClear();
    UpdateIcon();

    list = 0;
    loadMainMenu();

    loadContentInstall();
    loadFolders();
    loadingListWidget();
    setupConnections();

    detailsAURdefault = ui->details_aur->toHtml();
    initialDetailsDowngradeText = ui->details_downgrade->toHtml();

    ui->check_trayon->setChecked(trayon);
    ui->check_autostart->setChecked(autostart);
    ui->check_repair->setChecked(repair);
    ui->check_animload->setChecked(animload);
    ui->check_updateinstall->setChecked(updinst);
    ui->check_clearinstall->setChecked(clearinstall);
    ui->combo_mainpage->setCurrentIndex(mainpage);
    ui->check_saveurl->setChecked(saveurl);

    ui->https_proxy->setText(https);
    ui->http_proxy->setText(http);
    ui->check_proxy->setChecked(proxy);

    ui->line_ignored->setText(ignoredpkg);

    ui->check_trans->setChecked(trans);
    ui->check_colorlist->setChecked(colorlist);

    ui->time_update->setTime(timeupdate);
    ui->time_timeout->setTime(timeout);

    ui->spin_timerupdpkg->setValue(timerupdpkg);

    ui->dial_volnotify->setValue(volumenotify);

    if (*lang == "ru_RU")
        ui->combo_lang->setCurrentIndex(0);
    else if (*lang == "en_US")
        ui->combo_lang->setCurrentIndex(1);

    helper = "yay";

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onReplyFinished);

    loadDowngrades(packagesArchiveCat);
}

void MainWindow::checkVersionAndClear() {
    QSettings settings(filePath, QSettings::IniFormat);
    QString storedVersion = settings.value("Version").toString();
    QString storedLanguage = settings.value("Language").toString();

    if (storedVersion.isEmpty() || storedVersion != currentVersion) {
        settings.setValue("Version", currentVersion);
        settings.setValue("Language", storedLanguage);
        settings.sync();

        QStringList excludedFolders = {"clear", "journals", "bench", "other", "menu", "sh"};
        QDir baseDir(mainDir);

        const QStringList subDirs = baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : subDirs) {
            if (!excludedFolders.contains(subDir))
                removeDirectory(baseDir.absoluteFilePath(subDir));
        }

        removeScripts(shResourcePaths, mainDir + "sh/");
        removeDirectory(mainDir + "clear/");
        removeDirectory(mainDir + "journals/");
        removeDirectory(mainDir + "bench/");
        removeDirectory(mainDir + "other/");
        removeDirectory(mainDir + "menu/");
        sendNotification(tr("Обновление kLaus"), tr("Версия kLaus поменялась, конфигурация сброшена!"));
    }
}

void MainWindow::removeScripts(const QStringList& resourcePaths, const QString& baseDir) {
    QDir baseDirectory(baseDir);

    if (!baseDirectory.exists())
        return;

    const QStringList files = baseDirectory.entryList(QDir::Files);
    for (const QString& file : files)
        QFile::remove(baseDirectory.filePath(file));

    for (const QString& path : resourcePaths) {
        QString dirName = QFileInfo(path).absolutePath();
        QDir dirToRemove(baseDirectory.filePath(dirName));

        if (dirToRemove.exists())
            dirToRemove.removeRecursively();
    }

    const QStringList directories = baseDirectory.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const QString& dir : std::as_const(directories)) {
        QDir subDir(baseDirectory.filePath(dir));
        if (subDir.isEmpty())
            subDir.rmdir(subDir.path());
    }
}

void MainWindow::removeDirectory(const QString& dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return;

    foreach (QFileInfo item, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString itemPath = item.absoluteFilePath();
        if (item.isDir()) {

            if (item.fileName() == "Favorite")
                continue;
            QDir(itemPath).removeRecursively();
        } else
            QFile::remove(itemPath);
    }
}

void MainWindow::saveScripts(const QStringList& resourcePaths)
{
    for (const QString& path : resourcePaths)
    {
        QString relativePath = path.mid(2);
        QString fullDirPath = mainDir + "/" + QFileInfo(relativePath).path();
        QDir().mkpath(fullDirPath);

        QString destFilePath = fullDirPath + "/" + QFileInfo(path).fileName();
        QFile::copy(path, destFilePath);

        QFile::setPermissions(destFilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                                                QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup |
                                                QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther);

        QStringList languages = {"en_US", "ru_RU"};
        for (const QString& lang : languages) {
            QString iniPath = QString(":/%1/%2.ini").arg(QFileInfo(relativePath).path(), lang);
            QString destIniPath = fullDirPath + QString("/%1.ini").arg(lang);
            if (QFile::exists(iniPath)) {
                QFile::copy(iniPath, destIniPath);

                QFile::setPermissions(destIniPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                                                       QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                                                       QFileDevice::ReadOther | QFileDevice::WriteOther);
            }
        }
    }

    if (resourcePaths == menuResourcePaths)
        ui->action_updatelist->trigger();
}

MainWindow::~MainWindow()
{
    iconMap.clear();

    delete scene;
    delete previousAction;
    delete orphanButton;
    delete cacheButtonHelper;
    delete cacheButtonPacman;

    snapPackageNames.clear();
    shResourcePaths.clear();
    clearResourcePaths.clear();
    journalsResourcePaths.clear();
    benchResourcePaths.clear();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayon == 2)
        QCoreApplication::exit();
    else {
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

void MainWindow::updateCommandDescription(const QString& word) {
    if (commandDescriptions.contains(word))
        ui->details_cmd_grub->setText(commandDescriptions.value(word));
    else
        ui->details_cmd_grub->setText(tr("Наведите мышь в конфигурации и получите подсказку"));
}

void MainWindow::loadSettings()
{
    //-##################################################################################
    //-########################## НАСТРОЕННЫЕ ПЕРЕМЕННЫЕ ################################
    //-##################################################################################
    previousAction = ui->action_2; //[заглушка]

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
    //-##################################################################################
    //-############################## ОСНОВНАЯ ЧАСТЬ ####################################
    //-##################################################################################
    mainpage = settings.value("MainPage", 0).toInt();
    helpercache = settings.value("HelperCache", 0).toInt();
    trayon = settings.value("TrayOn", 0).toInt();
    autostart = settings.value("Autostart", 0).toInt();
    repair = settings.value("RepairBackup", 2).toInt();
    animload = settings.value("AnimLoad", 2).toInt();
    updinst = settings.value("UpdateInstall", 2).toInt();
    clearinstall = settings.value("ClearInstall", 2).toInt();
    volumenotify = settings.value("VolumeNotify", 30).toInt();
    saveurl = settings.value("SaveURL", 2).toInt();
    trans = settings.value("Trans", 2).toInt();
    colorlist = settings.value("ColorList", 2).toInt();

    https = settings.value("HTTPS").toString();
    http = settings.value("HTTP").toString();
    proxy = settings.value("Proxy", 0).toInt();

    ignoredpkg = settings.value("IgnoredPKG").toString();

    timerupdpkg = settings.value("Timerupdpkg", 2).toInt();

    lang = QSharedPointer<QString>::create(settings.value("Language").toString());
    teatext = QSharedPointer<QString>::create(settings.value("TeaText").toString());
    worktext = QSharedPointer<QString>::create(settings.value("WorkText").toString());
    animationname = QSharedPointer<QString>::create(settings.value("AnimationName").toString());

    gameurl = QSharedPointer<QString>::create(settings.value("GameURL").toString());
    customurl = QSharedPointer<QString>::create(settings.value("CustomURL").toString());
    aururl = QSharedPointer<QString>::create(settings.value("AurURL").toString());

    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");
    timeout = QTime::fromString(settings.value("TimeOut", "00:00:50").toString(), "HH:mm:ss");
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
            break;
        case 1:
            on_action_7_triggered();
            break;
        case 2:
            on_action_downgrade_triggered();
            break;
        case 3:
            on_action_9_triggered();
            break;
        case 4:
            on_action_17_triggered();
            break;
        case 5:
            on_action_12_triggered();
            break;
        default:
            return;
    }
    //-##################################################################################
    //-############################## СИГНАЛЫ И СЛОТЫ ###################################
    //-##################################################################################

    loadCommandDescriptions(mainDir + "other/grub/" + *lang + ".ini");

    connect(ui->text_grub, &CustomTextEdit::wordHovered, this, &MainWindow::updateCommandDescription);

    connect(ui->list_aur, &QListWidget::itemSelectionChanged, this, &::MainWindow::on_list_aur_itemSelectionChanged);
    connect(ui->list_journal, &QListWidget::itemSelectionChanged, this, &::MainWindow::on_list_journal_itemSelectionChanged);

    QList<QListWidget*> listWidgets = {
        ui->list_app,
        ui->list_sysapp,
        ui->list_dependencies,
        ui->list_unused_dependencies,
        ui->list_aur_packages,
        ui->list_official_packages
    };

    // Подключаем сигнал itemSelectionChanged для каждого виджета
    for (QListWidget* listWidget : listWidgets) {
        connect(listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::on_list_aurpkg_itemSelectionChanged);
    }

    connect(ui->time_update, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);

    connect(ui->details_aur, &QTextBrowser::anchorClicked, this, [=](const QUrl& link) {
        if (link.scheme() == "p") {
            ui->action_2->trigger();
            QString linkPath = link.path();

            int idxEqual = linkPath.indexOf('=');
            int idxGreater = linkPath.indexOf('>');
            int idxLess = linkPath.indexOf('<');

            int minIndex = linkPath.length();
            if (idxEqual != -1) minIndex = idxEqual;
            if (idxGreater != -1 && idxGreater < minIndex) minIndex = idxGreater;
            if (idxLess != -1 && idxLess < minIndex) minIndex = idxLess;

            ui->searchLineEdit->setText(linkPath.left(minIndex).trimmed());
            QCoreApplication::postEvent(ui->searchLineEdit, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
        }
    });

    connect(ui->details_aurpkg, &QTextBrowser::anchorClicked, this, [=](const QUrl& link) {
        if (link.scheme() == "p") {
            ui->action_2->trigger();
            QString linkPath = link.path();

            int idxEqual = linkPath.indexOf('=');
            int idxGreater = linkPath.indexOf('>');
            int idxLess = linkPath.indexOf('<');

            int minIndex = linkPath.length();
            if (idxEqual != -1) minIndex = idxEqual;
            if (idxGreater != -1 && idxGreater < minIndex) minIndex = idxGreater;
            if (idxLess != -1 && idxLess < minIndex) minIndex = idxLess;

            ui->searchLineEdit->setText(linkPath.left(minIndex).trimmed());
            QCoreApplication::postEvent(ui->searchLineEdit, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
        }
    });

    connect(ui->list_aur, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
        CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(item));
        if (itemWidget) {
            QString packageName = itemWidget->getPackageName();
            int row = ui->list_aur->row(item);
            onListItemClicked(packageName, row, item);
        } else
            onListItemClicked("", 0, item);
    });

    for (QListWidget* listWidget : listWidgets) {
        connect(listWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
            CustomListItemWidget *itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(item));
            QString packageName;
            int row = listWidget->row(item);

            if (itemWidget)
                packageName = itemWidget->getPackageName();

            onListItemClicked(packageName, row, item);
        });
    }

    connect(ui->list_downgrade, &QListWidget::itemClicked, this, &MainWindow::onListDowngradeItemClicked);

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
    QStorageInfo storageInfo = QStorageInfo::root();
    qint64 freeBytes = storageInfo.bytesAvailable();
    int usedPercentage = 100 - static_cast<int>((freeBytes * 100) / storageInfo.bytesTotal());

    QLabel* label = new QLabel(tr("свободно %1 ГиБ").arg(QString::number(freeBytes / (1024.0 * 1024.0 * 1024.0), 'f', 2)));
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    CustomProgressBar* customProgressBar = new CustomProgressBar();
    customProgressBar->setValue(usedPercentage);

    QWidget* containerWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(containerWidget);
    layout->addStretch();
    layout->addWidget(label);
    layout->addWidget(customProgressBar);
    ui->statusBar->addPermanentWidget(containerWidget, 1);

    QHBoxLayout* statusBarLayout = qobject_cast<QHBoxLayout*>(ui->statusBar->layout());
    if (statusBarLayout)
            statusBarLayout->setAlignment(Qt::AlignRight);

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
                errorShown = true;
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
    //-################################# ИКОНКИ ТРЕЯ ####################################
    //-##################################################################################
    trayIcon.setIcon(QIcon(":/img/2.png"));
    trayIcon.setToolTip("kLaus ;)");
    trayIcon.show();

    QMenu *trayMenu = new QMenu();

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
    connect(action_2, &QAction::triggered, this, &MainWindow::on_action_2_triggered);
    connect(action_7, &QAction::triggered, this, &MainWindow::on_action_7_triggered);
    connect(action_downgrade, &QAction::triggered, this, &MainWindow::on_action_downgrade_triggered);
    connect(action_9, &QAction::triggered, this, &MainWindow::on_action_9_triggered);
    connect(action_17, &QAction::triggered, this, &MainWindow::on_action_17_triggered);
    connect(action_12, &QAction::triggered, this, &MainWindow::on_action_12_triggered);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(&trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    //-##################################################################################
    //-##################################### GRUB #######################################
    //-##################################################################################
    QFile grub("/etc/default/grub");

    if (!grub.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->action_grub->setDisabled(true);
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
                if (section != "options")
                    sections.append(section);

            }
        }
        pacman.close();

        QString regexPattern = "^(" + sections.join("/\\S+|") + "/\\S+|aur/\\S+)";
        settings.setValue("Repository", regexPattern);
        repo = QSharedPointer<QString>::create(regexPattern);
    }
    //-##################################################################################
    //-########################### MINI LODING ANIMATION ################################
    //-##################################################################################
    loadingLabel.reset(new QLabel(this));
    loadingLabel->setVisible(false);
    //-##################################################################################
    //-############################# ЯЗЫК В ТЕРМИНАЛЕ ###################################
    //-##################################################################################
    env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", QString("%1.UTF-8").arg(*lang));

    enveng = QProcessEnvironment::systemEnvironment();
    enveng.insert("LANG", QString("en_US.UTF-8"));
    //-##################################################################################
    //-############################## ЗАГРУЗКА ИКОНОК ###################################
    //-##################################################################################
    ui->action_grub->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/grub-customizer.svg"));
    ui->action_pacman->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/kapman.svg"));
    ui->action_fstab->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/org.gnome.DiskUtility.svg"));
    //-##################################################################################
    //-#################### УБРАТЬ ПРОБЕЛЫ ИЗ ИГНОРИРОВАНИЯ ПАКЕТОВ #####################
    //-##################################################################################
    QRegularExpression noSpaceRegExp("[^\\s]+");
    QRegularExpressionValidator *noSpaceValidator = new QRegularExpressionValidator(noSpaceRegExp, this);
    ui->line_ignored->setValidator(noSpaceValidator);
    //-##################################################################################
    //-###################################### RSS #######################################
    //-##################################################################################
    rss = new QNetworkAccessManager(this);
    connect(rss, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::fetchData);

    int timerupdpkgInMilliseconds = timerupdpkg * 60 * 1000;
    timer->start(timerupdpkgInMilliseconds);

    fetchData();

    connect(ui->text_updatepkg, &QTextBrowser::anchorClicked, this, [=](const QUrl &link) {
        QString linkPath = link.path();

        if (linkPath.endsWith('/'))
            linkPath.chop(1);

        QString lastSegment = linkPath.section('/', -1);
        ui->searchLineEdit->setText(lastSegment);

        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QCoreApplication::postEvent(ui->searchLineEdit, event);
    });
    //-##################################################################################
    //-############################### ЦВЕТ ДЛЯ ССЫЛОК ##################################
    //-##################################################################################
    for (QTextBrowser* browser : {ui->text_updatepkg, ui->details_aur, ui->details_aurpkg}) browser->document()->setDefaultStyleSheet("a { text-decoration: underline; color: #c0a071 }");
}

void MainWindow::fetchData() {
    rss->get(QNetworkRequest(QUrl("https://archlinux.org/feeds/packages/")));
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString content = reply->readAll();
        parseRSS(content);
    } else
        ui->text_updatepkg->setText(tr("Ошибка: ") + reply->errorString());

    reply->deleteLater();
}

void MainWindow::parseRSS(const QString &rssContent) {
    QXmlStreamReader xml(rssContent);
    ui->text_updatepkg->clear();
    QTextCursor cursor(ui->text_updatepkg->textCursor());

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == QStringLiteral("item")) {
                QString title, link, description;
                while (!(xml.isEndElement() && xml.name() == QStringLiteral("item"))) {

                    if (xml.isStartElement()) {
                        if (xml.name() == QStringLiteral("title"))
                            title = xml.readElementText();
                        else if (xml.name() == QStringLiteral("link"))
                            link = xml.readElementText();
                        else if (xml.name() == QStringLiteral("description"))
                            description = xml.readElementText();
                    }

                    xml.readNext();
                }

                QIcon icon = getPackageIcon(title);
                QString htmlText;

                if (!icon.isNull()) {
                    QPixmap pixmap = icon.pixmap(16, 16);
                    QByteArray byteArray;
                    QBuffer buffer(&byteArray);
                    buffer.open(QIODevice::WriteOnly);
                    pixmap.save(&buffer, "PNG");
                    QString imgBase64 = "data:image/png;base64," + byteArray.toBase64();

                    htmlText += QString("<img src='%1' style='vertical-align: middle;' /> "
                                        "<a href='%2' style='vertical-align: middle;'>%3</a><br>%4<br><br>")
                                    .arg(imgBase64, link, title, description);
                } else {
                    htmlText += QString("<a href='%1'>%2</a><br>%3<br><br>")
                    .arg(link, title, description);
                }

                cursor.insertHtml(htmlText);
            }
        }
    }

    if (xml.hasError())
        ui->text_updatepkg->setText(tr("Ошибка при разборе RSS-канала: ") + xml.errorString());
}

void MainWindow::setupConnections()
{
    ui->toolBar->installEventFilter(this);
    ui->toolBar_2->installEventFilter(this);

    const auto toolBarActions = ui->toolBar->actions();
    for (auto it = toolBarActions.cbegin(); it != toolBarActions.cend(); ++it) {
        QAction *action = *it;
        if (action) {
            connect(action, &QAction::hovered, this, &MainWindow::handleActionHovered);
            action->installEventFilter(this);
        }
    }

    const auto toolBar2Actions = ui->toolBar_2->actions();
    for (auto it = toolBar2Actions.cbegin(); it != toolBar2Actions.cend(); ++it) {
        QAction *action = *it;
        if (action) {
            connect(action, &QAction::hovered, this, &MainWindow::handleActionHovered);
            action->installEventFilter(this);
        }
    }
}

void MainWindow::handleActionHovered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action)
        ui->label1->setText(action->text());
}

void MainWindow::loadCommandDescriptions(const QString& filePath) {
    QSettings settings(filePath, QSettings::IniFormat);
    settings.beginGroup("Descriptions");
    foreach (const QString &key, settings.allKeys()) {
        commandDescriptions.insert(key, settings.value(key).toString());
    }
    settings.endGroup();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // Обработка событий для панели инструментов
    if (obj == ui->toolBar || obj == ui->toolBar_2) {
        if (event->type() == QEvent::Enter) {
            QAction* action = qobject_cast<QAction*>(obj);
            if (action)
                ui->label1->setText(action->text());
        } else if (event->type() == QEvent::Leave) {
            ui->label1->setText(originalLabelText);
        }
    }

    // Обработка нажатия клавиш
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab && (page == 2 || page == 4) && obj == ui->searchLineEdit && !ui->searchLineEdit->text().isEmpty()) {
            completerModel->clear();
            handleServerResponseSearch(ui->searchLineEdit->text());
            return true;
        }
    }
    return QObject::eventFilter(obj, event); // Вызов базового метода для остальных событий
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        isHidden() ? show() : hide();
}

void MainWindow::removeToolButtonTooltips(QToolBar* toolbar) {
    const auto actions = toolbar->actions();
    for (auto it = actions.cbegin(); it != actions.cend(); ++it) {
        QAction* action = *it;
        if (QWidget* widget = toolbar->widgetForAction(action)) {
            widget->setToolTip("");
            widget->setToolTipDuration(0);
        }
    }
}

void MainWindow::mrpropper(int value, QAction* action) {
    if (previousAction)
        previousAction->setChecked(false);

    action->setChecked(true);
    previousAction = action;

    if (value == 9)
        setStyleSheet(this->styleSheet() + "QToolBar#toolBar QToolButton:checked { background-color: #3b2022; }");
    else if(value == 10)
        setStyleSheet(this->styleSheet() + "QToolBar#toolBar QToolButton:checked { background-color: #0071cd; }");
    else
        setStyleSheet(this->styleSheet() + "QToolBar#toolBar QToolButton:checked { background-color: #468783; }");

    ui->label1->setText(action->iconText());
    originalLabelText = ui->label1->text();

    miniAnimation(false, ui->details_aur);
    miniAnimation(false, ui->list_aur);
    page = value;

    errorShown = true;

    const auto actions = ui->toolBar_2->actions();
    for (auto it = actions.cbegin(); it != actions.cend(); ++it) {
        QAction* toolBarAction = *it;
        if (toolBarAction)
            toolBarAction->setVisible(false);
    }

    ui->label_warning->setVisible(page == 14);

    ui->combo_bench->setVisible(false);
    ui->searchLineEdit->setVisible(false);
    ui->searchLineEdit->clear();
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

void MainWindow::showLoadingAnimation(bool show, QWebEngineView* webView)
{
    if (animload == 0)
        return;

    QWidget* overlayWidget = webView->property("OverlayWidget").value<QWidget*>();
    QLabel* loadingLabel = webView->property("LoadingLabel").value<QLabel*>();

    if (show) {
        int leftShift = -100;

        if (!overlayWidget) {
            overlayWidget = new QWidget(webView);
            overlayWidget->setObjectName("OverlayWidget");
            overlayWidget->setGeometry(0, 0, width(), height());
            overlayWidget->raise();

            loadingLabel = new QLabel(overlayWidget);
            loadingLabel->setObjectName("LoadingLabel");
            loadingLabel->setAlignment(Qt::AlignCenter);
            loadingLabel->setFixedSize(600, 500);

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
                ui->action_allpkg->setVisible(false);
                ui->action_34->setVisible(false);
                ui->action_updatelist->setVisible(false);

                if(page == 2)
                    ui->action_infopkg->setVisible(false);
                else if (page == 4)
                    ui->action_infopkg_pkg->setVisible(false);
            }

            loadingMovie = new QMovie(":/img/loading.gif");
            backgroundColor = "#468682";
        }

        loadingLabel->setMovie(loadingMovie);
        loadingMovie->start();

        loadingLabel->move((overlayWidget->width() - loadingLabel->width()) / 2 + leftShift,
                           (overlayWidget->height() - loadingLabel->height()) / 2);

        overlayWidget->setStyleSheet(QString("QWidget#OverlayWidget { background-color: %1; }").arg(backgroundColor));
        overlayWidget->show();
        loadingLabel->show();
    } else {
        if (overlayWidget)
            overlayWidget->hide();

        if (loadingLabel)
            loadingLabel->hide();
    }
    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}

void MainWindow::processListItem(int row, QListWidget* listWidget, QTextBrowser* detailsWidget, const QString& package) {
    QString packageName = !package.isEmpty() ? package : listWidget->item(row)->text();

    if (row == lastSelectedRow && packageName == lastSelectedPackage)
        return;

    lastSelectedRow = row;
    lastSelectedPackage = packageName;
    currentPackageName = packageName;

    if (!actionInfopkgPressed)
        prepareDetails(listWidget, detailsWidget, packageName);

    if (page == 2 && actionInfopkgPressed) {
        QTreeWidget* treeWidget = ui->tree_aur;
        prepareFiles(treeWidget, packageName);
    } else if (page == 4 && actionInfopkgPkgPressed) {
        QTreeWidget* treeWidget = ui->tree_files_pkg;
        prepareFiles(treeWidget, packageName);
    }
}

void MainWindow::prepareDetails(QListWidget* listWidget, QTextBrowser* detailsWidget, const QString& packageName) {
    detailsWidget->clear();
    miniAnimation(true, detailsWidget);

    if (currentProcessDetails && currentProcessDetails->state() == QProcess::Running) {
        currentProcessDetails->kill();

        disconnect(currentProcessDetails.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
        disconnect(currentProcessDetails.data(), &QProcess::finished, nullptr, nullptr);
    }
    currentProcessDetails = QSharedPointer<QProcess>::create();

    int scrollBarValue = detailsWidget->verticalScrollBar()->value();
    auto watcher = new QFutureWatcher<QByteArray>(this);

    connect(currentProcessDetails.data(), &QProcess::readyReadStandardOutput, this, [=]() {
        QStringList lines = QString::fromUtf8(currentProcessDetails->readAllStandardOutput()).split("\n");
        QString processedInfo;
        static const QRegularExpression regex("\\b(\\S+)\\b");

        for (const QString& line : std::as_const(lines)) {
            if (line.isEmpty() || !line.contains(':')) continue;

            int colonIndex = line.indexOf(':');
            QString header = line.left(colonIndex).trimmed();
            QString content = line.mid(colonIndex + 1).trimmed();

            if (header == tr("Описание") && trans == 2) description = content;
            else if (QStringList{tr("Зависит от"), tr("Конфликтует с"), tr("Заменяет"), tr("Предоставляет"),
                                 tr("Обеспечивает"), tr("Группы"), tr("Зависимости сборки"),
                                 tr("Требуется"), tr("Опционально для"), tr("Ключевые слова"),
                                 tr("Зависимости проверки")}.contains(header)) {
                if (content != tr("Нет")) content.replace(regex, "<a href=\"p:\\1\">\\1</a>");
            } else if (QStringList{tr("Доп. зависимости"), tr("Факультативные зависимости")}.contains(header)) {
                if (content != tr("Нет")) {
                    int spaceIndex = content.indexOf(' ');
                    if (content.contains(':')) {
                        QString firstDependency = content.left(spaceIndex).replace(regex, "<a href=\"p:\\1\">\\1</a>");
                        content.replace(0, spaceIndex, firstDependency);
                    } else {
                        content.replace(regex, "<a href=\"p:\\1\">\\1</a>");
                    }
                }
            }

            header = header.at(0).isUpper() ? "<b>" + header + ":</b> " : header.replace(regex, "<a href=\"p:\\1\">\\1</a>") + ": ";
            processedInfo += "<p><span>" + header + "</span>" + content + "</p>";
        }
        detailsWidget->append(processedInfo);
        detailsWidget->verticalScrollBar()->setValue(scrollBarValue);

        if(trans == 2) {
            watcher->setFuture(QtConcurrent::run([=]() -> QByteArray {
                QProcess transProcess;
                transProcess.start("trans", QStringList() << "-brief" << ":" + lang->split('_')[0] << description);
                transProcess.waitForFinished();
                return transProcess.readAllStandardOutput().trimmed();
            }));
            connect(watcher, &QFutureWatcher<QByteArray>::finished, this, [=]() {
                detailsWidget->setHtml(detailsWidget->toHtml().replace(description, QString::fromUtf8(watcher->result())));
            });
        }
    });

    connect(currentProcessDetails.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode != 0 || exitStatus == QProcess::CrashExit) {
            if (currentProcessDetails && currentPackageName == packageName && listWidget->currentRow() == lastSelectedRow) {

                QString errorMessage = QString::fromUtf8(currentProcessDetails->readAllStandardError()).trimmed();

                QString formattedText;
                if (!errorMessage.isEmpty())
                    formattedText = QString("<span style=\"font-size: 11pt;\">%1</span>").arg(errorMessage);
                else if (listWidget == ui->list_aur)
                    formattedText = QString("<span style=\"font-size: 11pt;\">%1</span>").arg(tr("Пакет не найден!\nВозможно, он поменял свое название..."));
                else {
                    detailsWidget->clear();
                    return;
                }

                detailsWidget->setHtml(formattedText);

                if (listWidget == ui->list_aur && listWidget->currentItem()) {
                    auto userResponse = QMessageBox::question(
                        this,
                        tr("Пакет не найден"),
                        tr("Пакет не найден, перейти к его поиску?"),
                        QMessageBox::Yes | QMessageBox::No
                        );

                    if (userResponse == QMessageBox::Yes) {
                        ui->searchLineEdit->setText(listWidget->currentItem()->text());
                        QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                        QCoreApplication::postEvent(ui->searchLineEdit, event);
                    }
                }
            }
        }
        miniAnimation(false, detailsWidget);
    });

    QStringList command = packageCommands.value(0).value(page == 2 ? "show_info" : "info");
    currentProcessDetails->setProcessEnvironment(env);
    currentProcessDetails->start(command[0], QStringList() << command[1] << packageName);
}

void MainWindow::prepareFiles(QTreeWidget* treeWidget, const QString& packageName) {
    treeWidget->clear();
    miniAnimation(true, treeWidget);

    if (fileListProcess && fileListProcess->state() == QProcess::Running) {
        fileListProcess->kill();

        disconnect(fileListProcess.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
        disconnect(fileListProcess.data(), &QProcess::finished, nullptr, nullptr);
    }

    fileListProcess = QSharedPointer<QProcess>::create();
    connect(fileListProcess.data(), &QProcess::readyReadStandardOutput, this, [=]() {
        QStringList lines = QString::fromUtf8(fileListProcess->readAllStandardOutput()).split('\n', Qt::SkipEmptyParts);

        if (lines.isEmpty() && !fileListProcess->readAllStandardError().isEmpty()) {
            treeWidget->clear();
            return;
        }

        treeWidget->clear();
        QMap<QString, QTreeWidgetItem*> pathMap;

        QFileIconProvider iconProvider;

        for (const QString& line : std::as_const(lines)) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() < 2) continue;

            QString currentPath;
            QTreeWidgetItem* currentItem = nullptr;

            QString fullPath = "/" + QDir::cleanPath(parts[1]);

            QStringList splitParts = fullPath.split('/');
            for (const QString& part : std::as_const(splitParts)) {
                if (part.isEmpty()) continue;
                currentPath = currentPath.isEmpty() ? part : currentPath + "/" + part;

                if (!pathMap.contains(currentPath)) {
                    auto newItem = currentItem ? new QTreeWidgetItem(currentItem) : new QTreeWidgetItem(treeWidget);
                    QFileInfo fileInfo(fullPath);

                    QIcon icon;
                    if (fileInfo.isDir())
                        icon = iconProvider.icon(QFileIconProvider::Folder);
                    else
                        icon = iconProvider.icon(fileInfo);

                    newItem->setIcon(0, icon);
                    newItem->setText(0, part);

                    pathMap[currentPath] = newItem;
                    currentItem = newItem;
                } else
                    currentItem = pathMap[currentPath];
            }
        }
    });

    connect(fileListProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        miniAnimation(false, treeWidget);

        if (treeWidget->topLevelItemCount() == 0) {
            auto messageItem = new QTreeWidgetItem(treeWidget);
            messageItem->setText(0, tr("Не удалось получить список файлов для данного пакета."));
            messageItem->setForeground(0, QBrush(Qt::red));
            messageItem->setFlags(Qt::ItemIsEnabled);
        }
    });

    fileListProcess->start(packageCommands.value(0).value("query_list_files").at(0),
                           QStringList() << packageCommands.value(0).value("query_list_files").at(1) << packageName);
}

void MainWindow::onListItemClicked(const QString &packageName, int row, QListWidgetItem *item)
{
    if (page == 2)
    {
        row = ui->list_aur->row(item);
        CustomListItemWidget* customWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(item));

        if (item->data(Qt::UserRole).toString() == "back") {
            if (!currentCategory.isEmpty() && currentSubcategory.isEmpty()) {
                currentCategory.clear();
                loadMainMenu();
            } else if (!currentSubcategory.isEmpty()) {
                currentSubcategory.clear();
                loadSubcategories(currentCategory);
            }
            return;
        }

        if (packageName.isEmpty())
        {
            QString categoryPath = item->data(Qt::UserRole).toString();
            QString itemText = item->text();

            if (!categoryPath.contains('/') && !categoryPath.isEmpty()) {
                currentCategory = categoryPath;
                loadSubcategories(currentCategory);
            }
            else if (categoryPath.contains('/') && !itemText.isEmpty()) {
                QString category = categoryPath.section('/', 0, 0);
                QString subcategory = categoryPath.section('/', 1, 1);
                currentCategory = category;
                currentSubcategory = subcategory;
                loadPackages(category, subcategory);
            }
            else if (categoryPath.isEmpty() && !itemText.isEmpty())
                processListItem(row, ui->list_aur, ui->details_aur, packageName);
        }
        else if (customWidget) {
            QString fullPackageName = customWidget->getPackageName();
            processListItem(row, ui->list_aur, ui->details_aur, fullPackageName);
        }
    }

    if (page == 4)
    {
        QListWidget* listWidget = nullptr;

        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }

        row = listWidget->row(item);
        processListItem(row, listWidget, ui->details_aurpkg, "");
    }
}

void MainWindow::miniAnimation(bool visible, QWidget* targetWidget)
{
    static QLabel* miniLoadLabel = nullptr;

    if (!targetWidget->isVisible())
        return;

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
        targetWidget->setStyleSheet("background-color: #272727;");

        removeToolButtonTooltips(ui->toolBar);
        removeToolButtonTooltips(ui->toolBar_2);
    }
}

QIcon MainWindow::getPackageIcon(const QString& packageName) {
    QString appName = packageName.split(' ').first();

    auto findIconByAppName = [&](const QString& name) -> QIcon {

        QString iconPath = findIconPapirus(name.toLower());
        if (!iconPath.isEmpty())
            return QIcon(iconPath);

        if (iconMap.contains(name)) {
            QString iconName = iconMap[name];

            QIcon themeIcon = QIcon::fromTheme(iconName);
            if (!themeIcon.isNull())
                return themeIcon;

            QStringList searchPaths = {"/usr/share/icons", mainDir + "/share/icons"};
            for (const QString& searchPath : searchPaths) {
                QDirIterator it(searchPath, QDirIterator::Subdirectories);
                while (it.hasNext()) {
                    QString filePath = it.next();
                    if (QFileInfo(filePath).isFile()) {
                        QString fileName = QFileInfo(filePath).baseName();
                        if (fileName.contains(name, Qt::CaseInsensitive)) {
                            return QIcon(filePath);
                        }
                    }
                }
            }
        }

        return QIcon();
    };

    auto removeNumbers = [](QString name) -> QString {
        name.removeIf([](QChar c) { return c.isDigit(); });
        return name;
    };

    QStringList appNameParts = appName.split('-');
    while (!appNameParts.isEmpty()) {
        QString reducedAppName = appNameParts.join('-');

        QIcon icon = findIconByAppName(reducedAppName);
        if (!icon.isNull())
            return icon;

        QString reducedAppNameWithoutNumbers = removeNumbers(reducedAppName);
        if (reducedAppNameWithoutNumbers != reducedAppName) {
            icon = findIconByAppName(reducedAppNameWithoutNumbers);
            if (!icon.isNull())
                return icon;
        }

        appNameParts.removeLast();
    }

    if (iconMap.isEmpty()) {
        QStringList searchPaths = {
            "/usr/share/applications/",
            QDir::homePath() + "/.local/share/applications",
            "/usr/local/share/applications",
            "/var/lib/snapd/desktop/applications"
        };

        for (const QString& searchPath : searchPaths) {
            QDir desktopFilesDir(searchPath);
            QStringList desktopFiles = desktopFilesDir.entryList({"*.desktop"}, QDir::Files);

            for (const QString& desktopFileName : std::as_const(desktopFiles)) {
                QFile desktopFile(desktopFilesDir.filePath(desktopFileName));
                if (desktopFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream stream(&desktopFile);

                    QString iconName;
                    while (!stream.atEnd()) {
                        QString line = stream.readLine().trimmed();
                        if (line.startsWith("Icon=")) {
                            iconName = line.mid(5).trimmed();
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

    QIcon icon = findIconByAppName(appName);
    if (!icon.isNull())
        return icon;

    QChar firstChar = appName[0].toLower();
    if (firstChar.isLetterOrNumber()) {
        QString iconPath = QString(":/img/letter-%1.png").arg(firstChar);
        if (QFileInfo(iconPath).isFile())
            return QIcon(iconPath);
    }

    return QIcon("/usr/share/icons/Papirus/48x48/apps/application-default-icon.svg");
}

QString MainWindow::findIconPapirus(const QString& iconName) {
    QString basePath = "/usr/share/icons/Papirus/48x48/";
    QDir baseDir(basePath);

    QStringList subdirs = baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString& subdir : std::as_const(subdirs)) {
        QString iconPath = basePath + subdir + "/" + iconName + ".svg";
        if (QFile::exists(iconPath))
            return iconPath;
    }

    QString resourceIconPath = QString(":/img/%1.png").arg(iconName.toLower());

    if (QFile::exists(resourceIconPath))
        return resourceIconPath;

    return QString();
}

void MainWindow::loadMainMenu() {
    currentCategory = "";

    QDir menuDir(mainDir + "menu/");
    QStringList categories = menuDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    ui->list_aur->clear();

    QString translationFilePath = mainDir + "menu/" + *lang + ".ini";
    QSettings translations(translationFilePath, QSettings::IniFormat);

    QString iconFilePath = mainDir + "menu/icon.ini";
    QSettings icons(iconFilePath, QSettings::IniFormat);

    QStringList prioritizedCategories = {"Favorite", "Populaty"};

    for (const QString& category : prioritizedCategories) {
        if (categories.contains(category)) {
            QString iniFilePath = menuDir.filePath(category + "/cfg.ini");
            QSettings settings(iniFilePath, QSettings::IniFormat);

            QString categoryName = settings.value("name_ru_RU", category).toString();
            QString translatedCategory = translations.value(category, categoryName).toString();

            QString iconName = icons.value(category, "").toString();
            QIcon icon;

            QString iconPath = findIconPapirus(iconName);
            if (!iconPath.isEmpty())
                icon = QIcon(findIconPapirus(iconName));

            QListWidgetItem* item = new QListWidgetItem(translatedCategory, ui->list_aur);
            item->setIcon(icon);

            QColor color = generateRandomColor(colorlist);
            item->setForeground(color);

            item->setData(Qt::UserRole, category);
            item->setData(Qt::UserRole + 1, "category");

            categories.removeAll(category);
        }
    }

    for (const QString& category : std::as_const(categories)) {
        QString iniFilePath = menuDir.filePath(category + "/cfg.ini");
        QSettings settings(iniFilePath, QSettings::IniFormat);

        QString categoryName = settings.value("name_ru_RU", category).toString();
        QString translatedCategory = translations.value(category, categoryName).toString();

        QString iconName = icons.value(category, "").toString();
        QIcon icon;

        QString iconPath = findIconPapirus(iconName);
        if (!iconPath.isEmpty())
            icon = QIcon(findIconPapirus(iconName));

        QListWidgetItem* item = new QListWidgetItem(translatedCategory, ui->list_aur);
        item->setIcon(icon);

        QColor color = generateRandomColor(colorlist);
        item->setForeground(color);

        item->setData(Qt::UserRole, category);
        item->setData(Qt::UserRole + 1, "category");
    }
}

void MainWindow::loadSubcategories(const QString& category) {
    currentCategory = category;

    if (category == "Populaty" || category == "Favorite") {
        loadPackages(category, "");
        return;
    }

    QString categoryDir = mainDir + "menu/" + category;
    QString subcategoriesFile = categoryDir + "/" + category + ".txt";

    QString translationFilePath = mainDir + "menu/" + *lang + ".ini";
    QSettings translations(translationFilePath, QSettings::IniFormat);

    QString iconFilePath = mainDir + "menu/icon.ini";
    QSettings icons(iconFilePath, QSettings::IniFormat);

    QFile file(subcategoriesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sendNotification(tr("Внимание"), tr("В данной категории ничего не содержится"));
        return;
    }

    ui->list_aur->clear();
    QTextStream in(&file);
    QString currentCategory;
    bool hasSubcategories = false;

    QListWidgetItem* backItem = new QListWidgetItem("../", ui->list_aur);
    backItem->setData(Qt::UserRole, "back");
    backItem->setData(Qt::UserRole + 1, "subcategory");
    backItem->setIcon(QIcon("/usr/share/icons/Papirus/48x48/places/folder-paleorange-drag-accept.svg"));

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith('[') && line.endsWith(']')) {
            hasSubcategories = true;
            currentCategory = line.mid(1, line.length() - 2);

            QString translatedSubcategory = translations.value(currentCategory, currentCategory).toString();

            QString iconName = icons.value(currentCategory, "").toString();
            QIcon icon;

            QString iconPath = findIconPapirus(iconName);
            if (!iconPath.isEmpty())
                icon = QIcon(findIconPapirus(iconName));

            QListWidgetItem* item = new QListWidgetItem(translatedSubcategory, ui->list_aur);
            item->setIcon(icon);

            QColor color = generateRandomColor(colorlist);
            item->setForeground(color);

            item->setData(Qt::UserRole, category + "/" + currentCategory);
            item->setData(Qt::UserRole + 1, "subcategory");
        }
    }

    file.close();

    if (!hasSubcategories)
        sendNotification(tr("Внимание"), tr("В данной категории ничего не содержится"));
}

void MainWindow::loadPackages(const QString& category, const QString& subcategory) {

    QString subcategoriesFile = mainDir + "menu/" + category + "/" + category + ".txt";

    QFile file(subcategoriesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sendNotification(tr("Внимание"), tr("В данной категории ничего не содержится"));
        return;
    }

    QTextStream in(&file);
    QString currentCategory;
    bool foundSubcategory = false;
    bool hasPackages = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith('[') && line.endsWith(']')) {
            currentCategory = line.mid(1, line.length() - 2);
            foundSubcategory = (currentCategory == subcategory || subcategory.isEmpty());
        } else if (foundSubcategory && !line.isEmpty()) {
            hasPackages = true;
            break;
        }
    }

    file.close();

    if (!hasPackages) {
        sendNotification(tr("Внимание"), tr("В данной категории ничего не содержится"));
        return;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    ui->list_aur->clear();

    QListWidgetItem* backItem = new QListWidgetItem("../", ui->list_aur);
    backItem->setData(Qt::UserRole, "back");
    backItem->setData(Qt::UserRole + 1, "package");
    backItem->setIcon(QIcon("/usr/share/icons/Papirus/48x48/places/folder-paleorange-drag-accept.svg"));

    in.setDevice(&file);

    foundSubcategory = false;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith('[') && line.endsWith(']')) {
            currentCategory = line.mid(1, line.length() - 2);
            foundSubcategory = (currentCategory == subcategory || subcategory.isEmpty());
        } else if (foundSubcategory && !line.isEmpty()) {
            QIcon icon = getPackageIcon(line);

            QListWidgetItem* item = new QListWidgetItem(icon, line, ui->list_aur);

            QColor color = generateRandomColor(colorlist);
            item->setForeground(color);
        }
    }
    file.close();
}

void MainWindow::readProgramsFromFile(QFile& file, QVector<QString>& programs) {
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            int index = line.indexOf('}');
            if (index != -1) {
                QString program = line.mid(2, index - 3);
                programs.append(program);
            }
        }
    }
}

void MainWindow::processPackageName(const QString& packageName, bool valuepage) {
    QString iconPath;
    QString packageNameOrig;

    if (valuepage) {
        static const QRegularExpression repoRegex(R"(^([^/]+))");
        QRegularExpressionMatch repoMatch = repoRegex.match(packageName);
        if (repoMatch.hasMatch()) {
            QString repoName = repoMatch.captured(1);
            iconPath = findIconPapirus(repoName);
        }

        static const QRegularExpression packageNameRegex(R"(/(.+)$)");
        QRegularExpressionMatch packageNameMatch = packageNameRegex.match(packageName);
        if (packageNameMatch.hasMatch()) {
            packageNameOrig = packageNameMatch.captured(1);
        }

        appIcons[packageNameOrig] = iconPath;

    } else {
        QString packageNameIcon = packageName;
        auto removeNumbers = [](QString name) -> QString {
            name.removeIf([](QChar c) { return c.isDigit(); });
            return name;
        };

        auto findIconByAppName = [&](const QString& name) -> QString {
            QString iconPath = findIconPapirus(name.toLower());
            if (!iconPath.isEmpty())
               return QString();

            return QString();
        };

        QStringList appNameParts = packageNameIcon.split('-');
        QString foundIconPath;

        while (!appNameParts.isEmpty()) {
            QString reducedAppName = appNameParts.join('-');

            foundIconPath = findIconByAppName(reducedAppName);
            if (!foundIconPath.isEmpty())
                break;

            QString reducedAppNameWithoutNumbers = removeNumbers(reducedAppName);
            if (reducedAppNameWithoutNumbers != reducedAppName) {
                                foundIconPath = findIconByAppName(reducedAppNameWithoutNumbers);

                if (!foundIconPath.isEmpty())
                    break;
            }

            appNameParts.removeLast();
        }

        if (foundIconPath.isEmpty())
            foundIconPath = "/usr/share/icons/Papirus/48x48/mimetypes/x-package-repository.svg";

        iconPath = foundIconPath;
    }

    QColor color = generateRandomColor(colorlist);
    QListWidgetItem *item = new QListWidgetItem(valuepage ? packageNameOrig : packageName);

    item->setIcon(QIcon(iconPath));
    item->setForeground(color);

    ui->list_aur->addItem(item);
}

QString packageVersion(const QString& packageName) {
    static const QRegularExpression regex(R"-(\d+\.\d+(?:\.\d+)*)-");
    return regex.match(packageName).captured(0);
}

void MainWindow::onListDowngradeItemClicked(QListWidgetItem *currentItem) {

    bool isFolder = currentItem->data(Qt::UserRole).toBool();
    QString itemName = currentItem->text();

    if (itemName == "../") {
        QStringList pathParts = packagesArchiveCat.split('/', Qt::SkipEmptyParts);
        if (!pathParts.isEmpty()) {
            pathParts.removeLast();
            packagesArchiveCat = pathParts.join('/');
        }
        loadDowngrades(packagesArchiveCat);
    }
    else if (isFolder) {
        packagesArchiveCat += "/" + currentItem->text();
        loadDowngrades(packagesArchiveCat);
    } else {
        QString packageName;

        if (currentItem != nullptr)
            packageName = currentItem->text();
        else {
            sendNotification(tr("Внимание"), tr("Выберите пакет для установки!"));
            return;
        }

        Terminal terminal = getTerminal();
        QString installUrl = "https://archive.archlinux.org/packages/" + QString(packagesArchiveAUR.at(0)) + "/" + packagesArchiveAUR +  "/" + packageName;

        QFile lockFile(lockFilePath);
        if (lockFile.exists()) {
            sendNotification(tr("Внимание"), tr("Pacman уже используется! Завершите все операции в Pacman и попробуйте снова!"));
            return;
        }

        if (QMessageBox::question(this, tr("Установить пакет"), tr("Установить пакет из архива?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

        currentTerminalProcess = QSharedPointer<QProcess>::create(this);
        connect(currentTerminalProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
            //ничего делать не надо
        });
        currentTerminalProcess->setProgram(terminal.binary);
        currentTerminalProcess->setArguments(QStringList() << terminal.args << packageCommands.value(pkg).value("localinstall") << installUrl);
        currentTerminalProcess->setProcessChannelMode(QProcess::MergedChannels);
        currentTerminalProcess->start();
    }
}

void MainWindow::checkForDowngrades(const QString& packagesArchiveAUR)
{
    if (packagesArchiveAUR.isEmpty()) return;
    miniAnimation(true, ui->list_downgrade);
    addedLinks.clear();
    ui->list_downgrade->clear();
    QChar firstLetter = packagesArchiveAUR.at(0);
    QUrl url("https://archive.archlinux.org/packages/" + QString(firstLetter) + "/" + packagesArchiveAUR);
    QNetworkRequest request(url);
    manager->get(request);

    packagesArchiveCat = "packages/" + QString(firstLetter) + "/" + packagesArchiveAUR;
    ui->details_downgrade->setHtml(initialDetailsDowngradeText);
}

void MainWindow::loadDowngrades(const QString& packagesArchiveCatalog)
{
    miniAnimation(true, ui->list_downgrade);
    addedLinks.clear();
    ui->list_downgrade->clear();

    packagesArchiveCat = packagesArchiveCatalog;
    QUrl url("https://archive.archlinux.org/" + packagesArchiveCat);
    QNetworkRequest request(url);
    manager->get(request);

    ui->details_downgrade->setHtml(initialDetailsDowngradeText);
}

QString MainWindow::processPackageInfo(const QString& packageInfo)
{
    QStringList lines = packageInfo.split("\n");
    QString processedInfo;

    for (auto it = lines.cbegin(); it != lines.cend(); ++it) {
        const QString& line = *it;
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
        static QRegularExpression linkRegex("<a [^>]*href=\"([^\"]*)\"[^>]*>.*</a>");

        int pos = 0;
        QRegularExpressionMatch match;

        bool hasZstOrXzFiles = false;
        QString packageName;

        while ((match = linkRegex.match(htmlContent, pos)).hasMatch())
        {
            QString link = match.captured(1);
            addLinkToList(link);
            pos += match.capturedLength();

            if (link.endsWith(".zst") || link.endsWith(".xz"))
                hasZstOrXzFiles = true;
        }

        QStringList urlParts = packagesArchiveCat.split('/', Qt::SkipEmptyParts);
        packageName = urlParts.isEmpty() ? "" : urlParts.last();

        if (hasZstOrXzFiles && !packageName.isEmpty())
            processListItem(-1, ui->list_downgrade, ui->details_downgrade, packageName);
    }
    else
    {
        miniAnimation(false, ui->list_downgrade);
        loadDowngrades("packages");
        ui->details_downgrade->setText(tr("Пакет не найден в архиве"));
    }
    reply->deleteLater();
}


void MainWindow::addLinkToList(const QString &link) {
    QString cleanedLink = link;

    if (packagesArchiveCat == packagesArchiveDefault) {
        static const QRegularExpression dotDotExp("\\.\\./");
        cleanedLink.replace(dotDotExp, "");
    }

    static const QRegularExpression newlineExp("^\\n");
    cleanedLink.replace(newlineExp, "");

    miniAnimation(false, ui->list_downgrade);

    if (cleanedLink.isEmpty() || cleanedLink.contains(".sig") || addedLinks.contains(cleanedLink))
        return;

    QIcon icon;
    bool isArchive = cleanedLink.endsWith(".zst") || cleanedLink.endsWith(".xz");
    bool isFolder = !isArchive;

    if (cleanedLink == "../")
        icon = QIcon("/usr/share/icons/Papirus/48x48/places/folder-paleorange-drag-accept.svg");
    else if (isFolder)
        icon = QIcon("/usr/share/icons/Papirus/48x48/places/folder-paleorange-tar.svg");
    else if (isArchive)
        icon = QIcon("/usr/share/icons/Papirus/48x48/mimetypes/application-x-xz-pkg.svg");

    QListWidgetItem *item = new QListWidgetItem(icon, cleanedLink);
    item->setData(Qt::UserRole, isFolder);

    item->setForeground(generateRandomColor(colorlist));

    ui->list_downgrade->addItem(item);
    addedLinks.insert(cleanedLink);
}

// ----
void MainWindow::loadContentInstall() {
    QList<QPair<QStringList, QListWidget*>> commands = {
        {packageCommands.value(pkg).value("query_explicit"), ui->list_app},
        {packageCommands.value(pkg).value("query"), ui->list_sysapp},
        {packageCommands.value(pkg).value("query_dependencies"), ui->list_dependencies},
        {packageCommands.value(pkg).value("query_unused_dependencies"), ui->list_unused_dependencies},
        {packageCommands.value(pkg).value("query_aur_packages"), ui->list_aur_packages},
        {packageCommands.value(pkg).value("query_official_packages"), ui->list_official_packages}
    };

    auto loadPackages = [this](const QStringList& command, QListWidget* listWidget) {
        QStringList packages = executeCommand(command);
        loadPackageList(packages, listWidget);
    };

    for (const auto& commandPair : commands) {
        QStringList command = commandPair.first;
        QListWidget* listWidget = commandPair.second;

        QThreadPool::globalInstance()->start([=] {
            loadPackages(command, listWidget);
        });
    }
}

void MainWindow::loadPackageList(const QStringList& packages, QListWidget* listWidget) {
    listWidget->clear();

    for (const QString& package : packages) {
        QStringList packageParts = package.split(' ');
        QString packageName = packageParts.at(0);
        QIcon packageIcon = getPackageIcon(packageName);

        QListWidgetItem* item = new QListWidgetItem(packageName);
        item->setIcon(packageIcon);
        item->setForeground(generateRandomColor(colorlist));
        listWidget->addItem(item);
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
    miniAnimation(true, ui->list_aur);
    helperPackageNames.clear();
    stopProcessing = false;

    const QStringList& searchCommand = packageCommands.value(0).value("search");

    if (currentProcess && currentProcess->state() == QProcess::Running) {
        currentProcess->kill();

        disconnect(currentProcess.data(), &QProcess::readyReadStandardOutput, nullptr, nullptr);
        disconnect(currentProcess.data(), &QProcess::finished, nullptr, nullptr);
    }

    currentProcess = QSharedPointer<QProcess>::create(this);
    connect(currentProcess.data(), &QProcess::readyReadStandardOutput, this, &MainWindow::onCurrentProcessReadyRead);

    currentProcess->setProcessEnvironment(enveng);
    currentProcess->start(searchCommand.at(0), QStringList() << searchCommand.at(1) << reply);

    connect(currentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        miniAnimation(false, ui->list_aur);
    });
}

void MainWindow::onCurrentProcessReadyRead()
{
    searchTimer->singleShot(timeout.msecsSinceStartOfDay(), this, &MainWindow::onSearchTimeout);

    QString repoz, packageName, version, sizeDownload, sizeInstallation;
    int installed = 0, rating = 0, orphaned = 0, old = 0;
    QListWidgetItem* foundItem = nullptr;
    bool found = false;

    static const QRegularExpression repoRegex(R"(^([^/]+))");
    static const QRegularExpression packageNameRegex(R"(/\s*([^\s]+)\s)");
    static const QRegularExpression versionRegex(R"(\s([^\s]+)\s)");
    static const QRegularExpression sizeRegex(R"(\(([\d.]+ [KMGT]iB) ([\d.]+ [KMGT]iB)\))");
    static const QRegularExpression installedRegex(R"(\(Installed\))");
    static const QRegularExpression ratingRegex(R"(\(\+(\d+) ([\d.]+)\))");
    static const QRegularExpression outOfDateRegex(R"(\(Out-of-date: (\d{4}-\d{2}-\d{2})\))");
    static const QRegularExpression orphanedRegex(R"(\(Orphaned\))");

    QStringList favorites = readFavoritePackages();

    while (currentProcess->canReadLine()) {
        if (stopProcessing)
            break;

        QCoreApplication::processEvents();

        QByteArray line = currentProcess->readLine();
        QString lineString = QString::fromUtf8(line).trimmed();

        QRegularExpressionMatch match = QRegularExpression(*repo).match(lineString);
        if (match.hasMatch()) {
            QRegularExpressionMatch repoMatch = repoRegex.match(line);
            if (repoMatch.hasMatch())
                repoz = repoMatch.captured(1);

            QRegularExpressionMatch packageNameMatch = packageNameRegex.match(line);
            if (packageNameMatch.hasMatch())
                packageName = packageNameMatch.captured(1);

            QRegularExpressionMatch versionMatch = versionRegex.match(line);
            if (versionMatch.hasMatch())
                version = versionMatch.captured(1);

            QRegularExpressionMatch sizeMatch = sizeRegex.match(line);
            if (sizeMatch.hasMatch()) {
                sizeDownload = sizeMatch.captured(1);
                sizeInstallation = sizeMatch.captured(2);
            }

            installed = installedRegex.match(line).hasMatch() ? 1 : 0;

            QRegularExpressionMatch ratingMatch = ratingRegex.match(line);
            if (ratingMatch.hasMatch())
                rating = ratingMatch.captured(1).toInt();

            old = outOfDateRegex.match(line).hasMatch() ? 1 : 0;
            orphaned = orphanedRegex.match(line).hasMatch() ? 1 : 0;

            QColor color = generateRandomColor(colorlist);

            bool favorite = favorites.contains(packageName) ? 1 : 0;

            CustomListItemWidget *itemWidget = new CustomListItemWidget(repoz, packageName, installed, orphaned, old, rating, sizeInstallation, color, favorite, ui->list_aur);

            QString styleSheet = QString("background: none;").arg(color.name());
            itemWidget->setStyleSheet(styleSheet);

            QListWidgetItem *item = new QListWidgetItem();

            int itemHeight = 46;
            item->setSizeHint(QSize(ui->list_aur->width()-30, itemHeight));

            ui->list_aur->addItem(item);
            ui->list_aur->setItemWidget(item, itemWidget);

            QScrollBar *scrollBar = ui->list_aur->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());

            QString searchText = ui->searchLineEdit->text();
            if (packageName == searchText && !found) {
                foundItem = item;
                found = true;
            }
        }
    }

    if (foundItem) {
        int row = ui->list_aur->row(foundItem);
        onListItemClicked(ui->searchLineEdit->text(), row, foundItem);
        ui->list_aur->setCurrentItem(foundItem);
        ui->list_aur->scrollToItem(foundItem);
        ui->list_aur->selectionModel()->select(ui->list_aur->model()->index(ui->list_aur->row(foundItem), 0), QItemSelectionModel::Select);
    }
}

void MainWindow::onSearchTimeout()
{
    stopProcessing = true;
    miniAnimation(false, ui->list_aur);
}

void MainWindow::loadingListWidget()
{
    saveScripts(shResourcePaths);
    saveScripts(clearResourcePaths);
    saveScripts(journalsResourcePaths);
    saveScripts(benchResourcePaths);
    saveScripts(menuResourcePaths);
    saveScripts(otherResourcePaths);

    loadScripts(mainDir + "sh/", ui->list_sh);
    loadScripts(mainDir + "clear/", ui->list_clear);
    loadScripts(mainDir + "journals/", ui->list_journal);
    loadScripts(mainDir + "bench/", ui->list_bench);

    cacheButtonHelper = new QListWidgetItem(tr("Кэш пакетов Yay"), ui->list_clear);
    cacheButtonPacman = new QListWidgetItem(tr("Кэш пакетов Pacman"), ui->list_clear);
    orphanButton = new QListWidgetItem(tr("Пакеты сироты"), ui->list_clear);

    cacheButtonHelper->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/pacman.svg"));
    cacheButtonPacman->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/pacman.svg"));
    orphanButton->setIcon(QIcon("/usr/share/icons/Papirus/48x48/apps/ark.svg"));

    cacheButtonHelper->setForeground(generateRandomColor(colorlist));
    cacheButtonPacman->setForeground(generateRandomColor(colorlist));
    orphanButton->setForeground(generateRandomColor(colorlist));
}

void MainWindow::loadScripts(const QString& baseDir, QListWidget* listWidget)
{
    listWidget->clear();

    QDirIterator it(baseDir, QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        QDir scriptDir(it.next());
        QDirIterator scriptIt(scriptDir.path(), QStringList() << "*.sh", QDir::Files);

        while (scriptIt.hasNext()) {
            QFileInfo fileInfo(scriptIt.next());
            QString itemName = fileInfo.fileName();
            QString langFilePath = fileInfo.absolutePath() + "/" + *lang + ".ini";
            QString iconPath;

            if (QFile::exists(langFilePath)) {
                QSettings settings(langFilePath, QSettings::IniFormat);
                itemName = settings.value("name", itemName).toString();
                iconPath = findIconPapirus(settings.value("icon").toString());
            }

            QListWidgetItem* item = new QListWidgetItem(itemName);
            item->setForeground(generateRandomColor(colorlist));

            if (!iconPath.isEmpty())
                item->setIcon(QIcon(iconPath));

            listWidget->addItem(item);
        }
    }
    listWidget->sortItems(Qt::AscendingOrder);
}

void MainWindow::createArchive(const QString& folderPath, const QString& folderName)
{
    if (QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите удалить каталог \"%1\" %2?").arg(folderPath + folderName, (repair == 0) ? tr("с конфигурацией") : tr("и создать резервную копию")), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

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
        if (iconName.startsWith('.') && iconName.length() > 1)
            iconName = iconName.mid(1);

        QString iconPath = findIconPapirus(iconName.toLower());

        if (!QFileInfo::exists(iconPath))
            iconPath = "/usr/share/icons/Papirus/48x48/apps/gnome-do.svg";

        ui->list_repair->addItem(new QListWidgetItem(QIcon(iconPath), folderName));
        itemCount++;
    };

    QDir homeDir(QDir::homePath());
    QDir configDir(homeDir.absoluteFilePath(".config"));

    QStringList configFolderList = configDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto it = configFolderList.cbegin(); it != configFolderList.cend(); ++it) {
        const QString& folderName = *it;
        addFolderItem(folderName);
    }

    QFileInfoList homeFolderList = homeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    for (auto it = homeFolderList.cbegin(); it != homeFolderList.cend(); ++it) {
        const QFileInfo& fileInfo = *it;
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

    ui->label_repair->setText(tr("Конфигурации: %1").arg(itemCount));
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
    cacheButtonHelper->setText(tr("Кэш пакетов Yay"));
}

void MainWindow::on_combo_lang_currentIndexChanged(int index)
{
    QString newLang = (index == 0) ? "ru_RU" : (index == 1) ? "en_US" : "";

    if (*lang != newLang) {
        *lang = newLang;
        settings.setValue("Language", newLang);
        sendNotification(tr("Смена языка"), tr("Приложение будет перезагружено для смены языка"));

        QTimer::singleShot(0, this, [=]() {
            QTranslator translator;
            if (translator.load(":/lang/kLaus_" + newLang + ".qm")) {
                qApp->installTranslator(&translator);
                QCoreApplication::exit();
                QSharedPointer<QProcess>(new QProcess)->startDetached(qApp->arguments()[0], qApp->arguments());
            }
        });
    }
}

void MainWindow::on_combo_bench_currentIndexChanged(int index)
{
    ui->list_bench->clear();

    if (index == 0) {
        loadScripts(mainDir + "bench/", ui->list_bench);
        return;
    }

    QDirIterator it(mainDir + "bench/", QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        QDir scriptDir(it.next());
        QDirIterator scriptIt(scriptDir.path(), QStringList() << "*.sh", QDir::Files);

        while (scriptIt.hasNext()) {
            QFileInfo fileInfo(scriptIt.next());
            QString itemName = fileInfo.fileName();
            QString langFilePath = fileInfo.absolutePath() + "/" + *lang + ".ini";
            QString iconPath;

            if (QFile::exists(langFilePath)) {
                QSettings settings(langFilePath, QSettings::IniFormat);
                itemName = settings.value("name", itemName).toString();
                iconPath = findIconPapirus(settings.value("icon").toString());
            }

            auto* item = new QListWidgetItem(itemName);
            item->setForeground(generateRandomColor(colorlist));
            if (!iconPath.isEmpty()) item->setIcon(QIcon(iconPath));

            QStringList iconPaths;
            if (index == 1) iconPaths << "/usr/share/icons/Papirus/48x48/apps/org.kde.plasma.systemmonitor.svg"
                          << "/usr/share/icons/Papirus/48x48/apps/org.kde.plasma.systemmonitor.cpucore.svg";
            else if (index == 2) iconPaths << "/usr/share/icons/Papirus/48x48/apps/io.elementary.monitor.svg";
            else if (index == 3) iconPaths << "/usr/share/icons/Papirus/48x48/apps/multimedia-photo-viewer.svg";

            if (iconPaths.contains(iconPath))
                ui->list_bench->addItem(item);
            else
                delete item;
        }
    }

    ui->list_bench->sortItems(Qt::AscendingOrder);
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
        if (time < QTime(0, 0, 50)) {
            timeout = QTime(0, 0, 50);
            settings.setValue("TimeUpdate", timeout.toString("HH:mm:ss"));
            ui->time_timeout->setTime(timeout);
        } else {
            timeout = time;
            settings.setValue("TimeUpdate", timeout.toString("HH:mm:ss"));
        }
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}


void MainWindow::on_spin_timerupdpkg_valueChanged(int arg1)
{
    timerupdpkg = arg1;
    settings.setValue("Timerupdpkg", arg1);

    int timerupdpkgInMilliseconds = arg1 * 60 * 1000;
    timer->stop();
    timer->start(timerupdpkgInMilliseconds);
}

void MainWindow::on_https_proxy_textChanged(const QString &arg1)
{
    https = arg1;
    settings.setValue("HTTPS", arg1);
}

void MainWindow::on_http_proxy_textChanged(const QString &arg1)
{
    http = arg1;
    settings.setValue("HTTP", arg1);
}

void MainWindow::on_check_proxy_stateChanged(int arg1)
{
    proxy = arg1;
    settings.setValue("Proxy",arg1);
}

void MainWindow::on_line_ignored_textChanged(const QString &arg1)
{
    ignoredpkg = arg1;
    settings.setValue("IgnoredPKG", arg1);
}

void MainWindow::on_check_trans_stateChanged(int arg1)
{
    trans = arg1;
    settings.setValue("Trans", arg1);
}

void MainWindow::on_check_colorlist_stateChanged(int arg1)
{
    if (colorlist != arg1)
    {
        colorlist = arg1;
        settings.setValue("ColorList", arg1);
        QCoreApplication::exit();
        QSharedPointer<QProcess>(new QProcess)->startDetached(qApp->arguments()[0], qApp->arguments());
    }
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

void MainWindow::handleListItemClicked(QListWidgetItem *item, const QString& scriptDir) {
    if (!item) return;

    QString itemName = item->text(), msg, scriptPath, wikiUrl;
    QDirIterator it(scriptDir, QStringList() << *lang + "*.ini", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QSettings settings(it.next(), QSettings::IniFormat);
        if (itemName == settings.value("name").toString()) {
            scriptPath = it.fileInfo().absolutePath() + "/" + it.fileInfo().baseName() + ".sh";
            msg = settings.value("msg").toString();
            wikiUrl = settings.value("wiki").toString();
            break;
        }
    }

    if (scriptPath.isEmpty()) scriptPath = scriptDir + itemName;

    (scriptDir.contains("journals") ? ui->details_journal :
         scriptDir.contains("sh") ? ui->details_sh :
         ui->details_bench)->setHtml(msg);

    loadGifToGraphicsView(QFileInfo(scriptPath).absolutePath() + "/demo.gif");

    if (page == 111) //журналы
    {
        if (!wikiUrl.isEmpty()) loadWikiContent(wikiUrl);
        else {
            AnimationHelper::fadeOut(ui->details_wiki, 300);
            AnimationHelper::fadeOut(ui->label_wiki, 300);
        }
    }
}

void MainWindow::loadWikiContent(const QString& url) {
    currentLoadingUrl = url;
    QString cacheFilePath = mainDir + "tmp/" + QUrl(url).fileName() + ".html";

    if (QFile::exists(cacheFilePath)) {
        QFile cacheFile(cacheFilePath);
        if (cacheFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->details_wiki->setHtml(cacheFile.readAll());
            AnimationHelper::fadeIn(ui->details_wiki, 300);
            AnimationHelper::fadeIn(ui->label_wiki, 300);
            return;
        }
    }

    auto reply = networkManager.get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this, reply, cacheFilePath, url]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString content = formatWikiContent(reply->readAll());
            QDir().mkdir(QFileInfo(cacheFilePath).absolutePath());
            QFile cacheFile(cacheFilePath);
            if (cacheFile.open(QIODevice::WriteOnly | QIODevice::Text))
                cacheFile.write(content.toUtf8());

            if (url == currentLoadingUrl) {
                ui->details_wiki->setHtml(content);
                AnimationHelper::fadeIn(ui->details_wiki, 300);
                AnimationHelper::fadeIn(ui->label_wiki, 300);
            }
        }
        reply->deleteLater();
    });
}

QString MainWindow::formatWikiContent(const QString& content)
{
    QString tempContent = content;

    tempContent.replace("\n", "<br>")
        .replace(QRegularExpression("(?<=<br>|^)(NAME)(?=<br>|$)"), "<h2>NAME</h2>")
        .replace(QRegularExpression("(?<=<br>|^)(SYNOPSIS)(?=<br>|$)"), "<h2>SYNOPSIS</h2>")
        .replace(QRegularExpression("(?<=<br>|^)(DESCRIPTION)(?=<br>|$)"), "<h2>DESCRIPTION</h2>")
        .replace(QRegularExpression("(?<=<br>|^)(OPTIONS)(?=<br>|$)"), "<h2>OPTIONS</h2>");

    return "<html><body style='font-family: Courier; font-size: 10pt;'>" + tempContent + "</body></html>";
}

uint qHash(const QColor &color) {
    return qHash(color.rgb());
}

QColor MainWindow::getDominantColor(const QImage& image) {
    QHash<QColor, int> colorCount;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            if (color.alpha() > 0)
                colorCount[color] += 1;
        }
    }

    QColor dominantColor;
    int maxCount = 0;
    for (auto it = colorCount.begin(); it != colorCount.end(); ++it) {
        if (it.value() > maxCount) {
            maxCount = it.value();
            dominantColor = it.key();
        }
    }
    return dominantColor;
}

void MainWindow::loadGifToGraphicsView(const QString& gifPath) {
    if (page != 111 || gifPath.isEmpty()) {
        AnimationHelper::fadeOut(ui->graphics_screen, 300);
        AnimationHelper::fadeOut(ui->label_screen, 300);
        return;
    }

    auto scene = new QGraphicsScene(this);
    ui->graphics_screen->setScene(scene);

    auto movie = new QMovie(gifPath);
    if (!movie->isValid()) return;

    auto pixmapItem = scene->addPixmap(QPixmap());
    connect(movie, &QMovie::frameChanged, this, [pixmapItem, movie, this]() {
        auto currentPixmap = movie->currentPixmap().scaled(ui->graphics_screen->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmapItem->setPixmap(currentPixmap);
        ui->graphics_screen->fitInView(pixmapItem, Qt::KeepAspectRatio);
        AnimationHelper::fadeIn(ui->graphics_screen, 300);
        AnimationHelper::fadeIn(ui->label_screen, 300);

        QColor dominantColor = getDominantColor(currentPixmap.toImage());
        ui->graphics_screen->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(dominantColor.red()).arg(dominantColor.green()).arg(dominantColor.blue()));
    });

    movie->start();
}

void MainWindow::handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir) {
    Terminal terminal = getTerminal();

    if (item == orphanButton && QMessageBox::question(this, tr("Вопрос"), tr("Вы действительно хотите удалить пакеты сироты?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString command = packageCommands.value(pkg).value("remove_explicit").join(" ") + " $(" + packageCommands.value(pkg).value("query_depends").join(" ") + ")";
        QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "sh" << "-c" << command);
        return;
    }

    if (item == cacheButtonHelper || item == cacheButtonPacman) {
        QString command;
        QMessageBox::StandardButton replymod = QMessageBox::No;

        if (helpercache == 0) {
            replymod = QMessageBox::question(this, tr("Вопрос"), tr("Вы действительно хотите удалить кэш неустановленных пакетов?"), QMessageBox::Yes | QMessageBox::No);
            command = (item == cacheButtonHelper) ? packageCommands.value(pkg).value("clean_cache").join(" ") : "sudo pacman -Sc";
        } else if (helpercache == 1) {
            replymod = QMessageBox::question(this, tr("Вопрос"), tr("Вы действительно хотите удалить кэш всех пакетов?"), QMessageBox::Yes | QMessageBox::No);
            command = (item == cacheButtonHelper) ? packageCommands.value(pkg).value("clean_all_cache").join(" ") : "sudo pacman -Scc";
        } else if (helpercache == 2 && item == cacheButtonPacman) {
            replymod = QMessageBox::question(this, tr("Вопрос"), tr("Удалить кэш всех пакетов кроме последних трёх версий?"), QMessageBox::Yes | QMessageBox::No);
            command = "paccache -rvk3";
        }

        if (replymod == QMessageBox::Yes)
            QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << command);
        return;
    }

    QString itemName = item->text().trimmed(), scriptPath;
    QDirIterator it(scriptDir, QDir::Dirs | QDir::NoDotAndDotDot);

    while (it.hasNext()) {
        QSettings settings(it.next() + "/" + *lang + ".ini", QSettings::IniFormat);
        if (settings.value("name").toString().trimmed() == itemName) {
            scriptPath = it.fileInfo().absoluteFilePath() + "/" + it.fileInfo().baseName() + ".sh";
            break;
        }
    }

    if (scriptPath.isEmpty()) scriptPath = scriptDir + itemName;

    if (QFile::exists(lockFilePath)) {
        sendNotification(tr("Внимание"), tr("Pacman уже используется!"));
        return;
    }

    if (hasUpdates && updinst == 2 && page == 2 && QMessageBox::question(this, tr("Внимание"), tr("Обновить систему до последней версии?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        on_action_11_triggered();

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

void MainWindow::on_push_kde_clicked()
{
    if (QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите полностью сбросить конфигурацию DE? Вам придется заново все настроить."), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;

    Terminal terminal = getTerminal();
    QSharedPointer<QProcess>(new QProcess)->startDetached(terminal.binary, QStringList() << terminal.args << "sudo" << "rm" << QDir::homePath() + "/.config/kdeglobals");
}

void MainWindow::on_action_updatelist_triggered()
{
    if (page == 2)
    {
        miniAnimation(true, ui->list_aur);
        stopProcessing = true;

        if (currentProcess && currentProcess->state() == QProcess::Running)
            currentProcess->kill();

        ui->list_aur->clear();

        QTimer::singleShot(500, this, [=]() {
            list = 0;
            loadMainMenu();
            miniAnimation(false, ui->list_aur);
        });
    }
    else if (page == 4)
    {
        QListWidget* listWidget = nullptr;

        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }

        miniAnimation(true, listWidget);
        listWidget->clear();

        QTimer::singleShot(500, this, [=]() {
            miniAnimation(false, listWidget);
            loadContentInstall();
        });
    }
    else if (page == 14)
        loadDowngrades("packages");

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}


void MainWindow::writeToFile(const QString& filePath, const QString& content)
{
    QTemporaryFile tempFile;
    tempFile.open();
    tempFile.write(content.toUtf8());
    tempFile.close();

    QString command = "sh -c 'cat " + tempFile.fileName() + " > " + filePath + "'";

    if (filePath == "/etc/default/grub")
        command += " && sudo grub-mkconfig -o /boot/grub/grub.cfg";

    if (runPkexecCommand(command))
    {
        tempFile.remove();
        sendNotification(tr("Сохранение"), tr("Новые настройки успешно сохранены!"));
    }
    else
        sendNotification(tr("Ошибка"), tr("Не удалось выполнить аутентификацию"));
}

void MainWindow::on_push_pacman_clicked()
{
    writeToFile("/etc/pacman.conf", ui->text_pacman->toPlainText());
}

void MainWindow::on_push_fstab_clicked()
{
    writeToFile("/etc/fstab", ui->text_fstab->toPlainText());
}

void MainWindow::on_push_grub_clicked()
{
    writeToFile("/etc/default/grub", ui->text_grub->toPlainText());
}

QListWidgetItem* MainWindow::getCurrentListItem() {
    if (page == 2) {
        return ui->list_aur->currentItem();
    } else if (page == 4) {
        QListWidget* listWidget = nullptr;
        switch (ui->tabWidget_pkg->currentIndex()) {
            case 0: listWidget = ui->list_app; break;
            case 1: listWidget = ui->list_sysapp; break;
            case 2: listWidget = ui->list_dependencies; break;
            case 3: listWidget = ui->list_unused_dependencies; break;
            case 4: listWidget = ui->list_aur_packages; break;
            case 5: listWidget = ui->list_official_packages; break;
            default: break;
        }
        return listWidget ? listWidget->currentItem() : nullptr;
    }
    return nullptr;
}


void MainWindow::on_action_favorite_triggered() {
    QListWidgetItem* currentItem = getCurrentListItem();
    if (!currentItem) return;

    CustomListItemWidget* itemWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(currentItem));
    QString packageName = itemWidget ? itemWidget->getPackageName().section('/', -1) : currentItem->text();

    QStringList favorites = readFavoritePackages();

    if (favorites.contains(packageName)) {
        sendNotification(tr("Ошибка"), tr("Пакет '%1' уже существует в избранном").arg(packageName));
        return;
    }

    favorites.append(packageName);
    updateFavoritePackages(favorites);

    sendNotification(tr("Избранное"), tr("Пакет '%1' добавлен в избранное").arg(packageName));

    for (int i = 0; i < ui->list_aur->count(); ++i) {
        QListWidgetItem* item = ui->list_aur->item(i);
        CustomListItemWidget* widget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(item));
        if (widget && widget->getPackageName().section('/', -1) == packageName) {
            widget->updateFavoriteIcon(true);
        }
    }

    if (currentCategory == "Favorite") loadSubcategories("Favorite");
}

void MainWindow::on_action_favorite_del_triggered() {
    QListWidgetItem* currentItem = getCurrentListItem();
    if (!currentItem) return;

    CustomListItemWidget* itemWidget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(currentItem));
    QString packageName = itemWidget ? itemWidget->getPackageName().section('/', -1) : currentItem->text();

    QStringList favorites = readFavoritePackages();

    if (!favorites.removeOne(packageName)) {
        sendNotification(tr("Ошибка"), tr("Пакет '%1' не найден в избранном").arg(packageName));
        return;
    }

    updateFavoritePackages(favorites);

    sendNotification(tr("Избранное"), tr("Пакет '%1' удален из избранного").arg(packageName));

    for (int i = 0; i < ui->list_aur->count(); ++i) {
        QListWidgetItem* item = ui->list_aur->item(i);
        CustomListItemWidget* widget = qobject_cast<CustomListItemWidget*>(ui->list_aur->itemWidget(item));
        if (widget && widget->getPackageName().section('/', -1) == packageName) {
            widget->updateFavoriteIcon(false);
        }
    }

    if (favorites.isEmpty()) {
        currentCategory.clear();
        loadMainMenu();
    } else if (currentCategory == "Favorite") {
        loadSubcategories("Favorite");
    }
}

void MainWindow::on_action_searchpkg_triggered() {
    QListWidget* listWidget = nullptr;

    if (page == 2)
        listWidget = ui->list_aur;
    else if (page == 4) {
        int currentIndex = ui->tabWidget_pkg->currentIndex();
        switch (currentIndex) {
            case 0:
                listWidget = ui->list_app;
                break;
            case 1:
                listWidget = ui->list_sysapp;
                break;
            case 2:
                listWidget = ui->list_dependencies;
                break;
            case 3:
                listWidget = ui->list_unused_dependencies;
                break;
            case 4:
                listWidget = ui->list_aur_packages;
                break;
            case 5:
                listWidget = ui->list_official_packages;
                break;
            default:
                return;
        }
    } else
        return;

    if (listWidget == nullptr || listWidget->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
        return;
    }

    QListWidgetItem* currentItem = listWidget->currentItem();
    CustomListItemWidget* itemWidget = qobject_cast<CustomListItemWidget*>(listWidget->itemWidget(currentItem));
    QString basePackageName;

    if (itemWidget)
        basePackageName = itemWidget->getPackageName();
    else
        basePackageName = currentItem->text();

    QString packageName = basePackageName.section('/', -1);

    if (packageName.isEmpty())
        return;

    ui->action_2->trigger();
    ui->searchLineEdit->setText(packageName);
    QCoreApplication::postEvent(ui->searchLineEdit, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
}

void MainWindow::on_action_allpkg_triggered()
{
    currentTabIndex++;
    if (currentTabIndex > 5)
        currentTabIndex = 0;

    ui->tabWidget_pkg->setCurrentIndex(currentTabIndex);

    switch (currentTabIndex) {
        case 0:
            ui->action_allpkg->setText(tr("Все пакеты"));
            ui->action_allpkg->setIcon(QIcon(":/img/pkgall.png"));
            break;
        case 1:
            ui->action_allpkg->setText(tr("Зависимости пакетов"));
            ui->action_allpkg->setIcon(QIcon(":/img/pkgdep.png"));
            break;
        case 2:
            ui->action_allpkg->setText(tr("Ненужные зависимости пакетов"));
            ui->action_allpkg->setIcon(QIcon(":/img/pkgnodep.png"));
            break;
        case 3:
            ui->action_allpkg->setText(tr("Пакеты из AUR"));
            ui->action_allpkg->setIcon(QIcon(":/img/aur.png"));
            break;
        case 4:
            ui->action_allpkg->setText(tr("Пакеты из официальных репозиториев"));
            ui->action_allpkg->setIcon(QIcon(":/img/extra.png"));
            break;
        case 5:
            ui->action_allpkg->setText(tr("Пользовательские пакеты"));
            ui->action_allpkg->setIcon(QIcon(":/img/pkguser.png"));
            break;
        default:
            return;
    }
}
