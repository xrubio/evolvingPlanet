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

    auto title = Label::createWithSystemFont(LocalizedString::create("GOALS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 8))));
    //this->addChild(title, 1);

    MenuItem* nextButton;
    if (GameData::getInstance()->getGameStarted()) {
        nextButton = MenuItemImage::create(
            "MapButton.png", "MapButtonPressed.png", CC_CALLBACK_1(UIGoals::menuMapCallback, this));

    } else {
        nextButton = MenuItemImage::create(
            "NextButton.png", "NextButtonPressed.png", CC_CALLBACK_1(UIGoals::menuNextCallback, this));
    }

    auto menu = cocos2d::Menu::createWithItem(nextButton);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (nextButton->getContentSize().width / 2),
                           origin.y + (nextButton->getContentSize().height / 2)));
    //this->addChild(menu, 1);

    auto pageView = PageView::create();
    pageView->setTouchEnabled(true);
    pageView->setSize(Size(visibleSize.width, visibleSize.height));
    pageView->setPosition(Point(0, 0));

    auto layout = Layout::create();
    layout->setSize(Size(visibleSize.width, visibleSize.height));

    layout->addChild(title);
    //layout->addChild(menu);
    setLevelGoals(layout);
    pageView->addPage(layout);

    for (int i = 0; i < GameLevel::getInstance()->getNumInitialAgents().size(); i++) {
        GameLevel::getInstance()->setCurrentAgentType(i);
        auto scene = UIAgents::createScene();
        auto layout2 = Layout::create();
        layout2->setSize(Size(visibleSize.width, visibleSize.height));
        layout2->addChild(scene);
        pageView->addPage(layout2);
    }

    pageView->setTag(10);
    this->addChild(pageView);

    this->scheduleUpdate();

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

void UIGoals::setLevelGoals(Layout* layout)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto goalMap = Sprite::create(GameLevel::getInstance()->getMapFilename() + "Background" + ".png");
    goalMap->setScale(0.5);
    goalMap->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2));
    layout->addChild(goalMap, 1);

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

    layout->addChild(goal1, 1);
    layout->addChild(goal2, 1);
    layout->addChild(goal3, 1);
}

void UIGoals::update(float delta)
{
    PageView* p = (PageView*)this->getChildByTag(10);
    GameLevel::getInstance()->setCurrentAgentType(p->getCurPageIndex() - 1);
}
