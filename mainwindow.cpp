//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
#include <QSoundEffect>
#include <unistd.h>
#include <sys/utsname.h>
#include <QHostInfo>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString baseDir = QDir::homePath() + "/.config/kLaus/";
QString filePath = baseDir + "settings.ini";
QSettings settings(filePath, QSettings::IniFormat);
QString currentVersion = "6.5";

//---#####################################################################################################################################################
//--############################################################## ОПРЕДЕЛЕНИЕ ТЕРМИНАЛА ################################################################
//-#####################################################################################################################################################

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

    // Итерируемся по списку, пока не найдем установленный терминал
    for (const Terminal& terminal : terminalList) {
        if (QFile::exists(terminal.binary))
            return terminal;
    }

    // Возвращаем пустой объект Terminal, если не найден ни один терминал
    return Terminal();
}

//---#####################################################################################################################################################
//--################################################################# ОСНОВНЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_action_1_triggered()
{
    if (page == 1) return;
    mrpropper(1);
    ui->label1->setText(tr("Добавить пакет в AUR"));
    ui->tabWidget->setVisible(true);
    ui->tabWidget->setCurrentIndex(1);

    showLoadingAnimation(false);
}

void MainWindow::on_action_2_triggered()
{
    if (page == 2) return;
    mrpropper(2);
    ui->label1->setText(tr("Каталог пакетов"));
    ui->searchApp->setGeometry(510, 5, 221, 31);
    ui->push_search->setVisible(true);
    ui->push_search->setGeometry(693, 5, 31, 31);
    ui->action_4->setVisible(true);
    ui->action_6->setVisible(true);
    ui->action_30->setVisible(true);
    ui->action_34->setVisible(true);
    ui->text_details->setVisible(true);
    ui->table_aur->setVisible(true);
    ui->searchApp->setVisible(true);
    showLoadingAnimation(false);
}

void MainWindow::on_action_17_triggered()
{
    if (page == 3) return;
    mrpropper(3);
    ui->label1->setText(tr("Полезные скрипты"));
    ui->action_addsh->setVisible(true);
    ui->action_editsh->setVisible(true);
    ui->searchApp->setGeometry(390, 5, 221, 31);
    ui->searchApp->setVisible(true);
    ui->action_rmsh->setVisible(true);
    ui->list_sh->setVisible(true);

    showLoadingAnimation(false);
}

QIcon MainWindow::getPackageIcon(const QString& packageName) {

    QString appName = packageName.split(' ')[0];

    // Удаляем окончания
    for (const QString& ending : endingsToRemove) {
        if (appName.endsWith(ending)) {
            appName.chop(ending.length());
            break;
        }
    }

    if (iconMap.isEmpty()) {
        QStringList searchPaths = QStringList() << "/usr/share/applications"
                                                << QDir::homePath() + "/.local/share/applications"
                                                << "/usr/local/share/applications"
                                                << "/var/lib/snapd/desktop/applications";

        // Ищем соответствующие файлы .desktop во всех указанных путях
        for (const QString& searchPath : searchPaths) {
            QDir desktopFilesDir(searchPath);
            QStringList desktopFiles = desktopFilesDir.entryList(QStringList() << "*.desktop", QDir::Files);

            for (const QString& desktopFileName : desktopFiles) {
                QFile desktopFile(desktopFilesDir.filePath(desktopFileName));
                if (desktopFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream stream(&desktopFile);

                    // Парсим содержимое файла .desktop и получаем имя иконки и команду запуска
                    QString iconName;
                    while (!stream.atEnd()) {
                        QString line = stream.readLine().trimmed();
                        if (line.startsWith("Icon=")) {
                            iconName = line.mid(5).trimmed();
                        }

                        // Прерываем цикл, если мы уже получили оба значения
                        if (!iconName.isEmpty()) {
                            break;
                        }
                    }

                    desktopFile.close();

                    // Если удалось получить имя иконки и команду запуска, сохраняем информацию в словарях
                    if (!iconName.isEmpty()) {
                        QString desktopAppName = desktopFileName.split('.').first();
                        iconMap[desktopAppName] = iconName;
                    }
                }
            }
        }
    }

    // Проверяем, есть ли информация об иконке для данного приложения в словарях
    if (iconMap.contains(appName)) {
        return QIcon::fromTheme(iconMap[appName]);
    }

    // Если не удалось получить иконку из файла .desktop, возвращаем стандартную иконку
    return QIcon::fromTheme("package");
}

void MainWindow::on_action_7_triggered()
{
    if (page == 4) return;
    mrpropper(4);
    ui->searchApp->setGeometry(390, 5, 221, 31);
    // Выполняем команду yay -Qe | wc -l и получаем вывод
    QProcess process;
    process.start("sh", QStringList() << "-c" << "yay -Qe | wc -l");
    process.waitForFinished(-1);
    QString output = process.readAllStandardOutput();

    // Извлекаем число из вывода
    int numPackages = output.toInt();

    // Устанавливаем текст для label с количеством установленных пакетов
    ui->label1->setText(QString(tr("Каталог установленных пакетов: %1")).arg(numPackages));

    ui->action_5->setVisible(true);
    ui->action_6->setVisible(true);
    ui->action_11->setVisible(true);
    ui->list_manager->show();
    ui->list_manager->clear();

    // Выполняем команду yay -Qe и получаем вывод
    process.start("yay", QStringList() << "-Qe");
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();

    // Разбиваем вывод на строки и добавляем каждую строку в listWidgetManager
    QStringList packages = output.split("\n");
    for (const QString& package : packages) {
        if (!package.isEmpty()) {

            // Получаем иконку пакета
            QIcon packageIcon = getPackageIcon(package);

            // Раскрашиваем
            QColor color = generateRandomColor();

            // Создаем элемент списка с текстом и иконкой
            QListWidgetItem* item = new QListWidgetItem(package, ui->list_manager);
            item->setForeground(color);
            item->setIcon(packageIcon);

            // Добавляем элемент в список
            ui->list_manager->addItem(item);
        }
    }

    ui->searchApp->show();
    loadContent();
    showLoadingAnimation(false);
}


void MainWindow::on_action_9_triggered()
{
    if (page == 5) return;
    mrpropper(5);
    ui->action_27->setVisible(true);
    ui->action_bench->setVisible(true);
    ui->action_repair->setVisible(true);

    on_action_27_triggered();
    showLoadingAnimation(false);
}

void MainWindow::on_action_3_triggered()
{
    if (page == 6) return;
    mrpropper(6);
    ui->label1->hide();
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);

    if (lang == "en_US")
        ui->webEngineView->page()->load(QUrl("https://wiki.archlinux.org/title/General_recommendations"));
    else
        ui->webEngineView->page()->load(QUrl("https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));
}

void MainWindow::on_action_8_triggered()
{
    if (page == 7) return;
    QProcess process;
    process.start("yay", QStringList() << "-Q" << "ocs-url");
    if (process.waitForFinished()) {
        QString output = QString::fromUtf8(process.readAllStandardOutput());
        if (!output.contains("ocs-url")) {
            sendNotification(tr("Ошибка"), tr("Установите пакет ocs-url для установки тем!"));
            return;
        }
    }
    mrpropper(7);
    ui->label1->hide();
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);
    ui->action_35->setVisible(true);

    if (currentDesktop == "KDE")
        ui->webEngineView->page()->load(QUrl("https://store.kde.org/browse/"));
    else if (currentDesktop == "GNOME")
        ui->webEngineView->page()->load(QUrl("https://www.pling.com/s/Gnome/browse/"));
    else if (currentDesktop == "XFCE")
        ui->webEngineView->page()->load(QUrl("https://www.pling.com/s/XFCE/browse/"));
    else if (currentDesktop == "LXQt")
        ui->webEngineView->page()->load(QUrl("https://store.kde.org/browse?cat=446"));
    else if (currentDesktop == "Cinnamon")
        ui->webEngineView->page()->load(QUrl("https://www.pling.com/s/Cinnamon/browse/"));
    else if (currentDesktop == "MATE")
        ui->webEngineView->page()->load(QUrl("https://www.pling.com/s/Mate/browse/"));
    else if (currentDesktop == "Enlightenment")
        ui->webEngineView->page()->load(QUrl("https://www.pling.com/s/Enlightenment/browse/"));
    else {
        sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
        return;
    }
}

void MainWindow::on_action_10_triggered()
{
    if (page == 8) return;
    mrpropper(8);
    ui->label1->setText(tr("Информация о приложении (версия %1)").arg(currentVersion));
    ui->tabWidget->setVisible(true);
    ui->tabWidget->setCurrentIndex(4);
    showLoadingAnimation(false);
}

void MainWindow::on_action_12_triggered()
{
    if (page == 9) return;
    mrpropper(9);
    ui->label1->setText(tr("Настройки приложения"));
    ui->action_28->setVisible(true);
    ui->action_29->setVisible(true);
    ui->action_timer->setVisible(true);
    ui->action_system->setVisible(true);
    ui->tabWidget->setVisible(true);
    ui->tabWidget->setCurrentIndex(3);
    showLoadingAnimation(false);
}

void MainWindow::on_action_host_triggered()
{
    if (page == 10) return;
    mrpropper(10);
    ui->label1->setText(tr("Веб-сервер"));
    ui->action_5->setVisible(true);
    ui->action_restart->setVisible(true);
    ui->action_stop->setVisible(true);
    ui->action_catalog->setVisible(true);
    ui->scroll_site->setVisible(true);
    showLoadingAnimation(false);
}

//11-12 page заняты!

//---#####################################################################################################################################################
//--################################################################## БЫСТРЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################
void MainWindow::on_push_server_clicked()
{
    if(host == 1)
    {
        QProcess httpd;
        httpd.start("sh", QStringList() << "-c" << "httpd -v");
        httpd.waitForFinished(-1);
        QString output = QString::fromUtf8(httpd.readAllStandardOutput()).trimmed();
        sendNotification(tr("Информация"), output);
    } else
        sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
}

void MainWindow::on_push_php_clicked()
{
    QProcess php;
    php.start("sh", QStringList() << "-c" << "php -v");
    php.waitForFinished(-1);
    QString output = QString::fromUtf8(php.readAllStandardOutput()).trimmed();
    sendNotification(tr("Информация"), output);
}

