#include "mainwindow.h"
#include <QtWidgets>

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
        w.sendNotification("Внимание", "Приложение уже запущено!");
        return 1;
    }

    bool pacmanInstalled = isPackageInstalled("pacman");
    bool yayInstalled = isPackageInstalled("yay");


    if (!pacmanInstalled) {
        w.sendNotification("Ошибка", "Требуется Pacman!");
        return 1;
    }

    if (!yayInstalled) {
        w.sendNotification("Ошибка", "Требуется помощник yay!");
        return 1;
    }

    QProcess process;
    process.start("which", QStringList() << "notify-send");
    process.waitForFinished();

    if (process.exitCode() != 0) {
        w.sendNotification("Ошибка", "Требуется notify-send!");
        return 1;
    }

    Terminal terminal = getTerminal();

    if (terminal.binary.isEmpty()) {
        w.sendNotification("Ошибка", "Требуется любой из терминалов: konsole, gnome-terminal, xfce4-terminal, lxterminal, xterm, alacritty!");
        return 1;
    }

    w.show();

    // Создание иконки для трея
    QSystemTrayIcon trayIcon(QIcon(":/img/2.png"));
    trayIcon.setToolTip("kLaus ;)");

    // Создание контекстного меню для иконки трея
    QMenu *trayMenu = new QMenu();

    // Создание действия
    QAction *action_2 = new QAction("Каталог пакетов AUR", trayMenu);
    action_2->setIcon(QIcon(":/img/2.png"));
    trayMenu->addAction(action_2);

    QAction *action_7 = new QAction("Установленные пакеты", trayMenu);
    action_7->setIcon(QIcon(":/img/5.png"));
    trayMenu->addAction(action_7);

    QAction *action_11 = new QAction("Обновить пакеты", trayMenu);
    action_11->setIcon(QIcon(":/img/16.png"));
    trayMenu->addAction(action_11);

    QAction *action_9 = new QAction("Информация о системе", trayMenu);
    action_9->setIcon(QIcon(":/img/7.png"));
    trayMenu->addAction(action_9);

    QAction *action_12 = new QAction("Настройки", trayMenu);
    action_12->setIcon(QIcon(":/img/9.png"));
    trayMenu->addAction(action_12);

    QAction *exitAction = new QAction("Выход", trayMenu);
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
