/*
 * Copyright (c) 2015
 * MURPHY'S TOAST GAMES
 * 
 * This file is part of Evolving Planet.
 * Evolving Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Evolving Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Evolving Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

//
//  UIGoals.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#define COCOS2D_DEBUG 1

#include "UIGoals.h"
#include "GameData.h"
#include "LocalizedString.h"
#include "UIGlobalPower.h"
#include "UIAreaPower.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIGoals::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGoals::create();
    scene->addChild(layer);
    return scene;
}

UIGoals::~UIGoals()
{
}

bool UIGoals::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    //A UIPROGRESSMAP
    /*string mapString = GameLevel::getInstance()->getMapFilename();
    string backgroundImg = "Background";
    string hotSpotsBase = "HotSpotsBase";
    Director::getInstance()->getTextureCache()->addImage(mapString + backgroundImg + ".jpg");
    Director::getInstance()->getTextureCache()->addImage(mapString + hotSpotsBase + ".png");*/
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Director::getInstance()->setAnimationInterval(1.0 / 60);

    if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/main.mp3", true);
    }

    auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    //this->addChild(background, 0);

    hexagonButtonLevel0 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                        "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel0->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel0->setPosition(Vec2((visibleSize.width / 2) - (hexagonButtonLevel0->getBoundingBox().size.width * 3),
                                          visibleSize.height / 20));
    hexagonButtonLevel0->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel0->setEnabled(false);
    this->addChild(hexagonButtonLevel0, 1);
    
    hexagonButtonLevel1 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                     "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel1->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 20));
    hexagonButtonLevel1->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel1->setEnabled(false);
    this->addChild(hexagonButtonLevel1, 1);
    
    hexagonButtonLevel2 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                     "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel2->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel2->setPosition(Vec2((visibleSize.width / 2) + (hexagonButtonLevel2->getBoundingBox().size.width * 3),
                                          visibleSize.height / 20));
    hexagonButtonLevel2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel2->setEnabled(false);
    this->addChild(hexagonButtonLevel2, 1);

    auto title = Label::createWithTTF(LocalizedString::create("GOALS"), "fonts/arial_rounded_mt_bold.ttf", 180);
    title->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height - ((visibleSize.height / 8))));
    //this->addChild(title, 1);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIGoals::menuBackCallback, this));
    backButton->setPosition(Vec2((4 * visibleSize.width / 42),
        (34 * visibleSize.height / 36)));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(207, 203, 208));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    arrowBack = MenuItemImage::create("gui/ArrowBack.png", "gui/ArrowBackPressed.png", CC_CALLBACK_1(UIGoals::menuArrowBackCallback, this));
    arrowBack->setPosition(Vec2((2 * visibleSize.width / 42), visibleSize.height / 2));
    arrowBack->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowBack);

    arrowNext = MenuItemImage::create("gui/ArrowNext.png", "gui/ArrowNextPressed.png", CC_CALLBACK_1(UIGoals::menuArrowNextCallback, this));
    arrowNext->setPosition(Vec2((40 * visibleSize.width / 42), visibleSize.height / 2));
    arrowNext->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowNext);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);

    GameLevel::getInstance()->setCurrentAgentType(0);

    pages = PageView::create();
    pages->setCustomScrollThreshold(visibleSize.width / 6);
    pages->addChild(background);
    //pages->setBackGroundImage("MainMenuBackground.png");
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    //pages->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));

    //INTRODUCTION
    auto layoutContextIntroduction = Layout::create();
    //TRIAR ESTIL SEGONS EL LVL
    auto pageBackgroundIntroduction = Sprite::create("gui/PageBackground.png");
    pageBackgroundIntroduction->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pageBackgroundIntroduction->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContextIntroduction->addChild(pageBackgroundIntroduction, -1);
    layoutContextIntroduction->setSize(Size((36 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    auto contextIntroduction = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameLevel::getInstance()->getNumLevel()) + "_INTRO").c_str(), "text"), Size(visibleSize.width / (1.5 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "Arial Rounded MT Bold", 40);
    contextIntroduction->setColorSpaceHolder(Color4B(216, 229, 235, 255));
    contextIntroduction->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    contextIntroduction->setScaleX(GameData::getInstance()->getRaWConversion());
    contextIntroduction->setScaleY(GameData::getInstance()->getRaHConversion());
    layoutContextIntroduction->addChild(contextIntroduction);
    
    auto contextLabelIntroduction = Label::createWithTTF(LocalizedString::create("CONTEXT_TITLE_INTRO"),
                                                         "fonts/BebasNeue.otf", 100);
    contextLabelIntroduction->setPosition(Vec2(6 * visibleSize.width / 42, 25 * visibleSize.height / 31));
    contextLabelIntroduction->setColor(Color3B(211, 230, 236));
    contextLabelIntroduction->setAnchorPoint(Vec2(0, 0.5));
    contextLabelIntroduction->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContextIntroduction->addChild(contextLabelIntroduction);
    
    pages->addPage(layoutContextIntroduction);
    
    
    //DEPLOYMENT
    auto layoutContextDeployment = Layout::create();
    //TRIAR ESTIL SEGONS EL LVL
    auto pageBackgroundDeployment = Sprite::create("gui/PageBackground.png");
    pageBackgroundDeployment->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pageBackgroundDeployment->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContextDeployment->addChild(pageBackgroundDeployment, -1);
    layoutContextDeployment->setSize(Size((36 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    auto contextLabelDeployment = Label::createWithTTF(LocalizedString::create("CONTEXT_TITLE_DEPLOYMENT"),
                                        "fonts/BebasNeue.otf", 100);
    contextLabelDeployment->setPosition(Vec2(6 * visibleSize.width / 42, 25 * visibleSize.height / 31));
    contextLabelDeployment->setColor(Color3B(211, 230, 236));
    contextLabelDeployment->setAnchorPoint(Vec2(0, 0.5));
    contextLabelDeployment->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContextDeployment->addChild(contextLabelDeployment);
    
    auto contextDeployment = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameLevel::getInstance()->getNumLevel()) + "_DEPLOYMENT").c_str(), "text"), Size(visibleSize.width / (1.5 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "Arial Rounded MT Bold", 40);
    contextDeployment->setColorSpaceHolder(Color4B(216, 229, 235, 255));
    contextDeployment->setPosition(Vec2(visibleSize.width / 2, 3 * visibleSize.height / 12));
    contextDeployment->setScaleX(GameData::getInstance()->getRaWConversion());
    contextDeployment->setScaleY(GameData::getInstance()->getRaHConversion());
    layoutContextDeployment->addChild(contextDeployment);
    
    auto nameArt = ("art/Escenari" + to_string(GameLevel::getInstance()->getNumLevel()) + ".jpg").c_str();
    auto contextImage = MenuItemImage::create(nameArt, nameArt, CC_CALLBACK_1(UIGoals::zoomImageInCallback, this));
    contextImage->setScale(0.4 * GameData::getInstance()->getRaWConversion());
    contextImage->setPosition(visibleSize.width / 2, 6.8 * visibleSize.height / 12);
    auto menuContext = Menu::create(contextImage, NULL);
    menuContext->setPosition(0, 0);
    menuContext->setName("menuContext");
    layoutContextDeployment->addChild(menuContext, 2);
    
    pages->addPage(layoutContextDeployment);

    auto layout = Layout::create();
    auto pageBackground2 = Sprite::create("gui/PageBackground.png");
    pageBackground2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pageBackground2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(pageBackground2, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));

    setLevelGoals(layout);
    
    Vector<cocos2d::MenuItem*> menuButtonsStart;
    auto playButton = MenuItemImage::create("gui/ProgressMapPlayButton.png", "gui/ProgressMapPlayButtonPressed.png",
                                            CC_CALLBACK_1(UIGoals::menuStartCallback, this));
    playButton->setPosition(Vec2(34 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    playButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto startPlay = Label::createWithTTF(LocalizedString::create("START"), "fonts/BebasNeue.otf", 50);
    startPlay->setPosition(playButton->getContentSize().width / 2, playButton->getContentSize().height / 2);
    startPlay->setColor(Color3B(207, 203, 208));
    playButton->addChild(startPlay);
    menuButtonsStart.pushBack(playButton);
    
    Menu* menuStart = cocos2d::Menu::createWithArray(menuButtonsStart);
    menuStart->setPosition(Vec2(0, 0));
    layout->addChild(menuStart, 10);
    pages->addPage(layout);

    GameLevel::getInstance()->setCurrentAgentType(0);
    pages->setTag(0);

    //SI HI HA UNA CONFIGURACIO INICIAL NO BUIDA, SET CURRENT PAGE ALS ATRIBUTS
    //pages->scrollToPage(2);

    //pages->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(pages);

    //this->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    Director::getInstance()->getTextureCache()->addImage("gui/FingerSpot.png");
    Director::getInstance()->getTextureCache()->addImage("gui/FrameTop.png");
    Director::getInstance()->getTextureCache()->addImage("gui/FrameBottom.png");
    Director::getInstance()->getTextureCache()->addImage("gui/Quit.png");
    Director::getInstance()->getTextureCache()->addImage("gui/Repeat.png");
    Director::getInstance()->getTextureCache()->addImage("gui/RepeatPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlayButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlayButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PauseButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/EndedGameBackground.png");
    Director::getInstance()->getTextureCache()->addImage("gui/CheckpointArea.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressBarBorder.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressBarContent.png");
    Director::getInstance()->getTextureCache()->addImage("gui/GoalIcon.png");
    Director::getInstance()->getTextureCache()->addImage("gui/MinusButtonSmall.png");
    Director::getInstance()->getTextureCache()->addImage("gui/MinusButtonSmallPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlusButtonSmall.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlusButtonSmallPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/BlankAttributePointButtonSmall.png");
    Director::getInstance()->getTextureCache()->addImage("gui/EndedGameBackground.png");
    Director::getInstance()->getTextureCache()->addImage("gui/EndedGameWindow.png");

    this->scheduleUpdate();

    return true;
}

void UIGoals::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    GameData::getInstance()->setGameStarted(false);
    GameLevel::getInstance()->resetLevel();

    auto scene = UIProgressMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

//Faig dues funcions per si el primer cop que es comença la partida s'ha d'intercalar una pantalla de càrrega
void UIGoals::menuStartCallback(Ref* pSender)
{
    /*if (GameData::getInstance()->getGameStarted() == false) {
     GameLevel::getInstance()->createLevel(0);
     }
     GameData::getInstance()->setGameStarted(true);*/
    /*for (int index = 0; index < GameLevel::getInstance()->getNumInitialAgents().size(); index++) {
        map<string, vector<int> > temp = GameLevel::getInstance()->getAttributesValues()[index];
        int j = 0;
        for (map<string, vector<int> >::const_iterator it = temp.begin(); it != temp.end(); it++) {
            for (int k = 0; k < 6; k++) {
                auto v = (ui::TextField*)pages->getPage(3 + index)->getChildByTag((j * 6) + k);
                if (v->getString().empty() == false) {
                    GameLevel::getInstance()->setAttributesValues(index, it->first, k, stoi(v->getString()));
                }
            }
            j++;
        }
    }*/
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    
    Director::getInstance()->getTextureCache()->addImage("maps/Level"+to_string(GameLevel::getInstance()->getNumLevel())+"HotSpotsBase.png");
    
    GameLevel::getInstance()->setAgentAttributesInitToCurrent();
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}


void UIGoals::menuArrowBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurPageIndex() <= 0)
    {
        return;
    }
    if (pages->getCurPageIndex() == 1)
    {
        if (pages->getPage(1)->getChildByTag(100) != nullptr)
        {
            zoomImageOutCallback(nullptr);
        }
    }
    pages->scrollToPage(pages->getCurPageIndex() - 1);
}

