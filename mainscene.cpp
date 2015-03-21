#include <math.h>
#include <QTransform>
#include <QSvgRenderer>
#include <QRect>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QGraphicsBlurEffect>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include "mainscene.h"

QT_USE_NAMESPACE

int currentSelection;
bool isConnected = false;

QMediaPlayer *player;
QMediaPlaylist *playlist;

//radius de blur
float blurRadius = 80;
QGraphicsBlurEffect *effect;

MainScene::MainScene()
{
    effect = new QGraphicsBlurEffect();
    //en mode performance (pour les PCs lents aux membres de l'équipe)
    effect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    effect->setBlurRadius(blurRadius);

    //mapper = new QSignalMapper(this);
    
    
    //initialise le client QWebSocket
    //connect(&m_webSocket, &QWebSocket::connected, this, &MainScene::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &MainScene::closed);
    //m_webSocket = QWebSocket new QWebSocket (QStringLiteral("Chat Server"),
                     //            QWebSocketServer::NonSecureMode,
                       //          this);
    
    m_webSocket = new QWebSocket();
    m_webSocket->open(QUrl("ws://107.170.171.251:56453"));
    //QObject::connect(m_webSocket, SIGNAL(connected()),
    //      this, SLOT(wsDidConnect()));
    connect(m_webSocket, &QWebSocket::connected, this, &MainScene::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MainScene::onDisconnect);

    currentSelection = 1;
    
    //lis les reglages
    manager = new SettingsManager();
    manager->readConfigFile();
    
    loadSongs();
    
    //nouveau seed pour que les fonctions aléatoires soient réellement aléatoires
    srand (time(NULL));
    
    //connecte au websocket
    //using easywsclient::WebSocket;
    //ws = easywsclient::WebSocket::from_url("http://107.170.171.251:8001");

    //prend la taille de l'écran en points (pas en pixels)
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    //set la taille de la scène pour que ca soit plein écran
    this->setSceneRect(0, 0, screenWidth, screenHeight);
    
    //charge les flèches back et next à partir du fichier svg
    backArrow  = pixmapItemFromSvg(":arrowLine.svg");
    nextArrow  = pixmapItemFromSvg(":arrowLine.svg");
    //rotate la flèche next
    nextArrow->setRotation(180);
    //centre les 2 items
    layout.centerInScreen(nextArrow);
    layout.centerInScreen(backArrow);
    //les déplace à leur bonne position
    backArrow->moveBy(-643,  -75 + (75*qApp->devicePixelRatio()));
    nextArrow->moveBy(643+(56*qApp->devicePixelRatio()), 67 + (75*qApp->devicePixelRatio()));

    float cardWidth = 406;
    float cardHeight = 466;
    float cardSmallScale = 0.8;
    
    for (int i = 0; i <3; i++) {
        
        CardItem *item = new CardItem(0, 0, cardWidth, cardHeight, "", "");
        
        //ajoute la carte à la scene
        this->addItem(item);
        
        //si c'est la carte du milieu, la sélectionner
        if(i%2 == 1){
            item->setSelectedStyle(true);
        }else{
            //sinon, la mettre plus petite
            item->setScale(cardSmallScale);
            item->setSelectedStyle(false);
        }
        //centrer la carte
        layout.centerInScreen(item);
        //l'ajouter au QVector des cartes visibles
        visibleCards.append(item);
    }
    
    //déplace les cartes au bon endroit, chacune de leur bord
    visibleCards.at(0)->moveBy(-cardWidth+cardWidth*(1-cardSmallScale)/2,
                               cardHeight*(1-cardSmallScale)/2);
    
    visibleCards.at(2)->moveBy(cardWidth+cardWidth*(1-cardSmallScale)/2,
                               cardHeight*(1-cardSmallScale)/2);
    
    //rafraichis et configure les cartes
    refreshCurrentCards();
    
    /*
    QTransform m;
    m.scale(0.8069269949, 0.8069269949);
    m.rotate(-30,Qt::YAxis);
    item3->setTransform(m);
    QTransform m3;
    //m3.scale(1/0.8069269949, 1/0.8069269949);
    m3.rotate(0,Qt::YAxis);
    //item3->setTransform(m3);
    //item3->setTransform(m.inverted());
    
    
    QTransform m2;
    m2.scale(0.8069269949, 0.8069269949);
    //m2.translate(406, 0);
    m2.rotate(30,Qt::YAxis);
    //m2.translate(-406, 0);
    item2->setTransformOriginPoint(406, 0);
    item2->setTransform(m2);
    */

}
void MainScene::loadSongs(){
    //initialise player et playlist
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist(player);
    
    //loop
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    
    //navigue au bon dossier
    QDir dir = QDir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();

    //prend chaque musique pour chaque preset
    for (int i = 0; i < manager->getPresetArray().count(); i++) {
        playlist->addMedia(QUrl::fromLocalFile(dir.path() + "/" +manager->getPresetArray().at(i).musicPath.c_str()));
    }
    
    //fort
    player->setVolume(100);
    //assigne le playlist au player
    player->setPlaylist(playlist);
    

}
void MainScene::refreshCurrentCards(){
    
    //si la sélection avant la courante est 0...
    if (currentSelection-1 >= 0) {
        //configure la carte
        visibleCards.at(0)->configure(&manager->getPresetArray().at(currentSelection-1));
        //affiche l'update de configuration
        visibleCards.at(0)->update();
    }else{
        //sinon, cache la carte
        /*
         CardItem *item = visibleCards.at(0);
        
        QPropertyAnimation *animation = new QPropertyAnimation((QGraphicsObject*)item, "opacity");
        animation->setDuration(1000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        
        animation->start();
        */
        visibleCards.at(0)->setOpacity(0);
    }
    
    if (currentSelection < manager->getPresetArray().count()) {
        visibleCards.at(1)->configure(&manager->getPresetArray().at(currentSelection));
        visibleCards.at(1)->update();
        
        //affichage de l'arrière plan
        imageObject = new QImage();
        imageObject->load(manager->getPresetArray().at(currentSelection).imgPath.c_str());

        image = QPixmap::fromImage(*imageObject);
        delete imageObject;
        //enlève l'ancien arrière plan
        //this->removeItem(background);
        
        if(background == NULL)
            background = this->addPixmap(image);
        else
            background->setPixmap(image);
        
        //le met en arrière plan
        background->setZValue(-1);
        
        //aspect fill l'écran
        image.setDevicePixelRatio(getFullScreenPixelRatioForImage(&image));
        
        //rend l'arrière plan flou et cool
        blurBackgroundItem(background, &image);
        
        //change de musique et la joue
        playlist->setCurrentIndex(currentSelection);
        //TODO: remettre le play (ca gosse la musique par dessus la musique qu'on veut vraiment)
        player->play();

    }else{
        //sinon, cache la carte du milieu (ne devrait jamais arriver, dans une situation normale)
        visibleCards.at(1)->setOpacity(0);
    }
    

    //cache et met à jour la 3e carte
    if (currentSelection+1 < manager->getPresetArray().count()) {
        visibleCards.at(2)->configure(&manager->getPresetArray().at(currentSelection+1));
        visibleCards.at(2)->update();
    }else{
        visibleCards.at(2)->setOpacity(0);
    }
}
//navigue par en arrière si possible
void MainScene::navBack(){
   if(currentSelection-1 >= 0){
        currentSelection--;
       
       //change de musique et la joue
       playlist->setCurrentIndex(currentSelection);
       //TODO: remettre le play (ca gosse la musique par dessus la musique qu'on veut vraiment)
       player->play();
       
       //et rafraichis les cartes
        refreshCurrentCards();
       sendCurrentColorToServer();

   }
}
void MainScene::navForward(){
    if(currentSelection+1 < manager->getPresetArray().count()){
        currentSelection++;
        
        //change de musique et la joue
        playlist->setCurrentIndex(currentSelection);
        //TODO: remettre le play (ca gosse la musique par dessus la musique qu'on veut vraiment)
        player->play();
        
        refreshCurrentCards();
        sendCurrentColorToServer();

        
    }
}
void MainScene::navSelect(){
    //TODO
}

