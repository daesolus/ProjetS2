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
#include "settingsManager.h"
#include "cardItem.h"
#include <QTransform>

class MainScene : public QGraphicsScene
{
public:
    MainScene();
    ~MainScene();
    void keyPressEvent(QKeyEvent *event);
private:
    QVector<QGraphicsItem*> UIObjects;
    LayoutManager layout;
    QPixmap image;
    QImage  *imageObject;
    
    void blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap);
    QGraphicsItem* ambienceCard(const char* title, const char* image);
    QGraphicsItem* backArrow;
    QGraphicsItem* nextArrow;
    easywsclient::WebSocket::pointer ws;
    const char* hexColorFromRGB(int r, int g, int b);

    float getFullScreenPixelRatioForImage(QPixmap* image);
    QGraphicsItem* pixmapItemFromSvg(const char* svgTitle);
    void printItemPositions();
    void handle_message(const std::string & message);
    void sendColorToServer(const std::string & hexColor);
};

#endif // MAINSCENE_H
