#include "mainwindow.h"
#include <QtWidgets>
#include <QTranslator>
#include <QObject>
#include <QDialog>

bool isPackageInstalled(const QString& packageName)
{
    QProcess process;
    process.start("pacman", QStringList() << "-Q" << packageName);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    return !output.isEmpty();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString baseDir = QDir::homePath() + "/.config/kLaus/";
    // Создаем объект QSettings для чтения из файла INI
    QString filePath = baseDir + "settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    QString locale;

    // Проверяем, есть ли уже сохраненный язык в файле INI
    if (settings.contains("Language")) {
        QString savedLanguage = settings.value("Language").toString();
        if (savedLanguage == "ru_RU")
            locale = savedLanguage;
        else if (savedLanguage == "en_US")
            locale = savedLanguage;
        else {
            // Значение языка в файле INI недействительно
            QMessageBox::critical(nullptr, "Error", "Invalid language value in INI file");
            return 1;
        }

        // Загружаем и устанавливаем файл перевода
        QTranslator translator;
        if (translator.load("kLaus_" + locale, ":/lang"))
            a.installTranslator(&translator);
        else {
            QMessageBox::critical(nullptr, "Error", "Translation not available for selected language");
            return 1;
        }
    } else {
        // Выводим диалоговое окно с выбором языка
        QStringList languages;
        languages << "Русский" << "English"; // Добавьте поддерживаемые языки
        QString selectedLanguage = QInputDialog::getItem(nullptr, "Select Language", "Preferred Language:", languages);

        // Определяем соответствующий код языка
        if (selectedLanguage == "Русский")
            locale = QLocale(QLocale::Russian).name();
        else if (selectedLanguage == "English")
            locale = QLocale(QLocale::English).name();
        else {
            QMessageBox::critical(nullptr, "Error", "Invalid language selection");
            return 1;
        }

        // Записываем выбранный язык в файл INI
        settings.setValue("Language", locale);

        // Загружаем и устанавливаем файл перевода
        QTranslator translator;
        if (translator.load("kLaus_" + locale, ":/lang"))
            a.installTranslator(&translator);
        else {
            QMessageBox::critical(nullptr, "Error", "Translation not available for selected language");
            return 1;
        }
    }
    // Загружаем и устанавливаем файл перевода
    QTranslator translator;
    if (translator.load("kLaus_" + locale, ":/lang"))
        a.installTranslator(&translator);
    else {
        QMessageBox::critical(nullptr, "Error", "Translation not available for selected language");
        return 1;
    }

    MainWindow w;

    a.setWindowIcon(QIcon(":/img/2.png"));

    const char* semaphoreName = "kLausSemaphore";

    QSharedMemory semaphore(semaphoreName);

    // Попытка удаления объекта QSharedMemory с заданным именем
    if (semaphore.attach()) {
        semaphore.detach();
    }


    // Создаем семафор, если он не существует
    if (!semaphore.create(1)) {
        w.sendNotification(QObject::tr("Внимание"), QObject::tr("Приложение уже запущено!"));
        return 1;
    }

    bool pacmanInstalled = isPackageInstalled("pacman");
    bool yayInstalled = isPackageInstalled("yay");


    if (!pacmanInstalled) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется Pacman!"));
        return 1;
    }

    if (!yayInstalled) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется помощник yay!"));
        return 1;
    }

    QProcess process;
    process.start("which", QStringList() << "notify-send");
    process.waitForFinished();

    if (process.exitCode() != 0) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется notify-send!"));
        return 1;
    }

    Terminal terminal = getTerminal();

    if (terminal.binary.isEmpty()) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется любой из терминалов: konsole, gnome-terminal, xfce4-terminal, lxterminal, xterm, alacritty!"));
        return 1;
    }

    w.show();

    // Создание иконки для трея
    QSystemTrayIcon trayIcon(QIcon(":/img/2.png"));
    trayIcon.setToolTip("kLaus ;)");

    // Создание контекстного меню для иконки трея
    QMenu *trayMenu = new QMenu();

    // Создание действия
    QAction *action_2 = new QAction(QObject::tr("Каталог пакетов AUR"), trayMenu);
    action_2->setIcon(QIcon(":/img/2.png"));
    trayMenu->addAction(action_2);

    QAction *action_7 = new QAction(QObject::tr("Установленные пакеты"), trayMenu);
    action_7->setIcon(QIcon(":/img/5.png"));
    trayMenu->addAction(action_7);

    QAction *action_11 = new QAction(QObject::tr("Обновить пакеты"), trayMenu);
    action_11->setIcon(QIcon(":/img/16.png"));
    trayMenu->addAction(action_11);

    QAction *action_9 = new QAction(QObject::tr("Информация о системе"), trayMenu);
    action_9->setIcon(QIcon(":/img/7.png"));
    trayMenu->addAction(action_9);

    QAction *action_12 = new QAction(QObject::tr("Настройки"), trayMenu);
    action_12->setIcon(QIcon(":/img/9.png"));
    trayMenu->addAction(action_12);

    QAction *exitAction = new QAction(QObject::tr("Выход"), trayMenu);
    exitAction->setIcon(QIcon(":/img/18.png"));
    trayMenu->addAction(exitAction);

    // Установка контекстного меню для иконки трея
    trayIcon.setContextMenu(trayMenu);

    // Связывание событий нажатия на пункты меню с обработчиками
    QObject::connect(action_2, &QAction::triggered, &w, [&](){ w.show(); w.on_action_2_triggered(); });
    QObject::connect(action_7, &QAction::triggered, &w, [&](){ w.show(); w.on_action_7_triggered(); });
    QObject::connect(action_11, &QAction::triggered, &w, [&](){ w.show(); w.on_action_11_triggered(); });
    QObject::connect(action_9, &QAction::triggered, &w, [&](){ w.show(); w.on_action_9_triggered(); });
    QObject::connect(action_12, &QAction::triggered, &w, [&](){ w.show(); w.on_action_12_triggered(); });
    QObject::connect(exitAction, &QAction::triggered, &a, &QApplication::quit);

    // Связываем событие клика на иконке с обработчиком
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, &w, [&](QSystemTrayIcon::ActivationReason reason) {
        // Проверяем, что клик был левым кликом
        if (reason == QSystemTrayIcon::Trigger) {
            // Проверяем, было ли главное окно свернуто
            if (w.isHidden())
                w.show(); // Если главное окно свернуто, показываем его и делаем активным
            else
                w.hide(); // Если главное окно открыто, сворачиваем его
        }
    });

    // Отображение иконки в трее
    trayIcon.show();

    return a.exec();
}
