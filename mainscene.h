#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>
#include "UIUtilities.h"
#include <QKeyEvent>
#include <QVector>
#include <QVariant>
#include "cardItem.h"
#include <QTransform>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtWebSockets>
#include "NavArrow.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MainScene();
    void keyPressEvent(QKeyEvent *event);
    void navSendR();
    void navSendL();
    void navSendM();
    
private Q_SLOTS:
    void finishLoading();
    void onConnected();
    void onDisconnect();
    void updateHue();
    void wsMessageReceived(QString text);
    void refreshBackground();

private:
    
    void navBack(bool shouldSendColor);
    void navForward(bool shouldSendColor);
    void navSelect(bool shouldSendColor);
    void refreshCurrentCards();
    void loadSongs();
    void showSettingsForCurrentCard();
    
    QWebSocket *m_webSocket;
    SettingsManager *manager;
    UIUtilities layout;
    QPixmap image;
    QImage  imageObject;
    QVector<CardItem*> allCards;
    QGraphicsPixmapItem* background;
    void sendCurrentColorToServer();
    
    QGraphicsItem* ambienceCard(const char* title, const char* image);
    QGraphicsItem* backArrow;
    QGraphicsItem* nextArrow;
    
    void sendColorToServer(string hexColor);
    
    void sendColorToPhilipsHue(int lightNumber, const char* color, int transitionTime);
	bool ENABLE_SOUND = true;
    
    //QSignalMapper *mapper;
};

#endif // MAINSCENE_H
