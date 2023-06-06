//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
#include <QSoundEffect>
#include <unistd.h>
#include <sys/utsname.h>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString baseDir = QDir::homePath() + "/kLaus/";
QString filePath = baseDir + "/kLaus/settings.ini";
QSettings settings(filePath, QSettings::IniFormat);
QString currentVersion = "3.3";

//---#####################################################################################################################################################
//--############################################################## ОПРЕДЕЛЕНИЕ ТЕРМИНАЛА ################################################################
//-#####################################################################################################################################################

Terminal getTerminal()
{
    // Итерируемся по списку, пока не найдем установленный терминал
    for (const Terminal &terminal : m_terminalList) {
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
    mrpropper();
    page = 1;
    ui->label1->setText(tr("Добавить пакет в AUR"));
}

void MainWindow::on_action_2_triggered()
{
    if (page == 2) return;
    ui->webEngineView->page()->triggerAction(QWebEnginePage::Stop);
    mrpropper();
    page = 2;
    ui->label1->setText(tr("Каталог пакетов из AUR"));

    ui->action_4->setVisible(true);
    ui->action_5->setVisible(true);
    ui->action_6->setVisible(true);
    ui->action_16->setVisible(true);
    ui->action_30->setVisible(true);
    ui->action_34->setVisible(true);
    ui->table_aur->setVisible(true); //Показываем список устнановить
    ui->searchApp->setVisible(true);
    showLoadingAnimation(false);
}

void MainWindow::on_action_17_triggered()
{
    if (page == 3) return;
    mrpropper();
    page = 3;
    ui->label1->setText(tr("Очистка системы"));
    ui->label2->setVisible(true);
    ui->list_clear->setVisible(true);
    ui->list_sh->setVisible(true);
    ui->action_sh->setVisible(true);
    ui->action_18->setVisible(true);
    ui->action_addsh->setVisible(true);
    ui->action_editsh->setVisible(true);
    ui->action_rmsh->setVisible(true);
    showLoadingAnimation(false);
}

void MainWindow::on_action_7_triggered()
{
    if (page == 4) return;
    mrpropper();
    page = 4;

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
    ui->action_24->setVisible(true);
    ui->action_25->setVisible(true);
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

            // Раскрашиваем
            QColor color = generateRandomColor();
            QListWidgetItem* item = new QListWidgetItem(package);
            item->setForeground(color);
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
    mrpropper();
    page = 5;
    ui->label1->setText(tr("Параметры ядра"));
    ui->action_26->setVisible(true);
    ui->action_27->setVisible(true);
    ui->line_grub->setVisible(true);
    ui->spin_grub->setVisible(true);
    ui->label_grub->setVisible(true);
    ui->label3_grub->setVisible(true);
    ui->label4_grub->setVisible(true);
    ui->list_grub->setVisible(true);

    QString filename = "/etc/default/grub";
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->action_26->setDisabled(true);
        ui->line_grub->setDisabled(true);
        ui->spin_grub->setDisabled(true);
        ui->line_grub->setText(tr("GRUB не установлен"));
        // Ошибка открытия файла
    } else {

        QTextStream in(&file);
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

        int timeout = timeoutStr.toInt(); // получаем значение timeout из файла
        ui->spin_grub->setValue(timeout); // устанавливаем значение в QSpinBox
        ui->line_grub->setText(grubContent);
    }
    showLoadingAnimation(false);
}

void MainWindow::on_action_3_triggered()
{
    if (page == 6) return;
    ui->webEngineView->page()->triggerAction(QWebEnginePage::Stop);
    mrpropper();
    page = 6;
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
    process.start("yay", QStringList() << "-Qs" << "ocs-url");
    if (process.waitForFinished()) {
        QString output = QString::fromUtf8(process.readAllStandardOutput());
        if (!output.contains("ocs-url")) {
            sendNotification(tr("Ошибка"), tr("Установите пакет ocs-url для установки тем!"));
            return;
        }
    }
    ui->webEngineView->page()->triggerAction(QWebEnginePage::Stop);
    mrpropper();
    page = 7;
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
    mrpropper();
    page = 8;
    ui->label1->setText(tr("Информация о приложении (версия %1)").arg(currentVersion));
    ui->tabWidget->setVisible(true);
    ui->tabWidget->setCurrentIndex(2);
    showLoadingAnimation(false);
}