void UIGoals::menuArrowNextCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurPageIndex() >= pages->getPages().size() - 1)
    {
        return;
    }
    if (pages->getCurPageIndex() == 1)
    {
        if (pages->getPage(1)->getChildByTag(100) != nullptr)
        {
            zoomImageOutCallback(nullptr);
        }
    }
    pages->scrollToPage(pages->getCurPageIndex() + 1);
}

void UIGoals::zoomImageInCallback(Ref* pSender)
{
    auto image = (MenuItemImage*)pSender;
    if (image->getNumberOfRunningActions() != 0)
    {
        return;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Vector<MenuItem*> menuButtons;
    auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
                                                    CC_CALLBACK_1(UIGoals::zoomImageOutCallback, this));
    darkBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    darkBackground->setOpacity(0);
    darkBackground->runAction(FadeIn::create(0.5));
    menuButtons.pushBack(darkBackground);
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    pages->getPage(1)->addChild(menu, 1, 100);
    
    image->runAction(Spawn::create(ScaleTo::create(0.5, 0.8 * GameData::getInstance()->getRaWConversion()),
                                   MoveTo::create(0.5, Vec2(visibleSize.width / 2, 6 * visibleSize.height / 12)), NULL));
    image->setCallback(CC_CALLBACK_1(UIGoals::zoomImageOutCallback, this));
}

