#include <math.h>
#include <QTransform>
#include <QRect>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include "mainscene.h"
#include <QGestureRecognizer>
#include <QGesture>
#include <QSwipeGesture>
#include <ctime>
#include <QTouchEvent>
#include <QMessageBox>

QT_USE_NAMESPACE

const int ANIMATION_TIME_MS = 250;
const int LIGHTS_ANIMATION_TIME = 5;
string PHILIPS_HUE_URL = "10.0.1.34";//34
string PHILIPS_HUE_USERNAME = "lapfelixlapfelixlapfelix";
int PHILIPS_HUE_PORT = 80;

int currentSelection;

int mcount = 0;
int rcount = 0;
int lcount = 0;

int colorCycle = 0;

bool isConnected = false;

QMediaPlayer *player;
QMediaPlaylist *playlist;
QTimer *timer;
QNetworkAccessManager *netManager;
QNetworkRequest *request;
UIUtilities *UIManager;

QPoint backArrowOriginalPos;
QPoint nextArrowOriginalPos;

struct cardProperties{
	float x;
	float y;
	float scale;
	bool selected;
};

cardProperties cardPos[5];
MainScene::MainScene()
{

    UIManager = new UIUtilities();
    
	background = nullptr;

	netManager = new QNetworkAccessManager(this);

	//enregistre le username "lapfelix" comme utilisateur des Philips Hue
	//(le 'Link Button' doit être enfoncé moins de 30 secondes avant le launch pour que ca fonctionne (si il y a un changement/reset)
	string body = "{\"device type\":\"hue#ProjetS2\", \"username\":\"" + PHILIPS_HUE_USERNAME + "\"}";
	string URL = "http://" + PHILIPS_HUE_URL + ":" + to_string(PHILIPS_HUE_PORT) + "/api";
	request = new QNetworkRequest(QUrl(URL.c_str()));
	request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	netManager->post(*request, body.c_str());

	//prend la taille de l'écran en points (pas en pixels)
	QRect rec = QApplication::desktop()->screenGeometry();

	float heightConstant = 1;
	//qDebug() << "hellol" << (float)rec.height() / (float)rec.width();
#if TARGET_OS_IPHONE
	if( (float)rec.height()/(float)rec.width() == 0.75 )
		//c'est un iPad
		heightConstant = 1.23;
	else
		heightConstant = 3.4;
	float screenWidth =  1440;
#else
	float screenWidth = rec.width();
#endif

	float screenHeight = rec.height() * heightConstant;


	//hardcore hardcoding, sorry
	//optimisé pour un écran 1440x900 (retina), mais centre tt au centre pour que ca fonctionne avec
	//toute taille d'écran
	cardProperties props;
	props.x = (screenWidth / 2) - 568.4;
	props.y = (screenHeight / 2) - 186.4;
	props.scale = 0.8;
	cardPos[1] = props;
	props.x = props.x - 200;
	cardPos[0] = props;
	props.x = (screenWidth / 2) + 243.6;
	props.y = (screenHeight / 2) - 186.4;
	cardPos[3] = props;
	props.x = props.x + 200;
	cardPos[4] = props;
	props.x = (screenWidth / 2) - 196;
	props.y = (screenHeight / 2) - 233;
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

	//loadSongs();
	/*
	#ifdef __APPLE__
	#include "TargetConditionals.h"
	#ifdef TARGET_OS_IPHONE
	// iOS
	#elif TARGET_IPHONE_SIMULATOR
	// iOS Simulator
	#else
	// Unsupported platform
	loadSongs();
	#endif
	#else
	loadSongs();
	#endif
	*/
//#if TARGET_OS_IPHONE
	//nothing
//#else
    if(ENABLE_SOUND)
        loadSongs();
//#endif

	//nouveau seed pour que les fonctions aléatoires soient réellement aléatoires
	srand(time(NULL));

	//set la taille de la scène pour que ca soit plein écran
	this->setSceneRect(0, 0, screenWidth, screenHeight);

	//charge les flèches back et next à partir du fichier svg

	backArrow = (NavArrow*)UIManager->pixmapItemFromSvg(":arrowLine.svg", this);
	nextArrow = (NavArrow*)UIManager->pixmapItemFromSvg(":arrowLine.svg", this);
	//rotate la flèche next
	nextArrow->setRotation(180);
	//centre les 2 items
	layout.centerInScreen(nextArrow);
	layout.centerInScreen(backArrow);

	backArrow->moveBy(-643, -75);
	nextArrow->moveBy(643 + (56 * qApp->devicePixelRatio()), 67);

	backArrowOriginalPos = QPoint(backArrow->pos().x(), backArrow->pos().y());
	nextArrowOriginalPos = QPoint(nextArrow->pos().x(), nextArrow->pos().y());

	//////////////////////////////////////////////////
	QPointF textCenter;
	textCenter = QPointF(backArrow->pos().x(), backTransRect2.y() + (backTransRect2.height() / 2) - (40 / 7));
	drawText(*painter, textCenter, Qt::AlignVCenter | Qt::AlignHCenter, "«o»");





	float cardWidth = 406;
	float cardHeight = 466;
	float cardSmallScale = 0.8;

	for (int i = 0; i < manager->getPresetArray().count(); i++) {
		CardItem *item = new CardItem(0, 0, cardWidth, cardHeight, "", "");

		//ajoute la carte à la scene
		this->addItem(item);

		//si c'est la carte du milieu, la sélectionner
		if (i % 2 == 1){
			item->setSelectedStyle(true);
		}
		else{
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

		if (i < currentSelection - 1) {
			//cache à gauche
			UIManager->animateCard(allCards.at(i), QPoint(cardPos[0].x, cardPos[0].y), false, false, ANIMATION_TIME_MS);
		}
		else if (i == currentSelection - 1){
			//place à gauche
			UIManager->animateCard(allCards.at(i), QPoint(cardPos[1].x, cardPos[1].y), false, true, ANIMATION_TIME_MS);
		}
		else if (i == currentSelection){
			//place au milieu
			UIManager->animateCard(allCards.at(i), QPoint(cardPos[2].x, cardPos[2].y), true, true, ANIMATION_TIME_MS);
		}
		else if (i == currentSelection + 1){
			//place à droite
			UIManager->animateCard(allCards.at(i), QPoint(cardPos[3].x, cardPos[3].y), false, true, ANIMATION_TIME_MS);
		}
		else if (i > currentSelection + 1){
			//cache à droite
			UIManager->animateCard(allCards.at(i), QPoint(cardPos[4].x, cardPos[4].y), false, false, ANIMATION_TIME_MS);
		}
	}
    
    refreshBackground();
    
	//rafraichis et configure les cartes
	//refreshCurrentCards();

	//démarre le QTimer pour les Philips Hue
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateHue()));
	timer->start(1000);

}
MainScene::~MainScene()
{

}