void MainWindow::on_action_12_triggered()
{
    if (page == 9) return;
    mrpropper();
    page = 9;
    ui->label1->setText(tr("Дополнительные настройки"));
    ui->action_28->setVisible(true);
    ui->action_29->setVisible(true);
    ui->tabWidget->setVisible(true);

    ui->check_trayon->setChecked(trayon == 1);
    ui->check_description->setChecked(table1 == 1);
    ui->check_version->setChecked(table2 == 1);
    ui->check_voices->setChecked(table3 == 1);
    ui->check_popularity->setChecked(table4 == 1);
    ui->check_lastupdate->setChecked(table5 == 1);
    ui->combo_mainpage->setCurrentIndex(mainpage);
    ui->time_update->setTime(timeupdate);
    ui->spin_rating->setValue(fav);
    ui->tabWidget->setCurrentIndex(0);

    ui->dial_volmenu->setValue(volumemenu);
    ui->dial_volnotify->setValue(volumenotify);

    if(lang == "ru_RU")
        ui->combo_lang->setCurrentIndex(0);
    else if(lang == "en_US")
        ui->combo_lang->setCurrentIndex(1);

    showLoadingAnimation(false);
}

//---#####################################################################################################################################################
//--################################################################## БЫСТРЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################

void MainWindow::openDirectory(const QString &directoryPath)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::homePath() + directoryPath));
}

void MainWindow::on_action_sh_triggered()
{
    openDirectory("/kLaus/sh/");
}

void MainWindow::on_action_18_triggered()
{
    openDirectory("/.local/share/applications");
}

void MainWindow::on_action_27_triggered()
{
    openDirectory("/kLaus/journals/");
}

void MainWindow::on_action_28_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_action_29_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_action_31_triggered()
{
    QWebEnginePage* pagez = ui->webEngineView->page();

    if (page == 6)
    {
        showLoadingAnimation(true);
        if (lang == "en_US")
            pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations"));
        else
            pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));
    }
    else if (page == 7)
    {
        showLoadingAnimation(true);
        if(currentDesktop == "KDE")
            pagez->load(QUrl("https://store.kde.org/browse/"));
        else if(currentDesktop == "GNOME")
            pagez->load(QUrl("https://www.pling.com/s/Gnome/browse/"));
        else if(currentDesktop == "XFCE")
            pagez->load(QUrl("https://www.pling.com/s/XFCE/browse/"));
        else if(currentDesktop == "LXQt")
            pagez->load(QUrl("https://store.kde.org/browse?cat=446"));
        else if(currentDesktop == "Cinnamon")
            pagez->load(QUrl("https://www.pling.com/s/Cinnamon/browse/"));
        else if(currentDesktop == "MATE")
            pagez->load(QUrl("https://www.pling.com/s/Mate/browse/"));
        else if (currentDesktop == "Enlightenment")
            pagez->load(QUrl("https://www.pling.com/s/Enlightenment/browse/"));
        else {
            sendNotification(tr("Ошибка"), tr("Для вашего окружения тем не найдено!"));
            return;
        }
    }
}

void MainWindow::on_action_34_triggered()
{
    if (ui->table_aur->currentItem() != nullptr) {
        showLoadingAnimation(true);
        int currentRow = ui->table_aur->currentRow();
        QTableWidgetItem *item = ui->table_aur->item(currentRow, 0);
        QString url = item->data(Qt::UserRole).toString();
        if (!url.isEmpty()) {
            ui->webEngineView->page()->load(QUrl(url));
        } else
            sendNotification(tr("Внимание"), tr("URL отсутствует!"));
    } else
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для просмотра информации!"));
}

void MainWindow::on_action_35_triggered()
{
    if (page == 6 || page == 7)
    {
        ui->webEngineView->back();
        return;
    }

    ui->action_16->setVisible(true);
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
    Terminal terminal = getTerminal();
    QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -Syu");
}

void MainWindow::on_action_24_triggered()
{
    Terminal terminal = getTerminal();
    QString command = QString("bash -c 'yay -Rs $(yay -Qdtq)'");
    QProcess::startDetached(terminal.binary, QStringList() << terminal.args << command);
}

void MainWindow::on_action_25_triggered()
{
    Terminal terminal = getTerminal();

    switch(yaycache) {
        case 0:
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -Sc");
            break;
        case 1:
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -Scc");
            break;
        case 2:
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "paccache -rvk3");
            break;
        default:
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -Sc");
            break;
    }
}

void MainWindow::on_action_13_triggered()
{
    loadSystemInfo();
    sendNotification(tr("Информация"), tr("Информация успешно обновлена!"));
}

