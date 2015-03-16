#import "layoutManager.h"

void LayoutManager::centerInScreen(QGraphicsItem *item){
    
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    float width = item->boundingRect().width();
    float height = item->boundingRect().height();
    
    item->setPos((screenWidth /2)-(width /2),(screenHeight/2)-(height/2));
}