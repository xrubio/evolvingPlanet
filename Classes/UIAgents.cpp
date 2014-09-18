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

    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes();
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        keys.push_back(it->first);
        i++;
    }

    Label* att1Label = Label::createWithSystemFont(string(LocalizedString::create(keys.at(0).c_str())->getCString())
                                                   + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(0))) + ")",
                                                   "Arial", 80);
    att1Label->setPosition(origin.x + visibleSize.width / 6, origin.y + 3 * (visibleSize.height / 4));
    this->addChild(att1Label, 1, 100);

    Label* att2Label = Label::createWithSystemFont(string(LocalizedString::create(keys.at(1).c_str())->getCString())
                                                   + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(1))) + ")",
                                                   "Arial", 80);
    att2Label->setPosition(origin.x + visibleSize.width / 6, origin.y + 2 * (visibleSize.height / 4));
    this->addChild(att2Label, 1, 200);

    Label* att3Label = Label::createWithSystemFont(string(LocalizedString::create(keys.at(2).c_str())->getCString())
                                                   + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(2))) + ")",
                                                   "Arial", 80);
    att3Label->setPosition(origin.x + visibleSize.width / 6, origin.y + visibleSize.height / 4);
    this->addChild(att3Label, 1, 300);

    Vector<cocos2d::MenuItem*> attributesButtons;

    MenuItem* minusAtt1Button = MenuItemImage::create(
        "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIAgents::minusAtt1Callback, this));
    minusAtt1Button->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 8), att1Label->getPosition().y));
    attributesButtons.pushBack(minusAtt1Button);

    MenuItem* plusAtt1Button = MenuItemImage::create(
        "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIAgents::plusAtt1Callback, this));
    plusAtt1Button->setPosition(Vec2(origin.x + 7 * (visibleSize.width / 8), att1Label->getPosition().y));
    attributesButtons.pushBack(plusAtt1Button);

    MenuItem* minusAtt2Button = MenuItemImage::create(
        "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIAgents::minusAtt2Callback, this));
    minusAtt2Button->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 8), att2Label->getPosition().y));
    attributesButtons.pushBack(minusAtt2Button);

    MenuItem* plusAtt2Button = MenuItemImage::create(
        "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIAgents::plusAtt2Callback, this));
    plusAtt2Button->setPosition(Vec2(origin.x + 7 * (visibleSize.width / 8), att2Label->getPosition().y));
    attributesButtons.pushBack(plusAtt2Button);

    MenuItem* minusAtt3Button = MenuItemImage::create(
        "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIAgents::minusAtt3Callback, this));
    minusAtt3Button->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 8), att3Label->getPosition().y));
    attributesButtons.pushBack(minusAtt3Button);

    MenuItem* plusAtt3Button = MenuItemImage::create(
        "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIAgents::plusAtt3Callback, this));
    plusAtt3Button->setPosition(Vec2(origin.x + 7 * (visibleSize.width / 8), att3Label->getPosition().y));
    attributesButtons.pushBack(plusAtt3Button);

    Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    this->addChild(attributesMenu, 1, 1000);

    //si no es el primer cop consultar a domini, si ho es, tot blanc
    if (GameData::getInstance()->getGameStarted()) {
        int tag = 0;
        att1 = GameLevel::getInstance()->getAgentAttribute(keys.at(0));
        att2 = GameLevel::getInstance()->getAgentAttribute(keys.at(1));
        att3 = GameLevel::getInstance()->getAgentAttribute(keys.at(2));

        for (int j = 3; j > 0; j--) {
            float posX = minusAtt1Button->getPosition().x + (minusAtt1Button->getContentSize().width / 2);
            float incX = ((plusAtt1Button->getPosition().x - (plusAtt1Button->getContentSize().width / 2)) - posX) / 11;
            int posY = j * (visibleSize.height / 4);
            for (int i = 0; i < 10; i++) {
                posX = posX + incX;
                auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
                auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
                if (j == 3 and i >= att1) {
                    blankAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(blankAttribute, 1, tag);
                } else if (j == 3 and i < att1) {
                    filledAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(filledAttribute, 1, tag);
                } else if (j == 2 and i >= att2) {
                    blankAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(blankAttribute, 1, tag);
                } else if (j == 2 and i < att2) {
                    filledAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(filledAttribute, 1, tag);
                } else if (j == 1 and i >= att3) {
                    blankAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(blankAttribute, 1, tag);
                } else if (j == 1 and i < att3) {
                    filledAttribute->setPosition(Vec2(posX, posY));
                    this->addChild(filledAttribute, 1, tag);
                }
                tag++;
            }
        }
    } else {
        int tag = 0;
        for (int j = 3; j > 0; j--) {
            float posX = minusAtt1Button->getPosition().x + (minusAtt1Button->getContentSize().width / 2);
            float incX = ((plusAtt1Button->getPosition().x - (plusAtt1Button->getContentSize().width / 2)) - posX) / 11;
            int posY = j * (visibleSize.height / 4);
            for (int i = 0; i < 10; i++) {
                auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
                posX = posX + incX;
                blankAttribute->setPosition(Vec2(posX, posY));
                this->addChild(blankAttribute, 1, tag);
                tag++;
            }
        }
    }

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
    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel(0);
    }
    GameData::getInstance()->setGameStarted(true);
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIAgents::menuMapCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeed(GameLevel::getInstance()->getTimeSpeedBeforePause());
    Director::getInstance()->popScene();
}