void MainWindow::on_action_restart_triggered()
{
    if(host == 1)
    {
        QProcess httpProcess;
        httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "restart" << "httpd");
        httpProcess.closeWriteChannel();
        httpProcess.waitForFinished();
    }
    else if(host == 2)
    {
        QProcess httpProcess;
        httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "restart" << "xampp.service");
        httpProcess.closeWriteChannel();
        httpProcess.waitForFinished();
    }
    else
        sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
}

void MainWindow::on_action_stop_triggered()
{
    if (host == 1)
    {
        QProcess httpProcess;
        httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "stop" << "httpd");
        httpProcess.closeWriteChannel();
        httpProcess.waitForFinished();
    }
    else if(host == 2)
    {
        QProcess httpProcess;
        httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "stop" << "xampp.service");
        httpProcess.closeWriteChannel();
        httpProcess.waitForFinished();
    }
    else
        sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
}

void MainWindow::on_action_catalog_triggered()
{
    if(host == 1)
        QDesktopServices::openUrl(QUrl::fromLocalFile("/srv/http/"));
    else
        sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
}

void MainWindow::on_push_conf_clicked()
{
    if(host == 1)
        QDesktopServices::openUrl(QUrl::fromLocalFile("/etc/httpd/conf/httpd.conf"));
    else
        sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
}

void MainWindow::on_push_repair_clicked()
{
    // Открываем диалог выбора архива с помощью Zenity
    QProcess process;
    process.start("zenity", QStringList() << "--file-selection" << tr("--title=Выберите архив") << "--file-filter=*.zip");

    if (process.waitForFinished() && process.exitCode() == 0) {
        // Получаем путь выбранного архива
        QString archivePath = process.readAllStandardOutput().trimmed();

        // Восстанавливаем архив
        restoreArchive(archivePath);
    }
}

void MainWindow::openDirectory(const QString &directoryPath)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::homePath() + directoryPath));
}

void MainWindow::on_action_27_triggered()
{
    page = 11;
    ui->label1->setText(tr("Системные журналы и конфигурации"));
    ui->list_bench->setVisible(false);
    ui->scroll_repair->setVisible(false);
    ui->combo_bench->setVisible(false);
    ui->searchApp->setGeometry(390, 5, 221, 31);
    ui->searchApp->setVisible(true);
    ui->list_journal->setVisible(true);
}

void MainWindow::on_action_bench_triggered()
{
    page = 12;
    ui->label1->setText(tr("Бенчмарки"));
    ui->list_journal->setVisible(false);
    ui->scroll_repair->setVisible(false);
    ui->combo_bench->setVisible(true);
    ui->searchApp->setGeometry(390, 5, 221, 31);
    ui->searchApp->setVisible(true);
    ui->list_bench->setVisible(true);
}

void MainWindow::on_action_repair_triggered()
{
    ui->label1->setText(tr("Оптимизиция"));
    ui->list_journal->setVisible(false);
    ui->list_bench->setVisible(false);
    ui->combo_bench->setVisible(false);
    ui->searchApp->setVisible(false);
    ui->scroll_repair->setVisible(true);
}

void MainWindow::on_action_28_triggered()
{
    ui->label1->setText(tr("Настройки приложения"));
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_action_29_triggered()
{
    ui->label1->setText(tr("Настройки AUR"));
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_action_timer_triggered()
{
    ui->label1->setText(tr("Настройки таймеров"));
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_action_19_triggered()
{
    on_action_12_triggered();
    ui->label1->setText(tr("Настройки таймеров"));
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_action_system_triggered()
{
    ui->label1->setText(tr("Настройки системы"));
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_action_31_triggered()
{
    if (page == 6)
    {
        if (lang == "en_US")
            ui->webEngineView->load(QUrl("https://wiki.archlinux.org/title/General_recommendations"));
        else
            ui->webEngineView->load(QUrl("https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));
    }
    else if (page == 7)
    {
        if(currentDesktop == "KDE")
            ui->webEngineView->load(QUrl("https://store.kde.org/browse/"));
        else if(currentDesktop == "GNOME")
            ui->webEngineView->load(QUrl("https://www.pling.com/s/Gnome/browse/"));
        else if(currentDesktop == "XFCE")
            ui->webEngineView->load(QUrl("https://www.pling.com/s/XFCE/browse/"));
        else if(currentDesktop == "LXQt")
            ui->webEngineView->load(QUrl("https://store.kde.org/browse?cat=446"));
        else if(currentDesktop == "Cinnamon")
            ui->webEngineView->load(QUrl("https://www.pling.com/s/Cinnamon/browse/"));
        else if(currentDesktop == "MATE")
            ui->webEngineView->load(QUrl("https://www.pling.com/s/Mate/browse/"));
        else if (currentDesktop == "Enlightenment")
            ui->webEngineView->load(QUrl("https://www.pling.com/s/Enlightenment/browse/"));
        else {
            sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
            return;
        }
    }
}

void MainWindow::on_action_34_triggered()
{
    if (ui->table_aur->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для просмотра информации!"));
        return;
    }

    int currentRow = ui->table_aur->currentRow();
    QTableWidgetItem *item = ui->table_aur->item(currentRow, 0);
    QString packageName = item->text();

    QProcess process;
    process.start("yay", QStringList() << "-Si" << packageName);
    process.waitForFinished();

    QByteArray output = process.readAllStandardOutput();
    QString packageInfo = QString::fromUtf8(output);

    // Ищем ссылку в тексте packageInfo
    QString url;
    int urlIndex = packageInfo.indexOf("URL");
    if (urlIndex != -1) {
        int start = packageInfo.indexOf(":", urlIndex);
        int end = packageInfo.indexOf("\n", start);
        if (start != -1 && end != -1) {
            url = packageInfo.mid(start + 1, end - start - 1).trimmed();
        }
    }

    if (!url.isEmpty()) {
        showLoadingAnimation(true);
        ui->webEngineView->page()->load(QUrl(url));
    } else {
        sendNotification(tr("Внимание"), tr("URL отсутствует!"));
    }
}

void MainWindow::on_action_35_triggered()
{
    if (page == 6 || page == 7)
    {
        ui->webEngineView->back();
        return;
    }

    ui->action_34->setVisible(true);
    ui->action_35->setVisible(false);
    ui->webEngineView->hide();
}

void MainWindow::on_action_32_triggered()
{
    ui->webEngineView->reload();
}

void MainWindow::on_action_33_triggered()
{
    QUrl url = ui->webEngineView->url();
    QString urlString = url.toDisplayString();

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(urlString);
    sendNotification(tr("Буфер обмена"), tr("Ссылка успешно скопирована в буфер обмена!"));
}

void MainWindow::on_action_11_triggered()
{
    UpdateIcon();
    if (hasUpdates) {
        hide();
        Terminal terminal = getTerminal();
        QProcess process;
        process.setProgram(terminal.binary);
        process.setArguments(QStringList() << terminal.args << "yay" << "-Syu");
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start();
        process.waitForFinished(-1);

        if (process.exitCode() == QProcess::NormalExit) {
            UpdateIcon();
            show();
        }
    } else
        sendNotification(tr("Обновление"), tr("Система в актуальном состоянии!"));
}

void MainWindow::on_action_13_triggered()
{
    loadSystemInfo();
    sendNotification(tr("Информация"), tr("Информация успешно обновлена!"));
}

void MainWindow::on_action_5_triggered()
{
    if (page == 10)
    {
        if(host == 1)
        {
            QProcess httpProcess;
            httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "start" << "httpd");
            httpProcess.closeWriteChannel();
            httpProcess.waitForFinished();
        }
        else if(host == 2)
        {
            QProcess httpProcess;
            httpProcess.start("pkexec", QStringList() << "sudo" << "systemctl" << "start" << "xampp.service");
            httpProcess.closeWriteChannel();
            httpProcess.waitForFinished();
        }
        else
            sendNotification(tr("Ошибка"), tr("Сервер не выбран!"));
    }
    else
    {
        if (ui->list_manager->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
            return;
        }

        QString packageName = ui->list_manager->currentItem()->text();
        packageName = packageName.left(packageName.indexOf(" "));

        // Выполняем команду `pacman -Ql packageName` и захватываем вывод
        QProcess process;
        process.start("pacman", QStringList() << "-Ql" << packageName);
        process.waitForFinished(-1);
        QString output = process.readAllStandardOutput();

        // Ищем строку, содержащую путь к файлу .desktop
        QString desktopFilePath;
        QStringList lines = output.split('\n');
        for (const QString& line : lines) {
            if (line.contains(packageName) && line.contains(".desktop")) {
                desktopFilePath = line.split(' ').last();
                break;
            }
        }

        if (!desktopFilePath.isEmpty()) {
            // Запускаем файл .desktop
            QString desktopFileName = QFileInfo(desktopFilePath).fileName();
            QProcess::startDetached("gtk-launch", QStringList() << desktopFileName);
        } else
            sendNotification(tr("Ошибка"), tr("Файл .desktop не найден для пакета ") + packageName);
    }
}

void MainWindow::on_action_6_triggered()
{
    if (page == 2)
    {
        if (ui->table_aur->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
            return;
        }

        QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
        QProcess* process = new QProcess(this);

        // Обработчик вывода информации из процесса
        connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
            QString output = process->readAllStandardOutput();
            static QRegularExpression re(packageName);
            QRegularExpressionMatch match = re.match(output);
            if (match.hasMatch()) {

                Terminal terminal = getTerminal();
                QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -R " + packageName);
            } else
                sendNotification(tr("Пакет не найден"), tr("Пакет ") + packageName + tr(" не найден в системе!"));

            process->deleteLater();
        });

        // Обработчик вывода ошибок из процесса
        connect(process, &QProcess::readyReadStandardError, this, [=]() {
            QString error = process->readAllStandardError();
            sendNotification(tr("Ошибка"), error);
            process->deleteLater();
        });

        // Запускаем процесс
        process->start("yay", QStringList() << "-Q" << packageName);

    } else {
        if (ui->list_manager->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
            return;
        }

        QString packageName = ui->list_manager->currentItem()->text();
        packageName = packageName.left(packageName.indexOf(" "));
        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -R " + packageName);
    }
}

void MainWindow::on_action_4_triggered()
{
    if (hasUpdates && updinst == 1) {
        sendNotification(tr("Внимание"), tr("Перед установкой пакетов требуется обновить систему до актуального состояния! Это поможет предотвратить конфликт зависимостей и избежать кучи других проблем!"));
        return;
    }

    if (page == 2)
    {
        if (ui->table_aur->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
            return;
        }

        QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -S " + packageName);

    } else {
        if (ui->list_manager->currentItem() == nullptr) {
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
            return;
        }

        QString packageName = ui->list_manager->currentItem()->text();
        packageName = packageName.left(packageName.indexOf(" "));
        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -S " + packageName);
    }
}

void MainWindow::on_action_30_triggered()
{
    if (ui->table_aur->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
        return;
    }

    QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
    Terminal terminal = getTerminal();
    QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "bash" << baseDir + "sh/PKGBUILD.sh" << lang << packageName);
}

void MainWindow::on_push_grub_clicked()
{
    QString filename = "/etc/default/grub";
    QString grubContent = ui->line_grub->text().trimmed();
    QString timeout = ui->spin_grub->value() > 0 ? QString::number(ui->spin_grub->value()) : "5";

    // Создаем процесс для выполнения команды с pkexec
    QProcess process;
    process.setProgram("pkexec");
    QStringList arguments;
    arguments << "bash" << "-c" << "sed -i 's/^GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT=\"" + grubContent + "\"/; s/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=\"" + timeout + "\"/' " + filename + " && sudo grub-mkconfig -o /boot/grub/grub.cfg";
    process.setArguments(arguments);

    // Запускаем процесс с pkexec
    process.start();
    if (!process.waitForStarted()) {
        sendNotification(tr("Ошибка выполнения"), tr("Не удалось запустить pkexec"));
        return;
    }

    // Ждем, пока процесс завершится
    if (!process.waitForFinished(-1)) {
        sendNotification(tr("Ошибка выполнения"), tr("Не удалось выполнить команду pkexec"));
        return;
    }

    // Проверяем код выхода
    if (process.exitCode() != QProcess::NormalExit || process.exitStatus() != QProcess::ExitStatus::NormalExit) {
        return;
    }
    sendNotification(tr("GRUB изменен"), tr("Изменения GRUB вступят в силу после перезагрузки."));
}

void MainWindow::on_action_addsh_triggered()
{
    // Создание диалогового окна
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Добавить скрипт"));

    // Создание элементов управления
    QLabel* nameLabel = new QLabel(tr("Имя файла:"), dialog);
    QLineEdit* nameLineEdit = new QLineEdit(dialog);

    QLabel* nameRuLabel = new QLabel(tr("Название скрипта:"), dialog);
    QLineEdit* nameRuLineEdit = new QLineEdit(dialog);

    QLabel* msgRuLabel = new QLabel(tr("Описание скрипта:"), dialog);
    QLineEdit* msgRuLineEdit = new QLineEdit(dialog);

    QLabel* scriptLabel = new QLabel(tr("Тело скрипта:"), dialog);
    QTextEdit* scriptTextEdit = new QTextEdit(dialog);

    QPushButton* saveButton = new QPushButton(tr("Сохранить"), dialog);

    // Создание компоновщика для размещения элементов управления
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

    // Установка компоновщика для диалогового окна
    dialog->setLayout(layout);

    // Обработка нажатия на кнопку "Сохранить"
    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QString fileName = nameLineEdit->text() + ".sh";
        QString filePath = baseDir + "sh/" + fileName;

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            if(lang == "en_US")
            {
                stream << "#name_en_US " << nameRuLineEdit->text() << "\n";
                stream << "#msg_en_US " << msgRuLineEdit->text() << "\n";
            } else {
                stream << "#name_ru_RU " << nameRuLineEdit->text() << "\n";
                stream << "#msg_ru_RU " << msgRuLineEdit->text() << "\n";
            }
            stream << "#icon 31" << "\n";
            stream << "#!/bin/bash" << "\n";
            stream << scriptTextEdit->toPlainText();

            file.close();

            // Загрузка обновленного списка скриптов
            loadScripts(baseDir + "sh/", ui->list_sh);
            sendNotification(tr("Добавление"), tr("Скрипт успешно добавлен!"));
        }

        dialog->close();
        dialog->deleteLater();
    });

    // Отображение диалогового окна
    dialog->exec();
}

