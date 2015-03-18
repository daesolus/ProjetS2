#include "mainscene.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>
#include <QApplication>
#include <math.h>
#include <QSvgRenderer>
#include <QGraphicsBlurEffect>
#include <QTransform>

MainScene::MainScene()
{
    SettingsManager manager;
    manager.readConfigFile();
    
    srand (time(NULL));
    
    //connects to the websocket
    using easywsclient::WebSocket;
    ws = WebSocket::from_url("http://107.170.171.251:8001");
    
    //gets the screen height and width (in points, not pixels)
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    //sets the rect of the scene to fit the screen
    this->setSceneRect(0, 0, screenWidth, screenHeight);
    
    //creates image
    imageObject = new QImage();
    imageObject->load(":Etoile.jpg");
    image = QPixmap::fromImage(*imageObject);
    
    //aspect fill the screen
    image.setDevicePixelRatio(getFullScreenPixelRatioForImage(&image));
    qDebug() << getFullScreenPixelRatioForImage(&image);
    //adds it to the scene
    //this->addPixmap(image);
    QGraphicsItem* background = this->addPixmap(image);

    //adds lol test text
    QGraphicsTextItem *text = new QGraphicsTextItem();
    text->setPlainText("test");
    text->setFlag(QGraphicsItem::ItemIsMovable);
    this->addItem(text);
    
    backArrow  = pixmapItemFromSvg(":arrowLine.svg");
    nextArrow  = pixmapItemFromSvg(":arrowLine.svg");
    nextArrow->setRotation(180);
    
    CardItem *item = new CardItem(0, 0, 406, 466, "Été", ":Etoile.jpg");
    item->setSelectedStyle(true);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    this->addItem(item);
    
    layout.centerInScreen(item);
    CardItem *item2 = new CardItem(0, 0, 406, 466, "Été", ":Etoile.jpg");
    item2->setSelectedStyle(false);
    item2->setFlag(QGraphicsItem::ItemIsMovable);
    item2->setScale(0.8);
    this->addItem(item2);
    
    layout.centerInScreen(item);
    CardItem *item3 = new CardItem(0, 0, 406, 466, "Été", ":Etoile.jpg");
    item3->setSelectedStyle(false);
    item3->setFlag(QGraphicsItem::ItemIsMovable);
    item3->setScale(0.8);
    this->addItem(item3);
    /*
    QTransform m;
    m.scale(0.8069269949, 0.8069269949);
    m.rotate(-30,Qt::YAxis);
    item3->setTransform(m);
    QTransform m3;
    //m3.scale(1/0.8069269949, 1/0.8069269949);
    m3.rotate(0,Qt::YAxis);
    //item3->setTransform(m3);
    //item3->setTransform(m.inverted());
    
    
    QTransform m2;
    m2.scale(0.8069269949, 0.8069269949);
    //m2.translate(406, 0);
    m2.rotate(30,Qt::YAxis);
    //m2.translate(-406, 0);
    item2->setTransformOriginPoint(406, 0);
    item2->setTransform(m2);
    */
    
    layout.centerInScreen(item);
    
    qDebug() << this->sceneRect().height();
    qDebug() << this->sceneRect().width();
    
    blurBackgroundItem(background, &image);
}

/*
QGraphicsItem* MainScene::ambienceCard(const char* title, const char* image){
    QPixmap pixmap = QPixmap(50,73); //example size that match my case
    QRectF rect(0,0,48,11);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setWorldMatrixEnabled(false);
    painter.setPen(QPen()); //no pen
    painter.setBrush(QBrush(color));
    painter.drawRoundedRect(rect, 2.0, 2.0);
 
    QGraphicsItem* item = this->addPixmap(svgImage);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setScale(1/qApp->devicePixelRatio());
    UIObjects.append(item);
    layout.centerInScreen(item);
    return item;
}
*/
void MainScene::blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap){
    float blurRadius = 40;
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
    effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    effect->setBlurRadius(blurRadius);
    backgroundItem->setGraphicsEffect(effect);
    backgroundItem->setScale((1/getFullScreenPixelRatioForImage(referencePixmap))*1.1);
    backgroundItem->setX(-blurRadius/1.5);
    backgroundItem->setY(-blurRadius/1.5);
}
QGraphicsItem* MainScene::pixmapItemFromSvg(const char* svgTitle){
    QSvgRenderer renderer((QString(svgTitle)));
    QImage lineImage(58*qApp->devicePixelRatio(), 143*qApp->devicePixelRatio(), QImage::Format_ARGB32);
    lineImage.fill(qRgba(0, 0, 0, 0));
    QPainter painter(&lineImage);
    renderer.render(&painter);
    QPixmap svgImage = QPixmap::fromImage(lineImage);
    
    #if defined(Q_OS_MAC)
        //rien!
    #endif
    
    QGraphicsItem* item = this->addPixmap(svgImage);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setScale(1/qApp->devicePixelRatio());
    UIObjects.append(item);
    layout.centerInScreen(item);
    return item;
}
float MainScene::getFullScreenPixelRatioForImage(QPixmap* image){
    float imageHeight = image->size().height();
    float imageWidth = image->size().width();
    
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    float heightRatio = imageHeight/screenHeight;
    float widthRatio = imageWidth/screenWidth;
    
    return (heightRatio<widthRatio)?heightRatio:widthRatio;
}
void MainScene::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "KEY ID:" << event->key();
    
    sendColorToServer(hexColorFromRGB(rand() % 256, rand() % 256, rand() % 256));
    
    printItemPositions();
}
const char* MainScene::hexColorFromRGB(int r, int g, int b)
{
    QString rstring;
    rstring.setNum(r,16);
    QString gstring;
    gstring.setNum(g,16);
    QString bstring;
    bstring.setNum(b,16);
    
    QString *string = new QString();
    string->append('#');
    string->append(rstring);
    string->append(gstring);
    string->append(bstring);
    
    return string->toStdString().c_str();
}

void MainScene::sendColorToServer(const std::string & hexColor){
    /*
     if(ws->CLOSED)
    {
        using easywsclient::WebSocket;
        ws = WebSocket::from_url("http://107.170.171.251:8001");
    }
    
    qDebug() << "Sending " << &hexColor << " to the server";
    
    if (ws->OPEN) {
        ws->send(hexColor);
        ws->poll();
    }
     */
}

void MainScene::printItemPositions(){
    QRect rec = QApplication::desktop()->screenGeometry();
    float height = rec.height()/2;
    float width =  rec.width()/2;
    
    for (int i = 0; i<UIObjects.count(); i++) {
        
        QPointF objPos = UIObjects.at(i)->pos();
        
        float heightFromCenter = objPos.ry() - height;
        float widthFromCenter = objPos.rx() - width;
        
        qDebug() << heightFromCenter << "," << widthFromCenter;
        
    }
}

MainScene::~MainScene()
{

}


