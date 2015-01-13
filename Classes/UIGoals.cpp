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

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIGoals::menuBackCallback, this));
    menuButtons.pushBack(backButton);

    auto menuBack = Menu::createWithArray(menuButtons);
    menuBack->setPosition(Vec2(visibleSize.width - (backButton->getContentSize().width / 2),
                               visibleSize.height - backButton->getContentSize().height / 2));
    this->addChild(menuBack, 1);

    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->initializeAttributesCost();
    }
    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes(GameLevel::getInstance()->getCurrentAgentType());
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        //si el cost de l'atribut es diferent de 0, es modificable
        if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), it->first) != 0) {
            keys.push_back(it->first);
        }
        i++;
    }
    /*MenuItem* nextButton;
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
    this->addChild(menu, 1);*/

    pages = PageView::create();
    pages->setBackGroundImage(GameLevel::getInstance()->getMapFilename() + "Background" + ".png");
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));

    /*auto layoutContext = Layout::create();
    layoutContext->setSize(Size(visibleSize.width, visibleSize.height));
    auto context = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("CONTEXT_LEVEL_1")->getCString(),
                                                          Size(visibleSize.width - 200, visibleSize.height),
                                                          TextHAlignment::LEFT, "Arial", 50);
    context->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    context->setColor(Color3B::WHITE);
    layoutContext->addChild(context);
    pageView->addPage(layoutContext);*/

    auto layout = Layout::create();
    layout->setSize(Size(visibleSize.width, visibleSize.height));

    layout->addChild(title);
    //layout->addChild(menu);
    setLevelGoals(layout);
    pages->addPage(layout);

    for (int i = 0; i < GameLevel::getInstance()->getNumInitialAgents().size(); i++) {
        GameLevel::getInstance()->setCurrentAgentType(i);
        //auto scene = UIAgents::createScene();
        auto layout2 = Layout::create();
        layout2->setSize(Size(visibleSize.width, visibleSize.height));
        //layout2->addChild(scene);
        pages->addPage(layout2);
        createUIAgent(layout2);
    }

    pages->setTag(0);
    this->addChild(pages);

    this->scheduleUpdate();

    return true;
}

void UIGoals::menuNextCallback(Ref* pSender)
{
    auto scene = UIAgents::createScene();
    auto transition = TransitionMoveInR::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGoals::menuBackCallback(Ref* pSender)
{
    GameData::getInstance()->setGameStarted(false);
    auto scene = UIProgressMap::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIGoals::menuMapCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeed(GameLevel::getInstance()->getTimeSpeedBeforePause());
    Director::getInstance()->popScene();
}

//Faig dues funcions per si el primer cop que es comença la partida s'ha d'intercalar una pantalla de càrrega
void UIGoals::menuStartCallback(Ref* pSender)
{
    /*if (GameData::getInstance()->getGameStarted() == false) {
     GameLevel::getInstance()->createLevel(0);
     }
     GameData::getInstance()->setGameStarted(true);*/
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGoals::minusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 10;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) > 0) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1000);
        l->setString(string(LocalizedString::create(keys[i].c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")");

        auto filledAttribute = layout->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        layout->removeChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
        layout->addChild(blankAttribute, 1, GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
    }
}

void UIGoals::plusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 50;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) < 10) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1000);
        l->setString(string(LocalizedString::create(keys[i].c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")");

        auto blankAttribute = layout->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        layout->removeChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
        layout->addChild(filledAttribute, 1, (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
    }
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

void UIGoals::createUIAgent(Layout* layout)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithSystemFont(LocalizedString::create("AGENTS")->getCString() + to_string(GameLevel::getInstance()->getCurrentAgentType()), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 8))));
    layout->addChild(title, 1);

    Vector<cocos2d::MenuItem*> menuButtons;

    MenuItem* nextButton;
    if (GameData::getInstance()->getGameStarted()) {
        nextButton = MenuItemImage::create(
            "MapButton.png", "MapButtonPressed.png", CC_CALLBACK_1(UIGoals::menuMapCallback, this));

    } else {
        nextButton = MenuItemImage::create(
            "StartGameButton.png", "StartGameButtonPressed.png", CC_CALLBACK_1(UIGoals::menuStartCallback, this));
    }
    menuButtons.pushBack(nextButton);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (nextButton->getContentSize().width / 2),
                           origin.y + (nextButton->getContentSize().height / 2)));
    layout->addChild(menu, 1);

    Vector<cocos2d::MenuItem*> attributesButtons;
    int tag = 0;

    for (int i = 0; i < keys.size(); i++) {
        auto attLabel = Label::createWithSystemFont(string(LocalizedString::create(keys[i].c_str())->getCString())
                                                    + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")",
                                                    "Arial", 80);
        attLabel->setPosition(origin.x + visibleSize.width / 6, origin.y + (keys.size() - i) * (visibleSize.height / (keys.size() + 1)));
        layout->addChild(attLabel, 1, (i + 1) * 1000);

        auto minusAttButton = MenuItemImage::create(
            "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIGoals::minusAttCallback, this));
        minusAttButton->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 8), attLabel->getPosition().y));
        minusAttButton->setTag(i + 10);
        attributesButtons.pushBack(minusAttButton);

        auto plusAttButton = MenuItemImage::create(
            "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIGoals::plusAttCallback, this));
        plusAttButton->setPosition(Vec2(origin.x + 7 * (visibleSize.width / 8), attLabel->getPosition().y));
        plusAttButton->setTag(i + 50);
        attributesButtons.pushBack(plusAttButton);

        float posX = minusAttButton->getPosition().x + (minusAttButton->getContentSize().width / 2);
        float incX = ((plusAttButton->getPosition().x - (plusAttButton->getContentSize().width / 2)) - posX) / 11;
        int posY = (keys.size() - i) * (visibleSize.height / (keys.size() + 1));

        for (int j = 0; j < 10; j++) {
            posX = posX + incX;
            auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
            auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
            if (j >= GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                blankAttribute->setPosition(Vec2(posX, posY));
                layout->addChild(blankAttribute, 1, tag);
            } else if (j < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                filledAttribute->setPosition(Vec2(posX, posY));
                layout->addChild(filledAttribute, 1, tag);
            }
            tag++;
        }
    }

    Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    layout->addChild(attributesMenu, 1, 100000);

    Label* evolutionPointsLabel = Label::createWithTTF(string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
                                                       + ": " + to_string(GameLevel::getInstance()->getEvolutionPoints()),
                                                       "fonts/BebasNeue.otf", 65);
    evolutionPointsLabel->setPosition(origin.x + visibleSize.width / 6, origin.y + (nextButton->getContentSize().height / 2));
    layout->addChild(evolutionPointsLabel, 1);
}

void UIGoals::update(float delta)
{
    PageView* p = (PageView*)this->getChildByTag(0);
    GameLevel::getInstance()->setCurrentAgentType(p->getCurPageIndex() - 1);
}
