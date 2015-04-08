#include "settingsManager.h"


SettingsManager::SettingsManager(){
    presetArray = QList<Preset>();
}

SettingsManager::~SettingsManager(){
    
}

void SettingsManager::readConfigFile(){
    
    //charge le fichier json
    QFile loadFile(QStringLiteral(":presets.json"));
    
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open presets.");
        return;
    }
    
    //lis
    QByteArray saveData = loadFile.readAll();
    
	QJsonParseError errorStatus;
    //place le tout dans un QJsonDocument (qu'on peut facilement parser)
	QJsonDocument loadDoc = QJsonDocument::fromJson(saveData, &errorStatus);
	
	if (errorStatus.error == QJsonParseError::NoError)
	{
		//parse le document lu
		parseAndStore(loadDoc.object());
	}
	else
	{
		throw JsonException();
	}
    
}
const QList<Preset> SettingsManager::getPresetArray(){
    return presetArray;
}

void SettingsManager::parseAndStore(const QJsonObject &json){
    
    //initialise array de tout les presets
    QJsonArray array = json["presets"].toArray();
    
    //pour chaque preset...
    for (int i = 0; i < json["presets"].toArray().count(); i++) {
        //objet preset
        QJsonObject obj = array.at(i).toObject();
        
        Preset prst;
        
        //assigne les valeurs de prst aux valeures lues
        prst.name = obj["name"].toString().toStdString();
        prst.imgPath = obj["imgpath"].toString().toStdString();
        prst.musicPath = obj["musicpath"].toString().toStdString();
        prst.color1 = obj["colors"].toArray().at(0).toString().toUpper().toStdString();
        prst.color2 = obj["colors"].toArray().at(1).toString().toUpper().toStdString();
        prst.color3 = obj["colors"].toArray().at(2).toString().toUpper().toStdString();
        prst.color4 = obj["colors"].toArray().at(3).toString().toUpper().toStdString();
        presetArray.append(prst);
    }
    
}