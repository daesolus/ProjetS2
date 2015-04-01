#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>
#include "layoutManager.h"
#include <QKeyEvent>
#include <QVector>
#include <QVariant>
//#include "easywsclient.hpp"
//#include "settingsManager.h"
#include "cardItem.h"
#include <QTransform>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtWebSockets>
//#include "easywsclient.hpp"

class MainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MainScene();
    ~MainScene();
    void keyPressEvent(QKeyEvent *event);
    
    
    struct HsbColor
    {
        float hue;
        float saturation;
        float brightness;
    };
    /*
public Q_SLOTS:
    void wsDidConnect();*/
private Q_SLOTS:
    void onConnected();
    void onDisconnect();
    void wsMessageReceived(QString text);

private:
    HsbColor HsbColorFromRgb(double r, double g, double b);
    void navBack();
    void navForward();
    void navSelect();
    void refreshCurrentCards();
    void loadSongs();
    void animateCard(CardItem* card, QPoint position, bool selected, bool visible);
    
    QWebSocket *m_webSocket;
    SettingsManager *manager;
    LayoutManager layout;
    QPixmap image;
    QImage  *imageObject;
    QVector<CardItem*> allCards;
    QGraphicsPixmapItem* background;
    void sendCurrentColorToServer();

    void blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap);
    QGraphicsItem* ambienceCard(const char* title, const char* image);
    QGraphicsItem* backArrow;
    QGraphicsItem* nextArrow;
    const char * hexColorFromRGB(int r, int g, int b);
    
    float getFullScreenPixelRatioForImage(QPixmap* image);
    QGraphicsItem* pixmapItemFromSvg(const char* svgTitle);
    void handle_message(const std::string & message);
    void sendColorToServer(string hexColor);
    
    //QSignalMapper *mapper;
};

#endif // MAINSCENE_H
