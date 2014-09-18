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
    this->addChild(progressMap, 0);

    Vector<cocos2d::MenuItem*> menuButtons;

    MenuItem* backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    menuButtons.pushBack(backButton);

    MenuItem* level1Button = MenuItemImage::create(
        "Level1Button.png", "Level1ButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevel1Callback, this));
    level1Button->setPosition(backButton->getPosition().x - 1730, backButton->getPosition().y + 180);
    menuButtons.pushBack(level1Button);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                           origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

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