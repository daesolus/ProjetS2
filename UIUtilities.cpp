#include "UIUtilities.h"


UIUtilities::UIUtilities(){
    positionAnimation = new QPropertyAnimation();
    positionAnimation->setEasingCurve(QEasingCurve::OutCirc);
    
    //initialise l'animation d'échelle
    scaleAnimation = new QPropertyAnimation();
    scaleAnimation->setEasingCurve(positionAnimation->easingCurve());
    
    //initialise l'animation d'opacité
    //opacity = new QGraphicsOpacityEffect;
    opacityAnimation = new QPropertyAnimation();
    opacityAnimation->setEasingCurve(positionAnimation->easingCurve());
}

void UIUtilities::centerInScreen(QGraphicsItem *item){
    
    //prend les dimensions de l'écran
    //item->pare
    QRect rec = QApplication::desktop()->screenGeometry();
    float verticalAdjustment = 1.3;

	float screenWidth;
#ifdef  _WIN32
	verticalAdjustment = 1.15;
#endif
#if TARGET_OS_IPHONE
    if( (float)rec.height()/(float)rec.width() == 0.75 ){
        //c'est un iPad
        verticalAdjustment = 1.57;
    }else{
        verticalAdjustment = 4.2;
    }
	screenWidth =  1440;
#else
	screenWidth = rec.width();
#endif
	
    float screenHeight = (float)rec.height() * verticalAdjustment;
    
    
    //prend les dimensions de l'item
    float width = item->boundingRect().width();
    float height = item->boundingRect().height();
    
    //place l'item au milieu de l'écran
    item->setPos((screenWidth /2)-(width /2),(screenHeight/2)-(height/2));
}

float UIUtilities::getFullScreenPixelRatioForImage(QPixmap* image){
    
    //calcule les bonnes dimensions de l'image d'arrière plan pour que l'écran soit rempli
    float imageHeight = image->size().height();
    float imageWidth = image->size().width();
    
    //va prendre les dimensions de l'écran
    QRect rec = QApplication::desktop()->screenGeometry();
    
    
#if TARGET_OS_IPHONE
    float screenHeight = 900;//rec.height();
    float screenWidth =  1440;//rec.width();
#else
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
#endif
    
    float heightRatio = imageHeight/screenHeight;
    float widthRatio = imageWidth/screenWidth;

    //rapporte le ratio le plus petit pour que ca rentre sans qu'il reste de blanc
    return 1/((heightRatio<widthRatio)?heightRatio:widthRatio);
}

QGraphicsItem* UIUtilities::pixmapItemFromSvg(const char* svgTitle, QGraphicsScene *parentScene){
    //initialize le SVG renderer
    QSvgRenderer renderer((QString(svgTitle)));
    
    //initialize les dimensions selon la densité de pixels de l'écran (support pour écran Retina)
    QImage lineImage(58*qApp->devicePixelRatio(), 143*qApp->devicePixelRatio(), QImage::Format_ARGB32);
    //remplit de transparent l'image où on va "paint" le svg
    lineImage.fill(qRgba(0, 0, 0, 0));
    
    QPainter painter(&lineImage);
    //render le svg dans l'image
    renderer.render(&painter);
    QPixmap svgImage = QPixmap::fromImage(lineImage);
    
#if defined(Q_OS_MAC)
    //rien!
#endif
    
    //ajoute le tout à la scène
    QGraphicsItem* item = parentScene->addPixmap(svgImage);
    //retina display
    item->setScale(1/qApp->devicePixelRatio());
    return item;
}

void UIUtilities::animateCard(CardItem* card, QPoint position, bool selected, bool visible, const int ANIMATION_TIME_MS){

    //card->setOpacity(visible?1.0:0.0);
    if(ANIMATION_TIME_MS == 0){
        card->setPos(position);
        card->setScale(selected?1:0.8);
        card->setOpacity(visible?1.0:0.0);
    }else{
        
        //initialise l'animation de position
        positionAnimation = new QPropertyAnimation();
        positionAnimation->setTargetObject((QGraphicsObject*)card);
        positionAnimation->setPropertyName("pos");
        positionAnimation->setDuration(ANIMATION_TIME_MS);
        positionAnimation->setStartValue(card->pos());
        positionAnimation->setEndValue(position);
        
        
        //initialise l'animation d'échelle
        scaleAnimation = new QPropertyAnimation();
        scaleAnimation->setTargetObject((QGraphicsObject*)card);
        scaleAnimation->setPropertyName("scale");
        scaleAnimation->setDuration(ANIMATION_TIME_MS);
        scaleAnimation->setStartValue(card->scale());
        scaleAnimation->setEndValue(selected?1:0.8);

        float currentOpacity = 0;
        
        //prend en note l'opacité courante
        if((QGraphicsOpacityEffect*)card->graphicsEffect())
            currentOpacity = ((QGraphicsOpacityEffect*)card->graphicsEffect())->opacity();
        
        //initialise l'animation d'opacité
        QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
        opacityAnimation = new QPropertyAnimation();
        opacityAnimation->setTargetObject(opacity);
        opacityAnimation->setPropertyName("opacity");
        card->setGraphicsEffect( opacity );
        opacityAnimation->setDuration(ANIMATION_TIME_MS);
        opacityAnimation->setStartValue(currentOpacity);
        opacityAnimation->setEndValue( visible?1.0:0.0 );
        opacityAnimation->setEasingCurve(positionAnimation->easingCurve());

        //démarre toute les animations
        opacityAnimation->start();
        positionAnimation->start();
        scaleAnimation->start();
    }
}
