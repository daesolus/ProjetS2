#include "mainview.h"
#include "mainscene.h"
#include <QDesktopWidget>

#include <QApplication>
#include <QGraphicsScene>
#include <QWidget>
#include <QDebug>
#include <QEvent>


MainView::MainView(MainScene* parent):QGraphicsView(parent)
{
    /*
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    */
    
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
#endif
    
    this->setSceneRect(0, verticalAdjustment, 1440, 900);

}
/*
bool MainView::event(QEvent *event){
    if(event->type() == QEvent::Gesture)
    {
        qDebug() << "Gesture";
    }
    qDebug() << "this:" << event->type();
    
    return QWidget::event(event);
}
*/
MainView::~MainView()
{

}

