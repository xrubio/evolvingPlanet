//
//  UIAchievements.cpp
//  simulplay
//
//  Created by Guillem Laborda on 30/1/15.
//
//

#include "UIAchievements.h"
#include "LocalizedString.h"

Scene* UIAchievements::createScene()
{
    auto scene = Scene::create();
    auto layer = UIAchievements::create();
    scene->addChild(layer);
    return scene;
}

bool UIAchievements::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto title = Label::createWithSystemFont(LocalizedString::create("ACHIEVEMENTS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIAchievements::menuBackCallback, this));

    auto menu = cocos2d::Menu::createWithItem(backButton);
    menu->setPosition(Vec2(visibleSize.width - (backButton->getContentSize().width / 2),
                           (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

    ui::ListView* list = ui::ListView::create();
    list->setDirection(ui::ListView::Direction::VERTICAL);
    list->setTouchEnabled(true);
    list->setBounceEnabled(true);
    list->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    list->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));

    /*for (int i = 0; i < 15; i++) {
        auto text = TextFieldTTF::createWithTTF("TEXT", "Arial Rounded MT Bold", 40);
        ui::Widget* wid = ui::Widget::create();
        wid->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
        wid->addChild(text);
        list->pushBackCustomItem(wid);
    }*/

    return true;
}

void UIAchievements::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
}