void MainScene::blurBackgroundItem(QGraphicsItem *backgroundItem, QPixmap *referencePixmap){
    
    //assigne l'effet à l'item approprié
    backgroundItem->setGraphicsEffect(effect);
    //rend le scale plus grand pour cacher les bordures blanches
    backgroundItem->setScale((1/getFullScreenPixelRatioForImage(referencePixmap))*1.2);
    //cache les bordures en haut et à gauche
    backgroundItem->setX(-blurRadius/0.5);
    backgroundItem->setY(-blurRadius/0.5);
}

QGraphicsItem* MainScene::pixmapItemFromSvg(const char* svgTitle){
    //initialize le SVG renderer
    QSvgRenderer renderer((QString(svgTitle)));
    
    //initialize les dimensions selon la densité de pixels de l'écran (support pour écran Retina)
    QImage lineImage(58*qApp->devicePixelRatio(), 143*qApp->devicePixelRatio(), QImage::Format_ARGB32);
    //remplit de transparent l'image où on va "paint" le svg
    lineImage.fill(qRgba(0, 0, 0, 0));
    
    QPainter painter(&lineImage);
    //render le svg dans l'image
    renderer.render(&painter);
    QPixmap svgImage = QPixmap::fromImage(lineImage);
    
    #if defined(Q_OS_MAC)
        //rien!
    #endif
    
    //ajoute le tout à la scène
    QGraphicsItem* item = this->addPixmap(svgImage);
    //retina display
    item->setScale(1/qApp->devicePixelRatio());
    return item;
}

