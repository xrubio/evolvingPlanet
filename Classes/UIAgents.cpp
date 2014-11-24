//
//  UIAgents.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIAgents.h"
#include "GameData.h"
#include "LocalizedString.h"

Scene* UIAgents::createScene()
{
    auto scene = Scene::create();
    auto layer = UIAgents::create();
    scene->addChild(layer);
    return scene;
}

bool UIAgents::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->initializeAttributesCost();
    }

    auto title = Label::createWithSystemFont(LocalizedString::create("AGENTS")->getCString() + to_string(GameLevel::getInstance()->getCurrentAgentType()), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    Vector<cocos2d::MenuItem*> menuButtons;

    MenuItem* nextButton;
    if (GameData::getInstance()->getGameStarted()) {
        nextButton = MenuItemImage::create(
            "MapButton.png", "MapButtonPressed.png", CC_CALLBACK_1(UIAgents::menuMapCallback, this));

    } else {
        nextButton = MenuItemImage::create(
            "StartGameButton.png", "StartGameButtonPressed.png", CC_CALLBACK_1(UIAgents::menuStartCallback, this));
    }
    menuButtons.pushBack(nextButton);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (nextButton->getContentSize().width / 2),
                           origin.y + (nextButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes(GameLevel::getInstance()->getCurrentAgentType());
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        //si el cost de l'atribut es diferent de 0, es modificable
        if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), it->first) != 0) {
            keys.push_back(it->first);
        }
        i++;
    }

    Vector<cocos2d::MenuItem*> attributesButtons;
    int tag = 0;

    for (int i = 0; i < keys.size(); i++) {
        auto attLabel = Label::createWithSystemFont(string(LocalizedString::create(keys[i].c_str())->getCString())
                                                    + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")",
                                                    "Arial", 80);
        attLabel->setPosition(origin.x + visibleSize.width / 6, origin.y + (keys.size() - i) * (visibleSize.height / (keys.size() + 1)));
        this->addChild(attLabel, 1, (i + 1) * 1000);

        auto minusAttButton = MenuItemImage::create(
            "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIAgents::minusAttCallback, this));
        minusAttButton->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 8), attLabel->getPosition().y));
        minusAttButton->setTag(i + 10);
        attributesButtons.pushBack(minusAttButton);

        auto plusAttButton = MenuItemImage::create(
            "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIAgents::plusAttCallback, this));
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
                this->addChild(blankAttribute, 1, tag);
            } else if (j < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                filledAttribute->setPosition(Vec2(posX, posY));
                this->addChild(filledAttribute, 1, tag);
            }
            tag++;
        }
    }

    Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    this->addChild(attributesMenu, 1, 100000);

    Label* evolutionPointsLabel = Label::createWithSystemFont(string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
                                                              + ": " + to_string(GameLevel::getInstance()->getEvolutionPoints()),
                                                              "Arial", 65);
    evolutionPointsLabel->setPosition(origin.x + visibleSize.width / 6, origin.y + (nextButton->getContentSize().height / 2));
    this->addChild(evolutionPointsLabel, 1);

    return true;
}

//Faig dues funcions per si el primer cop que es comença la partida s'ha d'intercalar una pantalla de càrrega
void UIAgents::menuStartCallback(Ref* pSender)
{
    /*if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel(0);
    }
    GameData::getInstance()->setGameStarted(true);*/
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIAgents::menuMapCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeed(GameLevel::getInstance()->getTimeSpeedBeforePause());
    Director::getInstance()->popScene();
}

void UIAgents::minusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 10;

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) > 0) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)this->getChildByTag((i + 1) * 1000);
        l->setString(string(LocalizedString::create(keys[i].c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")");

        auto filledAttribute = this->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        this->removeChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
        this->addChild(blankAttribute, 1, GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 10));
    }
}

void UIAgents::plusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 50;

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) < 10) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)this->getChildByTag((i + 1) * 1000);
        l->setString(string(LocalizedString::create(keys[i].c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) + ")");

        auto blankAttribute = this->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        this->removeChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
        this->addChild(filledAttribute, 1, (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 10));
    }
}