#include "cardItem.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QLinearGradient>
#include <QUrl>
#include <cmath>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE


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
    
    QUrl imgURL(prst->imgPath.c_str());
    dlManager = new DownloadManager(imgURL);
    
    connect(dlManager, SIGNAL (downloadDone()), this, SLOT (processPictures()));

}
void CardItem::processPictures(){
    
    imageObject = new QImage();
    imageObject->loadFromData(dlManager->getData());
    
    image = QPixmap::fromImage(*imageObject);
    
    blurredBackground = QPixmap(imageObject->size()*qApp->devicePixelRatio());
    blurredBackground.fill(Qt::transparent);
    {
        QPainter painter(&blurredBackground);
        qt_blurImage(&painter, *imageObject, 1000 * ((float)image.height()*(float)image.width() / 1000000.f) / qApp->devicePixelRatio(), true, false);
    }
    
    this->update();
    
    //emit le slot
    emit cardLoaded();
}
QRectF CardItem::boundingRect() const { // outer most edges
    return QRectF(rect.x(), rect.y(), rect.width(), rect.height()+25);
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
    
    
#if TARGET_OS_IPHONE
    float penWidth = 2;
#else
    float penWidth = 1;
#endif
    
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
        painter->setPen(QPen(Qt::white, penWidth)); //no border
        painter->setBrush(QBrush(QColor("#111111")));
        painter->setOpacity(0.6);
    }
    
    if(rect.width() > rect.height())
        painter->drawRoundRect( rect.x(), rect.y(), rect.width(), rect.height(), cornerRadius, cornerRadius*(rect.width()/rect.height()));
    else
        painter->drawRoundRect( rect.x(), rect.y(), rect.width(), rect.height(), cornerRadius*(rect.height()/rect.width()), cornerRadius);
    
    painter->setOpacity(1.0);
    
    if(!hideContent){
        
        painter->setOpacity(this->opacity());
        
#ifdef __APPLE__
        QFont font("Helvetica Neue", 52);
        font.setStyleName("Light");
#else
        QFont font("Calibri", 38,QFont::Normal,true);
        font.setItalic(false);
#endif
        
        painter->setFont(font);
        painter->setPen(QPen(Qt::white));
        QPointF textCenter(rect.width()/2,rect.height()-(20+40));
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, title.c_str());
        
        float height = 340;
        float width = 340;
        
        float smallestDimension = image.width()<image.height()?image.width():image.height();
        painter->drawPixmap(QRect(rect.x()+(rect.width()/2)-(width/2), 30, width, height), image, QRect((smallestDimension-width)/4, 0, smallestDimension, smallestDimension));
        
        
        cornerRadius = 40.f;
        
        QColor transparentBlack = QColor(0, 0, 0);
        
        //constante alpha pour que l'animation fonctionne selon le scale lors du repaint
        //float alphaConstant = (scale() * 1.25) - 0.55;
        transparentBlack.setAlpha(255*0.7);
        
        painter->setPen(QPen(Qt::white,penWidth)); //no border
        painter->setBrush(QBrush(transparentBlack, Qt::SolidPattern));
        
        float phonemeWidth = 130;
        float phonemeHeight = 50;
        QRectF phonemeRect(rect.x()+(rect.width()/2)-(phonemeWidth/2), rect.y()+rect.height()-(phonemeHeight/2), phonemeWidth, phonemeHeight);
        painter->drawRoundRect( phonemeRect, cornerRadius, cornerRadius*(phonemeWidth/phonemeHeight));
        
        
#ifdef __APPLE__
        font = QFont("Helvetica Neue", 40);
        font.setStyleName("Thin");
#else
        font = QFont("Calibri", 26,QFont::Normal,true);
        font.setItalic(false);
#endif
        
        
#if TARGET_OS_IPHONE
        float xMiddle = ((this->pos().x()-7) + (this->boundingRect().width()/2))-(1440/2);
#else
        float xMiddle = ((this->pos().x()-7) + (this->boundingRect().width()/2))-(QApplication::desktop()->screenGeometry().width()/2);
#endif
        string phonemeString = "";
        if (abs(xMiddle) < 50) {
            phonemeString = "«o»";
        }else if(xMiddle > 0){
            phonemeString = "«a»";
        }else{
            phonemeString = "«u»";
        }
        
        painter->setFont(font);
        painter->setPen(QPen(Qt::white, penWidth));
        textCenter = QPointF(phonemeRect.x() + phonemeRect.width()/2,phonemeRect.y() + phonemeRect.height()/2 - 5);
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, phonemeString.c_str());
        
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
        QRect transRect1 = QRect(rect.x()+27, rect.y()+firstY, colorPresetWidth, colorPresetHeight);
        QRect transRect2 = QRect(rect.x()+27, rect.y()+firstY+(distanceBetween+colorPresetHeight), colorPresetWidth, colorPresetHeight);
        QRect transRect3 = QRect(rect.x()+27, rect.y()+firstY+((distanceBetween+colorPresetHeight)*2), colorPresetWidth, colorPresetHeight);
        QRect backTransRect1 = QRect(rect.x()+27+78, rect.y()+firstY, colorPresetWidth, colorPresetHeight);
        QRect backTransRect2 = QRect(rect.x()+27+78, rect.y()+firstY+(distanceBetween+colorPresetHeight), colorPresetWidth, colorPresetHeight);
        QRect backTransRect3 = QRect(rect.x()+27+78, rect.y()+firstY+((distanceBetween+colorPresetHeight)*2), colorPresetWidth, colorPresetHeight);
        
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
		float constanteDegueulasseMaisTellementCool1 = 5.95 / 7;
		float constanteDegueulasseMaisTellementCool2 = 40 / 7;

		
        switch (colorSetting) {
            case 1:
				//5.95/7
				textCenter = QPointF(backTransRect2.x() + (backTransRect2.width()*constanteDegueulasseMaisTellementCool1), backTransRect2.y() + (backTransRect2.height() >> 1) - (constanteDegueulasseMaisTellementCool2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«a»");
                break;
                
            case 2:
				textCenter = QPointF(backTransRect1.x() + (backTransRect1.width()*constanteDegueulasseMaisTellementCool1), backTransRect1.y() + (backTransRect1.height() >> 1) - (constanteDegueulasseMaisTellementCool2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«u»");
				textCenter = QPointF(backTransRect3.x() + (backTransRect3.width()*constanteDegueulasseMaisTellementCool1), backTransRect3.y() + (backTransRect3.height() >> 1) - (constanteDegueulasseMaisTellementCool2));
                drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«a»");
                break;
                
            case 3:
				textCenter = QPointF(backTransRect2.x() + (backTransRect2.width()*constanteDegueulasseMaisTellementCool1), backTransRect2.y() + (backTransRect2.height() >> 1) - (constanteDegueulasseMaisTellementCool2));
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
        QRect exitRect = QRect(rect.x()+14, rect.y()+14, 146, 60);
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
        drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«o»");
        
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
    
    //ok bonne nuit
    QTransform m2;
    
    m2.rotate(180,Qt::YAxis);
    
    this->setTransform(m2);
}