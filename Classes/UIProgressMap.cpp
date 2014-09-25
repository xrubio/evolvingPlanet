//
//  UIProgressMap.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIProgressMap.h"
#include "UIGoals.h"
#include "GameData.h"
#include "LevelLoader.h"
#include "SlidingMenu.h"
#include "CocosGUI.h"

using namespace ui;

Scene* UIProgressMap::createScene()
{
    auto scene = Scene::create();
    auto layer = UIProgressMap::create();
    scene->addChild(layer);
    return scene;
}

bool UIProgressMap::init()
{
    if (!Layer::init()) {
        return false;
    }

    //Alliberar memòria
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //Set background progress map and all its functionalities
    Sprite* progressMap = Sprite::create("ProgressMapBackground.png");
    progressMap->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                  visibleSize.height / 2 + origin.y));
    Sprite* progressMap2 = Sprite::create("ProgressMap2Background.png");
    progressMap2->setPosition(Vec2(visibleSize.width + (visibleSize.width / 2) + origin.x,
                                  visibleSize.height / 2 + origin.y));
    //this->addChild(progressMap, 0);

    Vector<MenuItem*> menuButtons;
    MenuItem* backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    menuButtons.pushBack(backButton);

    Menu* menu = Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                           origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);
    
    
    MenuItem* level1Button = MenuItemImage::create(
                                                   "Level1Button.png", "Level1ButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevel1Callback, this));
    level1Button->setPosition(288,180);
    Vector<MenuItem*> level1Buttons;
    level1Buttons.pushBack(level1Button);
    Menu* level1 = Menu::createWithArray(level1Buttons);
    level1->setPosition(0,0);
    progressMap->addChild(level1);
    
    Size scollFrameSize = Size(visibleSize.width, visibleSize.height);
    auto scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width*2,visibleSize.height));
    scrollView->setBackGroundColor(Color3B(200, 200, 200));
    scrollView->setSize(scollFrameSize);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

    scrollView->addChild(progressMap);
    scrollView->addChild(progressMap2);
    this->addChild(scrollView);

    return true;
}

void UIProgressMap::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIProgressMap::menuLevel1Callback(Ref* pSender)
{
    GameLevel::getInstance()->resetLevel();
    GameData::getInstance()->setGameStarted(false);

    LevelLoader loader;
    loader.loadXmlFile("level0");

    auto scene = UIGoals::createScene();
    auto transition = TransitionShrinkGrow::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}