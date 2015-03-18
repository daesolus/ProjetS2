#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include <QColor>

#include "mainscene.h"

class CardItem : public QGraphicsItem
{
public:
    CardItem();
    CardItem(float xPos, float yPos, float width, float height, const char* text, const char* imagePath);
    ~CardItem();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void setSelectedStyle(bool isSelected);
    //void dragMoveEvent ( QDragMoveEvent * event );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    QRectF boundingRect() const;
    void drawText(QPainter & painter, const QPointF & point, int flags, const QString & text, QRectF * boundingRect);
    
protected:
    QRectF rect;
    bool selected;
    const char* title;
    const char* imgPath;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CARDITEM_H
