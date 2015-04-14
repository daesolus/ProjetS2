#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <string>
#include "JsonException.h"
#include "DownloadManager.h"

using namespace std;

struct Preset {
    string name;
    string imgPath;
    string musicPath;
    string color1;
    string color2;
    string color3;
    string color4;
    
    const char* colors[5];
};

class SettingsManager : public QObject //: public QGraphicsItem
{
    Q_OBJECT
    
public:
    SettingsManager();
    const QList<Preset> getPresetArray();
    void downloadSettings();

Q_SIGNALS:
    void settingsReady();
    
private Q_SLOTS:
    void readConfigFile();
    
private:
    QList<Preset> presetArray;
	JsonException jsonExeption;
    void parseAndStore(const QJsonObject &json);
    DownloadManager *dlManager;
};

/*
class Preset //: public QGraphicsItem
{
public:
    SettingsManager();
private:
    
};
*/


#endif // SETTINGSMANAGER_H