#pragma mark - Audio

void MainScene::loadSongs(){

	//qDebug() << "HERE";

	//initialise player et playlist
	player = new QMediaPlayer;
	playlist = new QMediaPlaylist(player);

	//loop
	playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

	//navigue au bon dossier
	QDir dir = QDir(QCoreApplication::applicationDirPath());
#if !TARGET_OS_IPHONE
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
#endif

	//prend chaque musique pour chaque preset
	for (int i = 0; i < manager->getPresetArray().count(); i++) {

#ifdef __APPLE__
		playlist->addMedia(QUrl::fromLocalFile(dir.path() + "/" +manager->getPresetArray().at(i).musicPath.c_str()));
		//playlist->addMedia(QUrl::fromLocalFile(manager->getPresetArray().at(i).musicPath.c_str()));

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

void MainScene::refreshBackground(){
    /*
	//affichage de l'arrière plan
	imageObject = QImage();
	imageObject.load(manager->getPresetArray().at(currentSelection).imgPath.c_str());


	//extern QImage srcImg;//source image
	//QPixmap *pxDst( imageObject->size() );//blurred destination
	image = QPixmap(imageObject.size()*qApp->devicePixelRatio());
	image.fill(Qt::transparent);
	{
		QPainter painter(&image);
		qt_blurImage(&painter, imageObject, 200 * ((float)image.height()*(float)image.width() / 1000000) / qApp->devicePixelRatio(), false, false);//blur radius: 2px
	}
     */
    
	//this->removeItem(background);

	//if (background)
	//	delete background;

	//background = nullptr;
	if(background == NULL)
        background = this->addPixmap(*allCards.at(currentSelection)->getBlurredBackground());
    else
        background->setPixmap(*allCards.at(currentSelection)->getBlurredBackground());

	//#if TARGET_OS_IPHONE
	background->setOpacity(0.7);
	//#endif
	//else
	background->setPixmap(*allCards.at(currentSelection)->getBlurredBackground());

	//le met en arrière plan
	background->setZValue(-1);
	background->setScale(UIManager->getFullScreenPixelRatioForImage(&*allCards.at(currentSelection)->getBlurredBackground())*qApp->devicePixelRatio());
	//image.setDevicePixelRatio(UIManager->getFullScreenPixelRatioForImage(&image)*3);
	//aspect fill l'écran

	//rend l'arrière plan flou et cool
	//UIManager->blurBackgroundItem(background, &image);

#if TARGET_OS_IPHONE
	//return;
#endif
    
    //change de musique et la joue
    if (ENABLE_SOUND){
        playlist->setCurrentIndex(currentSelection);
		player->play();
    }

}

void MainScene::refreshCurrentCards(){

	//get the new positions
	//cache à gauche
	int lastPosition = manager->getPresetArray().count() - 1;
	
	//cache à gauche
	if (currentSelection == 0)
		UIManager->animateCard(allCards.at(lastPosition - 1), QPoint(cardPos[0].x, cardPos[0].y), false, false, ANIMATION_TIME_MS);
	else if (currentSelection - 2 == -1)
		UIManager->animateCard(allCards.at(lastPosition), QPoint(cardPos[0].x, cardPos[0].y), false, false, ANIMATION_TIME_MS);
	else
		UIManager->animateCard(allCards.at(currentSelection - 2), QPoint(cardPos[0].x, cardPos[0].y), false, false, ANIMATION_TIME_MS);
    
	//place à gauche
	if (currentSelection == 0)
		UIManager->animateCard(allCards.at(lastPosition), QPoint(cardPos[1].x, cardPos[1].y), false, true, ANIMATION_TIME_MS);
	else
		UIManager->animateCard(allCards.at(currentSelection - 1), QPoint(cardPos[1].x, cardPos[1].y), false, true, ANIMATION_TIME_MS);

	//place au milieu
	UIManager->animateCard(allCards.at(currentSelection), QPoint(cardPos[2].x, cardPos[2].y), true, true, ANIMATION_TIME_MS);

	//place à droite
	if (currentSelection + 1 == lastPosition + 1)
		UIManager->animateCard(allCards.at(0), QPoint(cardPos[3].x, cardPos[3].y), false, true, ANIMATION_TIME_MS);
	else
		UIManager->animateCard(allCards.at(currentSelection + 1), QPoint(cardPos[3].x, cardPos[3].y), false, true, ANIMATION_TIME_MS);

	//cache à droite
	if (currentSelection + 2 == lastPosition + 2)
		UIManager->animateCard(allCards.at(1), QPoint(cardPos[4].x, cardPos[4].y), false, false, ANIMATION_TIME_MS);
	else if (currentSelection + 2 == lastPosition + 1)
		UIManager->animateCard(allCards.at(0), QPoint(cardPos[4].x, cardPos[4].y), false, false, ANIMATION_TIME_MS);
	else
		UIManager->animateCard(allCards.at(currentSelection + 2), QPoint(cardPos[4].x, cardPos[4].y), false, false, ANIMATION_TIME_MS);
    
    
	refreshBackground();
}

//navigue par en arrière si possible
void MainScene::navBack(){
    
    if (!allCards.at(currentSelection)->getInSettingsView()){
        if (currentSelection == 0)
        {
            currentSelection = allCards.length() - 1;
        }
        else
            currentSelection--;
        
    /*
     if(!allCards.at(currentSelection)->getInSettingsView()){
       if(currentSelection-1 >= 0){
            currentSelection--;
     */
           
    //#if TARGET_OS_IPHONE
           //do nothing
   // #else
           //change de musique et la joue
           
           if(ENABLE_SOUND){
               playlist->setCurrentIndex(currentSelection);
               player->play();
           }
   // #endif
           
           //et rafraichis les cartes
            refreshCurrentCards();
           sendCurrentColorToServer();
       }
    else{
        //on est en settings view, fuck off
        //go down
        CardItem *thisCard = allCards.at(currentSelection);
        thisCard->changeColorSetting(false);
    }
    
}
void MainScene::navForward(){
	if (!allCards.at(currentSelection)->getInSettingsView()){
		if (currentSelection + 1 == manager->getPresetArray().count())
			currentSelection = 0;
		else
			currentSelection++;

//#if TARGET_OS_IPHONE
		//do nothing
//#else
		//change de musique et la joue
        if(ENABLE_SOUND){
            playlist->setCurrentIndex(currentSelection);
            player->play();
        }
//#endif
		refreshCurrentCards();
		sendCurrentColorToServer();
	}
	else{
		//on est en settings view, fuck off
		//go up
		CardItem *thisCard = allCards.at(currentSelection);
		thisCard->changeColorSetting(true);

	}
}

void MainScene::navSelect(){
	CardItem *currentCard = allCards.at(currentSelection);

	/*
	QPropertyAnimation *opacityAnimation2 = new QPropertyAnimation((QGraphicsObject*)nextArrow, "pos");
	opacityAnimation2->setDuration(ANIMATION_TIME_MS);
	opacityAnimation2->setStartValue(nextArrow->pos());
	opacityAnimation1->setEndValue(currentCard->getInSettingsView()?nextArrowOriginalPos:QPointF(nextArrowOriginalPos.rx()+100, nextArrowOriginalPos.ry()));
	opacityAnimation2->start();
	*/

	Q_FOREACH(CardItem *item, allCards){
		if (item != currentCard){
			//anime l'opacité à 0.6
			QPropertyAnimation *opacityAnimation = new QPropertyAnimation((QGraphicsObject*)item, "opacity");
			opacityAnimation->setDuration(ANIMATION_TIME_MS);
			opacityAnimation->setStartValue(item->opacity());
			opacityAnimation->setEndValue(currentCard->getInSettingsView() ? 1.0 : 0.4);
			opacityAnimation->start();
		}
	}

	//initialise l'animation d'échelle
	QPropertyAnimation *scaleAnimation = new QPropertyAnimation((QGraphicsObject*)currentCard, "scale");
	scaleAnimation->setDuration(ANIMATION_TIME_MS);
	scaleAnimation->setStartValue(currentCard->scale());

	//si yé pas deja dans la vue settings...
	if (!currentCard->getInSettingsView()){
		currentCard->setInSettingsView(true);
		scaleAnimation->setEndValue(1.1);
		scaleAnimation->setEasingCurve(QEasingCurve::OutBack);
		backArrow->setOpacity(0.3);
		nextArrow->setOpacity(0.3);
	}
	else{
		currentCard->setInSettingsView(false);
		scaleAnimation->setEndValue(1.0);
		backArrow->setOpacity(1.0);
		nextArrow->setOpacity(1.0);

	}


	/*
	QPropertyAnimation *opacityAnimation1 = new QPropertyAnimation((QGraphicsObject*)backArrow, "scale");
	opacityAnimation1->setDuration(ANIMATION_TIME_MS);
	opacityAnimation1->setStartValue(1.1);
	opacityAnimation1->setEndValue(0.1);
	opacityAnimation1->start();
	*/

	scaleAnimation->start();

	/*
	back
	QPropertyAnimation animate = new QPropertyAnimation(backArrow, "windowOpacity");

	animate.setDuration(100); animate.setStartValue(1); animate.setEndValue(0); animate.start();
	*/
}

void MainScene::showSettingsForCurrentCard(){
	//allCards.at(currentSelection)->
}
void MainScene::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "KEY ID:" << event->key();
    
    //arrière
    if (event->key() == 16777234) {
        navSendR();
    }
    //en bas
    if (event->key() == 16777237) {
        navSendM();
    }
    //avant
    if (event->key() == 16777236) {
        navSendL();
    }
}
void MainScene::navSendR(){
    m_webSocket->sendTextMessage("!r");
    rcount++;
    navBack();
}
void MainScene::navSendL(){
    m_webSocket->sendTextMessage("!l");
    lcount++;
    navForward();
}
void MainScene::navSendM(){
    m_webSocket->sendTextMessage("!m");
    mcount++;
    navSelect();
}
#pragma mark - WebSocket

void MainScene::sendColorToServer(string hexColor){

	if (m_webSocket->state() == QAbstractSocket::ConnectedState){
		m_webSocket->sendTextMessage(QString(hexColor.c_str()));
	}
}

void MainScene::sendCurrentColorToServer()
{

	//envoye les couleurs du thème au serveur
	string str = (manager->getPresetArray().at(currentSelection).color1) + "," +
		(manager->getPresetArray().at(currentSelection).color2) + "," +
		(manager->getPresetArray().at(currentSelection).color3) + "," +
		(manager->getPresetArray().at(currentSelection).color4);
	sendColorToServer(str);

	sendColorToPhilipsHue(1, (manager->getPresetArray().at(currentSelection).color3).c_str(), 3);
	sendColorToPhilipsHue(2, (manager->getPresetArray().at(currentSelection).color4).c_str(), 3);
	sendColorToPhilipsHue(3, (manager->getPresetArray().at(currentSelection).color1).c_str(), 3);
	sendColorToPhilipsHue(4, (manager->getPresetArray().at(currentSelection).color2).c_str(), 3);

}

void MainScene::wsMessageReceived(QString text){

	//vérifie le premier caractère pour savoir si ce qui a été reçu est une commande de navigation
	if (text.at(0) == '!'){
		qDebug() << text;
		switch (text.toStdString().c_str()[1]) {

		case 'r':
			if (rcount == 0)
				navBack();
			else
				rcount--;
			break;

		case 'l':
			if (lcount == 0)
				navForward();
			else
				lcount--;
			break;

		case 'm':
			if (mcount == 0)
				navSelect();
			else
				mcount--;
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

void MainScene::updateHue()
{
	qDebug() << (colorCycle % 2 ? "tic" : "toc");
	//colorCycle de 0 à 3 constamment
	colorCycle++;
	if (colorCycle == 4) {
		colorCycle = 0;
	}

	//se fait caller à chaque seconde
	CardItem *thisCard = allCards.at(currentSelection);
	const Preset *thisPreset = &manager->getPresetArray().at(currentSelection);

	switch (thisCard->getColorSetting()) {
	case 1:
            timer->setInterval(1000);
		//smooth
		sendColorToPhilipsHue(((colorCycle + 0) % 4) + 1, (thisPreset->color1).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((colorCycle + 1) % 4) + 1, (thisPreset->color2).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((colorCycle + 2) % 4) + 1, (thisPreset->color3).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((colorCycle + 3) % 4) + 1, (thisPreset->color4).c_str(), LIGHTS_ANIMATION_TIME);

		break;
	case 2:
		//hard
            
            
            sendColorToPhilipsHue(((colorCycle + 0) % 4) + 1, (thisPreset->color1).c_str(), 0);
            sendColorToPhilipsHue(((colorCycle + 1) % 4) + 1, (thisPreset->color2).c_str(), 0);
            sendColorToPhilipsHue(((colorCycle + 2) % 4) + 1, (thisPreset->color3).c_str(), 0);
           // sendColorToPhilipsHue(((colorCycle + 3) % 4) + 1, (thisPreset->color4).c_str(), LIGHTS_ANIMATION_TIME);
            /*
		sendColorToPhilipsHue(((colorCycle + 0) % 4) + 1, (thisPreset->color1).c_str(), 0);
		sendColorToPhilipsHue(((colorCycle + 1) % 4) + 1, (thisPreset->color2).c_str(), 0);
		sendColorToPhilipsHue(((colorCycle + 2) % 4) + 1, (thisPreset->color3).c_str(), 0);
		sendColorToPhilipsHue(((colorCycle + 3) % 4) + 1, (thisPreset->color4).c_str(), 0);
*/

		break;
	case 3:
		//single color
		sendColorToPhilipsHue(((0) % 4) + 1, (thisPreset->color1).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((1) % 4) + 1, (thisPreset->color2).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((2) % 4) + 1, (thisPreset->color3).c_str(), LIGHTS_ANIMATION_TIME);
		sendColorToPhilipsHue(((3) % 4) + 1, (thisPreset->color4).c_str(), LIGHTS_ANIMATION_TIME);

		break;
	default:
		break;
	}

	thisCard = nullptr;
	thisPreset = nullptr;

}
void MainScene::sendColorToPhilipsHue(int lightNumber, const char* color, int transitionTime){

	QColor couleur(color);
	couleur.hue();
	couleur.lightness();

	//le temps de transition est multiplié par 100ms, donc 7 = 700ms;

	//prend la couleur en HSB
	//HsbColor colorHSB =  color->getHSB();

	//monte la saturation de 25% pour simuler l'effet gamma d'un écran pour que les couleurs se ressemblent (écran et lumières)
	float moreSat = (couleur.saturationF() * 1.25) > 1 ? 1 : couleur.saturationF() * 1.25;

	string body = "{ \"on\": true, \"hue\":" + to_string((int)(couleur.hueF() * 65535)) + ", \"sat\":" + to_string((int)(moreSat * 255)) + ", \"bri\":" + to_string((int)(couleur.lightnessF() * 255)) + ", \"transitiontime\":" + to_string(transitionTime) + "}";
	string URL = "http://" + PHILIPS_HUE_URL + ":" + to_string(PHILIPS_HUE_PORT) + "/api/" + PHILIPS_HUE_USERNAME + "/lights/" + to_string(lightNumber) + "/state";

    //request = new QNetworkRequest(QUrl(URL.c_str()));
	//request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	netManager->put(QNetworkRequest(QUrl(URL.c_str())), body.c_str());

}

