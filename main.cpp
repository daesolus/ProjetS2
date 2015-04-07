#include "mainview.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFont>
#include <QImage>
#include <QPixmap>
QImage imageObject;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainScene scene;
    
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
#if TARGET_OS_IPHONE
    if( screenHeight/screenWidth == 0.75 )
        //c'est un iPad
        scene.setSceneRect(100, 0, 1440, 900);
    else
        //c'est un iPhone
        scene.setSceneRect(0, 0, 1440, 900);
#else
    scene.setSceneRect(0, 0, 1440, 900);
#endif
    
    QFont font("Helvetica Neue",30,-1,false);
    QFont sansFont("Helvetica", 30);
    
    a.setStyleSheet("background-color: black;");
    scene.setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    
    /*
     //Show off mode
     
     QTransform m;
    
        int x = 1440;
        m.translate(x,900);
        m.rotate(30,Qt::XAxis);
        m.translate(-x,-990);
        */
    
    MainView view(&scene);
    qDebug() << screenHeight/screenWidth;
    
#if TARGET_OS_IPHONE
    if( screenHeight/screenWidth == 0.75 )
        //c'est un iPad
        view.fitInView(QRect(0, 0, rec.height()/1.9, rec.width()/1.57), Qt::KeepAspectRatio);
    else
        //c'est un iPhone
        view.fitInView(QRect(0, 0, rec.height()*2.1, rec.width()*2.05), Qt::KeepAspectRatio);
#endif
    
    //view.setTransform(m);
    view.showFullScreen();
    //view.show();
    return a.exec();

}
