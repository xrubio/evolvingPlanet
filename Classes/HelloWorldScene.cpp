#include "HelloWorldScene.h"
#include <iostream>
#include <fstream>

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
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
    
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
                                origin.y + closeItem->getContentSize().height / 2));
    
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
    
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
	worldMap = Sprite::create("WorldMapIpad.png");
    
	// position the sprite on the center of the screen
	worldMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
	// add the sprite as a child to this layer
	this->addChild(worldMap, 0);
    
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    setBackgroundSize();
    
	return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif
    
	Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event)
{
	CCLOG("ON TOUCHES BEGAN");

    continentSelection(touches.at(0));
    for (auto touch : touches)
    {
        _touches.push_back(touch);
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
	CCLOG("ON TOUCHES MOVED: ");
    
    //ZOOM
    if (_touches.size() > 1)
    {
        CCLOG("ZOOM");
        for (auto touch : touches) {
            pinchZoomWithMovedTouch(touch);
        }
        CCLOG("%f",zoomScale);
        worldMap->setScale(zoomScale);
        
        Point reLocate = worldMap->getPosition();
        checkBackgroundLimitsInTheScreen(reLocate);
        while (!moveBackgroundLeft)
        {
            CCLOG("Left");
            reLocate.x -= 0.2;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundRight)
        {
            CCLOG("Right");
            reLocate.x += 0.2;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundUp)
        {
            CCLOG("Up");
            reLocate.y += 0.2;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundDown)
        {
            CCLOG("Down");
            reLocate.y -= 0.2;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        
        worldMap->setPosition(reLocate);
    }
    //PAN
    else if (_touches.size() == 1)
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
            
            cocos2d::Point destPost = worldMap->getPosition();
            if (moveBackgroundLeft and ccpSub(touchLocation, oldTouchLocation).x > 0)
            {
                destPost.x = newPos.x;
            }
            if (moveBackgroundRight and ccpSub(touchLocation, oldTouchLocation).x < 0)
            {
                destPost.x = newPos.x;
            }
            if (moveBackgroundUp and ccpSub(touchLocation, oldTouchLocation).y < 0)
            {
                destPost.y = newPos.y;
            }
            if (moveBackgroundDown and ccpSub(touchLocation, oldTouchLocation).y > 0)
            {
                destPost.y = newPos.y;
            }
            
            worldMap->cocos2d::Node::setPosition(destPost);
        }
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches,
                                cocos2d::Event *event)
{
	CCLOG("ON TOUCHES ENDED: ");
    
    for (auto touch : touches)
    {
        _touches.pop_back();
    }
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
    CCImage *img = new CCImage();
    img->initWithImageFile("WorldMapIpadR0.png");
    int x=3;
    if(img->hasAlpha()) x=4;
    unsigned char *data = new unsigned char[img->getDataLen()*x];
    data = img->getData();
    
    unsigned char *pixel = data + ((int)pt.x + (int)pt.y * img->getWidth()) * x;
    
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
