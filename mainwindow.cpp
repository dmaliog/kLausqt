//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
#include <QSoundEffect>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString kLausDir = QDir::homePath();
QString filePath = kLausDir + "/kLaus/settings.ini";
QSettings settings(filePath, QSettings::IniFormat);

int page; // какая страница используется
int trayon; // закрывать без трея
int soundon; // убрать звуки
int mainpage; // главная страница
int yaycache; // кэш
int table1; // описание
int table2; // версия
int table3; // голоса
int table4; // популярность
int table5; // последнее обновление
int fav; // последнее обновление
QString lang;
QTime timeupdate;

bool runScriptVK = false; // проверка на VK скрипт

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
    ui->action_19->setVisible(true);
    ui->action_20->setVisible(true);
    ui->action_21->setVisible(true);
    ui->action_22->setVisible(true);
    ui->action_23->setVisible(true);
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
    ui->label2_grub->setVisible(true);
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
                    timeoutStr.remove(QRegularExpression("[\"']")); // Удалить кавычки из значения
                }
                continue;
            }

            if (line.startsWith("GRUB_CMDLINE_LINUX_DEFAULT=")) {
                QRegularExpressionMatch match = grubCmdlineRegex.match(line);
                if (match.hasMatch()) {
                    grubContent = match.captured(1).trimmed();
                    grubContent.remove(QRegularExpression("[\"']")); // Удалить кавычки из значения
                }
                else {
                    // Если первая попытка не удалась, попробуйте другой вариант ковычек
                    grubCmdlineRegex.setPattern("^GRUB_CMDLINE_LINUX_DEFAULT=['\"]?(.*)['\"]?$");
                    match = grubCmdlineRegex.match(line);
                    if (match.hasMatch()) {
                        grubContent = match.captured(1).trimmed();
                        grubContent.remove(QRegularExpression("[\"']")); // Удалить кавычки из значения
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
    mrpropper();
    page = 6;
    ui->label1->hide();
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);

    QWebEnginePage* pagez = ui->webEngineView->page();
    runScriptVK = false; // reset the flag

    if (lang == "en_US")
        pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations"));
    else
        pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));

   QObject::connect(ui->webEngineView->page(), &QWebEnginePage::loadFinished, this, [=]() {
        if (page == 6) {
            showLoadingAnimation(false);
            ui->webEngineView->show();
        }
    });
}

void MainWindow::on_action_8_triggered()
{
    if (page == 7) return;
    mrpropper();
    page = 7;
    ui->label1->hide();
    ui->action_31->setVisible(true);
    ui->action_32->setVisible(true);
    ui->action_33->setVisible(true);

    QWebEnginePage* pagez = ui->webEngineView->page();
    runScriptVK = true;

    pagez->load(QUrl("https://vk.com/@linux2-main"));

    QObject::connect(pagez, &QWebEnginePage::loadFinished, this, [=]() {

        if (runScriptVK && page == 7) {
            QFile scriptFile(":/vk_dark_theme.user.js");
            if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&scriptFile);
                QString script = stream.readAll();
                pagez->runJavaScript(script);
                showLoadingAnimation(false);
                ui->webEngineView->show();
            }
        }
    });
}

void MainWindow::on_action_10_triggered()
{
    if (page == 8) return;
    mrpropper();
    page = 8;
    ui->label1->setText(tr("Информация о приложении"));
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
    ui->check_soundon->setChecked(soundon == 1);
    ui->check_description->setChecked(table1 == 1);
    ui->check_version->setChecked(table2 == 1);
    ui->check_voices->setChecked(table3 == 1);
    ui->check_popularity->setChecked(table4 == 1);
    ui->check_lastupdate->setChecked(table5 == 1);
    ui->combo_mainpage->setCurrentIndex(mainpage);
    ui->time_update->setTime(timeupdate);
    ui->spin_rating->setValue(fav);
    ui->tabWidget->setCurrentIndex(0);

    if(lang == "ru_RU")
        ui->combo_lang->setCurrentIndex(0);
    else if(lang == "en_US")
        ui->combo_lang->setCurrentIndex(1);

    showLoadingAnimation(false);
}

