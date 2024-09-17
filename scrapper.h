#ifndef SCRAPPER_H
#define SCRAPPER_H

#include <QObject>
#include <QUrl>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ArchWikiScraper : public QObject {
    Q_OBJECT
public:
    explicit ArchWikiScraper(QObject *parent = nullptr);

    void startScraping(const QList<QUrl> &urls);

private slots:
    void onPageLoaded(QNetworkReply *reply);

private:
    void parseHTML(const QString &html);
    void saveToFile(const QString &data, const QString &directoryPath, const QString &fileName);

    QNetworkAccessManager *manager;
    QQueue<QUrl> urlQueue;
    QUrl currentUrl;
};

#endif // SCRAPPER_H
