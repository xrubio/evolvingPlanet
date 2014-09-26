//
//  UIMainMenu.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIMainMenu.h"
#include "UICredits.h"
#include "UIConfiguration.h"
#include "UIProgressMap.h"
#include "LocalizedString.h"

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
        "MainButtonBackground.png", "MainButtonBackgroundPressed.png", CC_CALLBACK_1(UIMainMenu::menuStartCallback, this));
    startButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4 * (visibleSize.height / 8)));

    Sprite* strokeSprite = Sprite::create("MainButtonBackground.png");
    strokeSprite->setColor(Color3B::RED);
    strokeSprite->setPosition(startButton->getContentSize().width / 2, startButton->getContentSize().height / 2);
    strokeSprite->setScale(1.1, 1.2);
    startButton->addChild(strokeSprite, -1);

    auto lbl = LabelTTF::create(LocalizedString::create("START")->getCString(), "Calibri", 50);
    // amb Label*   lbl->enableShadow();
    lbl->enableShadow(Size(2, 2), 60, 40);
    lbl->enableStroke(Color3B::RED, 2);
    MenuItem* startLabel = MenuItemLabel::create(lbl);
    startLabel->setPosition(startButton->getContentSize().width / 2, startButton->getContentSize().height / 2);
    /*Vector<Node*> words = startLabel->getChildren();
    for (int i = 0; i < words.size(); i++) {
        Sprite* s = (Sprite*)words.at(i);
    }*/
    startButton->addChild(startLabel);
    menuButtons.pushBack(startButton);

    MenuItem* configurationButton = MenuItemImage::create(
        "ConfigurationButton.png", "ConfigurationButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuConfigurationCallback, this));
    configurationButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 3 * (visibleSize.height / 8)));
    menuButtons.pushBack(configurationButton);

    MenuItem* creditsButton = MenuItemImage::create(
        "CreditsButton.png", "CreditsButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuCreditsCallback, this));
    creditsButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * (visibleSize.height / 8)));
    menuButtons.pushBack(creditsButton);

    /*MenuItem* exitButton = MenuItemImage::create(
        "ExitButton.png", "ExitButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuExitCallback, this));
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 8)));
    menuButtons.pushBack(exitButton);*/

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

void UIMainMenu::menuConfigurationCallback(Ref* pSender)
{
    auto scene = UIConfiguration::createScene();
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