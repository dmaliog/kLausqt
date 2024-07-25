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

    QPixmap pixmap(":/img/splash.jpeg");
    QPixmap scaledPixmap = pixmap.scaled(pixmap.width() / 2, pixmap.height() / 2, Qt::KeepAspectRatio);
    QSplashScreen splash(scaledPixmap);
    splash.show();

    splash.showMessage(QObject::tr("Вылавливаем молюсков..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);


    #if QT_VERSION > QT_VERSION_CHECK(5, 7, 0)
        QGuiApplication::setDesktopFileName("klaus");
    #endif


    QString filePath = QDir::homePath() + "/.config/kLaus/" + "settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    QString locale = "ru_RU";
    int packageManagerIndex;

    splash.showMessage(QObject::tr("Проверяем сохраненный язык..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    if (settings.contains("Language") && settings.contains("PackageManager")) {
        QString savedLanguage = settings.value("Language").toString();
        packageManagerIndex = settings.value("PackageManager").toInt();

        if (savedLanguage == "ru_RU" || savedLanguage == "en_US")
            locale = savedLanguage;
        else
            return 1;

    } else {
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
            settings.sync();

        } else
            return 0;

        locale = settings.value("Language").toString();
    }

    QTranslator translator;
    if (translator.load(":/lang/kLaus_" + locale + ".qm"))
        qApp->installTranslator(&translator);
    else
        return 1;


    QString uniqueKey = "KlausKey";
    QSharedMemory sharedMemory(uniqueKey);
    if (!sharedMemory.create(1)) {
        return 1;
    }
    QSystemSemaphore systemSemaphore("KlausSemaphore", 1);
    systemSemaphore.acquire();

    a.setWindowIcon(QIcon(":/img/2.png"));

    splash.showMessage(QObject::tr("Проверяем наличие Pacman..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    bool pacmanInstalled = isPackageInstalled("pacman");

    if (!pacmanInstalled) {
        splash.showMessage(QObject::tr("Внимание! Требуется Pacman!"), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    splash.showMessage(QObject::tr("Проверяем наличие notify-send..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    process->start("which", QStringList() << "notify-send");
    process->waitForFinished();

    if (process->exitCode() != 0) {
        splash.showMessage(QObject::tr("Внимание! Требуется notify-send!"), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    Terminal terminal = getTerminal();
    splash.showMessage(QObject::tr("Проверяем наличие терминалов..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    if (terminal.binary.isEmpty()) {
        splash.showMessage(QObject::tr("Внимание! Требуется любой из терминалов: konsole, gnome-terminal, xfce4-terminal, lxterminal, xterm, alacritty!"), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    splash.showMessage(QObject::tr("Запускаем kLaus..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    MainWindow w;
    QTimer::singleShot(1000, [&splash, &w, &systemSemaphore]() {
        splash.close();
        w.show();
        systemSemaphore.release();
    });

    return a.exec();
}
