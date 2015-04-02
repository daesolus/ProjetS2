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
	QString police = "Calibri";
#ifdef __APPLE__
	police = "Helvetica Neue";
#endif
	QFont font(police, 30, -1, false);
#ifdef __APPLE__
	police = "Helvetica";
#endif
    QFont sansFont(police, 30);
    
    /*
     //Show off mode
     
     QTransform m;
    
        int x = 1440;
        m.translate(x,900);
        m.rotate(30,Qt::XAxis);
        m.translate(-x,-990);
        */

    MainView view(&scene);
    //view.setTransform(m);
    view.showFullScreen();

    return a.exec();

}
