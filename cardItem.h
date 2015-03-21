#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include <QColor>
#include "settingsManager.h"
//#include "mainscene.h"

class CardItem : public QGraphicsItem
{
public:
    CardItem();
    CardItem(float xPos, float yPos, float width, float height, string text, string imagePath);
    ~CardItem();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void setSelectedStyle(bool isSelected);
    //void dragMoveEvent ( QDragMoveEvent * event );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    QRectF boundingRect() const;
    void drawText(QPainter & painter, const QPointF & point, int flags, const QString & text, QRectF * boundingRect);
    void configure(const Preset *prst);
    
protected:
    QRectF rect;
    bool selected;
    string title;
    string imgPath;
    //QPixmap image;
    QImage *imageObject;
    QPixmap image;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CARDITEM_H
