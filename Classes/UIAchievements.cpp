//
//  UIAchievements.cpp
//  simulplay
//
//  Created by Guillem Laborda on 30/1/15.
//
//

#include "UIAchievements.h"
#include "LocalizedString.h"
#include "GameData.h"

#include <audio/include/SimpleAudioEngine.h>

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
    
    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
                                 visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);
    
    auto title = Sprite::create("MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
                            (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(title, 5, 0);
    
    auto planet2 = Sprite::create("MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
                              (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(planet2, 1, 2);
    
    auto popupBackground = Sprite::create("AchievementsBackground.png");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setAnchorPoint(Vec2(0, 0.5));
    popupBackground->setPosition(Vec2((2 * visibleSize.width / 25),
                                      (8 * visibleSize.height / 18)));
    this->addChild(popupBackground, 6);
    
    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create("ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png",
                                            CC_CALLBACK_1(UIAchievements::menuBackCallback, this));
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setPosition(Vec2((2 * visibleSize.width / 25)  ,
                                 (2 * visibleSize.height / 25)  ));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);
    
    auto configLabel = Label::createWithTTF(LocalizedString::create("CONFIGURATION")->getCString(), "fonts/BebasNeue.otf", 100);
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(11 * popupBackground->getContentSize().width / 28, 13 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);

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
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}