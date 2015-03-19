#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>
#include "layoutManager.h"
#include <QKeyEvent>
#include <QVector>
#include <QVariant>
#include "easywsclient.hpp"
//#include "settingsManager.h"
#include "cardItem.h"
#include <QTransform>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class MainScene : public QGraphicsScene
{
public:
    MainScene();
    ~MainScene();
    void keyPressEvent(QKeyEvent *event);
private:
    void navBack();
    void navForward();
    void navSelect();
    void refreshCurrentCards();
    void loadSongs();
    
    SettingsManager *manager;
    LayoutManager layout;
    QPixmap image;
    QImage  imageObject;
    QVector<CardItem*> visibleCards;
    QGraphicsItem* background;
    
    void blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap);
    QGraphicsItem* ambienceCard(const char* title, const char* image);
    QGraphicsItem* backArrow;
    QGraphicsItem* nextArrow;
    const char * hexColorFromRGB(int r, int g, int b);
    
    float getFullScreenPixelRatioForImage(QPixmap* image);
    QGraphicsItem* pixmapItemFromSvg(const char* svgTitle);
    void handle_message(const std::string & message);
    void sendColorToServer(string hexColor);
};

#endif // MAINSCENE_H
