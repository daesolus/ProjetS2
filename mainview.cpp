#include "mainview.h"
#include "mainscene.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsScene>
#include <QWidget>
#include <QDebug>
#include <QEvent>

QPointF oldPoint;
bool ignoreNextEnd = false;

MainView::MainView(MainScene* parent):QGraphicsView(parent)
{
    
    theScene = parent;
    
    oldPoint = QPointF(-1, -1);
    
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    
    //disable the scrollbars
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    
    
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    float verticalAdjustment = 0;
    
#if TARGET_OS_IPHONE
    if( screenHeight/screenWidth == 0.75 )
        //c'est un iPad
        verticalAdjustment = 30;
    else
        //c'est un iPhone
        verticalAdjustment = 30;
    
    this->setSceneRect(0, verticalAdjustment, 1440, 900);
#else
    this->setSceneRect(0, 0, screenWidth, screenHeight);
#endif
    
}

bool MainView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
        case QEvent::TouchBegin:{
            oldPoint = (static_cast<QTouchEvent *>(event))->touchPoints().first().pos();
            return true;
        }
        case QEvent::TouchUpdate:{
            
            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
            QPointF newPoint = touchPoints.first().pos();
            
            int xDiff = oldPoint.rx() - newPoint.rx(), yDiff = oldPoint.ry() - newPoint.ry();
            
            if(sqrt(pow(abs(xDiff),2)+pow(abs(yDiff),2)) > 80){
                
                ignoreNextEnd = true;
                
                //IT'S A SWIPE!
                oldPoint = newPoint;
                
                if ((abs(xDiff)/abs(yDiff)) > 1) {
                    //horizontal swipe
                    if(xDiff > 0){
                        //left
                        theScene->navSendL();
                    }else{
                        //right
                        theScene->navSendR();
                        
                    }
                }else{
                    //vertical swipe
                    if(yDiff > 0){
                        //down
                        theScene->navSendR();
                        
                    }else{
                        //up
                        theScene->navSendL();
                        
                    }
                }
            }
            
            
            return true;
        }
        case QEvent::TouchEnd:
        {
            if (ignoreNextEnd) {
                ignoreNextEnd = false;
                return true;
            }
            
            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
            QPointF newPoint = touchPoints.first().pos();
            
            if(oldPoint.rx() < 0){
                //wtf "TouchBegin" n'a jamais existé
                return true;
            }else{
                int xDiff = oldPoint.rx() - newPoint.rx(), yDiff = oldPoint.ry() - newPoint.ry();
                
                if(sqrt(pow(abs(xDiff),2)+pow(abs(yDiff),2)) < 10){
                    //IT'S A TAP!
                    theScene->navSendM();
                    return true;
                }
                /*
                if ((abs(xDiff)/abs(yDiff)) > 1) {
                    //horizontal swipe
                    if(xDiff > 0){
                        //left
                        theScene->navSendL();
                    }else{
                        //right
                        theScene->navSendR();
                        
                    }
                }else{
                    //vertical swipe
                    if(yDiff > 0){
                        //down
                        theScene->navSendR();
                        
                    }else{
                        //up
                        theScene->navSendL();
                        
                    }
                }
                 */
                
            }
            return true;
        }
        default:
            break;
    }
    return QGraphicsView::viewportEvent(event);
}

MainView::~MainView()
{

}

