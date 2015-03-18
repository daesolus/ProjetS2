#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include "effects.h"
#include <QVector>
#include <QSettings>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>

struct Preset {
    const char * name;
    const char * imgPath;
    const char * musicPath;
    QVector<RGBColor> colors;
};

class SettingsManager //: public QGraphicsItem
{
public:
    SettingsManager();
    ~SettingsManager();
    void readConfigFile();
private:
    QVector<Preset> presetArray;
    void parseAndStore(const QJsonObject &json);
};

/*
class Preset //: public QGraphicsItem
{
public:
    SettingsManager();
    ~SettingsManager();
private:
    
};
*/


#endif // SETTINGSMANAGER_H
