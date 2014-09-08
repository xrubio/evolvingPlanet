//
//  UIGoals.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIGoals.h"
#include "UIAgents.h"
#include "GameData.h"
#include "LocalizedString.h"

Scene* UIGoals::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGoals::create();
    scene->addChild(layer);
    return scene;
}

bool UIGoals::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Label* title = Label::createWithSystemFont(LocalizedString::create("GOALS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    Vector<cocos2d::MenuItem*> menuButtons;

    //Mirar si s'ha iniciat una partida amb un boolea al domini que indiqui si hi ha una partida començada
    //obtenir valor del domini (equivalent a gameStarted, esborrar de la vista) (false el primer cop), aleshores,
    //desde la pantalla d'agents (primer cop), modificar el valor a cert, per indicar que s'ha començat

    MenuItem* nextButton;
    if (GameData::getInstance()->getGameStarted()) {
        nextButton = MenuItemImage::create(
            "MapButton.png", "MapButtonPressed.png", CC_CALLBACK_1(UIGoals::menuMapCallback, this));

    } else {
        nextButton = MenuItemImage::create(
            "NextButton.png", "NextButtonPressed.png", CC_CALLBACK_1(UIGoals::menuNextCallback, this));
    }
    menuButtons.pushBack(nextButton);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (nextButton->getContentSize().width / 2),
                           origin.y + (nextButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

    setLevelGoals();

    return true;
}

void UIGoals::menuNextCallback(Ref* pSender)
{
    auto scene = UIAgents::createScene();
    auto transition = TransitionMoveInR::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGoals::menuMapCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeed(GameLevel::getInstance()->getTimeSpeedBeforePause());
    Director::getInstance()->popScene();
}

void UIGoals::setLevelGoals(void)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* goalMap = Sprite::create("GameLevelMapBackground.png");
    goalMap->setScale(0.5);
    goalMap->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2));
    this->addChild(goalMap, 1);

    //LocalizedString::create("GOAL_TEXT_LVL1")->getCString()
    auto goal1 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_1")->getCString(),
                                                        Size(visibleSize.width / 3, visibleSize.height / 4),
                                                        TextHAlignment::CENTER, "Arial", 40);
    auto goal2 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_2")->getCString(),
                                                        Size(visibleSize.width / 3, visibleSize.height / 4),
                                                        TextHAlignment::CENTER, "Arial", 40);
    auto goal3 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_3")->getCString(),
                                                        Size(visibleSize.width / 3, visibleSize.height / 4),
                                                        TextHAlignment::CENTER, "Arial", 40);

    goal1->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (5 * visibleSize.height / 8)));
    goal2->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (4 * visibleSize.height / 8)));
    goal3->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (3 * visibleSize.height / 8)));

    this->addChild(goal1, 1);
    this->addChild(goal2, 1);
    this->addChild(goal3, 1);
}