void MainWindow::on_action_rmsh_triggered()
{
    if (ui->list_sh->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите скрипт из списка для удаления!"));
        return;
    }

    // Получаем указатель на текущий выбранный элемент списка
    QListWidgetItem* selectedItem = ui->list_sh->currentItem();

    // Получаем содержимое элемента списка (название скрипта на основе языка)
    QString itemContent = selectedItem->text();

    // Создаем QMap для хранения соответствия имени скрипта и его пути
    QMap<QString, QString> scriptMap;

    // Ищем файлы со скриптами и заполняем QMap
    QDir dir(baseDir + "sh/");
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);
    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();

        // Читаем содержимое файла для сопоставления имени скрипта
        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + lang))
                {
                    QString itemName = line.mid(12).trimmed();
                    scriptMap[itemName] = filePath;
                    break;
                }
            }
            scriptFile.close();
        }
    }

    // Получаем путь к файлу, соответствующему выбранному элементу списка
    QString filePath = scriptMap.value(itemContent);

    // Удаляем файл и элемент списка, если путь найден
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
    return QString(); // Возвращаем пустую строку в случае ошибки чтения файла
}

void MainWindow::on_action_editsh_triggered()
{
    if (ui->list_sh->currentItem() == nullptr) {
        sendNotification(tr("Внимание"), tr("Выберите скрипт из списка для изменения!"));
        return;
    }

    // Получаем указатель на текущий выбранный элемент списка
    QListWidgetItem* selectedItem = ui->list_sh->currentItem();

    // Получаем содержимое элемента списка (название скрипта на основе языка)
    QString itemContent = selectedItem->text();

    // Ищем файл с соответствующим названием скрипта
    QDir dir(baseDir + "sh/");
    QStringList filter;
    filter << "*.sh";
    QFileInfoList fileList = dir.entryInfoList(filter);
    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();
        QString fileName = fileInfo.fileName();

        // Читаем содержимое файла для сопоставления имени скрипта
        QFile scriptFile(filePath);
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd())
            {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + lang))
                {
                    QString itemName = line.mid(12).trimmed();
                    if (itemName == itemContent)
                    {
                        // Закрываем файл, чтобы его можно было перезаписать
                        scriptFile.close();

                        // Создаем диалоговое окно для редактирования содержимого
                        QDialog* editDialog = new QDialog(this);
                        editDialog->setWindowTitle(tr("Редактировать скрипт"));
                        editDialog->resize(500, 300);

                        // Создаем элементы интерфейса
                        QLabel* nameLabel = new QLabel(tr("Имя файла:"));
                        QLineEdit* nameEdit = new QLineEdit(fileName);
                        QLabel* scriptLabel = new QLabel(tr("Тело скрипта:"));
                        QTextEdit* scriptEdit = new QTextEdit();
                        QPushButton* saveButton = new QPushButton(tr("Сохранить"));

                        // Устанавливаем содержимое скрипта в поле редактирования
                        scriptEdit->setPlainText(getScriptContent(filePath));

                        QVBoxLayout* layout = new QVBoxLayout(editDialog);
                        editDialog->setStyleSheet("QWidget{background-color:#2d2b79;} QLineEdit,QTextEdit{background-color:#21205b;padding:10px;border-radius:10px;} QLabel{color:#fff;font-size:10pt;}QPushButton{border-radius:10px;padding:5px 20px;background-color:#916ee4;color:#fff;}");

                        // Добавляем элементы в компоновщик
                        layout->addWidget(nameLabel);
                        layout->addWidget(nameEdit);
                        layout->addWidget(scriptLabel);
                        layout->addWidget(scriptEdit);
                        layout->addWidget(saveButton);

                        // Устанавливаем компоновщик для диалогового окна
                        editDialog->setLayout(layout);

                        // Подключаем сигнал нажатия кнопки "Сохранить" к соответствующему слоту
                        connect(saveButton, &QPushButton::clicked, this, [=]() mutable {
                            // Получаем новое имя файла и новое содержимое скрипта
                            QString newFileName = nameEdit->text();
                            QString newScriptContent = scriptEdit->toPlainText();

                            // Перезаписываем файл с новыми данными
                            QString newFilePath = baseDir + "sh/" + newFileName;
                            QFile newScriptFile(newFilePath);
                            if (newScriptFile.open(QIODevice::WriteOnly | QIODevice::Text))
                            {
                                QTextStream newScriptStream(&newScriptFile);
                                newScriptStream << newScriptContent;
                                newScriptFile.close();
                            }

                            // Обновляем элемент списка с новым именем
                            selectedItem->setText(newFileName);

                            // Закрываем окно редактирования скрипта
                            editDialog->close();

                            // Обновляем список скриптов
                            loadScripts(baseDir + "sh/", ui->list_sh);
                            sendNotification(tr("Сохранение"), tr("Скрипт успешно изменен!"));
                        });

                        // Отображаем диалоговое окно
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

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showLoadingAnimation(true);

    loadSettings();         //загрузка настроек
    checkVersionAndClear();
    UpdateIcon();           //получаем иконку трея
    loadContent();          //загрузка списков приложений игр и тп
    loadFolders();          //загрузка конфигов
    loadingListWidget();
    loadSystemInfo();

    ui->check_trayon->setChecked(trayon == 1);
    ui->check_repair->setChecked(repair == 1);
    ui->check_animload->setChecked(animload == 1);
    ui->check_updateinstall->setChecked(updinst == 1);
    ui->combo_mainpage->setCurrentIndex(mainpage);
    ui->combo_animload->setCurrentIndex(animloadpage);

    ui->time_update->setTime(timeupdate);
    ui->time_tea->setTime(timetea);
    ui->time_work->setTime(timework);
    ui->line_tea->setText(teatext);
    ui->line_work->setText(worktext);

    ui->dial_volmenu->setValue(volumemenu);
    ui->dial_volnotify->setValue(volumenotify);

    if(lang == "ru_RU")
        ui->combo_lang->setCurrentIndex(0);
    else if(lang == "en_US")
        ui->combo_lang->setCurrentIndex(1);

    ui->combo_host->setCurrentIndex(host);

    showLoadingAnimation(false);
}

void MainWindow::checkVersionAndClear() {
    QString settingsFilePath = baseDir + "settings.ini";
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    QString storedVersion = settings.value("Version").toString();
    QString storedLanguage = settings.value("Language").toString();

    if (storedVersion.isEmpty() || storedVersion != currentVersion) {
        settings.setValue("Version", currentVersion);
        settings.setValue("Language", storedLanguage);
        settings.sync();

        QStringList excludedFolders = {"clear", "journals", "bench", "other", "sh"};
        QDir baseDirDir(baseDir);

        QStringList subDirs = baseDirDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : subDirs) {
            if (!excludedFolders.contains(subDir)) {
                QDir subDirDir(baseDirDir.absoluteFilePath(subDir));
                subDirDir.removeRecursively();
            }
        }

        removeScripts(shResourcePaths, baseDir + "sh/");
        removeDirectory(baseDir + "clear/");
        removeDirectory(baseDir + "journals/");
        removeDirectory(baseDir + "bench/");

        sendNotification(tr("Обновление kLaus"), tr("Версия kLaus поменялась, конфигурация сброшена!"));
    }
}

