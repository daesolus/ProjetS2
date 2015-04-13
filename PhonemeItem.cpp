//
//  PhonemeItem.cpp
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-12.
//
//

#include "PhonemeItem.h"

void PhonemeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
    
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    
    float cornerRadius = 40.f;
    
    QColor transparentWhite = QColor(255, 255, 255);
    
    //constante alpha pour que l'animation fonctionne selon le scale lors du repaint
    float alphaConstant = (scale() * 1.25) - 0.55;
    transparentWhite.setAlpha(255*alphaConstant);
    
    painter->setPen(QPen(Qt::transparent)); //no border
    painter->setBrush(QBrush(transparentWhite, Qt::SolidPattern));
    
    
    if(this->boundingRect().width() > this->boundingRect().height())
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius, cornerRadius*(this->boundingRect().width()/this->boundingRect().height()));
    else
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius*(this->boundingRect().height()/this->boundingRect().width()), cornerRadius);
    
    
    
}
void PhonemeItem::setSelected(bool isSelected){
    selected = isSelected;
}
QRectF PhonemeItem::boundingRect() const { // outer most edges
    return rect;
}