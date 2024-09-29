#include "scrapper.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>

ArchWikiScraper::ArchWikiScraper(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this)) {
    connect(manager, &QNetworkAccessManager::finished, this, &ArchWikiScraper::onPageLoaded);
}

void ArchWikiScraper::startScraping(const QList<QUrl> &urls) {
    urlQueue.clear();
    for (const QUrl &url : urls) {
        urlQueue.enqueue(url);
    }

    if (!urlQueue.isEmpty()) {
        currentUrl = urlQueue.dequeue();
        QNetworkRequest request(currentUrl);
        manager->get(request);
    }
}


void ArchWikiScraper::onPageLoaded(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString html = reply->readAll();
        parseHTML(html);
    }

    reply->deleteLater();

    if (!urlQueue.isEmpty()) {
        currentUrl = urlQueue.dequeue();
        QNetworkRequest request(currentUrl);
        manager->get(request);
    }
}

void ArchWikiScraper::parseHTML(const QString &html) {
    // Регулярные выражения для заголовков и пакетов
    static const QRegularExpression headerRegex("<h2><span class=\"mw-headline\" id=\"[^\"]*\">([^<]*)</span></h2>");
    static const QRegularExpression packageRegex("<span class=\"plainlinks archwiki-template-pkg\"><a rel=\"nofollow\" class=\"external text\" href=\"[^\"]*\">([^<]*)</a></span>");

    QRegularExpressionMatchIterator headerIterator = headerRegex.globalMatch(html);
    QRegularExpressionMatchIterator packageIterator;

    QString currentCategory;
    QMap<QString, QSet<QString>> categories;

    int lastPos = 0;
    QString subHtml;

    while (headerIterator.hasNext()) {
        QRegularExpressionMatch headerMatch = headerIterator.next();
        QString headerText = headerMatch.captured(1);

        if (!currentCategory.isEmpty()) {
            subHtml = html.mid(lastPos, headerMatch.capturedStart() - lastPos);
            packageIterator = packageRegex.globalMatch(subHtml);
            while (packageIterator.hasNext()) {
                QRegularExpressionMatch packageMatch = packageIterator.next();
                QString packageName = packageMatch.captured(1);
                categories[currentCategory].insert(packageName);
            }
        }

        currentCategory = headerText;
        categories[currentCategory] = QSet<QString>();
        lastPos = headerMatch.capturedEnd();
    }

    if (!currentCategory.isEmpty()) {
        subHtml = html.mid(lastPos);
        packageIterator = packageRegex.globalMatch(subHtml);
        while (packageIterator.hasNext()) {
            QRegularExpressionMatch packageMatch = packageIterator.next();
            QString packageName = packageMatch.captured(1);
            categories[currentCategory].insert(packageName);
        }
    }

    // Подготавливаем вывод
    QStringList output;
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        if (!it.value().isEmpty()) {
            output << QString("[%1]").arg(it.key());
            output << it.value().values().join("\n");
        }
    }

    if (!output.isEmpty()) {
        QString pathSegment = currentUrl.path().section('/', -1);
        QString fileName = pathSegment + ".txt";
        QString mainDir = QDir::homePath() + "/.config/kLaus/";

        QString directoryPath = QString(mainDir + "menu/%1").arg(pathSegment);

        saveToFile(output.join("\n"), directoryPath, fileName);
    }
}

void ArchWikiScraper::saveToFile(const QString &data, const QString &directoryPath, const QString &fileName) {
    QDir dir(directoryPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(dir.filePath(fileName));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    }
}
