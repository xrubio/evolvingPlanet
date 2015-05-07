//
//  UICredits.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UICredits.h"
#include "LocalizedString.h"

Scene* UICredits::createScene()
{
    auto scene = Scene::create();
    auto layer = UICredits::create();
    scene->addChild(layer);
    return scene;
}

bool UICredits::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithSystemFont(LocalizedString::create("CREDITS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UICredits::menuBackCallback, this));

    auto menu = cocos2d::Menu::createWithItem(backButton);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
        origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

    return true;
}

void UICredits::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}