void MainWindow::removeScripts(const QStringList& resourcePaths, const QString& baseDir)
{
    QDir().mkpath(baseDir);
    for (const QString& path : resourcePaths)
    {
        QString fileName = QFileInfo(path).fileName();
        QString filePath = baseDir + fileName;
        QFile::remove(filePath);
    }
}

void MainWindow::removeDirectory(const QString& dirPath)
{
    QDir dir(dirPath);
    if (dir.exists()) {
        dir.removeRecursively(); // Удаляем папку и ее содержимое рекурсивно
    }
}

void MainWindow::saveScripts(const QStringList& resourcePaths, const QString& baseDir)
{
    QDir().mkpath(baseDir);
    for (const QString& path : resourcePaths)
    {
        QString fileName = QFileInfo(path).fileName();
        QString filePath = baseDir + fileName;

        QFile::copy(path, filePath);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(trayon == 1)
        QApplication::quit();
    else
    {
        hide();             // Скрываем главное окно
        event->ignore();    // Игнорируем событие закрытия
    }
}

void MainWindow::loadSound(int soundIndex)
{
    QSoundEffect* beep = new QSoundEffect(this);
    QString soundPath;

    if (soundIndex == 1)
        soundPath = "qrc:/media/message.wav";
    else
        soundPath = "qrc:/media/sound.wav";

    beep->setSource(QUrl(soundPath));

    if (soundIndex == 1) {
        float volnotify = static_cast<float>(volumenotify) / 100.0f;
        volnotify = static_cast<float>(static_cast<int>(volnotify * 10.0f)) / 10.0f;
        beep->setVolume(volnotify);
    } else {
        float volmenu = static_cast<float>(volumemenu) / 100.0f;
        volmenu = static_cast<float>(static_cast<int>(volmenu * 10.0f)) / 10.0f;
        beep->setVolume(volmenu);
    }

    beep->play();
}

void MainWindow::loadSettings()
{
    //-##################################################################################
    //-########################## НАСТРОЕННЫЕ ПЕРЕМЕННЫЕ ################################
    //-##################################################################################
    previousAction = ui->action_1; //предыдущий action [заглушка]
    ui->webEngineView->raise(); //браузер выше всех

    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);

    //-##################################################################################
    //-############################## ОСНОВНАЯ ЧАСТЬ ####################################
    //-##################################################################################
    mainpage = settings.value("MainPage", 0).toInt();
    animloadpage = settings.value("AnimLoadPage", 0).toInt();
    yaycache = settings.value("YayCache", 0).toInt();
    trayon = settings.value("TrayOn", 0).toInt();
    repair = settings.value("RepairBackup", 1).toInt();
    animload = settings.value("AnimLoad", 1).toInt();
    updinst = settings.value("UpdateInstall", 1).toInt();
    volumenotify = settings.value("VolumeNotify", 30).toInt();
    volumemenu = settings.value("VolumeMenu", 50).toInt();
    lang = settings.value("Language").toString();
    host = settings.value("Host").toInt();
    teatext = settings.value("TeaText").toString();
    worktext = settings.value("WorkText").toString();
    repo = settings.value("Repository").toString();

    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");
    timetea = QTime::fromString(settings.value("TimeTea").toString(), "HH:mm");
    timework = QTime::fromString(settings.value("TimeWork").toString(), "HH:mm");

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    currentDesktop = environment.value("XDG_CURRENT_DESKTOP");

    ui->webEngineView->setZoomFactor(0.9);
    ui->toolBar_2->setFixedWidth(100);

    //запретить выключать панели
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBar_2->setContextMenuPolicy(Qt::PreventContextMenu);

    // Запрещаем редактирование ячеек
    ui->table_aur->setEditTriggers(QAbstractItemView::NoEditTriggers);

    switch(mainpage) {
    case 0:
        on_action_10_triggered();
        ui->label1->setText(tr("Добро пожаловать в kLaus - Arch Manager!"));
        break;
    case 1:
        on_action_2_triggered();
        break;
    case 2:
        on_action_17_triggered();
        break;
    case 3:
        on_action_7_triggered();
        break;
    case 4:
        on_action_9_triggered();
        break;
    case 5:
        on_action_3_triggered();
        break;
    case 6:
        on_action_8_triggered();
        break;
    case 7:
        on_action_10_triggered();
        break;
    case 8:
        on_action_12_triggered();
        break;
    default:
        on_action_10_triggered();
        ui->label1->setText(tr("Добро пожаловать в kLaus - Arch Manager!"));
        break;
    }

    //-##################################################################################
    //-############################## СИГНАЛЫ И СЛОТЫ ###################################
    //-##################################################################################
    connect(ui->spin_grub, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_grub_valueChanged(int)));
    connect(ui->time_update, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(ui->table_aur, &QTableWidget::cellClicked, this, &MainWindow::onTableAurCellClicked);
    //-##################################################################################
    //-############################### ЗАНЯТОЕ МЕСТО ####################################
    //-##################################################################################
    QProgressBar* progressBar = new QProgressBar();
    progressBar->setFixedSize(50, 30);

    // Получаем информацию о корневом каталоге
    QStorageInfo storageInfo = QStorageInfo::root();

    // Получаем количество свободных байтов на диске
    qint64 freeBytes = storageInfo.bytesAvailable();

    // Вычисляем процент занятого места на диске
    int usedPercentage = 100 - static_cast<int>((freeBytes * 100) / storageInfo.bytesTotal());

    // Устанавливаем значение прогресс-бара в процентах
    progressBar->setValue(usedPercentage);

    // Устанавливаем текст на прогресс-баре, отображающий количество свободных гигабайтов
    progressBar->setFormat("");

    QLabel* label = new QLabel(tr("свободно %1 ГиБ").arg(QString::number(freeBytes / (1024.0 * 1024.0 * 1024.0), 'f', 2)));

    QWidget* containerWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(containerWidget);
    layout->addStretch(); // Добавляем растягивающий элемент, чтобы выровнять элементы по правому краю
    layout->addWidget(progressBar);
    layout->addWidget(label);
    layout->setContentsMargins(0, 0, 0, 0);

    ui->statusBar->addPermanentWidget(containerWidget, 1); // Установите растягиваемый элемент с индексом 1

    // Устанавливаем цвет для прогресс-бара
    QColor customColor(42, 40, 112);
    QPalette palette = progressBar->palette();
    palette.setColor(QPalette::Highlight, customColor);
    progressBar->setPalette(palette);

    // Выравниваем содержимое статусного бара по правому краю
    QHBoxLayout* statusBarLayout = qobject_cast<QHBoxLayout*>(ui->statusBar->layout());
    if (statusBarLayout)
        statusBarLayout->setAlignment(Qt::AlignRight);

    // Выравниваем текст метки по правому краю
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    //-##################################################################################
    //-############################# ВЕБ ЧАСТЬ / СКРИПТЫ ################################
    //-##################################################################################
    QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
    profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);

    QObject::connect(ui->webEngineView->page(), &QWebEnginePage::loadStarted, this, [=]() {

        if (page == 2 || page == 6 || page == 7) {

            QFile scriptFile(":/loading.browser.js"); // Путь к вашему файлу скрипта
            if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&scriptFile);
                QString script = stream.readAll();
                ui->webEngineView->page()->runJavaScript(script);
            }
        }
    });

    QObject::connect(ui->webEngineView->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success) {
            if (page == 6 || page == 7)
                ui->webEngineView->show();

            else if (page == 2) {
                ui->action_34->setVisible(false);
                ui->action_35->setVisible(true);
                ui->webEngineView->show();
            }
        } else if (!errorShown && page == 2) {
                sendNotification(tr("Ошибка"), tr("Страница не найдена (ошибка 404)"));
                errorShown = true;  // Устанавливаем флаг, что уведомление было показано
        }
        showLoadingAnimation(false);
    });
    QTableWidget *table = ui->table_aur;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    //-##################################################################################
    //-###################### КОПИРОВАНИЕ ИНФОРМАЦИИ О СИСТЕМЕ ##########################
    //-##################################################################################
    QList<QAction*> actionList;
    actionList << ui->action_memory << ui->action_cpu << ui->action_gpu << ui->action_hostname << ui->action_os
               << ui->action_packages << ui->action_release << ui->action_screen << ui->action_iphost << ui->action_iplocal;
    for (QAction* action : actionList) {

        QObject::connect(action, &QAction::triggered, this, [action, this]() {

            // Копирование текста в буфер обмена
            QString text = action->text();
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setText(text);

            // Копирование пользовательских данных в буфер обмена
            QVariant data = action->data();
            if (data.isValid()) {
                QString customData = data.toString();
                clipboard->setText(customData);
            }
            sendNotification(tr("Буфер обмена"), tr("Информация скопирована в буфер обмена"));
        });
    }

    //-##################################################################################
    //-########################### ГРОМКОСТЬ УВЕДОМЛЕНИЙ ################################
    //-##################################################################################
    QString labelvolmenu = QString(tr("Звук меню: %1/100")).arg(volumemenu);
    ui->label_volmenu->setText(labelvolmenu);
    QString labelvolnotify = QString(tr("Звук уведомлений: %1/100")).arg(volumenotify);
    ui->label_volnotify->setText(labelvolnotify);

    //-##################################################################################
    //-############################# ТАЙМЕР ИКОНКИ ТРЕЯ #################################
    //-##################################################################################
    updateIconTimer = new QTimer(this);
    updateIconTimer->setInterval(3600000); // 1 час
    updateIconTimer->setSingleShot(false);
    connect(updateIconTimer, &QTimer::timeout, this, &MainWindow::UpdateIcon);
    updateIconTimer->start();

    //-##################################################################################
    //-############################## ТАЙМЕР COFFETIME ##################################
    //-##################################################################################
    teaTimer = new QTimer(this);
    connect(teaTimer, &QTimer::timeout, this, &MainWindow::TeaTimer);

    workTimer = new QTimer(this);
    connect(workTimer, &QTimer::timeout, this, &MainWindow::WorkTimer);

    //-##################################################################################
    //-################################# ИКОНКИ ТРЕЯ ####################################
    //-##################################################################################
    trayIcon.setIcon(QIcon(":/img/2.png"));
    trayIcon.setToolTip("kLaus ;)");
    trayIcon.show();

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
        // Ошибка открытия файла
    } else {

        QTextStream in(&grub);
        QString grubContent;
        QString timeoutStr;

        // Обновленные регулярные выражения для поиска значений с обоими вариантами ковычек
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
                    // Если первая попытка не удалась, попробуйте другой вариант ковычек
                    grubCmdlineRegex.setPattern("^GRUB_CMDLINE_LINUX_DEFAULT=['\"]?(.*)['\"]?$");
                    match = grubCmdlineRegex.match(line);
                    if (match.hasMatch()) {
                        grubContent = match.captured(1).trimmed();
                        static const QRegularExpression quotationRegex("[\"']");
                        grubContent.remove(quotationRegex);
                    }
                }
                break; // Закончит цикл поиска
            }
        }
        grub.close();

        int timeout = timeoutStr.toInt(); // получаем значение timeout из файла
        ui->spin_grub->setValue(timeout); // устанавливаем значение в QSpinBox
        ui->line_grub->setText(grubContent);


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
            settings.setValue("Repository", regexPattern); // Сохранение значения regexPattern, а не repo
        }
    }
}