void MainWindow::on_action_13_triggered()
{
    if (page == 10) return;
    mrpropper();
    page = 10;
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
    if (lang == "en_US")
        openDirectory("/kLaus/en/sh/");
    else
        openDirectory("/kLaus/sh/");
}

void MainWindow::on_action_18_triggered()
{
    openDirectory("/.local/share/applications");
}

void MainWindow::on_action_19_triggered()
{
    openDirectory("/.local/share/plasma/desktoptheme");
}

void MainWindow::on_action_20_triggered()
{
    openDirectory("/.local/share/icons");
}

void MainWindow::on_action_21_triggered()
{
    openDirectory("/.local/share/color-schemes");
}

void MainWindow::on_action_22_triggered()
{
    openDirectory("/.icons");
}

void MainWindow::on_action_23_triggered()
{
    openDirectory("/.local/share/plasma/plasmoids");
}

void MainWindow::on_action_27_triggered()
{
    if (lang == "en_US")
        openDirectory("/kLaus/en/journals/");
    else
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
        runScriptVK = false; // reset the flag
        if (lang == "en_US")
            pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations"));
        else
            pagez->load(QUrl("https://wiki.archlinux.org/title/General_recommendations_(%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9)"));

        QObject::connect(pagez, &QWebEnginePage::loadFinished, this, [=]() {
            if (page == 6) {
                showLoadingAnimation(false);
                ui->webEngineView->show();
            }
        });
    }
    else if (page == 7)
    {
        showLoadingAnimation(true);
        runScriptVK = true; // set the flag to true
        pagez->load(QUrl("https://vk.com/@linux2-main"));

        QObject::connect(pagez, &QWebEnginePage::loadFinished, this, [=]() {

            // execute the script only if the flag is set to true
            if (runScriptVK && page == 7) {
                QFile scriptFile(":/vk_dark_theme.user.js"); // путь к файлу vk_dark_theme.user.js
                if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream stream(&scriptFile);
                    QString script = stream.readAll();
                    pagez->runJavaScript(script);
                    showLoadingAnimation(false);
                    ui->webEngineView->show();
                }
            }
        });
    }
}

void MainWindow::on_action_34_triggered()
{
    if (ui->table_aur->currentItem() != nullptr) {
        showLoadingAnimation(true);
        int currentRow = ui->table_aur->currentRow();
        QTableWidgetItem *item = ui->table_aur->item(currentRow, 0);
        QString url = item->data(Qt::UserRole).toString();
        if (url != nullptr) {
            if (!url.isEmpty()) {

                QWebEnginePage* pagez = ui->webEngineView->page();
                pagez->load(QUrl(url));

                QObject::connect(pagez, &QWebEnginePage::loadFinished, this, [=]() {
                    ui->action_16->setVisible(false);
                    ui->action_34->setVisible(false);
                    ui->action_35->setVisible(true);
                    showLoadingAnimation(false);
                    ui->webEngineView->show();
                });
            } else {
                sendNotification(tr("Внимание"), tr("URL отсутствует!"));
            }
        } else {
            sendNotification(tr("Внимание"), tr("URL отсутствует!"));
        }
    } else {
        sendNotification(tr("Внимание"), tr("Выберите пакет из списка для просмотра информации!"));
    }
}

void MainWindow::on_action_35_triggered()
{
    ui->action_16->setVisible(true);
    ui->action_34->setVisible(true);
    ui->action_35->setVisible(false);
    ui->webEngineView->hide();
}


void MainWindow::on_action_32_triggered()
{
    ui->webEngineView->reload();
    sendNotification(tr("Обновление"), tr("Страница успешно обновлена!"));
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
    checkUpdates();
}

