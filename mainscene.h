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

class MainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MainScene();
    ~MainScene();
    void keyPressEvent(QKeyEvent *event);
    
    
private Q_SLOTS:
    void onConnected();
    void onDisconnect();
    void wsMessageReceived(QString text);

private:
    void navBack();
    void navForward();
    void navSelect();
    void refreshCurrentCards();
    void loadSongs();
    
    QWebSocket *m_webSocket;
    SettingsManager *manager;
    UIUtilities layout;
    QPixmap image;
    QImage  *imageObject;
    QVector<CardItem*> allCards;
    QGraphicsPixmapItem* background;
    void sendCurrentColorToServer();

    QGraphicsItem* ambienceCard(const char* title, const char* image);
    QGraphicsItem* backArrow;
    QGraphicsItem* nextArrow;
    
    void sendColorToServer(string hexColor);
    
    void sendColorToPhilipsHue(int lightNumber, RGBColor color, int transitionTime);

    //QSignalMapper *mapper;
};

#endif // MAINSCENE_H
