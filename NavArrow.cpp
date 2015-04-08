//
//  NavArrow.cpp
//  ProjetS2
//
//  Created by Felix Lapalme on 2015-04-07.
//
//

#include "NavArrow.h"
#include <QDebug>

NavArrow::~NavArrow()
{
    
}
void NavArrow::setScale(qreal scale){
    qDebug() << "heylol";
    this->QGraphicsItem::setScale(scale);
}