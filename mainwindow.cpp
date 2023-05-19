//---#####################################################################################################################################################
//--############################################################## ПОДКЛЮЧЕННЫЕ ИНКЛУДЫ #################################################################
//-#####################################################################################################################################################

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtimer.h"
#include <QMessageBox>
#include <QProcess>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QDesktopServices>
#include <QStorageInfo>
#include <QProgressBar>
#include <QFileDialog>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QMovie>
#include <QSound>
#include <QtWebEngineWidgets>
#include <QDebug>

//---#####################################################################################################################################################
//--############################################################## ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ################################################################
//-#####################################################################################################################################################
QString kLausDir = QDir::homePath();
QString filePath = kLausDir + "/kLaus/settings.ini";
QSettings settings(filePath, QSettings::IniFormat);

int auth; // вошел юзер или нет
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

QTime timeupdate;
std::string secpass = "root"; // правильный пароль
bool runScriptVK = false; // проверка на VK скрипт

//---#####################################################################################################################################################
//--################################################################# ОСНОВНЫЕ ФУНКЦИИ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_pushButton_3_clicked()
{
//авторизация
}

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
    ui->toolBar_2->show();
    ui->tableWidgetApp->show(); //Показываем список устнановить
    ui->searchApp->show();
    showLoadingAnimation(false);
}

void MainWindow::on_action_17_triggered()
{
    if (page == 3) return;
    mrpropper();
    page = 3;
    ui->label1->setText(tr("Очистка системы"));

    ui->label2->setVisible(true);
    ui->listWidget_clear->setVisible(true);
    ui->listWidget_2->setVisible(true);
    ui->action_sh->setVisible(true);
    ui->action_18->setVisible(true);
    ui->action_19->setVisible(true);
    ui->action_20->setVisible(true);
    ui->action_21->setVisible(true);
    ui->action_22->setVisible(true);
    ui->action_23->setVisible(true);
    ui->toolBar_2->setVisible(true);
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
    ui->toolBar_2->setVisible(true);
    ui->listWidgetManager->show();

    // Очищаем listWidgetManager
    ui->listWidgetManager->clear();

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
            ui->listWidgetManager->addItem(item);
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
    ui->toolBar_2->setVisible(true);
    ui->textEdit_grub->setVisible(true);
    ui->spinBox_grub->setVisible(true);
    ui->label_grub->setVisible(true);
    ui->label2_grub->setVisible(true);
    ui->label3_grub->setVisible(true);
    ui->label4_grub->setVisible(true);
    ui->listWidget_grub->setVisible(true);

    QString filename = "/etc/default/grub";
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->action_26->setDisabled(true);
        ui->textEdit_grub->setDisabled(true);
        ui->spinBox_grub->setDisabled(true);
        ui->textEdit_grub->setText(tr("GRUB не установлен"));
        // Ошибка открытия файла
    } else {
        QTextStream in(&file);
        QString grubContent;
        QString timeoutStr;

        // статические объекты QRegularExpression
        static QRegularExpression timeoutRegex("^GRUB_TIMEOUT=\"(\\d+)\"$");
        static QRegularExpression grubCmdlineRegex("^GRUB_CMDLINE_LINUX_DEFAULT=\"(.*)\"$");

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("GRUB_TIMEOUT=")) {
                QRegularExpressionMatch match = timeoutRegex.match(line);
                if (match.hasMatch()) {
                    timeoutStr = match.captured(1).trimmed();
                }
                continue;
            }

            if (line.startsWith("GRUB_CMDLINE_LINUX_DEFAULT=")) {
                QRegularExpressionMatch match = grubCmdlineRegex.match(line);
                if (match.hasMatch()) {
                    grubContent = match.captured(1).trimmed();
                }
                break; // Закончит цикл поиска
            }
        }
        file.close();

        int timeout = timeoutStr.toInt(); // получаем значение timeout из файла
        ui->spinBox_grub->setValue(timeout); // устанавливаем значение в QSpinBox

        ui->textEdit_grub->setText(grubContent);
    }

    showLoadingAnimation(false);
}

