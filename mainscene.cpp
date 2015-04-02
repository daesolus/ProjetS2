#include <math.h>
#include <QTransform>
#include <QRect>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include "mainscene.h"

QT_USE_NAMESPACE

const int ANIMATION_TIME_MS = 200;
const bool ENABLE_SOUND = false;

string PHILIPS_HUE_URL = "10.0.1.34";
string PHILIPS_HUE_USERNAME = "lapfelixlapfelixlapfelix";
int PHILIPS_HUE_PORT = 80;

int currentSelection;
bool isConnected = false;

QMediaPlayer *player;
QMediaPlaylist *playlist;
QNetworkAccessManager * netManager;

struct cardProperties{
    float x;
    float y;
    float scale;
    bool selected;
};
cardProperties cardPos[5];
MainScene::MainScene()
{
    netManager = new QNetworkAccessManager(this);

    //enregistre le username "lapfelix" comme utilisateur des Philips Hue
    //(le 'Link Button' doit être enfoncé moins de 30 secondes avant le launch pour que ca fonctionne (si il y a un changement/reset)
    string body = "{\"device type\":\"hue#ProjetS2\", \"username\":\""+PHILIPS_HUE_USERNAME+"\"}";
    string URL = "http://"+PHILIPS_HUE_URL+":"+to_string(PHILIPS_HUE_PORT)+"/api";
    QNetworkRequest *request = new QNetworkRequest(QUrl(URL.c_str()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netManager->post(*request, body.c_str());
    
    //prend la taille de l'écran en points (pas en pixels)
    QRect rec = QApplication::desktop()->screenGeometry();
    float screenHeight = rec.height();
    float screenWidth =  rec.width();
    
    
    //hardcore hardcoding, sorry
    //optimisé pour un écran 1440x900 (retina), mais centre tt au centre pour que ca fonctionne avec
    //toute taille d'écran
    cardProperties props;
    props.x = (screenWidth/2) - 568.4;
    props.y = (screenHeight/2) - 186.4;
    props.scale = 0.8;
    cardPos[1] = props;
    props.x = props.x - 200;
    cardPos[0] = props;
    props.x = (screenWidth/2) + 243.6;
    props.y = (screenHeight/2) - 186.4;
    cardPos[3] = props;
    props.x = props.x + 200;
    cardPos[4] = props;
    props.x = (screenWidth/2) - 203;
    props.y = (screenHeight/2) - 233;
    props.scale = 1;
    cardPos[2] = props;
    
    m_webSocket = new QWebSocket();
    m_webSocket->open(QUrl("ws://107.170.171.251:56453"));

    connect(m_webSocket, &QWebSocket::connected, this, &MainScene::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MainScene::onDisconnect);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &MainScene::wsMessageReceived);

    currentSelection = 1;
    
    //lis les reglages
    manager = new SettingsManager();
    manager->readConfigFile();
    
    loadSongs();
    
    //nouveau seed pour que les fonctions aléatoires soient réellement aléatoires
    srand (time(NULL));
    
    //set la taille de la scène pour que ca soit plein écran
    this->setSceneRect(0, 0, screenWidth, screenHeight);
    
    //charge les flèches back et next à partir du fichier svg
    backArrow  = UIUtilities::pixmapItemFromSvg(":arrowLine.svg",this);
    nextArrow  = UIUtilities::pixmapItemFromSvg(":arrowLine.svg",this);
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
    
    for (int i = 0; i < manager->getPresetArray().count(); i++) {
        
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
        //le configure pour le bon preset
        item->configure(&manager->getPresetArray().at(i));
        
        //l'ajouter au QVector des cartes visibles
        allCards.append(item);
    }
    
    //rafraichis et configure les cartes
    refreshCurrentCards();

}

MainScene::~MainScene()
{
    
}

#pragma mark - Audio

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
		
#ifdef __APPLE__
        playlist->addMedia(QUrl::fromLocalFile(dir.path() + "/" +manager->getPresetArray().at(i).musicPath.c_str()));
#else
        playlist->addMedia(QUrl::fromLocalFile(manager->getPresetArray().at(i).musicPath.c_str()));
#endif

    }
    
    //fort
    player->setVolume(100);
    //assigne le playlist au player
    player->setPlaylist(playlist);
    

}

#pragma mark - Cartes/Navigation