void MainWindow::on_action_5_triggered()
{
    if (page == 2)
    {
        if (ui->table_aur->currentItem() != nullptr) {
            QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
            QProcess* process = new QProcess(this);

            // Обработчик вывода информации из процесса
            connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
                QString output = process->readAllStandardOutput();
                static QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
                QRegularExpressionMatch match = re.match(output);
                if (match.hasMatch()) {
                    QString appName = match.captured(1);
                    QString program = appName.split('/').last();
                    if (program.endsWith("-bin") || program.endsWith("-git") || program.endsWith("-qt") || program.endsWith("-qt4") || program.endsWith("-qt5") || program.endsWith("-qt6") || program.endsWith("qt-") || program.endsWith("qt4-") || program.endsWith("qt5-") || program.endsWith("qt6-") || program.endsWith("-gtk") || program.endsWith("-gtk2") || program.endsWith("-gtk3"))
                        program.chop(4); // Убираем "-bin" или "-git" из имени программы

                    Terminal terminal = getTerminal();
                    QProcess::startDetached(terminal.binary, QStringList() << terminal.args << program);
                } else
                    sendNotification(tr("Пакет не найден"), tr("Пакет ") + packageName + tr(" не найден в системе!"));
                process->deleteLater();
            });

            // Обработчик вывода ошибок из процесса
            connect(process, &QProcess::readyReadStandardError, this, [=]() {
                sendNotification(tr("Ошибка"), process->readAllStandardError());
                process->deleteLater();
            });

            // Запускаем процесс
            process->start("yay", QStringList() << "-Qi" << packageName);
        } else
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));

    } else {
        if (ui->list_manager->currentItem() != nullptr) {
            QString packageName = ui->list_manager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            Terminal terminal = getTerminal();
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << packageName);
        } else
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
    }
}

void MainWindow::on_action_6_triggered()
{
    if (page == 2)
    {
        if (ui->table_aur->currentItem() != nullptr) {
            QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
            QProcess* process = new QProcess(this);

            // Обработчик вывода информации из процесса
            connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
                QString output = process->readAllStandardOutput();
                static QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
                QRegularExpressionMatch match = re.match(output);
                if (match.hasMatch()) {
                    QString appName = match.captured(1);
                    QString program = appName.split('/').last();
                    if (program.endsWith("-bin") || program.endsWith("-git") || program.endsWith("-qt") || program.endsWith("-qt4") || program.endsWith("-qt5") || program.endsWith("-qt6") || program.endsWith("qt-") || program.endsWith("qt4-") || program.endsWith("qt5-") || program.endsWith("qt6-") || program.endsWith("-gtk") || program.endsWith("-gtk2") || program.endsWith("-gtk3"))
                        program.chop(4); // Убираем "-bin" или "-git" из имени программы

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
            process->start("yay", QStringList() << "-Qi" << packageName);
        } else
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));

    } else {
        if (ui->list_manager->currentItem() != nullptr) {
            QString packageName = ui->list_manager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            Terminal terminal = getTerminal();
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -R " + packageName);
        } else
             sendNotification(tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
    }
}

void MainWindow::on_action_4_triggered()
{
    if (page == 2)
    {
        if (ui->table_aur->currentItem() != nullptr) {
            QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
            Terminal terminal = getTerminal();
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -S " + packageName);
         } else
            sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));

    } else {
        if (ui->list_manager->currentItem() != nullptr) {
            QString packageName = ui->list_manager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            Terminal terminal = getTerminal();
            QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "yay -S " + packageName);
        } else
             sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
    }
}

void MainWindow::on_action_30_triggered()
{
    if (ui->table_aur->currentItem() != nullptr) {
        QString packageName = ui->table_aur->item(ui->table_aur->currentRow(), 0)->text();
        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "bash" << baseDir + "sh/PKGBUILD.sh" << lang << packageName);
    } else
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для установки!"));
}

void MainWindow::on_action_26_triggered()
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