void MainWindow::on_action_3_triggered()
{
    if (page == 6) return;
    mrpropper();
    page = 6;
    ui->label1->hide();

    QWebEnginePage* pagez = ui->webEngineView->page();
    runScriptVK = false; // reset the flag
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

    QWebEnginePage* pagez = ui->webEngineView->page();
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
    mrpropper(); // Обнуление всех других разделов
    page = 9;
    ui->label1->setText(tr("Дополнительные настройки"));
    ui->action_28->setVisible(true);
    ui->action_29->setVisible(true);
    ui->toolBar_2->setVisible(true);

    ui->tabWidget->setVisible(true);
    ui->checkBox_trayon->setChecked(trayon == 1);
    ui->checkBox_soundon->setChecked(soundon == 1);
    ui->checkBox_1->setChecked(table1 == 1);
    ui->checkBox_2->setChecked(table2 == 1);
    ui->checkBox_3->setChecked(table3 == 1);
    ui->checkBox_4->setChecked(table4 == 1);
    ui->checkBox_5->setChecked(table5 == 1);
    ui->comboBox_mainpage->setCurrentIndex(mainpage);
    ui->timeEdit_update->setTime(timeupdate);
    ui->spinBox->setValue(fav);
    ui->tabWidget->setCurrentIndex(0);
    showLoadingAnimation(false);
}

void MainWindow::on_action_13_triggered()
{
    if (page == 10) return;
    mrpropper(); //Обнуление всех других разделов
    page = 10;
    ui->label1->setText(tr("Приглашение"));

    ui->wlogin->show();
    ui->hello->show();
    ui->hello->setText("Вы можете получить приглашение у авторов проекта\nили в группе ВКонтакте - vk.com/linux2\n\nС помощью приглашения Вам станут доступны другие интересные разделы!");
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

void MainWindow::on_action_11_triggered()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.startDetached("konsole", QStringList() << "--hold" << "-e" << "yay -Syu");
}

void MainWindow::on_action_24_triggered()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.startDetached("konsole", QStringList() << "-e" << "bash" << "-c" << "yay -Rs $(yay -Qdtq); read");
}

void MainWindow::on_action_25_triggered()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    switch(yaycache) {
    case 0:
        process.startDetached("konsole", QStringList() << "--hold" << "-e" << "yay -Sc");
        break;
    case 1:
        process.startDetached("konsole", QStringList() << "--hold" << "-e" << "yay -Scc");
        break;
    case 2:
        process.startDetached("konsole", QStringList() << "--hold" << "-e" << "paccache -rvk3");
        break;
    default:
        process.startDetached("konsole", QStringList() << "--hold" << "-e" << "yay -Sc");
        break;
    }
}

void MainWindow::on_action_5_triggered()
{
    if (page == 2)
    {
        if (ui->tableWidgetApp->currentItem() != nullptr) {
            QString packageName = ui->tableWidgetApp->item(ui->tableWidgetApp->currentRow(), 0)->text();
            QProcess* process = new QProcess(this);

            // Обработчик вывода информации из процесса
            connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
                QString output = process->readAllStandardOutput();
                QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
                QRegularExpressionMatch match = re.match(output);
                if (match.hasMatch()) {
                    QString appName = match.captured(1);
                    QString program = appName.split('/').last();
                    if (program.endsWith("-bin") || program.endsWith("-git") || program.endsWith("-qt") || program.endsWith("-qt4") || program.endsWith("-qt5") || program.endsWith("-qt6") || program.endsWith("qt-") || program.endsWith("qt4-") || program.endsWith("qt5-") || program.endsWith("qt6-") || program.endsWith("-gtk") || program.endsWith("-gtk2") || program.endsWith("-gtk3")) {
                        program.chop(4); // Убираем "-bin" или "-git" из имени программы
                    }
                    process->startDetached("konsole", QStringList() << "-e" << program);
                } else {
                    QMessageBox::information(this, tr("Пакет не найден"), "Пакет " + packageName + tr(" не найден в системе!"));
                }
                process->deleteLater();
            });

            // Обработчик вывода ошибок из процесса
            connect(process, &QProcess::readyReadStandardError, this, [=]() {
                QString error = process->readAllStandardError();
                QMessageBox::critical(this, "Ошибка", error);
                process->deleteLater();
            });

            // Запускаем процесс
            process->start("yay", QStringList() << "-Qi" << packageName);
        } else
            QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для запуска!"));

    } else {
        if (ui->listWidgetManager->currentItem() != nullptr) {
            QString packageName = ui->listWidgetManager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            QProcess* process = new QProcess(this);
             process->startDetached("konsole", QStringList() << "--hold" << "-e" << packageName);
        } else
            QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для запуска!"));
    }
}

