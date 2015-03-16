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
    QFont font("Helvetica Neue",30,-1,false);
    QFont sansFont("Helvetica", 30);

    MainView view(&scene);
    view.showFullScreen();

    return a.exec();

}