void MainWindow::on_action_16_triggered()
{
    // Удаление содержимого таблицы
    ui->table_aur->clearContents();
    ui->table_aur->setRowCount(0);
    ui->table_aur->setColumnHidden(2, true);
    ui->table_aur->setColumnHidden(3, true);
    ui->table_aur->setColumnHidden(4, true);
    ui->table_aur->setColumnHidden(5, true);
    ui->table_aur->setColumnWidth(1, 680);

    QString sourceFilePath;
    QString targetFilePath;
    if(lang == "en_US")
    {
        sourceFilePath = ":/other/list_en.txt";
        targetFilePath = baseDir + "other/list_en.txt";
    } else {
        sourceFilePath = ":/other/list.txt";
        targetFilePath = baseDir + "other/list.txt";
    }

    QFileInfo fileInfo(targetFilePath);
    if (!fileInfo.exists()) {
        if (!QDir().mkpath(fileInfo.absoluteDir().path())) {
            sendNotification(tr("Ошибка"), tr("Не удалось создать каталог: ") + fileInfo.absoluteDir().path());
            return;
        }
        if (!QFile::copy(sourceFilePath, targetFilePath)) {
            sendNotification(tr("Ошибка"), tr("Не удалось скопировать файл из ") + sourceFilePath + tr(" в ") + targetFilePath);
            return;
        }
    }

    QFile file(targetFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sendNotification(tr("Ошибка"), tr("Не удалось открыть файл ресурсов"));
        return;
    }

    QTextStream in(&file);
    QVector<QPair<QString, QString>> programs;
    while (!in.atEnd()) {
        QString line = in.readLine();
        line = line.trimmed();
        if (!line.isEmpty()) {
            int index = line.indexOf(',');
            if (index != -1) {
                QString program = line.mid(2, index-3);
                QString description = line.mid(index+3, line.length()-index-4);
                programs.append(qMakePair(program, description));
            }
        }
    }

    for (int i = 0; i < programs.size(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem(programs[i].first);
        QTableWidgetItem *item2 = new QTableWidgetItem(programs[i].second);

        // Генерация случайного яркого цвета
        QColor color = generateRandomColor();

        // Установка цвета текста и фона ячеек таблицы
        item1->setForeground(color);
        item2->setForeground(color);

        ui->table_aur->insertRow(i);
        ui->table_aur->setItem(i, 0, item1);
        ui->table_aur->setItem(i, 1, item2);
    }
}

void MainWindow::removeToolButtonTooltips(QToolBar* toolbar) {
    // Получение стиля текущей темы
    QStyle* style = qApp->style();
    QList<QAction*> actions = toolbar->actions();

    // Отключение отображения подсказок для виджетов
    for (QAction* action : actions) {
        QWidget* widget = toolbar->widgetForAction(action);
        if (widget) {
            widget->setToolTip("");
            widget->setStyle(style);
            widget->setToolTipDuration(0); // Отключение подсказок
        }
    }
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
    // Получаем указатель на текущий выбранный элемент списка
    QListWidgetItem* selectedItem = ui->list_sh->currentItem();

    if (selectedItem)
    {
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
                            // Удаляем найденный файл
                            scriptFile.close();
                            if (QFile::remove(filePath))
                            {
                                // Удаляем выбранный элемент из списка
                                delete ui->list_sh->takeItem(ui->list_sh->row(selectedItem));
                                sendNotification(tr("Удаление"), tr("Скрипт успешно удален!"));
                            }
                            return;
                        }
                        break;
                    }
                }
                scriptFile.close();
            }
        }
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
    // Получаем указатель на текущий выбранный элемент списка
    QListWidgetItem* selectedItem = ui->list_sh->currentItem();

    if (selectedItem)
    {
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
}

//---#####################################################################################################################################################
//--################################################################# ОСНОВНЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
//-############################### ПРОДОЛЖАЕМ... ####################################
//-##################################################################################

    removeToolButtonTooltips(ui->toolBar);
    loadSettings(); //загрузка настроек
    checkVersionAndClear();

    loadContent(); //загрузка списков приложений игр и тп
    loadingListWidget();
    loadSystemInfo();


    showLoadingAnimation(false);
}

void MainWindow::checkVersionAndClear() {
    QString settingsFilePath = baseDir + "/settings.ini";
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    QString storedVersion = settings.value("Version").toString();
    QString storedLanguage = settings.value("Language").toString();

    if (storedVersion.isEmpty() || storedVersion != currentVersion) {
        settings.setValue("Version", currentVersion);
        settings.setValue("Language", storedLanguage);
        settings.sync();
        removeScripts(shResourcePaths, baseDir + "/sh/");
        removeScripts(clearResourcePaths, baseDir + "/clear/");
        removeScripts(journalsResourcePaths, baseDir + "/journals/");
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
        hide(); // Скрываем главное окно
        event->ignore(); // Игнорируем событие закрытия
    }
}

