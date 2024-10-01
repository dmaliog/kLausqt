#include "mainwindow.h"
#include "scrapper.h"
#include <QtWidgets>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QDir>

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

    QPixmap pixmap(":/img/splash.png");
    QPixmap scaledPixmap = pixmap.scaled(pixmap.width() / 2, pixmap.height() / 2, Qt::KeepAspectRatio);

    QSplashScreen splash(scaledPixmap);
    splash.show();

    splash.showMessage(QObject::tr("Вылавливаем молюсков..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);

    //scrapper
    splash.showMessage(QObject::tr("Скачиваем списки пакетов..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    ArchWikiScraper *scraper = new ArchWikiScraper();

    QList<QUrl> urls;
    urls << QUrl("https://wiki.archlinux.org/title/List_of_applications/Documents")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Internet")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Multimedia")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Science")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Utilities")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Security")
         << QUrl("https://wiki.archlinux.org/title/List_of_games")
         << QUrl("https://wiki.archlinux.org/title/Core_utilities")
         << QUrl("https://wiki.archlinux.org/title/List_of_applications/Other");

    scraper->startScraping(urls);

    #if QT_VERSION > QT_VERSION_CHECK(5, 7, 0)
        QGuiApplication::setDesktopFileName("klaus");
    #endif


    QString filePath = QDir::homePath() + "/.config/kLaus/" + "settings.ini";
    QSettings settings(filePath, QSettings::IniFormat);

    QString locale = "ru_RU";

    splash.showMessage(QObject::tr("Проверяем сохраненный язык..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    if (settings.contains("Language")) {
        QString savedLanguage = settings.value("Language").toString();

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

        QPushButton *saveButton = new QPushButton("Save", &dialog);
        saveButton->setGeometry(100, 200, 100, 30);

        QObject::connect(saveButton, &QPushButton::clicked, std::bind([&dialog, &settings, &languageComboBox]() {
            QString selectedLanguage = languageComboBox->currentData().toString();
            settings.setValue("Language", selectedLanguage);


            dialog.accept();
        }));

        if (dialog.exec() == QDialog::Accepted) {
            QString selectedLanguage = languageComboBox->currentData().toString();
            settings.setValue("Language", selectedLanguage);
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


    QSharedMemory sharedMemory("KlausKey");
    if (!sharedMemory.create(1)) {
        return 1;
    }

    QSystemSemaphore systemSemaphore("KlausSemaphore", 1);
    systemSemaphore.acquire();

    QObject::connect(&a, &QApplication::aboutToQuit, [&sharedMemory]() {
        sharedMemory.detach();
    });

    a.setWindowIcon(QIcon(":/img/2.png"));

    splash.showMessage(QObject::tr("Проверяем наличие Pacman..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    bool pacmanInstalled = isPackageInstalled("pacman");

    if (!pacmanInstalled) {
        splash.showMessage(QObject::tr("Внимание! Требуется Pacman!"), Qt::AlignCenter | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    splash.showMessage(QObject::tr("Проверяем наличие notify-send..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    process->start("which", QStringList() << "notify-send");
    process->waitForFinished();

    if (process->exitCode() != 0) {
        splash.showMessage(QObject::tr("Внимание! Требуется notify-send!"), Qt::AlignCenter | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    Terminal terminal = getTerminal();
    splash.showMessage(QObject::tr("Проверяем наличие терминалов..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    if (terminal.binary.isEmpty()) {
        splash.showMessage(QObject::tr("Внимание! Требуется любой из терминалов: konsole, gnome-terminal, xfce4-terminal, lxterminal, xterm, alacritty!"), Qt::AlignCenter | Qt::AlignBottom, Qt::white);

        QTimer::singleShot(10000, [&splash]() {
            splash.close();
            QApplication::quit();
        });
    }

    splash.showMessage(QObject::tr("Запускаем kLaus..."), Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    MainWindow w;
    QTimer::singleShot(0, [&splash, &w, &systemSemaphore]() {
        splash.close();
        w.raise();
        w.activateWindow();
        w.show();
        systemSemaphore.release();

    });

    return a.exec();
}
