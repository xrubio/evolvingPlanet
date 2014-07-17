//
//  MainMenu.cpp
//  simulplay
//
//  Created by Guillem Laborda on 09/07/14.
//
//

#include "MainMenu.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* MainMenu::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();
    
	// 'layer' is an autorelease object
	auto layer = MainMenu::create();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	/////////////////////////////
	// 3. add your codes below...
    
	// add a label shows "Hello World"
	// create and initialize a label
    
	LabelTTF *label = LabelTTF::create("SimulPlay", "Arial", 24);
    
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
	// add the label as a child to this layer
	this->addChild(label, 1);
    
    cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
    
    cocos2d::MenuItem* pCloseItem = cocos2d::MenuItemImage::create(
                                                                   "CloseNormal.png",
                                                                   "CloseSelected.png",
                                                                   this,
                                                                   menu_selector(MainMenu::menuCloseCallback));
    pCloseItem->setScale(3);
    pMenuItems.pushBack(pCloseItem); // add close item to vector
    
    cocos2d::MenuItem* pPauseItem = cocos2d::MenuItemImage::create(
                                                                   "PauseNormal.png",
                                                                   "PauseSelected.png",
                                                                   this,
                                                                   menu_selector(MainMenu::menuPauseCallback));
    
    pPauseItem->setPosition(pCloseItem->getPositionX()-120*3, pCloseItem->getPositionY());
    pPauseItem->setScale(3);
    pMenuItems.pushBack(pPauseItem); // add pause item to vector
    
    
    cocos2d::Menu* pMenu = cocos2d::Menu::createWithArray(pMenuItems);
    pMenu->setPosition(Point(cocos2d::Director::getInstance()->getWinSize().width - pCloseItem->getContentSize().width,
                             pCloseItem->getContentSize().height/2));

    this->addChild(pMenu, 1);
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("scene1track.mp3", true);
    
	return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
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

void MainMenu::menuPauseCallback(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    CCDirector::getInstance()->replaceScene(scene);
}