void MainWindow::mrpropper() //зачистка говна перед началом каждой вкладки
{
    ui->action_addsh->setVisible(false);
    ui->action_rmsh->setVisible(false);
    ui->action_editsh->setVisible(false);
    ui->table_aur->setVisible(false);
    ui->list_clear->setVisible(false);
    ui->list_sh->setVisible(false);
    ui->list_manager->setVisible(false);
    ui->list_grub->setVisible(false);
    ui->line_grub->setVisible(false);
    ui->spin_grub->setVisible(false);
    ui->label_grub->setVisible(false);
    ui->label3_grub->setVisible(false);
    ui->label4_grub->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->searchApp->setVisible(false);
    ui->action_35->setVisible(false);
    ui->action_4->setVisible(false);
    ui->action_5->setVisible(false);
    ui->action_6->setVisible(false);
    ui->action_11->setVisible(false);
    ui->action_16->setVisible(false);
    ui->action_sh->setVisible(false);
    ui->action_18->setVisible(false);
    ui->action_24->setVisible(false);
    ui->action_25->setVisible(false);
    ui->action_26->setVisible(false);
    ui->action_27->setVisible(false);
    ui->action_28->setVisible(false);
    ui->action_29->setVisible(false);
    ui->action_30->setVisible(false);
    ui->action_31->setVisible(false);
    ui->action_32->setVisible(false);
    ui->action_33->setVisible(false);
    ui->action_34->setVisible(false);
    ui->webEngineView->setVisible(false);
    ui->label2->setVisible(false);
    ui->label1->setVisible(true);
    showLoadingAnimation(true);
    loadSound(0);
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

    if (soundIndex == 1)
    {
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
    mainpage = settings.value("MainPage", 0).toInt();
    yaycache = settings.value("YayCache", 0).toInt();
    trayon = settings.value("TrayOn", 0).toInt();
    volumenotify = settings.value("VolumeNotify", 30).toInt();
    volumemenu = settings.value("VolumeMenu", 50).toInt();
    table1 = settings.value("Table1", 1).toInt();
    table2 = settings.value("Table2", 1).toInt();
    table3 = settings.value("Table3", 0).toInt();
    table4 = settings.value("Table4", 0).toInt();
    table5 = settings.value("Table5", 0).toInt();
    fav = settings.value("Favorite", 50).toInt();
    lang = settings.value("Language").toString();

    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");

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

    connect(ui->spin_grub, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_grub_valueChanged(int)));
    connect(ui->time_update, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(ui->spin_rating, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_spin_rating_valueChanged);

    QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
    profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);

    QObject::connect(ui->webEngineView->page(), &QWebEnginePage::loadStarted, this, [=]() {
        if (page == 2 || page == 6 || page == 7)
        {
            QFile scriptFile(":/loading.browser.js"); // Путь к вашему файлу скрипта
            if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&scriptFile);
                QString script = stream.readAll();
                ui->webEngineView->page()->runJavaScript(script);
            }
        }
    });

    // Флаг для отслеживания показа уведомления об ошибке
    bool errorShown = false;

    QObject::connect(ui->webEngineView->page(), &QWebEnginePage::loadFinished, this, [=](bool success) mutable{
        if (success) {
            if (page == 6 || page == 7)
                ui->webEngineView->show();
            else if (page == 2)
            {
                ui->action_16->setVisible(false);
                ui->action_34->setVisible(false);
                ui->action_35->setVisible(true);
                ui->webEngineView->show();
            }
        } else {
            // Проверяем, было ли уже показано уведомление об ошибке
            if (!errorShown) {
                if (page == 2)
                {
                    sendNotification(tr("Ошибка"), tr("Страница не найдена (ошибка 404)"));
                    errorShown = true;  // Устанавливаем флаг, что уведомление было показано
                }
            }
        }
        showLoadingAnimation(false);
    });
    QTableWidget *table = ui->table_aur;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    //копирование информации о системе
    QList<QAction*> actionList;
    actionList << ui->action_memory << ui->action_cpu << ui->action_gpu << ui->action_hostname << ui->action_os
               << ui->action_packages << ui->action_release << ui->action_screen;
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

    //Считываем volume
    QString labelvolmenu = QString(tr("Звук меню: %1/100")).arg(volumemenu);
    ui->label_volmenu->setText(labelvolmenu);
    QString labelvolnotify = QString(tr("Звук уведомлений: %1/100")).arg(volumenotify);
    ui->label_volnotify->setText(labelvolnotify);
}

