#include "HelloWorldScene.h"
#include <iostream>

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

	closeItem->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- closeItem->getContentSize().width / 2,
					origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = LabelTTF::create("Hello World", "Arial", 24);

	// position the label on the center of the screen
	label->setPosition(
			Vec2(
					origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							- label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	worldMap = Sprite::create("WorldMap.png");

	// scale the image to screen size ratio
	float scaleXFactor = 1.5;
	float scaleYFactor = 1.5;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	worldMap->setScaleX(
			(winSize.width / worldMap->getContentSize().width) * scaleXFactor);
	worldMap->setScaleY(
			(winSize.height / worldMap->getContentSize().height) * scaleYFactor);

	// position the sprite on the center of the screen
	worldMap->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));

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

	if (worldMap->getBoundingBox().size.width == CCDirector::sharedDirector()->getWinSize().width &&
		worldMap->getBoundingBox().size.height == CCDirector::sharedDirector()->getWinSize().height)
	{
		CCLOG("NOT ZOOMED");
	}
	else
	{

	}
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches,
		cocos2d::Event *event)
{
	CCLOG("ON TOUCHES MOVED: ");

	if (worldMap->getBoundingBox().size.width == CCDirector::sharedDirector()->getWinSize().width &&
		worldMap->getBoundingBox().size.height == CCDirector::sharedDirector()->getWinSize().height)
	{
		CCLOG("NOT ZOOMED");
	}
	else
	{
		for (auto touch : touches)
		{
				auto action = MoveTo::create(0.3,Point((worldMap->getPositionX()+
											(touch->getLocationInView().x - touch->getPreviousLocationInView().x)),
											(worldMap->getPositionY()-
											(touch->getLocationInView().y - touch->getPreviousLocationInView().y))));
				worldMap->runAction(action);

		}
	}
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches,
		cocos2d::Event *event)
{
	CCLOG("ON TOUCHES ENDED: ");

	if (worldMap->getBoundingBox().size.width <= CCDirector::sharedDirector()->getWinSize().width &&
		worldMap->getBoundingBox().size.height <= CCDirector::sharedDirector()->getWinSize().height)
	{
		CCLOG("NOT ZOOMED");
	}
	else
	{
		/*CCPoint location = touches[0]->getLocationInView();
		location = CCDirector::sharedDirector()->convertToGL(location);

		//auto action = MoveTo::create(1, Point(location.x,location.y));
		worldMap->setPosition(location.x, location.y);*/
	}

}

