#include "mainview.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFont>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QGLWidget>

QImage imageObject;
int main(int argc, char *argv[])
{

    
	QApplication a(argc, argv);
    
	try
	{
		MainScene scene(&a);

		QRect rec = QApplication::desktop()->screenGeometry();
		float screenHeight = rec.height();
		float screenWidth = rec.width();

#if TARGET_OS_IPHONE
		if( screenHeight/screenWidth == 0.75 )
			//c'est un iPad
			scene.setSceneRect(100, 0, 1440, 900);
		else
			//c'est un iPhone
			scene.setSceneRect(0, 0, 1440, 900);
#else
		scene.setSceneRect(0, 0, screenWidth, screenHeight);
#endif

		QFont font("Helvetica Neue", 30, -1, false);
		QFont sansFont("Helvetica", 30);

		a.setStyleSheet("background-color: black;");
		scene.setBackgroundBrush(QBrush(QColor(0, 0, 0)));
		
        MainView *view = new MainView(&scene);
        
        view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

        //view->setScene(&scene);
        
        
		//qDebug() << screenHeight / screenWidth;

#if TARGET_OS_IPHONE
		if( screenHeight/screenWidth == 0.75 )
			//c'est un iPad
			view->fitInView(QRect(0, 0, rec.height()/1.9, rec.width()/1.57), Qt::KeepAspectRatio);
		else
			//c'est un iPhone
			view->fitInView(QRect(0, 0, rec.height()*2.1, rec.width()*2.1), Qt::KeepAspectRatio);
#endif


#ifdef __APPLE__
#if TARGET_OS_IPHONE
		view->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
		view->setFrameShape(QFrame::NoFrame);
#else
		//rien. le défaut est parfait sur Mac
#endif
        
#else
		view->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
		view->setFrameShape(QFrame::NoFrame);
#endif

		//view.setTransform(m);
		view->showFullScreen();

        
		//view.show();
		return a.exec();
	}
	catch (std::exception& e){
		QMessageBox::critical(0, "Genius Couleur", e.what());
		return 0;
	}

}