void MainWindow::loadSystemInfo()
{
//-##################################################################################
//-############################ СИСТЕМНАЯ ИНФОРМАЦИЯ ################################
//-##################################################################################
    char* username = getlogin();
    if (username != nullptr) {
        QString name_2 = QString::fromUtf8(username);
        QString welcomeMessage = QString(tr("Добро пожаловать, %1!")).arg(name_2);
        ui->menu_2->setTitle(welcomeMessage);
    }

    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    QString hostnameMessage = QString(hostname);
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
    static QLabel* label = nullptr;
    static QTimer* hideTimer = nullptr;

    if (show) {
        // Если иконка уже показывается, не делаем ничего
        if (label != nullptr)
            return;

        // Создаем новый QLabel и загружаем в него изображение
        QPixmap pixmap(":/img/load_1.png");
        label = new QLabel(this);
        label->setPixmap(pixmap);
        label->setAlignment(Qt::AlignCenter);
        label->setMaximumSize(30, 30);

        // Устанавливаем QLabel в статусбар
        ui->statusBar->addWidget(label);

        // Меняем картинки с определенной задержкой
        QTimer *timer = new QTimer(this);
        int counter = 2;
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            QString fileName = QString(":/img/load_%1.png").arg(counter);

            QPixmap pixmap(fileName);
            if (label != nullptr)
                label->setPixmap(pixmap);

            if (counter >= 3)
                counter = 1;
            else
                counter++;
        });
        timer->start(200);

        // Отключаем таймер для скрытия иконки
        if (hideTimer != nullptr) {
            hideTimer->stop();
            hideTimer = nullptr;
        }
    } else {
        // Если иконка не показывается, не делаем ничего
        if (label == nullptr)
            return;

        // Запускаем таймер для скрытия иконки через 3 секунды
        hideTimer = new QTimer(this);
        hideTimer->setSingleShot(true);
        connect(hideTimer, &QTimer::timeout, this, [=]() mutable {
            // Отключаем таймер и отключаем связанный с ним слот
            if (label != nullptr) {
                QTimer* timer = label->findChild<QTimer*>();
                if (timer != nullptr) {
                    QObject::disconnect(timer, &QTimer::timeout, nullptr, nullptr);
                }

                ui->statusBar->removeWidget(label);
                delete label;
                label = nullptr;
            } else
                qDebug() << "QLabel is not created yet!";
        });
        hideTimer->start(300);
    }
    removeToolButtonTooltips(ui->toolBar_2);
}

void MainWindow::handleServerResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj["type"].toString() == "search") {
            QJsonArray results = jsonObj["results"].toArray();
            ui->table_aur->setRowCount(results.size()); // Устанавливаем количество строк в таблице
            ui->table_aur->setColumnCount(6); // Устанавливаем количество столбцов в таблице
            ui->table_aur->setShowGrid(false); // Убираем отображение сетки
            ui->table_aur->verticalHeader()->setVisible(false); // Убираем отображение номеров строк
            ui->table_aur->setColumnWidth(0, 250);
            ui->table_aur->setColumnWidth(1, 550);
            ui->table_aur->setColumnWidth(2, 130);
            ui->table_aur->setColumnWidth(3, 70);
            ui->table_aur->setColumnWidth(4, 110);
            ui->table_aur->setColumnWidth(5, 170);

            ui->table_aur->setHorizontalHeaderLabels({tr("Название"), tr("Описание"), tr("Версия"), tr("Голоса"), tr("Популярность"), tr("Последнее обновление")});

            ui->table_aur->setColumnHidden(1, table1 == 0);
            ui->table_aur->setColumnHidden(2, table2 == 0);
            ui->table_aur->setColumnHidden(3, table3 == 0);
            ui->table_aur->setColumnHidden(4, table4 == 0);
            ui->table_aur->setColumnHidden(5, table5 == 0);

            for (int i = 0; i < results.size(); i++) {
                QString name = results[i].toObject()["Name"].toString();
                QString version = results[i].toObject()["Version"].toString();
                QString description = results[i].toObject()["Description"].toString();
                int votes = results[i].toObject()["NumVotes"].toInt();
                double popularity = results[i].toObject()["Popularity"].toDouble();
                qint64 timestamp = results[i].toObject()["LastModified"].toInt();
                int outofdate = results[i].toObject()["OutOfDate"].toInt();
                QString packageURL = results[i].toObject()["URL"].toString();

                QColor color = generateRandomColor();

                QTableWidgetItem *item = new QTableWidgetItem(name);
                item->setForeground(color);
                ui->table_aur->setItem(i, 0, item);

                if (votes >= fav) {
                    QIcon icon(":/img/p_1.png");
                    item->setIcon(icon);
                }

                QTableWidgetItem *item1 = new QTableWidgetItem(description);
                item1->setForeground(color);
                ui->table_aur->setItem(i, 1, item1);

                QTableWidgetItem *item2 = new QTableWidgetItem(version);
                item2->setForeground(color);
                if (outofdate) {
                    item2->setFlags(item2->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
                    QFont font = item2->font();
                    font.setStrikeOut(true);
                    item2->setFont(font);
                } else
                    item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);

                ui->table_aur->setItem(i, 2, item2);

                QTableWidgetItem *item3 = new QTableWidgetItem();
                item3->setForeground(color);
                item3->setData(Qt::DisplayRole, votes);
                ui->table_aur->setItem(i, 3, item3);

                QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(popularity, 'f', 2));
                item4->setForeground(color);
                ui->table_aur->setItem(i, 4, item4);

                QDateTime dateTime;
                dateTime.setMSecsSinceEpoch(timestamp * 1000);
                QString date = dateTime.toString("yyyy-MM-dd");
                QTableWidgetItem *item5 = new QTableWidgetItem(date);
                item5->setForeground(color);
                ui->table_aur->setItem(i, 5, item5);

                item->setForeground(color);
                item->setData(Qt::UserRole, packageURL); // Установка данных пользовательской роли
                ui->table_aur->setItem(i, 0, item);
            }
        }
        // установка атрибутов сортировки и выбора для ячеек заголовка столбца
        for(int i = 0; i < ui->table_aur->columnCount(); i++) {
            QTableWidgetItem *headerItem = ui->table_aur->horizontalHeaderItem(i);
            headerItem->setFlags(headerItem->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        // подключение сигнала клика по заголовку столбца к слоту сортировки таблицы
        QObject::connect(ui->table_aur->horizontalHeader(), &QHeaderView::sectionClicked, ui->table_aur, [this](int index){
            Qt::SortOrder order = ui->table_aur->horizontalHeader()->sortIndicatorOrder();
            ui->table_aur->sortByColumn(index, order);
        });
    }
    reply->deleteLater();
}

