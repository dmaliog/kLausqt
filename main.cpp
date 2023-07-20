#include "mainwindow.h"
#include <QtWidgets>

bool isPackageInstalled(const QString& packageName)
{
    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();

    process->start("pacman", QStringList() << "-Q" << packageName);
    process->waitForFinished();

    QString output = process->readAllStandardOutput();
    return !output.isEmpty();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //костыль wayland icon
    #if QT_VERSION > QT_VERSION_CHECK(5, 7, 0)
        QGuiApplication::setDesktopFileName("klaus");
    #endif

    QString mainDir = QDir::homePath() + "/.config/kLaus/";
    // Создаем объект QSettings для чтения из файла INI
    QString filePath = mainDir + "settings.ini";
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
        settings.sync(); // Сохраняем изменения в файл INI

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

    if (!pacmanInstalled) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется Pacman!"));
        return 1;
    }

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();

    process->start("which", QStringList() << "notify-send");
    process->waitForFinished();

    if (process->exitCode() != 0) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется notify-send!"));
        return 1;
    }

    Terminal terminal = getTerminal();

    if (terminal.binary.isEmpty()) {
        w.sendNotification(QObject::tr("Ошибка"), QObject::tr("Требуется любой из терминалов: konsole, gnome-terminal, xfce4-terminal, lxterminal, xterm, alacritty!"));
        return 1;
    }
    w.show();

    return a.exec();
}
