#include "mainscene.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>
#include <QApplication>
#include <math.h>
#include <QSvgRenderer>


MainScene::MainScene()
{
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
    
    //adds it to the scene
    this->addPixmap(image);

    //adds lol test text
    QGraphicsTextItem *text = new QGraphicsTextItem();
    text->setPlainText("test");
    text->setFlag(QGraphicsItem::ItemIsMovable);
    this->addItem(text);
    
    backArrow  = pixmapItemFromSvg(":arrowLine.svg");
    nextArrow  = pixmapItemFromSvg(":arrowLine.svg");
    nextArrow->setRotation(180);
    
    qDebug() << this->sceneRect().height();
    qDebug() << this->sceneRect().width();


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
    
    //génère une couleur random en hex
    char str[7];
    for(int i = 1; i < 7; i++) {
        sprintf(str + i, "%x", rand() % 16);
    }
    str[0] = '#';
    
    sendColorToServer(str);
    
    printItemPositions();
}

void MainScene::sendColorToServer(const std::string & hexColor){
    qDebug() << "Sending " << &hexColor << " to the server";
    ws->send(hexColor);
    ws->poll();
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