void UIGoals::zoomImageOutCallback(Ref* pSender)
{
    auto image = (MenuItemImage*)pages->getPage(1)->getChildByName("menuContext")->getChildren().at(0);
    if (image->getNumberOfRunningActions() != 0)
    {
        return;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    image->runAction(Spawn::create(ScaleTo::create(0.5, 0.4 * GameData::getInstance()->getRaWConversion()),
                                   MoveTo::create(0.5, Vec2(visibleSize.width / 2, 6.8 * visibleSize.height / 12)), NULL));
    image->setCallback(CC_CALLBACK_1(UIGoals::zoomImageInCallback, this));
    pages->getPage(1)->getChildByTag(100)->runAction(FadeOut::create(0.5));
    pages->getPage(1)->removeChildByTag(100);
}

void UIGoals::setLevelGoals(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto featuresLabel = Label::createWithTTF(LocalizedString::create("LEVEL_FEATURES"),
                                                         "fonts/BebasNeue.otf", 100);
    featuresLabel->setPosition(Vec2(6 * visibleSize.width / 42, 25 * visibleSize.height / 31));
    featuresLabel->setColor(Color3B(211, 230, 236));
    featuresLabel->setAnchorPoint(Vec2(0, 0.5));
    featuresLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(featuresLabel);
    
    auto attributesLabel = Label::createWithTTF(LocalizedString::create("ATTRIBUTES"), "fonts/BebasNeue.otf", 80);
    attributesLabel->setPosition(Vec2(6 * visibleSize.width / 42, 22 * visibleSize.height / 31));
    attributesLabel->setColor(Color3B::WHITE);
    attributesLabel->setAnchorPoint(Vec2(0, 0.5));
    attributesLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(attributesLabel);
    
    //const GameLevel::Levels & atts = GameLevel::getInstance()->getAgentAttributes(0);
    for(size_t i=0; i<GameLevel::getInstance()->getModifiableAttr().size(); i++)
    {
        //ATRIBUT MODIFICABLE
        auto attLabel = Label::createWithTTF(LocalizedString::create(GameLevel::getInstance()->convertAttIntToString(GameLevel::getInstance()->getModifiableAttr().at(i)).c_str()) + ":    Explicació de l'atribut",
                                         "fonts/BebasNeue.otf", 60);
        attLabel->setPosition(Vec2(7 * visibleSize.width / 42, (20 - (i * 2)) * visibleSize.height / 31));
        attLabel->setColor(Color3B(211, 230, 236));
        attLabel->setAnchorPoint(Vec2(0, 0.5));
        attLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->addChild(attLabel);
    }
    
    auto powersLabel = Label::createWithTTF(LocalizedString::create("POWERS"), "fonts/BebasNeue.otf", 80);
    powersLabel->setPosition(Vec2(6 * visibleSize.width / 42, 13 * visibleSize.height / 31));
    powersLabel->setColor(Color3B::WHITE);
    powersLabel->setAnchorPoint(Vec2(0, 0.5));
    powersLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(powersLabel);
    
    for(size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
    {
        if (GameLevel::getInstance()->getPowers().at(i)->getType() == "Global") {
            auto power = new UIGlobalPower(GameLevel::getInstance()->getPowers().at(i));
            power->setPosition(9 * visibleSize.width / 42, (10 - (i * 4)) * visibleSize.height / 31);
            layout->addChild(power->getIcon());
        }
        else if (GameLevel::getInstance()->getPowers().at(i)->getType() == "Area") {
            auto power = new UIAreaPower(GameLevel::getInstance()->getPowers().at(i));
            power->setPosition(9 * visibleSize.width / 42, (10 - (i * 4)) * visibleSize.height / 31);
            layout->addChild(power->getIcon());
        }
        
        auto powerLabel = Label::createWithTTF("Explicació del poder",
                                               "fonts/BebasNeue.otf", 60);
        powerLabel->setPosition(Vec2(11 * visibleSize.width / 42, (10 - (i * 4)) * visibleSize.height / 31));
        powerLabel->setColor(Color3B(211, 230, 236));
        powerLabel->setAnchorPoint(Vec2(0, 0.5));
        powerLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->addChild(powerLabel);
    }
    //POWERS NOT AVAILABLE
    if( GameLevel::getInstance()->getPowers().size() == 0)
    {
        auto powersNotAvailableLabel = Label::createWithTTF(LocalizedString::create("NOT_AVAILABLE"), "fonts/BebasNeue.otf", 80);
        powersNotAvailableLabel->setPosition(Vec2(7 * visibleSize.width / 42, 10 * visibleSize.height / 31));
        powersNotAvailableLabel->setColor(Color3B::GRAY);
        powersNotAvailableLabel->setAnchorPoint(Vec2(0, 0.5));
        powersNotAvailableLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->addChild(powersNotAvailableLabel);
    }
}

void UIGoals::update(float delta)
{
    //GameLevel::getInstance()->setCurrentAgentType(pages->getCurPageIndex() - 2);
    if (pages->getCurPageIndex() == 0)
    {
        hexagonButtonLevel0->setEnabled(true);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 1)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(true);
        hexagonButtonLevel2->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 2)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(true);

        /*Layout* layout = pages->getPage(2);
        Menu* attributesMenu = (Menu*)layout->getChildByTag(100000);
        for (int i = 0; i < GameLevel::getInstance()->getModifiableAttr().size(); i++) {
            MenuItem* plus = (MenuItem*) attributesMenu->getChildByTag(i + 50);
            if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr()[i]) >
                GameLevel::getInstance()->getEvolutionPoints())
            {
                plus->setEnabled(false);
            }
            else
            {
                plus->setEnabled(true);
            }
        }*/
    }
    
    if (pages->getCurPageIndex() > 0 and pages->getCurPageIndex() < pages->getPages().size() - 1) {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    }
    else {
        if (pages->getCurPageIndex() == 0) {
            arrowBack->setVisible(false);
            arrowNext->setVisible(true);
        }
        else if (pages->getCurPageIndex() == pages->getPages().size() - 1) {
            arrowBack->setVisible(true);
            arrowNext->setVisible(false);
        }
    }

    //evolutionPointsNumberIcon->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));
}