void MainWindow::removeToolButtonTooltips(QToolBar* toolbar) {
    // Получение стиля текущей темы
    QList<QAction*> actions = toolbar->actions();

    // Отключение отображения подсказок для виджетов
    for (QAction* action : actions) {
        QWidget* widget = toolbar->widgetForAction(action);
        if (widget) {
            widget->setToolTip("");
            widget->setToolTipDuration(0); // Отключение подсказок
        }
    }
}

bool isDescendantOfTabWidget(QWidget* widget) {
    while (widget) {
        if (qobject_cast<QTabWidget*>(widget->parentWidget())) {
            return true;
        }
        widget = widget->parentWidget();
    }
    return false;
}

void MainWindow::mrpropper(int value) //зачистка говна перед началом каждой вкладки
{
    page = value;

    showLoadingAnimation(true);
    loadSound(0);

    //останавливаем страницы и ошибки
    ui->webEngineView->page()->triggerAction(QWebEnginePage::Stop);
    errorShown = true;

    //удаляем все listwidget-ы
    QList<QListWidget*> allListWidgets = findChildren<QListWidget*>();
    for (QListWidget* listWidget : allListWidgets) {
        QWidget* parentWidget = listWidget->parentWidget();

        bool isInsideScrollArea = false;
        while (parentWidget) {
            if (qobject_cast<QScrollArea*>(parentWidget)) {
                isInsideScrollArea = true;
                break;
            }
            parentWidget = parentWidget->parentWidget();
        }

        if (!isInsideScrollArea) {
            listWidget->setVisible(false);
        }
    }

    //удаляем все action-ы
    QList<QAction*> allActions = ui->toolBar_2->actions();
    for (QAction* action : allActions) {
        action->setVisible(false);
    }

    ui->scroll_repair->setVisible(false);

    ui->table_aur->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->searchApp->setVisible(false);
    ui->webEngineView->setVisible(false);
    ui->scroll_site->setVisible(false);
    ui->combo_bench->setVisible(false);
    ui->text_details->setVisible(false);
    ui->push_search->setVisible(false);
    ui->label1->setVisible(true);
}

void MainWindow::TeaTimer()
{
    if (teatext.isEmpty())
        sendNotification(tr("Отдохни!"), tr("Пора пить чай!"));
    else
        sendNotification(tr("Отдохни!"), teatext);
}

void MainWindow::WorkTimer()
{
    if (worktext.isEmpty())
        sendNotification(tr("Отдохни!"), tr("Пора сделать зарядку!"));
    else
        sendNotification(tr("Отдохни!"), worktext);
}

void MainWindow::setHasUpdates(bool updates)
{
    trayIcon.setIcon(QIcon(updates ? ":/img/tray.png" : ":/img/2.png"));
    hasUpdates = updates; // Обновление значения переменной hasUpdates
}

void MainWindow::UpdateIcon()
{
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.start("sh", QStringList() << "-c" << "checkupdates | wc -l");
    process.waitForFinished();

    if (process.exitCode() == QProcess::NormalExit) {
        QByteArray output = process.readAll();
        int numUpdates = output.trimmed().toInt();

        if (numUpdates == 0) {
            setHasUpdates(false);
            updateIconTimer->start();
        } else {
            setHasUpdates(true);
            updateIconTimer->stop();
        }
    } else
        setHasUpdates(false); // Обработка ошибки при выполнении команды
}

void MainWindow::loadSystemInfo()
{
    //-##################################################################################
    //-############################ СИСТЕМНАЯ ИНФОРМАЦИЯ ################################
    //-##################################################################################
    char hostname[256];
    char* username = getlogin();
    QString name_2 = QString::fromUtf8(username);
    gethostname(hostname, sizeof(hostname));
    QString hostnameMessage = QString("%1 (%2)").arg(hostname, name_2);
    ui->action_hostname->setText(hostnameMessage);

    // Получаем информацию о системе
    struct utsname systemInfo;
    if (uname(&systemInfo) == 0) {
        QString release = QString::fromUtf8(systemInfo.release);
        QString machine = QString::fromUtf8(systemInfo.machine);

        QProcess distributionProcess;
        distributionProcess.start("lsb_release", QStringList() << "-s" << "-d");
        distributionProcess.waitForFinished();
        QString distribution = QString::fromUtf8(distributionProcess.readAllStandardOutput()).trimmed();

        // Отобразить информацию о системе в меню
        QString osText = QString("%1 (%2)").arg(distribution, machine);
        ui->action_os->setText(osText);

        QString release_text = QString(tr("Ядро: %1")).arg(release);
        ui->action_release->setText(release_text);
    }

    // Получаем количество пакетов pacman
    QProcess pacmanProcess;
    pacmanProcess.start("sh", QStringList() << "-c" << "yay -Qq | wc -l");
    pacmanProcess.waitForFinished(-1);
    QString pacmanPackagesCount = QString::fromUtf8(pacmanProcess.readAllStandardOutput()).trimmed();

    // Получаем количество пакетов flatpak
    QProcess flatpakProcess;
    flatpakProcess.start("sh", QStringList() << "-c" << "flatpak list --app | wc -l");
    flatpakProcess.waitForFinished(-1);
    QString flatpakPackagesCount = QString::fromUtf8(flatpakProcess.readAllStandardOutput()).trimmed();

    QString packagesText = QString("Pacman: %1, Flatpak: %2").arg(pacmanPackagesCount, flatpakPackagesCount);
    ui->action_packages->setText(packagesText);

    // Получаем разрешение
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();

    QString resolutionText = QString(tr("Разрешение: %1x%2")).arg(screenWidth).arg(screenHeight);
    ui->action_screen->setText(resolutionText);

    // Получаем CPU
    QProcess cpuProcess;
    cpuProcess.start("sh", QStringList() << "-c" << "LC_ALL=C lscpu");
    cpuProcess.waitForFinished(-1);
    QString cpuOutput = QString::fromUtf8(cpuProcess.readAllStandardOutput());

    QStringList cpuLines = cpuOutput.split('\n');
    QString cpuInfo;
    for (const QString& line : cpuLines) {
        if (line.startsWith("Model name:")) {
            cpuInfo = line.mid(12).trimmed();
            break;
        }
    }
    ui->action_cpu->setText(cpuInfo);

    // Получаем GPU
    QProcess gpuProcess;
    gpuProcess.start("sh", QStringList() << "-c" << "lspci | grep -i 'VGA'");
    if (gpuProcess.waitForFinished()) {
        QString output = QString::fromUtf8(gpuProcess.readAllStandardOutput()).trimmed();

        QStringList parts = output.split(':');
        if (parts.size() > 2) {
            QString gpuInfo = parts[2].trimmed();
            ui->action_gpu->setText(gpuInfo);
        }
    }

    // Получаем Memory
    QProcess memoryProcess;
    memoryProcess.start("sh", QStringList() << "-c" << "free -m");
    memoryProcess.waitForFinished();
    QString memoryOutput = QString::fromUtf8(memoryProcess.readAllStandardOutput());

    QStringList memoryLines = memoryOutput.split('\n');
    QString memoryInfo;
    for (const QString& line : memoryLines) {
        if (line.startsWith("Mem:")) {
            QStringList memoryParts = line.split(' ', Qt::SkipEmptyParts);
            memoryParts.removeAll("");
            if (memoryParts.length() >= 7) {
                QString totalMemory = memoryParts.at(1);
                QString usedMemory = memoryParts.at(2);
                memoryInfo = QString(tr("Память: %1 / %2")).arg(usedMemory, totalMemory);
                break;
            }
        }
    }
    ui->action_memory->setText(memoryInfo);

    QString externalIp;
    QProcess process;
    process.start("curl", QStringList() << "ifconfig.me");
    process.waitForFinished();
    if (process.exitCode() == QProcess::NormalExit) {
        QByteArray response = process.readAllStandardOutput();
        externalIp = QString::fromUtf8(response).trimmed();
    }

    QString localIp;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> addresses = interface.addressEntries();
            for (const QNetworkAddressEntry& entry : addresses) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    localIp = entry.ip().toString();
                    break;
                }
            }
        }
    }

    ui->action_iphost->setText(tr("Внешний IP: %1").arg(externalIp));
    ui->action_iplocal->setText(tr("Внутренний IP: %1").arg(localIp));
}