void MainWindow::on_action_6_triggered()
{
    if (page == 2)
    {
        if (ui->tableWidgetApp->currentItem() != nullptr) {
            QString packageName = ui->tableWidgetApp->item(ui->tableWidgetApp->currentRow(), 0)->text();
            QProcess* process = new QProcess(this);

            // Обработчик вывода информации из процесса
            connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
                QString output = process->readAllStandardOutput();
                QRegularExpression re("Название\\s+\\:\\s+(\\S+)");
                QRegularExpressionMatch match = re.match(output);
                if (match.hasMatch()) {
                    QString appName = match.captured(1);
                    QString program = appName.split('/').last();
                    if (program.endsWith("-bin") || program.endsWith("-git") || program.endsWith("-qt") || program.endsWith("-qt4") || program.endsWith("-qt5") || program.endsWith("-qt6") || program.endsWith("qt-") || program.endsWith("qt4-") || program.endsWith("qt5-") || program.endsWith("qt6-") || program.endsWith("-gtk") || program.endsWith("-gtk2") || program.endsWith("-gtk3")) {
                        program.chop(4); // Убираем "-bin" или "-git" из имени программы
                    }

                    process->startDetached("konsole", QStringList() << "-e" << "yay -R " + packageName);
                } else
                    QMessageBox::information(this, tr("Пакет не найден"), tr("Пакет ") + packageName + tr(" не найден в системе!"));

                process->deleteLater();
            });

            // Обработчик вывода ошибок из процесса
            connect(process, &QProcess::readyReadStandardError, this, [=]() {
                QString error = process->readAllStandardError();
                QMessageBox::critical(this, "Ошибка", error);
                process->deleteLater();
            });

            // Запускаем процесс
            process->start("yay", QStringList() << "-Qi" << packageName);
        } else
            QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для удаления!"));

    } else {
        if (ui->listWidgetManager->currentItem() != nullptr) {
            QString packageName = ui->listWidgetManager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            QProcess* process = new QProcess(this);
             process->startDetached("konsole", QStringList() << "-e" << "yay -R " + packageName);
        } else
             QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для удаления!"));
    }
}


void MainWindow::on_action_4_triggered()
{
    if (page == 2)
    {
        if (ui->tableWidgetApp->currentItem() != nullptr) {
            QString packageName = ui->tableWidgetApp->item(ui->tableWidgetApp->currentRow(), 0)->text();
            QProcess* process = new QProcess(this);
            process->startDetached("konsole", QStringList() << "-e" << "yay -S " + packageName);
         } else
            QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для установки!"));

    } else {
        if (ui->listWidgetManager->currentItem() != nullptr) {
            QString packageName = ui->listWidgetManager->currentItem()->text();
            packageName = packageName.left(packageName.indexOf(" "));
            QProcess* process = new QProcess(this);
             process->startDetached("konsole", QStringList() << "-e" << "yay -S " + packageName);
        } else
             QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для установки!"));
    }
}