void MainScene::refreshCurrentCards(){
   
    //get the new positions
    for (int i = 0; i < manager->getPresetArray().count(); i++) {
        if (i < currentSelection-1) {
            //cache à gauche
            UIUtilities::animateCard(allCards.at(i), QPoint(cardPos[0].x, cardPos[0].y), false, false, ANIMATION_TIME_MS);
        }else if(i == currentSelection-1){
            //place à gauche
            UIUtilities::animateCard(allCards.at(i), QPoint(cardPos[1].x, cardPos[1].y), false, true, ANIMATION_TIME_MS);
        }else if(i == currentSelection){
            //place au milieu
            UIUtilities::animateCard(allCards.at(i), QPoint(cardPos[2].x, cardPos[2].y), true, true, ANIMATION_TIME_MS);
        }else if(i == currentSelection+1){
            //place à droite
            UIUtilities::animateCard(allCards.at(i), QPoint(cardPos[3].x, cardPos[3].y), false, true, ANIMATION_TIME_MS);
        }else if(i > currentSelection+1){
            //cache à droite
            UIUtilities::animateCard(allCards.at(i), QPoint(cardPos[4].x, cardPos[4].y), false, false, ANIMATION_TIME_MS);
        }
    }
    
    if (currentSelection < manager->getPresetArray().count()) {
        
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
        image.setDevicePixelRatio(UIUtilities::getFullScreenPixelRatioForImage(&image));
        
        //rend l'arrière plan flou et cool
        UIUtilities::blurBackgroundItem(background, &image);
        
        //change de musique et la joue
        playlist->setCurrentIndex(currentSelection);

        if(ENABLE_SOUND)
            player->play();

    }
    
}

//navigue par en arrière si possible
void MainScene::navBack(){
   if(currentSelection-1 >= 0){
        currentSelection--;
       
       //change de musique et la joue
       playlist->setCurrentIndex(currentSelection);
       
       if(ENABLE_SOUND)
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

        if(ENABLE_SOUND)
            player->play();
        
        refreshCurrentCards();
        sendCurrentColorToServer();

        
    }
}
void MainScene::navSelect(){
    //TODO (!)
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
        RGBColor color(rand() % 256, rand() % 256, rand() % 256);
        sendColorToServer(color.getHex());
    }
}

#pragma mark - WebSocket

void MainScene::sendColorToServer(string hexColor){
    
    if(m_webSocket->state() == QAbstractSocket::ConnectedState){
        m_webSocket->sendTextMessage(QString(hexColor.c_str()));
    }
}

void MainScene::sendCurrentColorToServer()
{
    
    //envoye les couleurs du thème au serveur
    string str =    (manager->getPresetArray().at(currentSelection).color1) + "," +
    (manager->getPresetArray().at(currentSelection).color2) + "," +
    (manager->getPresetArray().at(currentSelection).color3) + "," +
    (manager->getPresetArray().at(currentSelection).color4);
    sendColorToServer(str);
    
    sendColorToPhilipsHue(1, RGBColor((manager->getPresetArray().at(currentSelection).color3).c_str()), 3);
    sendColorToPhilipsHue(2, RGBColor((manager->getPresetArray().at(currentSelection).color4).c_str()), 3);
    sendColorToPhilipsHue(3, RGBColor((manager->getPresetArray().at(currentSelection).color1).c_str()), 3);
    sendColorToPhilipsHue(4, RGBColor((manager->getPresetArray().at(currentSelection).color2).c_str()), 3);
    
}

void MainScene::wsMessageReceived(QString text){
    
    //vérifie le premier caractère pour savoir si ce qui a été reçu est une commande de navigation
    if(text.at(0) == '!'){
        qDebug() << text;
        switch (text.toStdString().c_str()[1]) {
                
            case 'r':
                navBack();
                break;
                
            case 'l':
                navForward();
                break;
                
            case 'm':
                navSelect();
                break;
                
            default:
                break;
        }
        
    }
    
}

void MainScene::onConnected(){
    qDebug() << "did connect";
    isConnected = true;
    //envoye la couleur courante au serveur (au cas ou la déconnexion était lors d'une tentative d'envoi de la couleur au serveur)
    sendCurrentColorToServer();
}


void MainScene::onDisconnect(){
    
    qDebug() << "disconnected ... trying to reconnect";
    isConnected = false;
    
    //tente une connection au serveur
    m_webSocket->open(QUrl("ws://107.170.171.251:56453"));
}


#pragma mark - Philips Hue

void MainScene::sendColorToPhilipsHue(int lightNumber, RGBColor color, int transitionTime){
    //le temps de transition est multiplié par 100ms, donc 7 = 700ms;
    
    //prend la couleur en HSB
    HsbColor colorHSB =  color.getHSB();
    
    //monte la saturation de 25% pour simuler l'effet gamma d'un écran pour que les couleurs se ressemblent (écran et lumières)
    float moreSat = (colorHSB.brightness * 1.25)>1?1:colorHSB.brightness * 1.25;
    
    string body = "{\"hue\":"+to_string((int)(colorHSB.hue * 65535))+", \"sat\":"+to_string((int)(colorHSB.saturation * 255))+", \"bri\":"+to_string((int)(moreSat * 255))+", \"transitiontime\":"+to_string(transitionTime)+"}";
    string URL = "http://"+PHILIPS_HUE_URL+":"+to_string(PHILIPS_HUE_PORT)+"/api/"+PHILIPS_HUE_USERNAME+"/lights/"+to_string(lightNumber)+"/state";
    
    QNetworkRequest *request = new QNetworkRequest(QUrl(URL.c_str()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netManager->put(*request, body.c_str());

}