QColor MainWindow::generateRandomColor()
{
    QColor color;
    do {
        int hue = QRandomGenerator::global()->bounded(360);
        int saturation = QRandomGenerator::global()->bounded(200);
        int value = QRandomGenerator::global()->bounded(150, 256);
        color = QColor::fromHsv(hue, saturation, value);

    } while (color.blue() > 200 || !color.isValid());
    return color;
}

void MainWindow::onTimeChanged(const QTime& time)
{
    // Если время не задано (00:00), ничего не делаем
    if (time == QTime(0, 0))
        return;

    // Получаем текущее время
    QTime currentTime = QTime::currentTime();

    // Получаем выбранное время
    QTime selectedTime = time;

    // Вычисляем разницу между текущим временем и выбранным временем в миллисекундах
    int interval = currentTime.msecsTo(selectedTime);

    // Если выбранное время уже прошло, добавляем 24 часа к интервалу
    if (interval < 0)
        interval += 24 * 60 * 60 * 1000; // 24 часа в миллисекундах

    // Устанавливаем таймер для первого вызова функции
    QTimer::singleShot(interval, this, &MainWindow::on_action_11_triggered);

    // Устанавливаем таймер для повторных вызовов функции каждый день
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::on_action_11_triggered);

    timer->start(24 * 60 * 60 * 1000); // 24 часа в миллисекундах
}

void MainWindow::showLoadingAnimation(bool show)
{
    if (animload == 0) return;

    static QWidget* overlayWidget = nullptr;
    static QLabel* loadingLabel = nullptr;
    static QPushButton* closeButton = nullptr;

    if (show) {
        if (!overlayWidget) {
            overlayWidget = new QWidget(this);
            overlayWidget->setGeometry(rect());
            overlayWidget->setObjectName("OverlayWidget");
            if (animloadpage == 0)
                overlayWidget->setStyleSheet("QWidget#OverlayWidget { background-color: #472e91; }");
            else
                overlayWidget->setStyleSheet("QWidget#OverlayWidget { background-color: #2d2b79; }");
            overlayWidget->raise();
        }
        overlayWidget->show();

        if (!loadingLabel) {
            loadingLabel = new QLabel(overlayWidget);
            loadingLabel->setObjectName("LoadingLabel");
            loadingLabel->setAlignment(Qt::AlignCenter);
            loadingLabel->setFixedSize(800, 600);
        }

        QMovie* loadingMovie = (animloadpage == 0) ? new QMovie(":/img/loading.gif") : new QMovie(":/img/loading2.gif");

        loadingLabel->setMovie(loadingMovie);
        loadingMovie->start();

        loadingLabel->move((width() - loadingLabel->width()) / 2, (height() - loadingLabel->height()) / 2);
        loadingLabel->show();

        if (!closeButton) {
            closeButton = new QPushButton(overlayWidget);
            closeButton->setObjectName("CloseButton");
            closeButton->setIcon(QIcon(":/img/18.png"));
            closeButton->setFixedSize(30, 30);
            closeButton->move(width() - closeButton->width() - 10, 10);
            connect(closeButton, &QPushButton::clicked, this, [this]() {
                showLoadingAnimation(false);
            });
        }
        closeButton->show();

    } else {
        if (overlayWidget) {
            overlayWidget->hide();
            overlayWidget->deleteLater();
            overlayWidget = nullptr;
        }

        if (loadingLabel) {
            loadingLabel->hide();
            loadingLabel->clear();
            loadingLabel->deleteLater();
            loadingLabel = nullptr;
        }

        if (closeButton) {
            closeButton->hide();
            closeButton->deleteLater();
            closeButton = nullptr;
        }
    }

    //удаление подсказок
    removeToolButtonTooltips(ui->toolBar);
    removeToolButtonTooltips(ui->toolBar_2);
}

void MainWindow::onTableAurCellClicked(int row)
{
    QTableWidgetItem* nameItem = ui->table_aur->item(row, 0);
    QString packageName = nameItem->text();

    currentProcess = new QProcess(this);

    // Сохраняем текущую позицию прокрутки
    int scrollBarValue = ui->text_details->verticalScrollBar()->value();

    connect(currentProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = currentProcess->readAllStandardOutput();
        QString packageInfo = QString::fromUtf8(output);

        // Process the packageInfo string
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

        // Устанавливаем информацию после обработки всей строки
        ui->text_details->append(processedInfo);

        // Восстанавливаем позицию прокрутки
        ui->text_details->verticalScrollBar()->setValue(scrollBarValue);

        miniAnimation(0,0,false);
    });

    // Очищаем ui->text_details перед началом нового запроса
    ui->text_details->clear();

    miniAnimation(440,260,true);

    currentProcess->start("yay", QStringList() << "-Si" << packageName);
}

void MainWindow::miniAnimation(int x, int y, bool visible)
{
    ui->label_miniload->setGeometry(x, y, 221, 31);

    if (visible) {
        ui->label_miniload->setVisible(true);
        QMovie* movie = new QMovie(":/img/load.gif");
        ui->label_miniload->setMovie(movie);
        movie->start();
    } else {
        ui->label_miniload->setVisible(false);
    }
}


void MainWindow::loadContent()
{
    ui->table_aur->setColumnCount(1); // Устанавливаем количество столбцов в таблице
    ui->table_aur->setShowGrid(false); // Убираем отображение сетки
    ui->table_aur->verticalHeader()->setVisible(false); // Убираем отображение номеров строк
    ui->table_aur->setColumnWidth(0, 320);

    ui->table_aur->setHorizontalHeaderLabels({tr("Названия пакетов")});

    // Очищаем таблицу
    ui->table_aur->clearContents();
    ui->table_aur->setRowCount(0);

    QString sourceFilePath;
    QString targetFilePath;
    sourceFilePath = ":/other/list.txt";
    targetFilePath = baseDir + "other/list.txt";

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
            int index = line.indexOf(',');

            if (index != -1)
            {
                QString program = line.mid(2, index - 4);
                programs.append(program);
            }
        }
    }

    file.close(); // Закрываем файл после использования


    for (int i = 0; i < programs.size(); i++) {
        QString packageName = programs[i];
        QColor color = generateRandomColor();
        QTableWidgetItem *item = new QTableWidgetItem();

        QString iconPath = "";
        QString prefixToRemove = "";

        static const QRegularExpression regex("(\\w+)/\\S+");
        QRegularExpressionMatch match = regex.match(packageName);

        if (match.hasMatch()) {
            QString repoName = match.captured(1);
            iconPath = ":/img/" + repoName + ".png";
            prefixToRemove = repoName + "/";
        }

        item->setIcon(QIcon(iconPath));

        // Удаление префикса, если применимо
        if (!prefixToRemove.isEmpty()) {
            packageName.remove(0, prefixToRemove.length());
        }

        item->setText(packageName);
        item->setForeground(color);

        ui->table_aur->insertRow(i);
        ui->table_aur->setItem(i, 0, item);
    }

    //событие на кнопку Enter
    connect(ui->searchApp, &QLineEdit::returnPressed, this, [=]() {
        if (page == 2)
        {
            QString text = ui->searchApp->text();
            handleServerResponse(text);

        }
    });

    //событие на нажатую кнопку
    connect(ui->push_search, &QPushButton::clicked, this, [=]() {
        if (page == 2)
        {
            QString text = ui->searchApp->text();
            handleServerResponse(text);
        }
    });

    connect(ui->searchApp, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (page == 2)
        {
            // Выполняем поиск и выделение в таблице
            int rowCount = ui->table_aur->rowCount();
            for (int i = 0; i < rowCount; ++i) {
                QTableWidgetItem *item = ui->table_aur->item(i, 0);
                if (item) {
                    QString cellText = item->text();
                    QStringList words = cellText.split(' ', Qt::SkipEmptyParts);
                    if (!words.isEmpty() && words.first().startsWith(text, Qt::CaseInsensitive)) {

                        // Выделяем всю строку
                        ui->table_aur->setCurrentCell(item->row(), 0);
                        ui->table_aur->setSelectionBehavior(QAbstractItemView::SelectRows);
                        ui->table_aur->scrollToItem(item, QAbstractItemView::EnsureVisible);
                        break;
                    }
                }
            }
        }
        else {
            if (page == 11)
                searchAndScroll(ui->list_journal, text);
            else if (page == 12)
                searchAndScroll(ui->list_bench, text);
            else if (page == 3)
                searchAndScroll(ui->list_sh, text);
            else
                searchAndScroll(ui->list_manager, text);
        }
    });
}

