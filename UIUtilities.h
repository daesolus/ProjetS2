#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QApplication>
#include <QDesktopWidget>
#include <QSvgRenderer>
#include <QGraphicsPixmapItem>
#include "cardItem.h"
#include <QPropertyAnimation>
#include <QGraphicsEffect>

class UIUtilities
{
public:
    //UIUtilities();
    //~UIUtilities();
    static void centerInScreen(QGraphicsItem *item);
    static float getFullScreenPixelRatioForImage(QPixmap* image);
    static QGraphicsItem* pixmapItemFromSvg(const char* svgTitle, QGraphicsScene *parentScene);
    static void animateCard(CardItem* card, QPoint position, bool selected, bool visible, int ANIMATION_TIME_MS);
    static void blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap);
};