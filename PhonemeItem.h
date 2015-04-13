//
//  PhonemeItem.h
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-12.
//
//

#ifndef __ProjetS2__PhonemeItem__
#define __ProjetS2__PhonemeItem__

#include <stdio.h>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

class PhonemeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    
    //Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    //Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    
public:
    PhonemeItem() : QObject(), QGraphicsItem(){
        rect = QRectF(this->pos().x(), this->pos().y(), 130, 52);
    }
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    
    void setSelected(bool isSelected);
    
private:
    QRectF rect;
    QRectF boundingRect() const;
    bool selected;
    
};
#endif /* defined(__ProjetS2__PhonemeItem__) */
