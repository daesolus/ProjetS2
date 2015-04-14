//
//  DownloadManager.h
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-14.
//
//

#ifndef __ProjetS2__DownloadManager__
#define __ProjetS2__DownloadManager__

#include <stdio.h>
#include <QObject>
#include <QNetworkRequest>
#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class DownloadManager : public QObject
{
    Q_OBJECT
    
public:
    QByteArray getData();
    DownloadManager(QUrl downloadURL);

Q_SIGNALS:
    void downloadDone();
    
private Q_SLOTS:
    void dataDownloaded(QNetworkReply* reply);
    
private:
    QNetworkAccessManager accessManager;
    QByteArray data;
};
#endif /* defined(__ProjetS2__DownloadManager__) */
