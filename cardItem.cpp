#include "cardItem.h"


CardItem::CardItem()
{
    rect = QRectF(0, 0, 50, 100);
    title = "";
    imgPath = "";
    
    imageObject = new QImage();
    imageObject->load(imgPath.c_str());
    image = QPixmap::fromImage(*imageObject);
    delete imageObject;

}

CardItem::CardItem(float xPos, float yPos, float width, float height, string text, string imagePath)
{
    rect = QRectF(xPos, yPos, width, height);
    
    title = text;
    imgPath = imagePath;
    
    imageObject = new QImage();
    
    imageObject->load(imgPath.c_str());
    image = QPixmap::fromImage(*imageObject);
    delete imageObject;
}

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
    
    float cornerRadius = 20.f;

    QColor transparentWhite = QColor(255, 255, 255);
    if (selected) {
        transparentWhite.setAlpha(255*0.7);
    }else{
        transparentWhite.setAlpha(255*0.45);
    }
    painter->setPen(QPen(Qt::transparent)); //no pen
    painter->setBrush(QBrush(transparentWhite, Qt::SolidPattern));
    
    if(rect.width() > rect.height())
        painter->drawRoundRect( rect.x(), rect.y(), rect.width(), rect.height(), cornerRadius, cornerRadius*(rect.width()/rect.height()));
    else
        painter->drawRoundRect( rect.x(), rect.y(), rect.width(), rect.height(), cornerRadius*(rect.height()/rect.width()), cornerRadius);
    
    QFont font("Helvetica Neue", 52);
    font.setStyleName("Light");
    painter->setFont(font);
    painter->setPen(QPen(Qt::white));
    QPointF textCenter(rect.width()/2,rect.height()-(20+25));
    drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, title.c_str());
    
    float height = 340;
    float width = 340;
    
    //QPixmap image = QPixmap::fromImage(*imageObject);
    float smallestDimension = image.width()<image.height()?image.width():image.height();
    painter->drawPixmap(QRect(rect.x()+(rect.width()/2)-(width/2), 30, width, height), image, QRect((smallestDimension-width)/4, 0, smallestDimension, smallestDimension));
    
}
void CardItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    
}
void CardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    
}