void MainWindow::loadContent() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    // Создаем объект запроса с параметрами
    QUrlQuery params;
    params.addQueryItem("type", "search");
    params.addQueryItem("arg", "-qt"); // замените на ваше приложение
    QUrl url("https://aur.archlinux.org/rpc/?v=5&" + params.toString());
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);
    // Обрабатываем ответ от сервера
    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        handleServerResponse(reply);

        // Выполняем поиск и выделение в таблице
        const QString searchText = ui->searchApp->text();
        if (page != 2 || searchText.isEmpty())
            return;

        QTableWidget *table = ui->table_aur;
        int rowCount = table->rowCount();
        for (int i = 0; i < rowCount; ++i) {
            QTableWidgetItem *item = table->item(i, 0);
            if (item) {
                QString cellText = item->text();
                QStringList words = cellText.split(' ', Qt::SkipEmptyParts);
                if (!words.isEmpty() && words.first().startsWith(searchText, Qt::CaseInsensitive)) {
                    // Выделяем строку
                    table->setCurrentItem(item);
                    table->scrollToItem(item, QAbstractItemView::EnsureVisible);
                    break;
                }
            }
        }
    });

    // Очищаем таблицу
    QTableWidget *table = ui->table_aur;
    table->clearContents();
    table->setRowCount(0);

    connect(ui->searchApp, &QLineEdit::textChanged, this, [=](const QString &text) {
        if (page == 2)
        {
            // Обновление параметра "arg"
            QUrlQuery newParams = params;
            newParams.removeQueryItem("arg");
            newParams.addQueryItem("arg", text);

            // Отправка запроса на сервер
            QUrl url("https://aur.archlinux.org/rpc/?v=5&" + newParams.toString());
            QNetworkRequest request(url);
            QNetworkReply *reply = manager->get(request);

            // Обработка ответа от сервера
            QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
                handleServerResponse(reply);

                // Выполняем поиск и выделение в таблице
                int rowCount = table->rowCount();
                for (int i = 0; i < rowCount; ++i) {
                    QTableWidgetItem *item = table->item(i, 0);
                    if (item) {
                        QString cellText = item->text();
                        QStringList words = cellText.split(' ', Qt::SkipEmptyParts);
                        if (!words.isEmpty() && words.first().startsWith(text, Qt::CaseInsensitive)) {
                            // Выделяем строку
                            // Выделяем всю строку
                            table->setCurrentCell(item->row(), 0);
                            table->setSelectionBehavior(QAbstractItemView::SelectRows);
                            table->scrollToItem(item, QAbstractItemView::EnsureVisible);
                            break;
                        }
                    }
                }
            });
        }
        else {
            //поиск по listWidgetManager
            QList<QListWidgetItem*> matchingItems = ui->list_manager->findItems(text, Qt::MatchContains);
            if (!matchingItems.empty()) {
                ui->list_manager->setCurrentItem(matchingItems.first());
                ui->list_manager->scrollToItem(matchingItems.first(), QAbstractItemView::PositionAtCenter);
            }
        }
    });
}

