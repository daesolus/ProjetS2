#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QWidget>

#include "mainscene.h"

class MainView : public QGraphicsView
{
public:
    MainView(MainScene*);
    ~MainView();
private:
    //virtual bool event(QEvent *event);
};

#endif // MAINVIEW_H