void UIAgents::minusAtt1Callback(Ref* pSender)
{
    if (att1 > 0) {
        att1--;
        GameLevel::getInstance()->setAgentAttribute(keys.at(0), att1);
        GameLevel::getInstance()->setAttributeCost(keys.at(0), GameLevel::getInstance()->getAttributeCost(keys.at(0)) + 1);
        Label* l = (Label*)this->getChildByTag(100);
        l->setString(string(LocalizedString::create(keys.at(0).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(0))) + ")");

        auto filledAttribute = this->getChildByTag(att1);
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        this->removeChildByTag(att1);
        this->addChild(blankAttribute, 1, att1);
    }
}

void UIAgents::plusAtt1Callback(Ref* pSender)
{
    if (att1 < 10) {
        att1++;
        GameLevel::getInstance()->setAgentAttribute(keys.at(0), att1);
        GameLevel::getInstance()->setAttributeCost(keys.at(0), GameLevel::getInstance()->getAttributeCost(keys.at(0)) + 1);
        Label* l = (Label*)this->getChildByTag(100);
        l->setString(string(LocalizedString::create(keys.at(0).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(0))) + ")");

        auto blankAttribute = this->getChildByTag(att1 - 1);
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        this->removeChildByTag(att1 - 1);
        this->addChild(filledAttribute, 1, att1 - 1);
    }
}

void UIAgents::minusAtt2Callback(Ref* pSender)
{
    if (att2 > 0) {
        att2--;
        GameLevel::getInstance()->setAgentAttribute(keys.at(1), att2);
        GameLevel::getInstance()->setAttributeCost(keys.at(1), GameLevel::getInstance()->getAttributeCost(keys.at(1)) + 1);
        Label* l = (Label*)this->getChildByTag(200);
        l->setString(string(LocalizedString::create(keys.at(1).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(1))) + ")");

        auto filledAttribute = this->getChildByTag(att2 + 10);
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        this->removeChildByTag(att2 + 10);
        this->addChild(blankAttribute, 1, att2 + 10);
    }
}

void UIAgents::plusAtt2Callback(Ref* pSender)
{
    if (att2 < 10) {
        att2++;
        GameLevel::getInstance()->setAgentAttribute(keys.at(1), att2);
        GameLevel::getInstance()->setAttributeCost(keys.at(1), GameLevel::getInstance()->getAttributeCost(keys.at(1)) + 1);
        Label* l = (Label*)this->getChildByTag(200);
        l->setString(string(LocalizedString::create(keys.at(1).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(1))) + ")");

        auto blankAttribute = this->getChildByTag(att2 + 10 - 1);
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        this->removeChildByTag(att2 + 10 - 1);
        this->addChild(filledAttribute, 1, att2 + 10 - 1);
    }
}

void UIAgents::minusAtt3Callback(Ref* pSender)
{
    if (att3 > 0) {
        att3--;
        GameLevel::getInstance()->setAgentAttribute(keys.at(2), att3);
        GameLevel::getInstance()->setAttributeCost(keys.at(2), GameLevel::getInstance()->getAttributeCost(keys.at(2)) + 1);
        Label* l = (Label*)this->getChildByTag(300);
        l->setString(string(LocalizedString::create(keys.at(2).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(2))) + ")");

        auto filledAttribute = this->getChildByTag(att3 + 20);
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        this->removeChildByTag(att3 + 20);
        this->addChild(blankAttribute, 1, att3 + 20);
    }
}

void UIAgents::plusAtt3Callback(Ref* pSender)
{
    if (att3 < 10) {
        att3++;
        GameLevel::getInstance()->setAgentAttribute(keys.at(2), att3);
        GameLevel::getInstance()->setAttributeCost(keys.at(2), GameLevel::getInstance()->getAttributeCost(keys.at(2)) + 1);
        Label* l = (Label*)this->getChildByTag(300);
        l->setString(string(LocalizedString::create(keys.at(2).c_str())->getCString())
                     + " (" + to_string(GameLevel::getInstance()->getAttributeCost(keys.at(2))) + ")");

        auto blankAttribute = this->getChildByTag(att3 + 20 - 1);
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        this->removeChildByTag(att3 + 20 - 1);
        this->addChild(filledAttribute, 1, att3 + 20 - 1);
    }
}
