//
//  DownloadManager.cpp
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-14.
//
//

#include "DownloadManager.h"


DownloadManager::DownloadManager(QUrl imageUrl) :QObject()
{
    //connecte le signal de completion de download du accessManager
    connect(&accessManager, SIGNAL (finished(QNetworkReply*)),this, SLOT (dataDownloaded(QNetworkReply*)));
    //initialize la requête
    QNetworkRequest request(imageUrl);
    //execute la requête
    accessManager.get(request);
}

QByteArray DownloadManager::getData() {
    return data;}

void DownloadManager::dataDownloaded(QNetworkReply* reply) {
    //lis et enregistre les données
    data = reply->readAll();
    reply->deleteLater();
    
    //emit le slot
    emit downloadDone();
}