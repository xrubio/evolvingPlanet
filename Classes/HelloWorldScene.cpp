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

int HelloWorld::getValueAtPoint(cocos2d::Point pt)
{
    // set the value to 255, a default no hotspot selected value
    int retValue = 255;
    int imageWidth = 1024;
    int imageHeight = 768;
    cocos2d::Rect image(0, 0, imageWidth, imageHeight);
    
    CCLOG("%d x %d", imageWidth,imageHeight);
    
    Data *data = new Data(cocos2d::FileUtils::getInstance()->getDataFromFile("/Users/glaborda/Desktop/simulplay/Resources/_WorldMapIpadR0RAW.raw"));
    unsigned char *c = data->getBytes();
    
    //_20110705_piranah_hotspot
    FILE *file = fopen("/Users/glaborda/Desktop/simulplay/Resources/_WorldMapIpadR0RAW.raw", "rb");
    
    // Check if the image lookup data is present and the value is within
    // the bounds of the image
    if(image.containsPoint(pt))// and file != NULL)
    {
        // scale the image to screen size ratio (in case there is no specific assets for specific resolution)
        /*float scaleXFactor = 1;
        float scaleYFactor = 1;
        cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
        hotSpot->setScaleX((winSize.width / hotSpot->getContentSize().width) * scaleXFactor);
        hotSpot->setScaleY((winSize.height / hotSpot->getContentSize().height) * scaleYFactor);
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        // position the sprite on the center of the screen
        hotSpot->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        Color3B *buffer = new Color3B();
        glReadPixels(pt.x, pt.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        retValue = buffer[0].r;
        CCLOG("%d",buffer[0].r);*/
        
        // The raw data assumes a top left origin
        
        //pt.y = pt.y - imageHeight - 1;
        
        // calculate an offset
        int offset = pt.y * imageWidth + pt.x;
        
        //1
        retValue = (int)c[offset];
        CCLOG("%d, retv = %d",offset, retValue);
        
        //2
        /*char buff[1];
        fseek(file, offset, SEEK_SET);
        fread(buff, 1, 1, file);
        retValue = atoi(buff);
        CCLOG("%d",atoi(buff));
        CCLOG("Offset: %d, Val = %d",offset,retValue);
        
        //3
         
        char buff2[1];
        std::ifstream binaryFile;
        binaryFile.open("/Users/glaborda/Desktop/simulplay/Resources/_WorldMapIpadR0RAW.raw", std::ifstream::in|std::ifstream::binary);
        binaryFile.seekg(offset, binaryFile.beg);
        binaryFile.read(buff2, 1);
        CCLOG("%d", (int)(buff2[0]));
        retValue = atoi(buff2);
        CCLOG("Offset: %d, Val = %d",offset,retValue);*/
        
    }
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
    
    //loc.y = 768 - loc.y;
    
    //NSLog(@"touch (%g %g)",loc.x, loc.y);
    
    CCLOG("%f - %f",loc.x,loc.y);
    //1073741824 - 1081737869
    int value = getValueAtPoint(loc);
    
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
