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
    UIUtilities();
    void centerInScreen(QGraphicsItem *item);
    float getFullScreenPixelRatioForImage(QPixmap* image);
    QGraphicsItem* pixmapItemFromSvg(const char* svgTitle, QGraphicsScene *parentScene);
    void animateCard(CardItem* card, QPoint position, bool selected, bool visible, int ANIMATION_TIME_MS);
    //static void blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap);
    
private:
    QPropertyAnimation *opacityAnimation;
    QPropertyAnimation *positionAnimation;
    QPropertyAnimation *scaleAnimation;
};