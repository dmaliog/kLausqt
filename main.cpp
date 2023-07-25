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
    int packageManagerIndex;

    // Проверяем, есть ли уже сохраненные значения в файле INI
    if (settings.contains("Language") && settings.contains("PackageManager")) {
        QString savedLanguage = settings.value("Language").toString();
        packageManagerIndex = settings.value("PackageManager").toInt();

        if (savedLanguage == "ru_RU" || savedLanguage == "en_US")
            locale = savedLanguage;
        else {
            QMessageBox::critical(nullptr, "Error", "Invalid language value in INI file");
            return 1;
        }
    } else {
        // Выводим диалоговое окно с выбором языка и менеджера пакетов
        QDialog dialog;
        dialog.setFixedSize(300, 250);
        dialog.setWindowTitle("Language selection and package manager");

        QLabel *languageLabel = new QLabel("Select a language:", &dialog);
        languageLabel->setGeometry(20, 20, 260, 20);

        QComboBox *languageComboBox = new QComboBox(&dialog);
        languageComboBox->setGeometry(20, 50, 260, 30);
        languageComboBox->addItem("Русский", "ru_RU");
        languageComboBox->addItem("English", "en_US");

        QLabel *packageManagerLabel = new QLabel("Select the package manager:", &dialog);
        packageManagerLabel->setGeometry(20, 100, 260, 20);

        QComboBox *packageManagerComboBox = new QComboBox(&dialog);
        packageManagerComboBox->setGeometry(20, 130, 260, 30);
        packageManagerComboBox->addItem("yay");
        packageManagerComboBox->addItem("paru");

        QPushButton *saveButton = new QPushButton("Save", &dialog);
        saveButton->setGeometry(100, 200, 100, 30);

        QObject::connect(saveButton, &QPushButton::clicked, std::bind([&dialog, &settings, &packageManagerComboBox, &languageComboBox]() {
            QString selectedLanguage = languageComboBox->currentData().toString();
            settings.setValue("Language", selectedLanguage);

            int packageManagerIndex = packageManagerComboBox->currentIndex();
            settings.setValue("PackageManager", packageManagerIndex);

            dialog.accept();
        }));

        if (dialog.exec() == QDialog::Accepted) {
            QString selectedLanguage = languageComboBox->currentData().toString();
            settings.setValue("Language", selectedLanguage);

            packageManagerIndex = packageManagerComboBox->currentIndex();
            settings.setValue("PackageManager", packageManagerIndex);
            settings.sync(); // Сохраняем изменения в файл INI

        } else
            return 0; // Пользователь нажал отмену, выходим из приложения


        locale = settings.value("Language").toString();
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
