#include "cardItem.h"
#include <QApplication>
#include <QDesktopWidget>

/*
CardItem::CardItem()
{
    

}
*/
/*
CardItem::CardItem(float xPos, float yPos, float width, float height, string text, string imagePath)
{
    rect = QRectF(xPos, yPos, width, height);
    
    title = text;
    imgPath = imagePath;
    
    imageObject = new QImage();
    
    imageObject->load(imgPath.c_str());
    image = QPixmap::fromImage(*imageObject);
    delete imageObject;
}*/

CardItem::~CardItem()
{
    
}
void CardItem::configure(const Preset *prst){
    this->setOpacity(1);
    title = prst->name;
    imgPath = prst->imgPath;
    
    imageObject = new QImage();
    imageObject->load(imgPath.c_str());
    image = QPixmap::fromImage(*imageObject);
    delete imageObject;

}

QRectF CardItem::boundingRect() const { // outer most edges
    return rect;
}

void CardItem::setSelectedStyle(bool isSelected){
    selected = isSelected;
}
void CardItem::drawText(QPainter & painter, const QPointF & point, int flags,
              const QString & text, QRectF * boundingRect = 0)
{
    //painter.setRenderHint(QPainter::Antialiasing);

    const qreal size = 32767.0;
    QPointF corner(point.x(), point.y() - size);
    if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
    else if (flags & Qt::AlignRight) corner.rx() -= size;
    if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
    else if (flags & Qt::AlignTop) corner.ry() += size;
    else flags |= Qt::AlignBottom;
    QRectF rect(corner, QSizeF(size, size));
    painter.drawText(rect, flags, text, boundingRect);
}
void CardItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
    
#ifdef __APPLE__
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
#endif
    float cornerRadius = 20.f;

    QColor transparentWhite = QColor(255, 255, 255);
    
    //constante alpha pour que l'animation fonctionne selon le scale lors du repaint
    float alphaConstant = (scale() * 1.25) - 0.55;
    transparentWhite.setAlpha(255*alphaConstant);
    
    painter->setPen(QPen(Qt::transparent)); //no pen
    painter->setBrush(QBrush(transparentWhite, Qt::SolidPattern));
    
    if(this->boundingRect().width() > this->boundingRect().height())
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius, cornerRadius*(this->boundingRect().width()/this->boundingRect().height()));
    else
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius*(this->boundingRect().height()/this->boundingRect().width()), cornerRadius);
    
    QFont font("Helvetica Neue", 52);
    font.setStyleName("Light");
    painter->setFont(font);
    painter->setPen(QPen(Qt::white));
    QPointF textCenter(this->boundingRect().width()/2,this->boundingRect().height()-(20+25));
    drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, title.c_str());
    
    float height = 340;
    float width = 340;
    
    //QPixmap image = QPixmap::fromImage(*imageObject)
    float smallestDimension = image.width()<image.height()?image.width():image.height();
    painter->drawPixmap(QRect(this->boundingRect().x()+(this->boundingRect().width()/2)-(width/2), 30, width, height), image, QRect((smallestDimension-width)/4, 0, smallestDimension, smallestDimension));
    
    //tentative de rotation selon la position x au milieu par rapport au centre de l'écran
    
    
    
    
    
    
}
void CardItem::setScale(qreal scale){
    this->prepareGeometryChange();
    
    float xMiddle = ((this->pos().x()+28) + (this->boundingRect().width()/2))-(QApplication::desktop()->screenGeometry().width()/2);
    QTransform m2;
    //m2.translate(406, 0);
    qDebug() << xMiddle;
    //m2.translate(-406, 0);
    qDebug() << boundingRect();
    if(xMiddle > 28){
        
        xMiddle *= 0.9103139013;
        
        //this->setTransformOriginPoint(this->boundingRect().width(), 0);
        
        //m2.translate(xMiddle, 0);
        //m2.rotate(xMiddle*0.1,Qt::YAxis);
        //m2.translate(-xMiddle, 0);
        
        
        //m2.translate(this->boundingRect().width()/2, 0);
        //m2.rotate(xMiddle*0.1,Qt::YAxis);
        //m2.translate(-this->boundingRect().width()/2, 0);
        
        
        m2.translate(0, this->boundingRect().height()/2);
        m2.rotate(xMiddle*0.1,Qt::YAxis);
        m2.translate(0, -this->boundingRect().height()/2);
    }else{
        xMiddle *= 1.1092896175;
        
        xMiddle *= 1.05;
        //this->setTransformOriginPoint(this->boundingRect().width(), 0);

        //this->setTransformOriginPoint(-0, 0);
        m2.translate(this->boundingRect().width(), this->boundingRect().height()/2);
        m2.rotate(xMiddle*0.1,Qt::YAxis);
        m2.translate(-this->boundingRect().width(), -this->boundingRect().height()/2);
        //scale = scale+(xMiddle*0.00001);
        
        
        //m2.translate(-xMiddle, 0);
        //m2.rotate(xMiddle*0.1,Qt::YAxis);
        //m2.translate(xMiddle, 0);
    }

    //title = to_string(xMiddle);
    
    this->setTransform(m2);
    
    this->QGraphicsItem::setScale(scale);
    this->update();

}
