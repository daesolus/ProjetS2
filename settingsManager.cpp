#include "settingsManager.h"


SettingsManager::SettingsManager(){
    
}

SettingsManager::~SettingsManager(){
    
}

void SettingsManager::readConfigFile(){
    
    QFile loadFile(QStringLiteral(":presets.json"));
    
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open presets.");
        return;
    }
    
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc((QJsonDocument::fromJson(saveData)));
    parseAndStore(loadDoc.object());
    
    /*
    QDir dir = QDir::home();
    dir.cdUp();
    QSettings settings(QDir::currentPath() + "/Config.ini", QSettings::IniFormat);
    
    settings.setIniCodec("UTF-8");
    
    settings.sync();
    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, "test");
    //settings.beginWriteArray("lol");
    //settings.endArray();
    
    qDebug() << dir.absolutePath();
    
    int size = settings.beginReadArray("Presets");
    for (int i = 0; i < size; ++i) {
        
        qDebug() << "loL";
        settings.setArrayIndex(i);
        Preset thePreset;
        thePreset.name = settings.value("name").toString().toStdString().c_str();
        thePreset.imgPath = settings.value("imgpath").toString().toStdString().c_str();
        thePreset.musicPath = settings.value("musicpath").toString().toStdString().c_str();
        
        thePreset.colors.append(RGBColor(settings.value("color1").toString().toStdString().c_str()));
        thePreset.colors.append(RGBColor(settings.value("color2").toString().toStdString().c_str()));
        thePreset.colors.append(RGBColor(settings.value("color3").toString().toStdString().c_str()));
        thePreset.colors.append(RGBColor(settings.value("color4").toString().toStdString().c_str()));
        thePreset.colors.append(RGBColor(settings.value("color5").toString().toStdString().c_str()));
        
        presetArray.append(thePreset);
    }
    
    qDebug() << settings.status();
    settings.endArray();
     */
}

void SettingsManager::parseAndStore(const QJsonObject &json){
    
    /*
    mName = json["name"].toString();
    mLevel = json["level"].toDouble();
    mClassType = ClassType(qRound(json["classType"].toDouble()));
     */
}