void MainWindow::handleServerResponse(const QString& reply)
{
    ui->table_aur->clearContents();
    miniAnimation(65, 260, true);

    currentProcess = new QProcess(this);

    connect(currentProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        QStringList packageNames;
        while (currentProcess->canReadLine()) {
            QByteArray line = currentProcess->readLine();
            QString lineString = QString::fromUtf8(line).trimmed();

            QRegularExpression regex(repo); // Добавлено условие для extra
            QRegularExpressionMatch match = regex.match(lineString);
            if (match.hasMatch()) {
                QString packageName = match.captured(1);
                packageNames.append(packageName);
            }
        }

        if (!currentProcess->atEnd()) {
            // Весь вывод еще не получен, ждем следующего сигнала readyReadStandardOutput
            return;
        }

        // Установка общего количества строк в таблице
        ui->table_aur->setRowCount(packageNames.size());
        ui->table_aur->setColumnCount(1);

        for (int i = 0; i < packageNames.size(); i++) {
            QString packageName = packageNames[i];
            QColor color = generateRandomColor();
            QTableWidgetItem *item = new QTableWidgetItem();

            QString iconPath = "";
            QString prefixToRemove = "";

            static const QRegularExpression regex("(\\w+)/\\S+");
            QRegularExpressionMatch match = regex.match(packageName);

            if (match.hasMatch()) {
                QString repoName = match.captured(1);
                iconPath = ":/img/" + repoName + ".png";
                prefixToRemove = repoName + "/";
            }

            item->setIcon(QIcon(iconPath));

            // Удаление префикса, если применимо
            if (!prefixToRemove.isEmpty()) {
                packageName.remove(0, prefixToRemove.length());
            }

            item->setText(packageName);
            item->setForeground(color);

            ui->table_aur->setItem(i, 0, item);
        }

        miniAnimation(0, 0, false);
        currentProcess->deleteLater(); // Удаляем объект QProcess после использования
    });

    currentProcess->start("yay", QStringList() << "-Ss" << reply);
}

void MainWindow::searchAndScroll(QListWidget* listWidget, const QString& text)
{
    QList<QListWidgetItem*> matchingItems = listWidget->findItems(text, Qt::MatchContains);
    if (!matchingItems.empty()) {
        listWidget->setCurrentItem(matchingItems.first());
        listWidget->scrollToItem(matchingItems.first(), QAbstractItemView::PositionAtCenter);
    }
}

void MainWindow::loadingListWidget()
{
    ui->list_sh->clear();
    ui->list_clear->clear();
    ui->list_journal->clear();
    ui->list_bench->clear();

    saveScripts(shResourcePaths, baseDir + "sh/");
    saveScripts(clearResourcePaths, baseDir + "clear/");
    saveScripts(journalsResourcePaths, baseDir + "journals/");
    saveScripts(benchResourcePaths, baseDir + "bench/");

    loadScripts(baseDir + "sh/", ui->list_sh);
    loadScripts(baseDir + "clear/", ui->list_clear);

    cacheButtonYay = new QListWidgetItem(tr("Кэш пакетов Yay"), ui->list_clear);
    cacheButtonPacman = new QListWidgetItem(tr("Кэш пакетов Pacman"), ui->list_clear);
    orphanButton = new QListWidgetItem(tr("Пакеты сироты"), ui->list_clear);

    cacheButtonYay->setIcon(QIcon(":/img/14.png"));
    cacheButtonPacman->setIcon(QIcon(":/img/14.png"));
    orphanButton->setIcon(QIcon(":/img/14.png"));

    cacheButtonYay->setForeground(generateRandomColor());
    cacheButtonPacman->setForeground(generateRandomColor());
    orphanButton->setForeground(generateRandomColor());

    loadScripts(baseDir + "journals/", ui->list_journal);
    loadScripts(baseDir + "bench/", ui->list_bench);

    QDir().mkpath(baseDir + "other/");

    QFile::copy(":/other/notify.png", baseDir + "other/notify.png");
    QFile::copy(":/other/translations_en_US.txt", baseDir + "other/translations_en_US.txt");
    QFile::copy(":/other/translations_ru_RU.txt", baseDir + "other/translations_ru_RU.txt");
    QFile::copy(":/other/main.sh", baseDir + "other/main.sh");

}

void MainWindow::loadScripts(const QString& baseDir, QListWidget* listWidget)
{
    // Очистка списка
    listWidget->clear();

    // Загрузка скриптов из папки
    QDir dir(baseDir);
    QStringList filter;
    filter << "*.sh"; // Фильтр для выбора только файлов со скриптами
    QFileInfoList fileList = dir.entryInfoList(filter);

    // Создаем два отдельных списка для скриптов
    QList<QListWidgetItem*> iconSortScripts;
    QList<QListWidgetItem*> otherScripts;

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
                if (line.startsWith("#name_" + lang))
                {
                    itemName = line.mid(12).trimmed();
                }
                else if (line.startsWith("#icon"))
                {
                    QString iconNumber = line.mid(6).trimmed();
                    iconPath = QString(":/img/%1.png").arg(iconNumber);
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

        // Сортировка
        if (benchlist == 1)
        {
            if (iconPath == ":/img/29.png" || iconPath == ":/img/53.png")
                iconSortScripts.append(item);
        }
        else if (benchlist == 2)
        {
            if (iconPath == ":/img/49.png")
                iconSortScripts.append(item);
        }
        else if (benchlist == 3)
        {
            if (iconPath == ":/img/21.png")
                iconSortScripts.append(item);
        }
        else
            otherScripts.append(item);
    }

    if(benchlist >= 1)
    {
        // Добавление элементов списка в QListWidget
        for (QListWidgetItem* item : iconSortScripts)
        {
            listWidget->addItem(item);
        }
    }
    else
    {
        for (QListWidgetItem* item : otherScripts)
        {
            listWidget->addItem(item);
        }
    }
}

void MainWindow::sendNotification(const QString& title, const QString& message)
{
    QStringList arguments;
    arguments << title << message << "-i" << baseDir + "other/notify.png" << "-a" << "kLaus" << "-t" << "10000";
    QProcess::startDetached("notify-send", arguments);
    loadSound(1);
}

void MainWindow::createArchive(const QString& folderPath, const QString& folderName)
{
    if (repair == 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите удалить каталог \"%1\" с конфигурацией?").arg(folderPath + folderName), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) return;

        QDir folderDir(folderPath);
        // Переходим в папку перед запуском процесса архивации
        if (!folderDir.cd(folderPath)) {
            sendNotification(tr("Ошибка"), tr("Не удалось перейти в каталог конфигурации!"));
            return;
        }

        // Удаляем папку и ее содержимое
        QDir folder2(folderPath + folderName);
        folder2.removeRecursively();
        loadFolders();
        sendNotification(tr("Конфигурация"), tr("Конфигурация успешно удалена!"));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Вы уверены, что хотите удалить каталог \"%1\" и создать резервную копию?\n\nВажно: Архивы начинающиеся с точки - это конфигурации из домашней директории (они скрыты по умолчанию/включите видимость скрытых файлов)").arg(folderPath + folderName), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    QProcess process;
    process.start("zenity", QStringList() << "--file-selection" << "--directory" << tr("--title=Выберите путь сохранения архива"));
    process.waitForFinished();

    if (process.exitCode() != 0) return;
        QString selectedPath = process.readAllStandardOutput().trimmed();

    if (selectedPath.isEmpty()) return;
    QDir folderDir(folderPath);

    // Проверяем, существует ли папка
    if (!folderDir.exists()) {
        sendNotification(tr("Ошибка"), tr("Каталог конфигурации не существует!"));
        return;
    }

    QString archivePath = selectedPath + "/" + folderName + ".zip";

    // Переходим в папку перед запуском процесса архивации
    if (!folderDir.cd(folderPath)) {
        sendNotification(tr("Ошибка"), tr("Не удалось перейти в каталог конфигурации!"));
        return;
    }

    QProcess zipProcess;
    zipProcess.setWorkingDirectory(folderDir.absolutePath());
    zipProcess.start("zip", QStringList() << "-r" << archivePath << folderName);
    zipProcess.waitForFinished();

    if (zipProcess.exitCode() != 0) {
        sendNotification(tr("Ошибка"), tr("Резервная копия %1 не создана!").arg(archivePath));
        return;
    }

    // Удаляем папку и ее содержимое
    QDir folder2(folderPath + folderName);
    folder2.removeRecursively();
    loadFolders();
    sendNotification(tr("Конфигурация"), tr("Резервная копия %1 успешно создана! Конфигурация удалена!").arg(archivePath));
}

void MainWindow::loadFolders()
{
    QString homeDirectory = QDir::homePath();
    QString configDirectory = homeDirectory + "/.config/";
    QDir configDir(configDirectory);
    QDir homeDir(homeDirectory);

    if (!homeDir.exists() || !configDir.exists())
        return; // Каталоги не существуют

    ui->list_repair->clear();
    int itemCount = 0;
    // Загрузка папок из папки .config
    QStringList configFolderList = configDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString& folderName : configFolderList) {

        ui->list_repair->addItem(folderName);
        itemCount++;
    }

    // Загрузка папок из домашнего каталога
    QFileInfoList homeFolderList = homeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);

    for (const QFileInfo& fileInfo : homeFolderList) {
        QString folderName = fileInfo.fileName();
        // Проверяем, начинается ли имя папки с точки
        // и не соответствует ли она заданным исключениям
        if (!folderName.startsWith('.') ||
            folderName == ".config" ||
            folderName == ".local" ||
            folderName == ".themes" ||
            folderName == ".var" ||
            folderName == ".icons" ||
            folderName == ".cache")
            continue; // Пропускаем папки без точек или исключения
        ui->list_repair->addItem(folderName);
        itemCount++;
    }

    ui->label_repair->setText(tr("Конфигурации приложений: %1").arg(itemCount));
}

void MainWindow::restoreArchive(const QString& archivePath)
{
    // Получаем имя архива без расширения
    QFileInfo fileInfo(archivePath);
    QString archiveName = fileInfo.fileName();

    // Определяем целевую директорию в зависимости от имени архива
    QString targetDir; // Путь к целевой директории

    if (!archiveName.isEmpty() && archiveName.startsWith("."))
        targetDir = QDir::homePath(); // Если имя архива начинается с ".", размещаем в домашней директории
    else
        targetDir = QDir::homePath() + "/.config"; // Иначе размещаем в папке ".config"

    // Запускаем процесс распаковки архива в целевую директорию
    QProcess process;
    process.setWorkingDirectory(targetDir);
    process.start("unzip", QStringList() << "-q" << archivePath);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        // Успешно распаковано
        sendNotification(tr("Резервная копия"), tr("Резервная копия успешно восстановлена в папку: ") + targetDir);
        loadFolders();
    } else
        sendNotification(tr("Ошибка"), tr("Не удалось восстановить конфигурацию через архив"));
}


