#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <time.h>

USING_NS_CC;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();
    
	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
    
    createThreadForNewLevel();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	/////////////////////////////
	// 3. add your codes below...
    
	// add a label shows "Hello World"
	// create and initialize a label
    
	label = LabelTTF::create("Hello World", "Arial", 24);
    
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
	// add the label as a child to this layer
	this->addChild(label, 1);
    
	// add "HelloWorld" splash screen"
	worldMap = Sprite::create("WorldMap.png");
    
	// position the sprite on the center of the screen
	worldMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    backgroundHotSpot = new Image();
    backgroundHotSpot->initWithImageFile("WorldMapR0.png");
    int x=3;
    if(backgroundHotSpot->hasAlpha()) x=4;
    dataBackgroundHotSpot = new unsigned char[backgroundHotSpot->getDataLen()*x];
    dataBackgroundHotSpot = backgroundHotSpot->getData();

    
	// add the sprite as a child to this layer
	this->addChild(worldMap, 0);
    
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    mario = Sprite::create("mario.png");
    mario->cocos2d::Node::setPosition(newLevel->positionPlayer.x, newLevel->positionPlayer.y);
    worldMap->addChild(mario);
    
    cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
    
    cocos2d::MenuItem* pCloseItem = cocos2d::MenuItemImage::create(
                                                                   "CloseNormal.png",
                                                                   "CloseSelected.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::menuCloseCallback));
    pCloseItem->setScale(3);
    pMenuItems.pushBack(pCloseItem); // add close item to vector
    cocos2d::MenuItem* pPauseItem = cocos2d::MenuItemImage::create(
                                                                   "PauseNormal.png",
                                                                   "PauseSelected.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::menuPauseCallback));
    
    pPauseItem->setPosition(pCloseItem->getPositionX()-120*3, pCloseItem->getPositionY());
    pPauseItem->setScale(3);
    pMenuItems.pushBack(pPauseItem); // add pause item to vector
    
    cocos2d::MenuItem* pPlayItem = cocos2d::MenuItemImage::create(
                                                                   "PlayNormal.png",
                                                                   "PlaySelected.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::menuPlayCallback));
    
    pPlayItem->setPosition(pCloseItem->getPositionX()-80*3, pCloseItem->getPositionY());
    pPlayItem->setScale(3);
    pMenuItems.pushBack(pPlayItem); // add pause item to vector
    
    cocos2d::MenuItem* pFastForwardItem = cocos2d::MenuItemImage::create(
                                                                  "FastForwardNormal.png",
                                                                  "FastForwardSelected.png",
                                                                  this,
                                                                  menu_selector(HelloWorld::menuFastForwardCallback));
    
    pFastForwardItem->setPosition(pCloseItem->getPositionX()-40*3, pCloseItem->getPositionY());
    pFastForwardItem->setScale(3);
    pMenuItems.pushBack(pFastForwardItem); // add pause item to vector
    
    
    cocos2d::Menu* pMenu = cocos2d::Menu::createWithArray(pMenuItems);
    pMenu->setPosition(Point(cocos2d::Director::getInstance()->getWinSize().width - pCloseItem->getContentSize().width,
                             pCloseItem->getContentSize().height*3/2));
    
    this->addChild(pMenu, 1);

    //Creat root node frome export file by scene editor.
    //cocos2d::Node *pNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("MainMenu_1.json");
    //add the node to new scene
    //this->addChild(pNode,0,1);

    setBackgroundSize();
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("scene2track.mp3", true);
    
    time(&timeTaken);
    
    //planificar l'update quan es vulgui
    this->schedule(schedule_selector(HelloWorld::update), 0.01);
    
    //quan es necessiti renderitzar, es fa update automatic
    //this->scheduleUpdate();
    
	return true;
}


void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event)
{
    for (auto touch : touches)
    {
        _touches.pushBack(touch);
    }
    if (touches.size() == 1)
    {
        firstTouchLocation = touches.at(0)->getLocation();
    }
    
    for (auto touch : touches) {
        cocos2d::Point touchLocation = this->convertTouchToNodeSpace(touch);
        if (selectSpriteForTouch(worldMap, touchLocation))
            moveBackground = true;
        else
            moveBackground = false;
    }
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event)
{
    //ZOOM
    if (touches.size() == 2)
    {
        CCLOG("ZOOM");
        for (auto touch : touches) {
            pinchZoomWithMovedTouch(touch);
        }
        CCLOG("%f",zoomScale);
        worldMap->setScale(zoomScale);

        /*cocos2d::Point touch0 = touches.at(0)->getLocation();
        cocos2d::Point touch1 = touches.at(1)->getLocation();
        cocos2d::Point pointBetweenFingers = ccpAdd(touch0,(ccpSub(touch1,touch0)/2));
        Point visible = CCDirector::sharedDirector()->getVisibleSize();
        Point currentCenter = worldMap->getPosition() - ( pointBetweenFingers - visible/2 ) ;
        worldMap->setPosition(currentCenter);
        worldMap->setScale(zoomScale);
*/
        
        Point reLocate = worldMap->getPosition();
        checkBackgroundLimitsInTheScreen(reLocate);
        while (!moveBackgroundLeft)
        {
            //CCLOG("Left");
            reLocate.x -= 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundRight)
        {
            //CCLOG("Right");
            reLocate.x += 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundUp)
        {
            //CCLOG("Up");
            reLocate.y += 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundDown)
        {
            //CCLOG("Down");
            reLocate.y -= 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        
        worldMap->setPosition(reLocate);
    }
    //PAN
    else if (touches.size() == 1)
    {
        CCLOG("PAN");
        if (moveBackground)
        {
            cocos2d::Point touchLocation = this->convertTouchToNodeSpace(touches.at(0));
            
            cocos2d::Point oldTouchLocation = touches.at(0)->getPreviousLocationInView();
            oldTouchLocation = cocos2d::Director::getInstance()->convertToGL(oldTouchLocation);
            oldTouchLocation = convertToNodeSpace(oldTouchLocation);
            
            cocos2d::Point translation = ccpSub(touchLocation, oldTouchLocation);
            cocos2d::Point newPos = ccpAdd(worldMap->getPosition(), translation);
            
            checkBackgroundLimitsInTheScreen(newPos);
            
            cocos2d::Point destPos = worldMap->getPosition();
            if (moveBackgroundLeft and translation.x > 0)
            {
                destPos.x = newPos.x;
            }
            if (moveBackgroundRight and translation.x < 0)
            {
                destPos.x = newPos.x;
            }
            if (moveBackgroundUp and translation.y < 0)
            {
                destPos.y = newPos.y;
            }
            if (moveBackgroundDown and translation.y > 0)
            {
                destPos.y = newPos.y;
            }
            
            worldMap->cocos2d::Node::setPosition(destPos);
        }
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event)
{
    if (touches.size() == 1 and firstTouchLocation.fuzzyEquals(touches.at(0)->getLocation(), 10.0))
    {
        continentSelection(touches.at(0));
    }
    _touches.clear();
}

bool HelloWorld::selectSpriteForTouch(cocos2d::Sprite *sprite, cocos2d::Point touchLocation)
{
    if (sprite->getBoundingBox().containsPoint(touchLocation))
        return true;
    else
        return false;
}

void HelloWorld::checkBackgroundLimitsInTheScreen(cocos2d::Point destPoint)
{
    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    float worldMapBoundingBoxWidth = worldMap->getBoundingBox().size.width;
    float worldMapBoundingBoxHeight = worldMap->getBoundingBox().size.height;
    
    // LEFT-RIGHT
    if ((destPoint.x - worldMapBoundingBoxWidth/2) > 0)
    {
        moveBackgroundLeft = false;
    }
    else
    {
        moveBackgroundLeft = true;
    }
    if ((destPoint.x + worldMapBoundingBoxWidth/2) < winSize.width)
    {
        moveBackgroundRight = false;
    }
    else
    {
        moveBackgroundRight = true;
    }
    // UP-DOWN
    if ((destPoint.y - worldMapBoundingBoxHeight/2) > 0)
    {
        moveBackgroundDown = false;
    }
    else
    {
        moveBackgroundDown = true;
    }
    if ((destPoint.y + worldMapBoundingBoxHeight/2) < winSize.height)
    {
        moveBackgroundUp = false;
    }
    else
    {
        moveBackgroundUp = true;
    }
    
}


int HelloWorld::getValueAtPointFromPNG(Point pt)
{
    int retValue = 255;
    int x=3;
    if(backgroundHotSpot->hasAlpha()) x=4;
    unsigned char *pixel = dataBackgroundHotSpot + ((int)pt.x + (int)pt.y * backgroundHotSpot->getWidth()) * x;
    
    // You can see/change pixels' RGBA value(0-255) here !
    unsigned char r = *pixel;

    retValue = (int)r;
    
    return retValue;
}

void HelloWorld::continentSelection(cocos2d::Touch *touch)
{
    // get a touch in the UIKit coordinate system
    cocos2d::Point loc(touch->getLocationInView());
    
    // convert the UIKit coordinate  to an openGl coordinate
    // world space (origin in lower left corner)
    loc = cocos2d::Director::getInstance()->convertToGL(loc);
    
    // node space - relative to piranha image. Without
    // this we would get the coordinate relative to the layer
    // not the piranah image. As the Piranah image is offset
    // by 100 pixels to the right and 50 pixels up the
    // hit areas would not be correct
    loc = worldMap->convertToNodeSpace(loc);
    loc.y = backgroundHeight - loc.y;
    
    int value = getValueAtPointFromPNG(loc);
    
    // update the label
    switch (value) {
        case 0:
            label->setString("AFRICA");
            break;
        default:
            label->setString("NOT AFRICA");
            break;
    }
}

void HelloWorld::pinchZoomWithMovedTouch(cocos2d::Touch *movedTouch)
{
    float minDistSqr = FLT_MAX;
    cocos2d::Touch * nearestTouch = nullptr;
    Point newLocation = movedTouch->getLocationInView();
    for (auto touch : _touches)
    {
        if (touch != movedTouch)
        {
            float distSqr = sqrOfDistanceBetweenPoints(touch->getLocationInView(),newLocation);
            if (distSqr < minDistSqr)
            {
                minDistSqr = distSqr;
                nearestTouch = touch;
            }
        }
    }
    if (nearestTouch)
    {
        float prevDistSqr = sqrOfDistanceBetweenPoints(nearestTouch->getLocationInView(),
                                                         movedTouch->getPreviousLocationInView());
        float pinchDiff = sqrtf(minDistSqr) - sqrtf(prevDistSqr);
        zoomScale += pinchDiff * 0.005; // kPinchZoomCoeff is constant = 1.0 / 200.0f Adjust it for your needs
        if (zoomScale < 1)
        {
            zoomScale = 1;
        }
        if (zoomScale > 3)
        {
            zoomScale = 3;
        }
    }
}

float HelloWorld::sqrOfDistanceBetweenPoints(Point p1, Point p2)
{
    Point diff = ccpSub(p1, p2);
    return diff.x * diff.x + diff.y * diff.y;
}

void HelloWorld::setBackgroundSize()
{
    auto screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();

    if (2048 == screenSize.width or 2048 == screenSize.height)
    {
        backgroundWidth = 2048;
        backgroundHeight = 1536;
    }
    else if (1024 == screenSize.width or 1024 == screenSize.height)
    {
        backgroundWidth = 1024;
        backgroundHeight = 768;
    }
    else if (1136 == screenSize.width or 1136 == screenSize.height)
    {
        backgroundWidth = 1136;
        backgroundHeight = 640;
    }
    else if (960 == screenSize.width or 960 == screenSize.height)
    {
        backgroundWidth = 960;
        backgroundHeight = 640;
    }
    else
    {
        if (screenSize.width > 1080)
        {
            backgroundWidth = 960;
            backgroundHeight = 640;
        }
        else
        {
            backgroundWidth = 480;
            backgroundHeight = 320;
        }
    }
}

void HelloWorld::setBackgroundWidth(int width)
{
    backgroundWidth = width;
}

void HelloWorld::setBackgroundHeight(int height)
{
    backgroundHeight = height;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuPauseCallback(Ref* pSender)
{
    newLevel->velocityMultiplier = 0;
}

void HelloWorld::menuPlayCallback(Ref* pSender)
{
    newLevel->velocityMultiplier = 1;
}

void HelloWorld::menuFastForwardCallback(Ref* pSender)
{
    newLevel->velocityMultiplier = 4;
}

void HelloWorld::update(float delta)
{
    time_t currentTime;
    double seconds;
    
    time(&currentTime);  /* get current time; same as: timer = time(NULL)  */
    
    seconds = difftime(currentTime,timeTaken);

    timeTaken = currentTime;

    auto action = MoveTo::create(0.2,cocos2d::Point(newLevel->positionPlayer.x, newLevel->positionPlayer.y));
    //+(backgroundWidth/1000*velocityMultiplier);
    //mario->runAction(action);
    mario->setPosition(newLevel->positionPlayer);
    //CCLOG("seconds: %f",seconds);
    
}

void HelloWorld::createThreadForNewLevel(void)
{
    pthread_t thread;
    newLevel = new LevelPlayDomain();
    SimpleStructure* args = new SimpleStructure();
    args->lvl = newLevel;
    args->hllwrld = this;
    pthread_create(&thread, NULL, &HelloWorld::createLevel, args);
}

void* HelloWorld::createLevel(void* arg)
{
    SimpleStructure* l = (SimpleStructure*)arg;
    l->hllwrld->playLevel(l->lvl);
    delete l;
    return nullptr;
}

void HelloWorld::playLevel(void* lvl)
{
    pthread_mutex_lock(&mutex);
    LevelPlayDomain *nwLvl = (LevelPlayDomain*)lvl;
    nwLvl->playLevel();
    pthread_mutex_unlock(&mutex);
}
