#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include <QColor>
#include "settingsManager.h"
//#include "mainscene.h"

class CardItem : public QObject, public QGraphicsItem
{
    
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    //Q_OBJECT
    //Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    
public:
    //CustomRectItem(const QRectF &rect) : QObject(0),QGraphicsRectItem(rect)
    CardItem() : QObject(), QGraphicsItem(){
        rect = QRectF(0, 0, 50, 100);
        title = "";
        imgPath = "";
        
        imageObject = new QImage();
        imageObject->load(imgPath.c_str());
        image = QPixmap::fromImage(*imageObject);
        delete imageObject;
    }
    CardItem(float xPos, float yPos, float width, float height, string text, string imagePath): QObject(), QGraphicsItem(){
        rect = QRectF(xPos, yPos, width, height);
        
        title = text;
        imgPath = imagePath;
        
        imageObject = new QImage();
        
        imageObject->load(imgPath.c_str());
        image = QPixmap::fromImage(*imageObject);
        delete imageObject;
    }

    ~CardItem();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void setSelectedStyle(bool isSelected);
    //void dragMoveEvent ( QDragMoveEvent * event );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    QRectF boundingRect() const;
    void drawText(QPainter & painter, const QPointF & point, int flags, const QString & text, QRectF * boundingRect);
    void configure(const Preset *prst);
    virtual void setScale(qreal scale);

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