void MainWindow::loadingListWidget()
{
    ui->list_sh->clear();
    ui->list_clear->clear();
    ui->list_grub->clear();

    saveScripts(shResourcePaths, baseDir + "sh/");
    saveScripts(clearResourcePaths, baseDir + "clear/");
    saveScripts(journalsResourcePaths, baseDir + "journals/");

    loadScripts(baseDir + "sh/", ui->list_sh);
    loadScripts(baseDir + "clear/", ui->list_clear);
    loadScripts(baseDir + "journals/", ui->list_grub);

    QString notifyFileName = QFileInfo(":/other/notify.png").fileName();
    QString notifyFilePath = baseDir + "other/" + notifyFileName;
    QString translationsEnFilePath = baseDir + "other/" + "translations_en_US.txt";
    QString translationsRuFilePath = baseDir + "other/" + "translations_ru_RU.txt";

    QDir().mkpath(baseDir + "other/");

    QFile::copy(":/other/notify.png", notifyFilePath);
    QFile::copy(":/other/translations_en_US.txt", translationsEnFilePath);
    QFile::copy(":/other/translations_ru_RU.txt", translationsRuFilePath);

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
    for (const QFileInfo& fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();
        QString fileName = fileInfo.fileName();

        QString itemName = fileName;

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
                    break;
                }
            }
            scriptFile.close();
        }

        QListWidgetItem* item = new QListWidgetItem(itemName, listWidget);
        item->setForeground(generateRandomColor());
    }
}

void MainWindow::sendNotification(const QString& title, const QString& message)
{
    QStringList arguments;
    arguments << title << message << "-i" << baseDir + "other/notify.png" << "-a" << "kLaus" << "-t" << "10000";
    QProcess::startDetached("notify-send", arguments);
    loadSound(1);
}

//---#####################################################################################################################################################
//--################################################################## СОБЫТИЯ ФУНКЦИЙ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_combo_mainpage_currentIndexChanged()
{
    mainpage = ui->combo_mainpage->currentIndex();
    settings.setValue("MainPage", mainpage);
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
        sendNotification(tr("Смена языка"), tr("Приложение будет перезагружено для смены языка"));
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
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

void MainWindow::on_check_trayon_stateChanged()
{
    trayon = ui->check_trayon->isChecked() ? 1 : 0;
    settings.setValue("TrayOn", trayon);
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

void MainWindow::on_check_description_stateChanged()
{
    //описание
    table1 = ui->check_description->isChecked() ? 1 : 0;
    settings.setValue("Table1", table1);
    ui->table_aur->setColumnHidden(1, !ui->check_description->isChecked());
}

void MainWindow::on_check_version_stateChanged()
{
    //версия
    table2 = ui->check_version->isChecked() ? 1 : 0;
    settings.setValue("Table2", table2);
    ui->table_aur->setColumnHidden(2, !ui->check_version->isChecked());
}

void MainWindow::on_check_voices_stateChanged()
{
    //голоса
    table3 = ui->check_voices->isChecked() ? 1 : 0;
    settings.setValue("Table3", table3);
    ui->table_aur->setColumnHidden(3, !ui->check_voices->isChecked());
}

void MainWindow::on_check_popularity_stateChanged()
{
    //популярность
    table4 = ui->check_popularity->isChecked() ? 1 : 0;
    settings.setValue("Table4", table4);
    ui->table_aur->setColumnHidden(4, !ui->check_popularity->isChecked());
}

void MainWindow::on_check_lastupdate_stateChanged()
{
    //последнее обновление
    table5 = ui->check_lastupdate->isChecked() ? 1 : 0;
    settings.setValue("Table5", table5);
    ui->table_aur->setColumnHidden(5, !ui->check_lastupdate->isChecked());
}

void MainWindow::on_spin_rating_valueChanged(int arg1)
{
    fav = arg1;
    settings.setValue("Favorite", fav);
    loadContent();
}

void MainWindow::on_list_itemDoubleClicked(QListWidgetItem *item, const QString& scriptDir) {
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
    if (reply == QMessageBox::Yes) {
        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "bash" << scriptPath << lang);
    }
}

void MainWindow::on_list_sh_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "sh/";
    on_list_itemDoubleClicked(item, scriptDir);
}

void MainWindow::on_list_grub_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "journals/";
    on_list_itemDoubleClicked(item, scriptDir);
}

void MainWindow::on_list_clear_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = baseDir + "clear/";
    on_list_itemDoubleClicked(item, scriptDir);
}