void MainWindow::on_action_30_triggered()
{
    if (ui->tableWidgetApp->currentItem() != nullptr) {
        QString packageName = ui->tableWidgetApp->item(ui->tableWidgetApp->currentRow(), 0)->text();

        QString script = R"(
            #name Изменить PKGBUILD пакета перед установкой
            #msg Вы действительно хотите изменить PKGBUILD пакета перед установкой?
            #!/bin/bash

            program_name="%1"

            # Создание каталога, если он не существует
            if [ ! -d "$HOME/kLaus/pkg/" ]; then
                mkdir "$HOME/kLaus/pkg/"
            fi

            cd "$HOME/kLaus/pkg/"
            read -p "Нажмите Enter, чтобы начать компиляцию пакета..."

            # Получение PKGBUILD
            yay --getpkgbuild "$program_name"

            # Путь к PKGBUILD файлу
            pkgbuild_path="$HOME/kLaus/pkg/$program_name/PKGBUILD"

            # Проверка наличия PKGBUILD файла
            if [ -f "$pkgbuild_path" ]; then
                notify-send "Редактор PKGBUILD" "Найден PKGBUILD файл для пакета $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

                # Открытие редактора для редактирования PKGBUILD
                read -p "Нажмите Enter, чтобы открыть редактор для редактирования PKGBUILD..."
                $EDITOR "$pkgbuild_path"

                # Компиляция пакета
                read -p "Нажмите Enter, чтобы начать компиляцию пакета..."
                cd "$HOME/kLaus/pkg/$program_name" && makepkg -f

                # Установка нового пакета
                read -p "Нажмите Enter, чтобы установить новый пакет..."
                sudo yay -U "$HOME/kLaus/pkg/$program_name"/*.pkg.tar.zst

            else
                notify-send "Редактор PKGBUILD" "PKGBUILD файл не найден для пакета $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
            fi
        )";

        QString command = QString("bash -c '%1'").arg(script.arg(packageName));

        QProcess* process = new QProcess(this);
        process->startDetached("konsole", QStringList() << "-e" << command);
    } else {
        QMessageBox::information(this, tr("Внимание"), tr("Выберите пакет из списка для установки!"));
    }
}

void MainWindow::on_action_26_triggered()
{
    QString filename = "/etc/default/grub";
    QString grubContent = ui->textEdit_grub->toPlainText().trimmed();
    QString timeout = ui->spinBox_grub->value() > 0 ? QString::number(ui->spinBox_grub->value()) : "5";

    // Создаем процесс для выполнения команды с pkexec
    QProcess process;
    process.setProgram("pkexec");
    QStringList arguments;
    arguments << "bash" << "-c" << "sed -i 's/^GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT=\"" + grubContent + "\"/; s/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=\"" + timeout + "\"/' " + filename + " && sudo grub-mkconfig -o /boot/grub/grub.cfg";
    process.setArguments(arguments);

    // Запускаем процесс с pkexec
    process.start();
    if (!process.waitForStarted()) {
        QMessageBox::warning(this, tr("Ошибка выполнения"), tr("Не удалось запустить pkexec"));
        return;
    }

    // Ждем, пока процесс завершится
    if (!process.waitForFinished(-1)) {
        QMessageBox::warning(this, tr("Ошибка выполнения"), tr("Не удалось выполнить команду pkexec"));
        return;
    }

    // Проверяем код выхода
    if (process.exitCode() != QProcess::NormalExit || process.exitStatus() != QProcess::ExitStatus::NormalExit) {
        return;
    }
    QMessageBox::information(this, tr("GRUB изменен"), tr("Изменения GRUB вступят в силу после перезагрузки."));
}

void MainWindow::on_action_16_triggered()
{
    // Удаление содержимого таблицы
    ui->tableWidgetApp->clearContents();
    ui->tableWidgetApp->setRowCount(0);
    ui->tableWidgetApp->setColumnHidden(2, true);
    ui->tableWidgetApp->setColumnHidden(3, true);
    ui->tableWidgetApp->setColumnHidden(4, true);
    ui->tableWidgetApp->setColumnHidden(5, true);
    ui->tableWidgetApp->setColumnWidth(1, 680);

    QString sourceFilePath = ":/other/list.txt";
    QString targetFilePath = QDir::homePath() + "/kLaus/other/list.txt";

    QFileInfo fileInfo(targetFilePath);
    if (!fileInfo.exists()) {
        if (!QDir().mkpath(fileInfo.absoluteDir().path())) {
            qDebug() << "Failed to create directory:" << fileInfo.absoluteDir().path();
            return;
        }

        if (!QFile::copy(sourceFilePath, targetFilePath)) {
            qDebug() << "Failed to copy file from" << sourceFilePath << "to" << targetFilePath;
            return;
        }
    }

    QFile file(targetFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл ресурсов";
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

        ui->tableWidgetApp->insertRow(i);
        ui->tableWidgetApp->setItem(i, 0, item1);
        ui->tableWidgetApp->setItem(i, 1, item2);
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
    progressBar->setFixedSize(200, 30);

    // Получаем информацию о корневом каталоге
    QStorageInfo storageInfo = QStorageInfo::root();

    // Получаем общий объем диска в байтах
    qint64 totalBytes = storageInfo.bytesTotal();

    // Получаем количество свободных байтов на диске
    qint64 freeBytes = storageInfo.bytesAvailable();

    // Вычисляем процент занятого места на диске
    int usedPercentage = 100 - static_cast<int>((freeBytes * 100) / totalBytes);

    // Получаем количество свободных гигабайтов на диске
    double freeGB = freeBytes / (1024.0 * 1024.0 * 1024.0);

    // Устанавливаем значение прогресс-бара в процентах
    progressBar->setValue(usedPercentage);

    // Устанавливаем текст на прогресс-баре, отображающий количество свободных гигабайтов
    QString text = QString(tr("свободно %1 ГиБ")).arg(QString::number(freeGB, 'f', 2));
    progressBar->setFormat(text);

    ui->statusBar->addPermanentWidget(progressBar);

    // Устанавливаем цвет для прогресс-бара
    QColor customColor(42, 40, 112); // устанавливаем оранжевый цвет
    QPalette palette = progressBar->palette();
    palette.setColor(QPalette::Highlight, customColor);
    progressBar->setPalette(palette);
    loadingListWidget();

//-##################################################################################
//-############################### ПРОДОЛЖАЕМ... ####################################
//-##################################################################################

    mrpropper(); //зачистка говна
    loadSettings(); //загрузка настроек
    loadContent(); //загрузка списков приложений игр и тп

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

    connect(ui->spinBox_grub, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_grub_valueChanged(int)));
    connect(ui->timeEdit_update, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_spinBox_valueChanged);
    showLoadingAnimation(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    if(trayon == 1)
        QApplication::quit();
}

void MainWindow::mrpropper() //зачистка говна перед началом каждой вкладки
{
    if(soundon == 0)
    {
        QSound *sound = new QSound(":/media/sound.wav", this);
        sound->play();
    }

    ui->tableWidgetApp->setVisible(false);
    ui->listWidget_clear->setVisible(false);
    ui->listWidget_2->setVisible(false);
    ui->listWidgetManager->setVisible(false);
    ui->listWidget_grub->setVisible(false);
    ui->textEdit_grub->setVisible(false);
    ui->spinBox_grub->setVisible(false);
    ui->label_grub->setVisible(false);
    ui->label2_grub->setVisible(false);
    ui->label3_grub->setVisible(false);
    ui->label4_grub->setVisible(false);
    ui->wlogin->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->searchApp->setVisible(false);
    ui->toolBar_2->setVisible(false);
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
    ui->webEngineView->setVisible(false);
    ui->label2->setVisible(false);
    showLoadingAnimation(true);
    ui->label1->show();
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
    timeupdate = QTime::fromString(settings.value("TimeUpdate").toString(), "HH:mm");

    QString savedLogin = settings.value("login", "").toString();
    QString savedPassword = settings.value("password", "").toString();

    // Проверяем наличие значений настроек "login" и "password"
    if (savedPassword == QString::fromStdString(secpass)) {

        // Форматируем строку приветствия
        QString loginwelcome = QString(tr("Добро пожаловать %1!")).arg(savedLogin);

        // Устанавливаем заголовок и текст метки с приветствием
        ui->menu_2->setTitle(loginwelcome);
        ui->label1->setText(loginwelcome);

        // Делаем мой профиль доступным
        ui->action_13->setVisible(false);
        ui->action_14->setVisible(false);
        ui->action_15->setVisible(false);

        //Делаем меню для аворизации/регистрации и тп недоступным
        ui->menu_2->setEnabled(false);

        // Включаем недоступные функции
        ui->action_1->setEnabled(true);

        mrpropper();
        auth = 1;
    }
    ui->webEngineView->setZoomFactor(0.9);
    ui->toolBar_2->setFixedWidth(100);
    //запретить выключать панели
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBar_2->setContextMenuPolicy(Qt::PreventContextMenu);
    // Запрещаем редактирование ячеек
    ui->tableWidgetApp->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    // Вызов нужной функции при изменении времени
    if (time == QTime(0, 0)) {
        // Если время не задано (00:00), ничего не делаем
        return;
    }

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
    if (interval < 0) {
        interval += 24 * 60 * 60 * 1000; // 24 часа в миллисекундах
    }

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
        if (label != nullptr) {
            return;
        }

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
            if (label != nullptr) {
                label->setPixmap(pixmap);
            }

            if (counter >= 3) {
                counter = 1;
            } else {
                counter++;
            }
        });
        timer->start(200);

        // Отключаем таймер для скрытия иконки
        if (hideTimer != nullptr) {
            hideTimer->stop();
            hideTimer = nullptr;
        }
    } else {
        // Если иконка не показывается, не делаем ничего
        if (label == nullptr) {
            return;
        }

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
            } else {
                qDebug() << "QLabel is not created yet!";
            }
        });
        hideTimer->start(300);
    }
}

void MainWindow::handleServerResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj["type"].toString() == "error") {
            qDebug() << "Ошибка при получении списка приложений из AUR:" << jsonObj["results"].toString();
        } else if (jsonObj["type"].toString() == "search") {
            QJsonArray results = jsonObj["results"].toArray();
            ui->tableWidgetApp->setRowCount(results.size()); // Устанавливаем количество строк в таблице
            ui->tableWidgetApp->setColumnCount(6); // Устанавливаем количество столбцов в таблице
            ui->tableWidgetApp->setShowGrid(false); // Убираем отображение сетки
            ui->tableWidgetApp->verticalHeader()->setVisible(false); // Убираем отображение номеров строк
            ui->tableWidgetApp->setColumnWidth(0, 250);
            ui->tableWidgetApp->setColumnWidth(1, 550);
            ui->tableWidgetApp->setColumnWidth(2, 130);
            ui->tableWidgetApp->setColumnWidth(3, 70);
            ui->tableWidgetApp->setColumnWidth(4, 110);
            ui->tableWidgetApp->setColumnWidth(5, 170);
            ui->tableWidgetApp->setHorizontalHeaderLabels({tr("Название"), tr("Описание"), tr("Версия"), tr("Голоса"), tr("Популярность"), tr("Последнее обновление")});

            ui->tableWidgetApp->setColumnHidden(1, table1 == 0);
            ui->tableWidgetApp->setColumnHidden(2, table2 == 0);
            ui->tableWidgetApp->setColumnHidden(3, table3 == 0);
            ui->tableWidgetApp->setColumnHidden(4, table4 == 0);
            ui->tableWidgetApp->setColumnHidden(5, table5 == 0);

            for (int i = 0; i < results.size(); i++) {
                QString name = results[i].toObject()["Name"].toString();
                QString version = results[i].toObject()["Version"].toString();
                QString description = results[i].toObject()["Description"].toString();
                int votes = results[i].toObject()["NumVotes"].toInt();
                double popularity = results[i].toObject()["Popularity"].toDouble();
                qint64 timestamp = results[i].toObject()["LastModified"].toInt();
                int outofdate = results[i].toObject()["OutOfDate"].toInt();

                QColor color = generateRandomColor();

                QTableWidgetItem *item = new QTableWidgetItem(name);
                item->setForeground(color);
                ui->tableWidgetApp->setItem(i, 0, item);

                if (votes >= fav) {
                    QIcon icon(":/img/p_1.png");
                    item->setIcon(icon);
                }

                QTableWidgetItem *item1 = new QTableWidgetItem(description);
                item1->setForeground(color);
                ui->tableWidgetApp->setItem(i, 1, item1);

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
                ui->tableWidgetApp->setItem(i, 2, item2);

                QTableWidgetItem *item3 = new QTableWidgetItem();
                item3->setForeground(color);
                item3->setData(Qt::DisplayRole, votes);
                ui->tableWidgetApp->setItem(i, 3, item3);

                QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(popularity, 'f', 2));
                item4->setForeground(color);
                ui->tableWidgetApp->setItem(i, 4, item4);

                QDateTime dateTime;
                dateTime.setMSecsSinceEpoch(timestamp * 1000);
                QString date = dateTime.toString("yyyy-MM-dd");
                QTableWidgetItem *item5 = new QTableWidgetItem(date);
                item5->setForeground(color);
                ui->tableWidgetApp->setItem(i, 5, item5);
            }
        }
        // установка атрибутов сортировки и выбора для ячеек заголовка столбца
        for(int i = 0; i < ui->tableWidgetApp->columnCount(); i++) {
            QTableWidgetItem *headerItem = ui->tableWidgetApp->horizontalHeaderItem(i);
            headerItem->setFlags(headerItem->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        // подключение сигнала клика по заголовку столбца к слоту сортировки таблицы
        QObject::connect(ui->tableWidgetApp->horizontalHeader(), &QHeaderView::sectionClicked, ui->tableWidgetApp, [this](int index){
            Qt::SortOrder order = ui->tableWidgetApp->horizontalHeader()->sortIndicatorOrder();
            ui->tableWidgetApp->sortByColumn(index, order);
        });
    } else
        qDebug() << "Ошибка при получении списка приложений из AUR:" << reply->errorString();
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
    });

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
            });
        }
        else {
            //поиск по listWidgetManager
            QList<QListWidgetItem*> matchingItems = ui->listWidgetManager->findItems(text, Qt::MatchContains);
            if (!matchingItems.empty()) {
                ui->listWidgetManager->setCurrentItem(matchingItems.first());
                ui->listWidgetManager->scrollToItem(matchingItems.first(), QAbstractItemView::PositionAtCenter);
            }
        }
    });
    connect(ui->tableWidgetApp, &QTableWidget::cellClicked, this, [=]() {
        // Выделяем всю строку при клике на ячейку
        ui->tableWidgetApp->setSelectionBehavior(QAbstractItemView::SelectRows);
    });
}

void MainWindow::loadingListWidget()
{
    ui->listWidget_2->clear();
    ui->listWidget_clear->clear();
    ui->listWidget_grub->clear();

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

    loadScripts(shResourcePaths, baseDir + "sh/", ui->listWidget_2);
    loadScripts(clearResourcePaths, baseDir + "clear/", ui->listWidget_clear);
    loadScripts(journalsResourcePaths, baseDir + "journals/", ui->listWidget_grub);

    QString baseDir2 = QDir::homePath() + "/kLaus/other/";
    QString fileName2 = QFileInfo(":/other/notify.png").fileName();
    QString filePath2 = baseDir2 + fileName2;
    QDir().mkpath(baseDir2);
    QFile::copy(":/other/notify.png", filePath2);

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
                if (line.startsWith("#name"))
                {
                    itemName = line.mid(6).trimmed();
                    break;
                }
            }
            scriptFile.close();
        }

        QListWidgetItem* item = new QListWidgetItem(itemName, listWidget);
        item->setForeground(generateRandomColor());
    }
}

//---#####################################################################################################################################################
//--################################################################## СОБЫТИЯ ФУНКЦИЙ ##################################################################
//-#####################################################################################################################################################

void MainWindow::on_comboBox_mainpage_currentIndexChanged()
{
    mainpage = ui->comboBox_mainpage->currentIndex();
    settings.setValue("MainPage", mainpage);
}

void MainWindow::on_comboBox_yaycache_currentIndexChanged()
{
    yaycache = ui->comboBox_yaycache->currentIndex();
    settings.setValue("YayCache", yaycache);
}

void MainWindow::on_timeEdit_update_timeChanged(const QTime &time)
{
    // Проверяем, что время корректно
    if (time.isValid()) {
        timeupdate = time; // Обновляем глобальную переменную timeupdate
        settings.setValue("TimeUpdate", timeupdate.toString("HH:mm")); // Сохраняем значение времени в настройках
    } else
        QMessageBox::critical(nullptr, tr("Ошибка"), tr("Неверный формат времени."));
}

void MainWindow::on_checkBox_trayon_stateChanged()
{
    trayon = ui->checkBox_trayon->isChecked() ? 1 : 0;
    settings.setValue("TrayOn", trayon);
}

void MainWindow::on_checkBox_soundon_stateChanged()
{
    soundon = ui->checkBox_soundon->isChecked() ? 1 : 0;
    settings.setValue("SoundOn", soundon);
}

void MainWindow::on_checkBox_1_stateChanged()
{
    //описание
    table1 = ui->checkBox_1->isChecked() ? 1 : 0;
    settings.setValue("Table1", table1);
    ui->tableWidgetApp->setColumnHidden(1, !ui->checkBox_1->isChecked());
}

void MainWindow::on_checkBox_2_stateChanged()
{
    //версия
    table2 = ui->checkBox_2->isChecked() ? 1 : 0;
    settings.setValue("Table2", table2);
    ui->tableWidgetApp->setColumnHidden(2, !ui->checkBox_2->isChecked());

}

void MainWindow::on_checkBox_3_stateChanged()
{
    //голоса
    table3 = ui->checkBox_3->isChecked() ? 1 : 0;
    settings.setValue("Table3", table3);
    ui->tableWidgetApp->setColumnHidden(3, !ui->checkBox_3->isChecked());
}


void MainWindow::on_checkBox_4_stateChanged()
{
    //популярность
    table4 = ui->checkBox_4->isChecked() ? 1 : 0;
    settings.setValue("Table4", table4);
    ui->tableWidgetApp->setColumnHidden(4, !ui->checkBox_4->isChecked());
}


void MainWindow::on_checkBox_5_stateChanged()
{
    //последнее обновление
    table5 = ui->checkBox_5->isChecked() ? 1 : 0;
    settings.setValue("Table5", table5);
    ui->tableWidgetApp->setColumnHidden(5, !ui->checkBox_5->isChecked());
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    fav = arg1;
    settings.setValue("Favorite", fav);
    loadContent();

}

void MainWindow::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = QDir::homePath() + "/kLaus/sh/";
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
                if (line.startsWith("#name")) {
                    QString name = line.mid(6).trimmed();
                    if (name == itemName) {
                        scriptPath = fileInfo.absoluteFilePath();
                    }
                }
                else if (line.startsWith("#msg")) {
                    msg = line.mid(5).trimmed();
                }
            }
            scriptFile.close();
        }
        if (!scriptPath.isEmpty()) {
            break;
        }
    }

    if (scriptPath.isEmpty()) {
        scriptPath = scriptDir + itemName;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), msg, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QProcess::startDetached("konsole", QStringList() << "-e" << "bash" << scriptPath);
    }
}

void MainWindow::on_listWidget_grub_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = QDir::homePath() + "/kLaus/journals/";
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
                if (line.startsWith("#name")) {
                    QString name = line.mid(6).trimmed();
                    if (name == itemName) {
                        scriptPath = fileInfo.absoluteFilePath();
                    }
                }
                else if (line.startsWith("#msg")) {
                    msg = line.mid(5).trimmed();
                }
            }
            scriptFile.close();
        }
        if (!scriptPath.isEmpty()) {
            break;
        }
    }

    if (scriptPath.isEmpty()) {
        scriptPath = scriptDir + itemName;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), msg, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QProcess::startDetached("konsole", QStringList() << "-e" << "bash" << scriptPath);
    }
}

void MainWindow::on_listWidget_clear_itemDoubleClicked(QListWidgetItem *item) {
    QString scriptDir = QDir::homePath() + "/kLaus/clear/";
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
                if (line.startsWith("#name")) {
                    QString name = line.mid(6).trimmed();
                    if (name == itemName) {
                        scriptPath = fileInfo.absoluteFilePath();
                    }
                }
                else if (line.startsWith("#msg")) {
                    msg = line.mid(5).trimmed();
                }
            }
            scriptFile.close();
        }
        if (!scriptPath.isEmpty()) {
            break;
        }
    }

    if (scriptPath.isEmpty()) {
        scriptPath = scriptDir + itemName;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Вопрос"), msg, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QProcess::startDetached("konsole", QStringList() << "-e" << "bash" << scriptPath);
    }
}

