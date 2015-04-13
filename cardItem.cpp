#include "cardItem.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QLinearGradient>


QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE


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

void CardItem::configure(const Preset *prst){
    
    colorSetting = 1;
    
    color1 = QColor(QString(prst->color1.c_str()));
    color2 = QColor(QString(prst->color2.c_str()));
    color3 = QColor(QString(prst->color3.c_str()));
    color4 = QColor(QString(prst->color4.c_str()));
    
    this->setOpacity(1);
    title = prst->name;
    imgPath = prst->imgPath;
    inSettingsView = false;
    hideContent = false;
    imageObject = new QImage();
    imageObject->load(imgPath.c_str());
    image = QPixmap::fromImage(*imageObject);
    
    //background transparent
    //affichage de l'arrière plan
    //imageObject = QImage();
    //imageObject.load(manager->getPresetArray().at(currentSelection).imgPath.c_str());
    
    
    //extern QImage srcImg;//source image
    //QPixmap *pxDst( imageObject->size() );//blurred destination
    blurredBackground = QPixmap(imageObject->size()*qApp->devicePixelRatio());
    blurredBackground.fill(Qt::transparent);
    {
        QPainter painter(&blurredBackground);
        qt_blurImage(&painter, *imageObject, 1000 * ((float)image.height()*(float)image.width() / 1000000.f) / qApp->devicePixelRatio(), true, false);//blur radius: 2px
    }
    
    //delete imageObject;

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
    
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    
    float cornerRadius = 20.f;
    
    QColor transparentWhite = QColor(255, 255, 255);
    
    //constante alpha pour que l'animation fonctionne selon le scale lors du repaint
    float alphaConstant = (scale() * 1.25) - 0.55;
    transparentWhite.setAlpha(255*alphaConstant);
    
    if(!hideContent){
        painter->setPen(QPen(Qt::transparent)); //no border
        painter->setBrush(QBrush(transparentWhite, Qt::SolidPattern));
    }else{
        painter->setPen(QPen(Qt::white)); //no border
        painter->setBrush(QBrush(QColor("#111111")));
        painter->setOpacity(0.6);
    }
    
    if(this->boundingRect().width() > this->boundingRect().height())
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius, cornerRadius*(this->boundingRect().width()/this->boundingRect().height()));
    else
        painter->drawRoundRect( this->boundingRect().x(), this->boundingRect().y(), this->boundingRect().width(), this->boundingRect().height(), cornerRadius*(this->boundingRect().height()/this->boundingRect().width()), cornerRadius);
    
    painter->setOpacity(1.0);
    
    //QFontDatabase::addApplicationFont ( ":font.ttf" );
    //QFont font("BN BenWitch Project", 52);
    
    //QFont font("Helvetica Neue", 52);
    if(!hideContent){
        
        QString police = "Calibri";
    #ifdef __APPLE__
        police = "Helvetica Neue";
		QFont font(police, 52);
	#elif _WIN32
		QFont font(police, 38,QFont::Normal,true);
		font.setItalic(false);
	#endif
        
        font.setStyleName("Light");
        painter->setFont(font);
        painter->setPen(QPen(Qt::white));
        QPointF textCenter(this->boundingRect().width()/2,this->boundingRect().height()-(20+25));
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, title.c_str());
        
        float height = 340;
        float width = 340;
        
        float smallestDimension = image.width()<image.height()?image.width():image.height();
        painter->drawPixmap(QRect(this->boundingRect().x()+(this->boundingRect().width()/2)-(width/2), 30, width, height), image, QRect((smallestDimension-width)/4, 0, smallestDimension, smallestDimension));
    }else{
        //les bebelles de settings
        
#ifdef __APPLE__
        QFont font("Helvetica Neue", 40);
#else
        QFont font("Calibri", 24,QFont::Normal,true);
		font.setItalic(false);
#endif
        
        font.setStyleName("Thin");
        painter->setFont(font);
        
        float colorPresetWidth = 280;
        float colorPresetHeight = 90;
        float firstY = 100;
        float distanceBetween = 30;
        cornerRadius = 15;
        QRect transRect1 = QRect(this->boundingRect().x()+27, this->boundingRect().y()+firstY, colorPresetWidth, colorPresetHeight);
        QRect transRect2 = QRect(this->boundingRect().x()+27, this->boundingRect().y()+firstY+(distanceBetween+colorPresetHeight), colorPresetWidth, colorPresetHeight);
        QRect transRect3 = QRect(this->boundingRect().x()+27, this->boundingRect().y()+firstY+((distanceBetween+colorPresetHeight)*2), colorPresetWidth, colorPresetHeight);
        QRect backTransRect1 = QRect(this->boundingRect().x()+27+78, this->boundingRect().y()+firstY, colorPresetWidth, colorPresetHeight);
        QRect backTransRect2 = QRect(this->boundingRect().x()+27+78, this->boundingRect().y()+firstY+(distanceBetween+colorPresetHeight), colorPresetWidth, colorPresetHeight);
        QRect backTransRect3 = QRect(this->boundingRect().x()+27+78, this->boundingRect().y()+firstY+((distanceBetween+colorPresetHeight)*2), colorPresetWidth, colorPresetHeight);
        
        painter->setPen(QPen(QBrush(Qt::transparent),1));
        painter->setBrush(QBrush(Qt::white));
        painter->setOpacity(0.44);
        
        float backCornerRadius = cornerRadius*2.5;
        if (colorSetting != 2)
            painter->drawRoundRect( backTransRect2, backCornerRadius, backCornerRadius*(colorPresetWidth/colorPresetHeight));
        if (colorSetting == 2){
            painter->drawRoundRect( backTransRect1, backCornerRadius, backCornerRadius*(colorPresetWidth/colorPresetHeight));
            painter->drawRoundRect( backTransRect3, backCornerRadius, backCornerRadius*(colorPresetWidth/colorPresetHeight));
        }
        
        painter->setOpacity(1.0);
        
        //dessine le "o"
        painter->setPen(QPen(Qt::white));
        painter->setOpacity(1.00);
        QPointF textCenter;


		//SECTION Felix
		float constanteDegueulasse1 = 5.95 / 7;
		float constanteDegueulasse2 = 40 / 7;

		
        switch (colorSetting) {
            case 1:
				//5.95/7
				textCenter = QPointF(backTransRect2.x() + (backTransRect2.width()*constanteDegueulasse1), backTransRect2.y() + (backTransRect2.height() >> 1) - (constanteDegueulasse2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«o»");
                break;
                
            case 2:
				textCenter = QPointF(backTransRect1.x() + (backTransRect1.width()*constanteDegueulasse1), backTransRect1.y() + (backTransRect1.height() >> 1) - (constanteDegueulasse2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«u»");
				textCenter = QPointF(backTransRect3.x() + (backTransRect3.width()*constanteDegueulasse1), backTransRect3.y() + (backTransRect3.height() >> 1) - (constanteDegueulasse2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«o»");
                break;
                
            case 3:
				textCenter = QPointF(backTransRect2.x() + (backTransRect2.width()*constanteDegueulasse1), backTransRect2.y() + (backTransRect2.height() >> 1) - (constanteDegueulasse2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«u»");
                break;
                
            default:
                break;
        }
        
        QLinearGradient gradient = QLinearGradient(QPoint(transRect1.x(),transRect1.y()),QPoint(transRect1.x()+transRect1.width(), transRect1.y()));
        
        gradient.setColorAt(0.00, color1);
        gradient.setColorAt(0.33, color2);
        gradient.setColorAt(0.66, color3);
        gradient.setColorAt(1.00, color4);
        
        painter->setBrush(QBrush(gradient));
        
        if(colorSetting == 1)
            painter->setPen(QPen(QBrush(Qt::white),10));
        else
            painter->setPen(QPen(QBrush(Qt::transparent),2));
        
        //if(this->boundingRect().width() > this->boundingRect().height())
        painter->drawRoundRect( transRect1, cornerRadius, cornerRadius*(colorPresetWidth/colorPresetHeight));
        
        gradient.setColorAt(0.249, color1);
        gradient.setColorAt(0.250, color2);
        gradient.setColorAt(0.499, color2);
        gradient.setColorAt(0.501, color3);
        gradient.setColorAt(0.749, color3);
        gradient.setColorAt(0.750, color4);
        
        painter->setBrush(QBrush(gradient));

        
        if(colorSetting == 2)
            painter->setPen(QPen(QBrush(Qt::white),10));
        else
            painter->setPen(QPen(QBrush(Qt::transparent),2));
        
        painter->drawRoundRect( transRect2, cornerRadius, cornerRadius*(colorPresetWidth/colorPresetHeight));
        
        painter->setBrush(QBrush(color1));
        
        
        if(colorSetting == 3)
            painter->setPen(QPen(QBrush(Qt::white),10));
        else
            painter->setPen(QPen(QBrush(Qt::transparent),2));
        
        painter->drawRoundRect( transRect3, cornerRadius, cornerRadius*(colorPresetWidth/colorPresetHeight));
        
        /*------------------------------*/
        
        painter->setBrush(QBrush("#FFFFFF"));
        painter->setOpacity(0.44);
        painter->setPen(QPen(QBrush(Qt::transparent),1));
        cornerRadius = 42    ;
        QRect exitRect = QRect(this->boundingRect().x()+14, this->boundingRect().y()+14, 146, 60);
        painter->drawRoundRect( exitRect, cornerRadius, cornerRadius*(146.f/60.f));

        painter->setBrush(QBrush("#000000"));
        painter->setOpacity(0.76);
        
        float diameter = 27;
        //painter->setPen(QPen(QBrush(Qt::white),10));
        painter->drawEllipse(exitRect.x()+4, exitRect.y()+3, diameter*2, diameter*2);
        
        /*------------------------------*/
        
    
        //dessine le "a"
        painter->setPen(QPen(Qt::white));
        painter->setOpacity(1.00);
        textCenter = QPointF(exitRect.x()+(exitRect.width()*2/3),exitRect.y()+(exitRect.height()/2)-(40/7));
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«a»");
        
        //dessine le X
        font.setPointSize(35);
        painter->setPen(QPen(Qt::white));
        painter->setOpacity(0.73);
        textCenter = QPointF(exitRect.x()+(exitRect.width()*3/14),exitRect.y()+(exitRect.height()/2));//+(40/4));
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "X");
        
    }
    
    
}
void CardItem::setScale(qreal scale){
    
    if(scale > 1.05){
        hideContent = true;
    }else{
        hideContent = false;
    }
    
    this->prepareGeometryChange();
    
    QTransform m2;

    if(scale > 1){
        
        //set un angle approprié avec un scale de 1.0 à 1.1
        float angle = (scale - 1) * 10 * 180 - ((scale > 1.05)?180:0);
        
        m2.translate(this->boundingRect().width()/2, this->boundingRect().height()/2);
        m2.rotate(angle,Qt::YAxis);
        m2.translate(-this->boundingRect().width()/2 - (18 * (10*(scale-1))), -this->boundingRect().height()/2);
        
    }else{

        //distance par rapport au milieu
        
        
#if TARGET_OS_IPHONE
        float xMiddle = ((this->pos().x()-7) + (this->boundingRect().width()/2))-(1440/2);
#else
        float xMiddle = ((this->pos().x()-7) + (this->boundingRect().width()/2))-(QApplication::desktop()->screenGeometry().width()/2);
#endif
        
        if(xMiddle > 7){
            
            //constante
            xMiddle *= 0.9103139013;
            
            //déplace l'objet avant de le rotater
            m2.translate(0, this->boundingRect().height()/2);
            //rotate
            m2.rotate(xMiddle*0.1,Qt::YAxis);
            //redéplace à la bonne position
            m2.translate(0, -this->boundingRect().height()/2);
        }else{
            xMiddle *= 1.1092896175;
            
            //constante 2
            xMiddle *= 1.05;
            
            m2.translate(this->boundingRect().width(), this->boundingRect().height()/2);
            m2.rotate(xMiddle*0.1,Qt::YAxis);
            m2.translate(-this->boundingRect().width(), -this->boundingRect().height()/2);
            
        }
        
    }
    this->setTransform(m2);

    this->QGraphicsItem::setScale(scale);
    //appelle la 'vraie' fonction 'setScale'
    
}
QImage& CardItem::getImage(){
    return *imageObject;
}
QPixmap* CardItem::getPixmap(){
    return &image;
}
QPixmap* CardItem::getBlurredBackground(){
    return &blurredBackground;
}
void CardItem::changeColorSetting(bool up){
    if (up) {
        if (colorSetting != 3) {
            colorSetting++;
            this->update();
        }
    }else{
        if (colorSetting != 1) {
            colorSetting--;
            this->update();
        }
    }
}
int CardItem::getColorSetting(){
    return colorSetting;
}
void CardItem::setCurrentColorSetting(int currentColorSetting){
    colorSetting = currentColorSetting;
}
void CardItem::setInSettingsView(bool inSetView){
    inSettingsView = inSetView;
}
bool CardItem::getInSettingsView(){
    return inSettingsView;
}

void CardItem::turnForSettings(){
    //NOPE, PAS BON, FAUT FAIRE CA SELON LE SCALE
    
    //ok bonne nuit
    QTransform m2;
    
    m2.rotate(180,Qt::YAxis);
    
    this->setTransform(m2);
}