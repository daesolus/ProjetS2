#include "UIUtilities.h"

void UIUtilities::centerInScreen(QGraphicsItem *item){
    
    //prend les dimensions de l'écran
    //item->pare
    QRect rec = QApplication::desktop()->screenGeometry();
    float verticalAdjustment = 1.3;
    
#if TARGET_OS_IPHONE
    if( (float)rec.height()/(float)rec.width() == 0.75 ){
        //c'est un iPad
        verticalAdjustment = 1.57;
    }else{
        verticalAdjustment = 3.5;
    }
#endif
    
    float screenHeight = (float)rec.height() * verticalAdjustment;
    float screenWidth =  1440;//rec.width();
    
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
    float screenHeight = 900;//rec.height();
    float screenWidth =  1440;//rec.width();
    
    float heightRatio = imageHeight/screenHeight;
    float widthRatio = imageWidth/screenWidth;
    
    //rapporte le ratio le plus petit pour que ca rentre sans qu'il reste de blanc
    return (heightRatio<widthRatio)?heightRatio*2:widthRatio*2;
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
#if TARGET_OS_IPHONE
    //card->pos() = position;
    //card->setScale(selected?1:0.8);
    //card->setOpacity(visible?1.0:0.0);
    //return;
#endif
    //initialise l'animation de position
    QPropertyAnimation *positionAnimation = new QPropertyAnimation((QGraphicsObject*)card, "pos");
    positionAnimation->setDuration(ANIMATION_TIME_MS);
    positionAnimation->setStartValue(card->pos());
    positionAnimation->setEndValue(position);
    
    //initialise l'animation d'échelle
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation((QGraphicsObject*)card, "scale");
    scaleAnimation->setDuration(ANIMATION_TIME_MS);
    scaleAnimation->setStartValue(card->scale());
    scaleAnimation->setEndValue(selected?1:0.8);
    float currentOpacity = 0;
    
    //prend en note l'opacité courante
    if((QGraphicsOpacityEffect*)card->graphicsEffect())
        currentOpacity = ((QGraphicsOpacityEffect*)card->graphicsEffect())->opacity();
    
    //initialise l'animation d'opacité
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacity, "opacity" );
    card->setGraphicsEffect( opacity );
    opacityAnimation->setDuration(ANIMATION_TIME_MS);
    opacityAnimation->setStartValue(currentOpacity);
    opacityAnimation->setEndValue( visible?1.0:0.0 );
    
    //démarre toute les animations
    opacityAnimation->start();
    positionAnimation->start();
    scaleAnimation->start();
}

/*
void UIUtilities::blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap){
    
#ifdef TARGET_OS_IPHONE
    return;
#endif
    
    float blurRadius = 80.f;
    
    //désactive le blur si le compileur compile pour un ordinateur qui n'est pas un
    //Mac pour des raisons (mystérieuses(?)) de performance
#ifdef __APPLE__
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect();
    effect->setBlurHints(QGraphicsBlurEffect::AnimationHint);
    effect->setBlurRadius(blurRadius);
    
    //assigne l'effet à l'item approprié
    backgroundItem->setGraphicsEffect(effect);
#endif
    
    //rend le scale plus grand pour cacher les bordures blanches
    //backgroundItem->setScale((1/getFullScreenPixelRatioForImage(referencePixmap))*1.2);
    //cache les bordures en haut et à gauche
    backgroundItem->setX(-blurRadius/0.5);
    backgroundItem->setY(-blurRadius/0.5);
    
}
 */