void MainWindow::on_action_24_triggered()
{
    Terminal terminal = getTerminal();
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(terminal.binary, QStringList() << terminal.args << "bash" << "-c" << "yay -Rs $(yay -Qdtq)");
    process.waitForFinished();
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
                QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
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
                QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
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
        QString script;

        Terminal terminal = getTerminal();
        QProcess::startDetached(terminal.binary, QStringList() << terminal.args << "bash" << QDir::homePath() + "/kLaus/sh/PKGBUILD.sh" << lang << packageName);
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
        targetFilePath = QDir::homePath() + "/kLaus/other/list_en.txt";
    } else
    {
        sourceFilePath = ":/other/list.txt";
        targetFilePath = QDir::homePath() + "/kLaus/other/list.txt";
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
    checkVersionAndClear();
    mrpropper(); //зачистка говна
    loadSettings(); //загрузка настроек
    loadContent(); //загрузка списков приложений игр и тп

    loadingListWidget();

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

    QTableWidget *table = ui->table_aur;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    showLoadingAnimation(false);
}

void MainWindow::checkVersionAndClear() {
    QString kLausDir = QDir::homePath() + "/kLaus";
    QString settingsFilePath = kLausDir + "/settings.ini";
    QString currentVersion = "2.6";
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    QString storedVersion = settings.value("Version").toString();
    QString storedLanguage = settings.value("Language").toString();

    if (storedVersion.isEmpty() || storedVersion != currentVersion) {
        QDir directory(kLausDir);
        directory.removeRecursively();
        settings.setValue("Version", currentVersion);
        settings.setValue("Language", storedLanguage);
        settings.sync();
        sendNotification(tr("Обновление kLaus"), tr("Версия kLaus поменялась, конфигурация сброшена!"));
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
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
    ui->table_aur->setVisible(false);
    ui->list_clear->setVisible(false);
    ui->list_sh->setVisible(false);
    ui->list_manager->setVisible(false);
    ui->list_grub->setVisible(false);
    ui->line_grub->setVisible(false);
    ui->spin_grub->setVisible(false);
    ui->label_grub->setVisible(false);
    ui->label2_grub->setVisible(false);
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
    ui->action_19->setVisible(false);
    ui->action_20->setVisible(false);
    ui->action_21->setVisible(false);
    ui->action_22->setVisible(false);
    ui->action_23->setVisible(false);
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

    if(soundon == 0)
        loadSound(0);

    showLoadingAnimation(true);
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
    beep->setVolume(1.0f);
    beep->play();
}

void MainWindow::loadSettings()
{
    mainpage = settings.value("MainPage", 0).toInt();
    yaycache = settings.value("YayCache", 0).toInt();
    trayon = settings.value("TrayOn", 0).toInt();
    soundon = settings.value("SoundOn", 0).toInt();
    table1 = settings.value("Table1", 1).toInt();
    table2 = settings.value("Table2", 1).toInt();
    table3 = settings.value("Table3", 0).toInt();
    table4 = settings.value("Table4", 0).toInt();
    table5 = settings.value("Table5", 0).toInt();
    fav = settings.value("Favorite", 50).toInt();
    lang = settings.value("Language").toString();

    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");

    ui->webEngineView->setZoomFactor(0.9);
    ui->toolBar_2->setFixedWidth(100);
    //запретить выключать панели
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBar_2->setContextMenuPolicy(Qt::PreventContextMenu);
    // Запрещаем редактирование ячеек
    ui->table_aur->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

    // Получаем текущее время и дату
    QTime currentTime = QTime::currentTime();
    QDate currentDate = QDate::currentDate();

    // Получаем выбранное время
    QTime selectedTime = time;

    // Создаем QDateTime для выбранного времени и текущей даты
    QDateTime selectedDateTime(currentDate, selectedTime);

    // Вычисляем разницу между текущим временем и выбранным временем в миллисекундах
    int interval = currentTime.msecsTo(selectedTime);

    // Если выбранное время уже прошло, добавляем 24 часа к интервалу
    if (interval < 0)
        interval += 24 * 60 * 60 * 1000; // 24 часа в миллисекундах

    // Устанавливаем таймер для первого вызова функции
    QTimer::singleShot(interval, this, &MainWindow::checkUpdates);

    // Устанавливаем таймер для повторных вызовов функции каждый день
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkUpdates);
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
                } else {
                    item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
                }
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

