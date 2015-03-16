#include "mainview.h"
#include "mainscene.h"
#include <QDesktopWidget>

#include <QApplication>
#include <QGraphicsScene>
#include <QWidget>


MainView::MainView(MainScene* parent):QGraphicsView(parent)
{
    //disable the scrollbars
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    
    
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    this->setSceneRect(0, 0, screenWidth, screenHeight);

}

MainView::~MainView()
{

}