float MainScene::getFullScreenPixelRatioForImage(QPixmap* image){
    
    //calcule les bonnes dimensions de l'image d'arrière plan pour que l'écran soit rempli
    float imageHeight = image->size().height();
    float imageWidth = image->size().width();
    
    //va prendre les dimensions de l'écran
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    float heightRatio = imageHeight/screenHeight;
    float widthRatio = imageWidth/screenWidth;
    
    //rapporte le ratio le plus petit pour que ca rentre sans qu'il reste de blanc
    return (heightRatio<widthRatio)?heightRatio:widthRatio;
}

void MainScene::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "KEY ID:" << event->key();
    
    //arrière
    if (event->key() == 16777234) {
        navBack();
    }
    //en bas
    if (event->key() == 16777237) {
        navSelect();
    }
    //avant
    if (event->key() == 16777236) {
        navForward();
    }
    //touche 1 (pour debuggage)
    if (event->key() == 49) {
        sendColorToServer(hexColorFromRGB(rand() % 256, rand() % 256, rand() % 256));
    }
}

const char * MainScene::hexColorFromRGB(int r, int g, int b)
{
    //transforme les valeurs en string hexa
    QString rstring;
    rstring.setNum(r,16);
    QString gstring;
    gstring.setNum(g,16);
    QString bstring;
    bstring.setNum(b,16);
    
    //assemble le tout
    QString *string = new QString();
    string->append('#');
    string->append(rstring);
    string->append(gstring);
    string->append(bstring);
    
    return string->toStdString().c_str();
}

void MainScene::sendCurrentColorToServer()
{
    //envoye les couleurs du thème au serveur
    string str =    (manager->getPresetArray().at(currentSelection).color1) + "," +
    (manager->getPresetArray().at(currentSelection).color2) + "," +
    (manager->getPresetArray().at(currentSelection).color3) + "," +
    (manager->getPresetArray().at(currentSelection).color4);
    sendColorToServer(str);
}

void MainScene::sendColorToServer(string hexColor){
    //if(m_webSocket->state() != QAbstractSocket::ConnectedState)
      //  m_webSocket->open(QUrl("ws://107.170.171.251:8001"));
    
    //if (m_webSocket) {
      //  
    //}
    if(m_webSocket->state() == QAbstractSocket::ConnectedState){
        //qDebug() << "SENDING: " << hexColor.c_str() << "CurrentState:" <<m_webSocket.state();
        m_webSocket->sendTextMessage(QString(hexColor.c_str()));
    }
    //if(m_webSocket->sendTextMessage(QString(hexColor.c_str())) == 0){
    //    m_webSocket->open(QUrl("ws://107.170.171.251:8001"));
    //    m_webSocket->sendTextMessage(QString(hexColor.c_str()));
    //}
}
//destructeur
MainScene::~MainScene()
{

}

void MainScene::onConnected(){
    qDebug() << "did connect";
    isConnected = true;
    sendCurrentColorToServer();
}


void MainScene::onDisconnect(){
   // if(isConnected == true){
    qDebug() << "disconnected ... trying to reconnect";
    isConnected = false;
    //m_webSocket.close();
    m_webSocket->open(QUrl("ws://107.170.171.251:56453"));
    //}
}


