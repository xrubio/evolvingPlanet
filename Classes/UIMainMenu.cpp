//
//  UIMainMenu.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIMainMenu.h"
#include "UICredits.h"
#include "UIProgressMap.h"

USING_NS_CC;

Scene* UIMainMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = UIMainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool UIMainMenu::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Label* title = Label::createWithSystemFont("SimulPlay", "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 5))));
    this->addChild(title, 1);

    Vector<cocos2d::MenuItem*> menuButtons;

    MenuItem* startButton = MenuItemImage::create(
        "StartButton.png", "StartButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuStartCallback, this));
    startButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4 * (visibleSize.height / 8)));
    menuButtons.pushBack(startButton);

    MenuItem* languageButton = MenuItemImage::create(
        "CreditsButton.png", "CreditsButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuCreditsCallback, this));
    languageButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 3 * (visibleSize.height / 8)));
    menuButtons.pushBack(languageButton);

    MenuItem* creditsButton = MenuItemImage::create(
        "CreditsButton.png", "CreditsButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuCreditsCallback, this));
    creditsButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * (visibleSize.height / 8)));
    menuButtons.pushBack(creditsButton);

    MenuItem* exitButton = MenuItemImage::create(
        "ExitButton.png", "ExitButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuExitCallback, this));
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 8)));
    menuButtons.pushBack(exitButton);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);

    return true;
}

void UIMainMenu::menuStartCallback(Ref* pSender)
{
    auto scene = UIProgressMap::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIMainMenu::menuCreditsCallback(Ref* pSender)
{
    auto scene = UICredits::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIMainMenu::menuExitCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