//---#####################################################################################################################################################
//--################################################################## СОБЫТИЯ ФУНКЦИЙ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_combo_mainpage_currentIndexChanged()
{
    mainpage = ui->combo_mainpage->currentIndex();
    settings.setValue("MainPage", mainpage);
}

void MainWindow::on_combo_animload_currentIndexChanged()
{
    animloadpage = ui->combo_animload->currentIndex();
    settings.setValue("AnimLoadPage", animloadpage);
}

void MainWindow::on_combo_host_currentIndexChanged(int index)
{
    host = index;
    settings.setValue("Host", host);

    if (host == 1)
    {
        QProcess process;
        process.start("httpd", QStringList() << "-v");
        process.waitForFinished();

        if (process.exitCode() == 0) {
            // Apache установлен
            ui->combo_host->setCurrentIndex(1);
        }
        else {
            // Apache не установлен, предложить установку
            int reply = QMessageBox::question(this, "Установка Apache", "Apache не установлен. Хотите установить Apache?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QProcess installProcess;
                installProcess.setProcessChannelMode(QProcess::MergedChannels);

                Terminal terminal = getTerminal();
                installProcess.start(terminal.binary, QStringList() << terminal.args << "yay -S apache");

                installProcess.closeWriteChannel();
                installProcess.waitForFinished();
                if (installProcess.exitCode() == 0)
                    ui->combo_host->setCurrentIndex(1);
                else {
                    QMessageBox::critical(this, "Ошибка установки", "Произошла ошибка при установке Apache");
                    ui->combo_host->setCurrentIndex(0);
                }
            } else
                ui->combo_host->setCurrentIndex(0);
        }
    }
    else if (host == 2)
    {
        QProcess process;
        process.start("/opt/lampp/lampp", QStringList() << "status");
        process.waitForFinished();

        if (process.exitCode() == 0) {
            ui->combo_host->setCurrentIndex(2);
        }
        else {
            int reply = QMessageBox::question(this, "Установка XAMPP", "XAMPP не установлен. Хотите установить XAMPP?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QProcess installProcess;
                installProcess.setProcessChannelMode(QProcess::MergedChannels);

                Terminal terminal = getTerminal();
                installProcess.start(terminal.binary, QStringList() << terminal.args << "yay -S xampp");

                installProcess.closeWriteChannel();
                installProcess.waitForFinished();
                if (installProcess.exitCode() == 0)
                    ui->combo_host->setCurrentIndex(2);
                else {
                    QMessageBox::critical(this, "Ошибка установки", "Произошла ошибка при установке XAMPP");
                    ui->combo_host->setCurrentIndex(0);
                }
            } else
                ui->combo_host->setCurrentIndex(0);
        }
    }
}

void MainWindow::on_combo_lang_currentIndexChanged(int index)
{
    QString lang;
    if (index == 0)
        lang = "ru_RU";
    else if (index == 1)
        lang = "en_US";

    // Получение текущего значения lang из настроек
    QString currentLang = settings.value("Language", "en_US").toString();

    // Проверка, совпадает ли выбранный язык с текущим языком
    if (currentLang != lang) {
        settings.setValue("Language", lang);

        QTranslator translator;
        if (lang == "ru_RU") {
            if (translator.load(":/lang/kLaus_ru.qm")) {
                qApp->installTranslator(&translator);
            }
        } else if (lang == "en_US") {
            if (translator.load(":/lang/kLaus_en.qm")) {
                qApp->installTranslator(&translator);
            }
        }

        sendNotification(tr("Смена языка"), tr("Приложение будет перезагружено для смены языка"));

        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_combo_bench_currentIndexChanged(int index)
{
    benchlist = index;
    loadScripts(baseDir + "bench/", ui->list_bench);
}

void MainWindow::on_combo_cache_currentIndexChanged()
{
    yaycache = ui->combo_cache->currentIndex();
    settings.setValue("YayCache", yaycache);
}

void MainWindow::on_time_update_timeChanged(const QTime &time)
{
    // Проверяем, что время корректно
    if (time.isValid()) {
        timeupdate = time; // Обновляем глобальную переменную timeupdate
        settings.setValue("TimeUpdate", timeupdate.toString("HH:mm")); // Сохраняем значение времени в настройках
    } else
        sendNotification(tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_time_tea_timeChanged(const QTime &time)
{
    // Проверяем, что время корректно
    if (time.isValid()) {
        timetea = time; // Обновляем глобальную переменную timetea
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
    // Проверяем, что время корректно
    if (time.isValid()) {
        timework = time; // Обновляем глобальную переменную timetea
        settings.setValue("TimeWork", timework.toString("HH:mm")); // Сохраняем значение времени в настройках

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
    teatext = arg1;
    settings.setValue("TeaText", arg1);
}

void MainWindow::on_line_work_textChanged(const QString &arg1)
{
    worktext = arg1;
    settings.setValue("WorkText", arg1);
}

void MainWindow::on_check_trayon_stateChanged()
{
    trayon = ui->check_trayon->isChecked() ? 1 : 0;
    settings.setValue("TrayOn", trayon);
}

void MainWindow::on_check_repair_stateChanged()
{
    repair = ui->check_repair->isChecked() ? 1 : 0;
    settings.setValue("RepairBackup", repair);
}

void MainWindow::on_check_animload_stateChanged()
{
    animload = ui->check_animload->isChecked() ? 1 : 0;
    settings.setValue("AnimLoad", animload);
}

void MainWindow::on_check_updateinstall_stateChanged()
{
    updinst = ui->check_updateinstall->isChecked() ? 1 : 0;
    settings.setValue("UpdateInstall", updinst);
}

void MainWindow::on_dial_volmenu_valueChanged(int value)
{
    volumemenu = value;
    settings.setValue("VolumeMenu", value);

    QString labelvolmenu = QString(tr("Звук меню: %1/100")).arg(volumemenu);
    ui->label_volmenu->setText(labelvolmenu);
}

void MainWindow::on_dial_volnotify_valueChanged(int value)
{
    volumenotify = value;
    settings.setValue("VolumeNotify", value);
    QString labelvolnotify = QString(tr("Звук уведомлений: %1/100")).arg(volumenotify);
    ui->label_volnotify->setText(labelvolnotify);
}


void MainWindow::handleListItemDoubleClick(QListWidgetItem *item, const QString& scriptDir) {
    Terminal terminal = getTerminal();

    if (item == orphanButton) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), tr("Обычно, когда пакет становится сиротой, это означает, что он был установлен в качестве зависимости другого пакета, но этот пакет был удален, и больше нет других пакетов, которые бы зависели от данного. Удаление сирот из системы помогает поддерживать систему более чистой и оптимизированной. Вы действительно хотите удалить пакеты сироты?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString command = QString("bash -c 'yay -Rs $(yay -Qdtq)'");
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << command);
        }
        return;
    }
    if (item == cacheButtonYay || item == cacheButtonPacman) {
        QString command;
        QMessageBox::StandardButton replymod = QMessageBox::No; // Инициализация значением по умолчанию
        switch (yaycache) {
            case 0: {
                replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш только неустановленных пакетов (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                command = (item == cacheButtonYay) ? "yay -Sc" : "sudo pacman -Sc";
                break;
            }
            case 1: {
                replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш пакетов всех пакетов (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                command = (item == cacheButtonYay) ? "yay -Scc" : "sudo pacman -Scc";
                break;
            }
            case 2: {
                if(item == cacheButtonYay)
                    sendNotification(tr("Ошибка"), tr("Yay так не умеет, измените настройки удаления кэша"));
                else
                {
                    replymod = QMessageBox::question(this, tr("Вопрос"), tr("При обновлении пакетов старые версии пакетов сохраняются в кэше, чтобы вы могли откатиться к предыдущим версиям, если это необходимо. Однако, если вы не планируете откатываться к предыдущим версиям пакетов, удаление кэша может помочь вам освободить дополнительное место на диске. Вы действительно хотите удалить кэш всех пакетов кроме последних трех версий (можно изменить в настройках)?"), QMessageBox::Yes | QMessageBox::No);
                    command = "paccache -rvk3";
                }
                break;
            }
        }

        if (replymod == QMessageBox::Yes)
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << command);

        return;
    }


    QString scriptPath;
    QString msg;
    QString itemName = item->text();

    QDir dir(scriptDir);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    for (const auto& fileInfo : fileInfoList) {
        QFile scriptFile(fileInfo.absoluteFilePath());
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream scriptStream(&scriptFile);
            while (!scriptStream.atEnd()) {
                QString line = scriptStream.readLine();
                if (line.startsWith("#name_" + lang)) {
                    QString name = line.mid(12).trimmed();
                    if (name == itemName)
                        scriptPath = fileInfo.absoluteFilePath();
                }
                else if (line.startsWith("#msg_" + lang))
                    msg = line.mid(11).trimmed();
            }
            scriptFile.close();
        }
        if (!scriptPath.isEmpty())
            break;
    }

    if (scriptPath.isEmpty())
        scriptPath = scriptDir + itemName;

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), msg, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "bash" << scriptPath << lang);
}

void MainWindow::on_list_sh_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "sh/";
    handleListItemDoubleClick(item, scriptDir);
}


void MainWindow::on_list_journal_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "journals/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_bench_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "bench/";
    handleListItemDoubleClick(item, scriptDir);
}

void MainWindow::on_list_clear_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "clear/";
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

void MainWindow::on_push_site_clicked()
{
    openUrl("http://localhost/");
}

void MainWindow::openUrl(const QString& url)
{
    QProcess process;
    process.start("xdg-open", QStringList() << url);
    process.waitForFinished();

    // Проверяем код возврата
    if (process.exitCode() != 0) {
        // Проверяем наличие Firefox
        QString firefoxPath = QStandardPaths::findExecutable("firefox");
        if (!firefoxPath.isEmpty()) {
            QProcess processSettings;
            processSettings.start("xdg-settings", QStringList() << "set default-web-browser firefox.desktop");
            processSettings.waitForFinished();

            QProcess process;
            process.start("xdg-open", QStringList() << url);
            process.waitForFinished();
        }
        else
            sendNotification(tr("Ошибка"), tr("Установите Firefox или выберите рабочий браузер в качестве основного в настройках системы!"));
    }
}

