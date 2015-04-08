//
//  NavArrow.h
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-07.
//
//

#ifndef __ProjetS2__NavArrow__
#define __ProjetS2__NavArrow__

#include <QGraphicsItem>
#include <QObject>

class NavArrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    
    //Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    //Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    
public:
    NavArrow() : QObject(), QGraphicsItem(){
    }
    ~NavArrow();
    virtual void setScale(qreal scale);

};
#endif /* defined(__ProjetS2__NavArrow__) */