void MainWindow::checkUpdates()
{
    Terminal terminal = getTerminal();

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    // Запуск команды "yay -Qu | wc -l"
    process.start("bash", QStringList() << "-c" << "yay -Qu | wc -l");
    process.waitForFinished();

    // Получение вывода команды
    QByteArray output = process.readAllStandardOutput();
    QString countStr = QString(output).trimmed();

    if (!countStr.isEmpty()) {
        int count = countStr.toInt();
        if (count > 0) {
            QString message = QString(tr("Доступно %1 обновлений. Пожалуйста, обновите систему.")).arg(count);
            sendNotification(tr("Обновление"), message);

            QProcess* updateProcess = new QProcess(this);
            updateProcess->setProcessChannelMode(QProcess::MergedChannels);
            updateProcess->start(terminal.binary,QStringList() << terminal.args << "yay" << "-Syu");
        } else
            sendNotification(tr("Обновление"), tr("Доступных обновлений нет!"));
    }
}

void MainWindow::loadingListWidget()
{
    ui->list_sh->clear();
    ui->list_clear->clear();
    ui->list_grub->clear();

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
    QString baseDir = QDir::homePath() + "/kLaus/";
    loadScripts(shResourcePaths, baseDir + "sh/", ui->list_sh);
    loadScripts(clearResourcePaths, baseDir + "clear/", ui->list_clear);
    loadScripts(journalsResourcePaths, baseDir + "journals/", ui->list_grub);

    QString baseDir2 = QDir::homePath() + "/kLaus/other/";
    QString notifyFileName = QFileInfo(":/other/notify.png").fileName();
    QString notifyFilePath = baseDir2 + notifyFileName;
    QString translationsEnFilePath = baseDir2 + "translations_en_US.txt";
    QString translationsRuFilePath = baseDir2 + "translations_ru_RU.txt";
    QDir().mkpath(baseDir2);
    QFile::copy(":/other/notify.png", notifyFilePath);
    QFile::copy(":/other/translations_en_US.txt", translationsEnFilePath);
    QFile::copy(":/other/translations_ru_RU.txt", translationsRuFilePath);

}

void MainWindow::loadScripts(const QStringList& resourcePaths, const QString& baseDir, QListWidget* listWidget)
{
    QDir().mkpath(baseDir);

    for (const QString& path : resourcePaths)
    {
        QString fileName = QFileInfo(path).fileName();
        QString filePath = baseDir + fileName;

        QFile::copy(path, filePath);

        if (!QFile::exists(filePath))
            continue;

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
    arguments << title << message << "-i" << QDir::homePath() + "/kLaus/other/notify.png" << "-a" << "kLaus" << "-t" << "10000";
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

void MainWindow::on_check_soundon_stateChanged()
{
    soundon = ui->check_soundon->isChecked() ? 1 : 0;
    settings.setValue("SoundOn", soundon);
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
    QString scriptDir = QDir::homePath() + "/kLaus/sh/";
    on_list_itemDoubleClicked(item, scriptDir);
}

void MainWindow::on_list_grub_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = QDir::homePath() + "/kLaus/journals/";
    on_list_itemDoubleClicked(item, scriptDir);
}

void MainWindow::on_list_clear_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = QDir::homePath() + "/kLaus/clear/";
    on_list_itemDoubleClicked(item, scriptDir);
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
