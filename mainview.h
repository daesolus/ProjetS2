#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGestureEvent>
#include <QWidget>

#include "mainscene.h"

class MainView : public QGraphicsView
{
public:
    MainView(MainScene*);
private:
    MainScene *theScene;
    
    bool viewportEvent(QEvent *event);

    void tapTriggered();
    void swipeTriggered(QSwipeGesture *gesture);
};

#endif // MAINVIEW_H
