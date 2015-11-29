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
//  UIGameplayMap.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIGameplayMap.h"
#include "UIGoals.h"
#include "GameData.h"
#include "LocalizedString.h"
#include "UIGlobalPower.h"
#include "UIAreaPower.h"
#include "CollectionGoal.h"
#include "ExpansionGoal.h"
#include "Timing.h"
#include "LevelLoader.h"
#include "Message.h"
#include "Tutorial.h"
#include "UITransitionScene.h"
#include <audio/include/SimpleAudioEngine.h>

Scene* UIGameplayMap::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGameplayMap::create();
    scene->addChild(layer);
    return scene;
}

UIGameplayMap::~UIGameplayMap()
{
    delete[] agentsTextureData;
    delete[] exploitedMapTextureData;
    if(_tutorial)
    {
        delete _tutorial;
    }
}

bool UIGameplayMap::init()
{
    if (!Layer::init()) {
        return false;
    }

    _tutorial = nullptr;
    Director::getInstance()->setAnimationInterval(1.0 / 60);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Size contentSize;
    contentSize.width = GameData::getInstance()->getResourcesWidth();
    contentSize.height = GameData::getInstance()->getResourcesHeight();

    string map = "maps/" + GameLevel::getInstance()->getMapFilename();
    //fer DEFINES
    string ext = ".png";
    string background = "Background";
    string hotSpotsBase = "HotSpotsBase";
    string resources = "Resources";
    string forest = "Forest";

    //Set background gameplay map and all its functionalities
    gameplayMap = Sprite::create(map + background + ".jpg");
    gameplayMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    gameplayMap->setName("gameplayMap");
    this->addChild(gameplayMap, 0);
    
    //HOTSPOT
    gameplayMapHotSpot = new Image();
    gameplayMapHotSpot->initWithImageFile(map + hotSpotsBase + ext);
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    dataGameplayMapHotSpot = new unsigned char[gameplayMapHotSpot->getDataLen() * x];
    dataGameplayMapHotSpot = gameplayMapHotSpot->getData();

    //FINGER SPOT
    fingerSpot = MenuItemImage::create("gui/FingerSpot.png", "gui/FingerSpot.png", CC_CALLBACK_1(UIGameplayMap::removeFingerSpot, this));
    fingerSpot->setVisible(false);
    fingerSpot->setAnchorPoint(Vec2(0.5, 0));
    Menu* mFinger = Menu::createWithItem(fingerSpot);
    mFinger->setPosition(0, 0);
    mFinger->setName("finger");
    gameplayMap->addChild(mFinger, 3);

    //FRAMES
    auto topFrame = Sprite::create("gui/FrameTop.png");
    topFrame->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    topFrame->setPosition(visibleSize.width / 2, visibleSize.height - (topFrame->getBoundingBox().size.height / 2));
    topFrame->setName("topFrame");
    this->addChild(topFrame, 1);
    auto bottomFrame = Sprite::create("gui/FrameBottom.png");
    bottomFrame->setName("bottomFrame");
    bottomFrame->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    bottomFrame->setPosition(visibleSize.width / 2, bottomFrame->getBoundingBox().size.height / 2);
    this->addChild(bottomFrame, 1);

    string space = " ";
    string lvl = LocalizedString::create("LEVEL") + space + to_string(GameLevel::getInstance()->getNumLevel());
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    levelLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    levelLabel->setColor(Color3B(139, 146, 154));
    levelLabel->setAnchorPoint(Vec2(0, 0.5));
    levelLabel->setPosition(Vec2(13 * visibleSize.width / 204, 145 * visibleSize.height / 155));
    this->addChild(levelLabel, 5);

    //QUIT / RETRY
    Vector<MenuItem*> quitRetryVec;
    MenuItem* quitButton = MenuItemImage::create("gui/Quit.png", "gui/QuitPressed.png", CC_CALLBACK_1(UIGameplayMap::quitCallback, this));
    quitButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    quitButton->setPosition(Vec2(quitButton->getBoundingBox().size.width / 2,
        visibleSize.height - (quitButton->getBoundingBox().size.height / 2)));
    quitButton->setName("quitButton");
    quitRetryVec.pushBack(quitButton);

    MenuItem* retryButton = MenuItemImage::create("gui/Repeat.png", "gui/RepeatPressed.png", CC_CALLBACK_1(UIGameplayMap::retryCallback, this));
    retryButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    retryButton->setPosition(Vec2(retryButton->getBoundingBox().size.width / 2,
        quitButton->getPositionY() - quitButton->getBoundingBox().size.height));
    retryButton->setName("retryButton");
    quitRetryVec.pushBack(retryButton);

    Menu* quitRetryMenu = Menu::createWithArray(quitRetryVec);
    quitRetryMenu->setPosition(0, 0);
    quitRetryMenu->setName("quitRetryMenu");
    this->addChild(quitRetryMenu, 10);

    //RESOURCES MAP (IF ANY)
    for(size_t i = 0; i < GameLevel::getInstance()->getGoals().size(); i++)
    {
        if (GameLevel::getInstance()->getGoals().at(i)->getGoalType() == Collection)
        {
            resourcesMap = true;
        }
    }
    if (resourcesMap) {
        /*gameplayMapResources = new Image();
        gameplayMapResources->initWithImageFile(map + resources + ext);
        x = 3;
        if (gameplayMapResources->hasAlpha()) {
            x = 4;
        }
        dataGameplayMapResources = new unsigned char[gameplayMapResources->getDataLen() * x];
        dataGameplayMapResources = gameplayMapResources->getData();*/

        exploitedMapTexture = new Texture2D;
        auto im = new Image();
        im->initWithImageFile(map + forest + ext);
        //4 = alpha
        unsigned char* data = new unsigned char[im->getDataLen() * 4];
        data = im->getData();

        exploitedMapTexture->initWithData(exploitedMapTextureData, GameData::getInstance()->getResourcesWidth() * GameData::getInstance()->getResourcesHeight(), Texture2D::PixelFormat::RGBA8888, GameData::getInstance()->getResourcesWidth(), GameData::getInstance()->getResourcesHeight(), contentSize);
        for (int i = 0; i < im->getWidth(); i++) {
            for (int j = 0; j < im->getHeight(); j++) {
                unsigned char* pixel = data + ((int)i + (int)j * im->getWidth()) * 4;
                exploitedMapTextureData[i + (j * im->getWidth())] = Color4B(*(pixel), *(pixel + 1), *(pixel + 2), *(pixel + 3));
            }
        }
        exploitedMapSprite = Sprite::createWithTexture(exploitedMapTexture);
        exploitedMapSprite->setPosition(Vec2(gameplayMap->getBoundingBox().size.width / 2, gameplayMap->getBoundingBox().size.height / 2));
        gameplayMap->addChild(exploitedMapSprite, 2);
    }

    GameLevel::getInstance()->setUIGameplayMap(this);

    //INITIALIZE AGENTS AND EXPLOITED MAP TEXTUREDATA
    Color4B white;
    white.r = 127;
    white.g = 127;
    white.b = 127;
    white.a = 0;
    for (int i = 0; i < (int)Director::getInstance()->getVisibleSize().width * (int)Director::getInstance()->getVisibleSize().height; i++) {
        agentsTextureData[i] = white; // i is an index running from 0 to w*h-1
        //exploitedMapTextureData[i] = white; // i is an index running from 0 to w*h-1
    }

    agentsTexture = new Texture2D;
    agentsTexture->initWithData(agentsTextureData, GameData::getInstance()->getResourcesWidth() * GameData::getInstance()->getResourcesHeight(), Texture2D::PixelFormat::RGBA8888,
        GameData::getInstance()->getResourcesWidth(), GameData::getInstance()->getResourcesHeight(), contentSize);
    agentsSprite = Sprite::createWithTexture(agentsTexture);
    agentsSprite->setPosition(Vec2(gameplayMap->getBoundingBox().size.width / 2, gameplayMap->getBoundingBox().size.height / 2));
    gameplayMap->addChild(agentsSprite, 2);
    agentsSprite->setCascadeOpacityEnabled(true);

    //EVOLUTION POINTS
    evolutionPointsIcon = Sprite::create("gui/EvolutionPoints.png");
    evolutionPointsIcon->setAnchorPoint(Vec2(0.5, 1));
    evolutionPointsIcon->setPosition(3.4 * bottomFrame->getContentSize().width / 13, 4.9 * bottomFrame->getContentSize().height / 6);
    evolutionPointsLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getEvolutionPoints()), "fonts/BebasNeue.otf", 80 * GameData::getInstance()->getRaConversion());
    evolutionPointsLabel->setAlignment(TextHAlignment::CENTER);
    evolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
    evolutionPointsLabel->setColor(Color3B(216, 229, 235));
    evolutionPointsIcon->addChild(evolutionPointsLabel, 2);
    restaEvolutionPointsLabel = Label::createWithTTF("- ", "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    restaEvolutionPointsLabel->setColor(Color3B(211, 197, 0));
    restaEvolutionPointsLabel->setOpacity(0);
    evolutionPointsIcon->addChild(restaEvolutionPointsLabel, 2);
    bottomFrame->addChild(evolutionPointsIcon, 1);

    //TIME BUTTONS
    Vector<MenuItem*> timeButtons;
    MenuItem* playButton = MenuItemImage::create("gui/PlayButton.png", "gui/PlayButtonPressed.png");
    playButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    playButton->setName("playButton");

    MenuItem* pauseButton = MenuItemImage::create("gui/PauseButton.png", "gui/PauseButton.png");
    pauseButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    pauseButton->setEnabled(false);
    pauseButton->setName("pauseButton");

    pauseDarkBackground = Sprite::create("gui/EndedGameBackground.png");
    pauseDarkBackground->setName("pauseDarkBackground");
    pauseDarkBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    pauseDarkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pauseDarkBackground->setOpacity(150);
    auto pauseDarkLabel = Label::createWithTTF(string(LocalizedString::create("SET_ATTRIBUTES")), "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    pauseDarkLabel->setTextColor(Color4B(216, 229, 235, 60));
    pauseDarkLabel->setPosition(Vec2(pauseDarkBackground->getContentSize().width / 2, pauseDarkBackground->getContentSize().height / 2));
    pauseDarkLabel->setName("pauseDarkLabel");
    pauseDarkBackground->addChild(pauseDarkLabel);
    addChild(pauseDarkBackground, 0);

    MenuItemToggle *toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(UIGameplayMap::togglePlay, this), playButton, pauseButton, NULL);
    toggle->setPosition(Vec2(184 * visibleSize.width / 204, 144 * visibleSize.height / 155));
    toggle->setName("playToggle");

    Menu* timeMenu = Menu::create(toggle, NULL);
    timeMenu->setName("timeMenu");
    timeMenu->setPosition(Vec2(0, 0));
    this->addChild(timeMenu, 2);

    //Powers
    vector<Power*> pws = GameLevel::getInstance()->getPowers();
    for (size_t i = 0; i < pws.size(); i++)
    {
        Vec2 pos;
        if (i == 0)
        {
            pos.x = 14 * visibleSize.width / 204;
            pos.y = 11 * visibleSize.height / 155;
        }
        else
        {
            pos.x = powerButtons.at(i - 1)->getIcon()->getPosition().x + (4 * visibleSize.width / 204) + powerButtons.at(i - 1)->getIcon()->getBoundingBox().size.width;
            pos.y = powerButtons.at(i - 1)->getIcon()->getPosition().y;
        }

        if (pws.at(i)->getType() == "Global")
        {
            powerButtons.push_back(new UIGlobalPower(pws.at(i)));
            powerButtons.at(i)->setPosition(pos.x, pos.y);
            powerButtons.at(i)->getIcon()->setName("power"+std::to_string(i));
            this->addChild(powerButtons.at(i)->getIcon(), 3);
        }
        else if (pws.at(i)->getType() == "Area")
        {
            powerButtons.push_back(new UIAreaPower(pws.at(i)));
            powerButtons.at(i)->setPosition(pos.x, pos.y);
            powerButtons.at(i)->getIcon()->setName("power"+std::to_string(i));
            this->addChild(powerButtons.at(i)->getIcon(), 3);
            gameplayMap->addChild(((UIAreaPower*)powerButtons.at(i))->getArea(), 3);
        }
    }

    //SET GOALS ON MAP
    for (size_t i = 0; i < GameLevel::getInstance()->getGoals().size(); i++)
    {
        Goal * goal = GameLevel::getInstance()->getGoals().at(i);
        //Set Checkpoint Area
        if(goal->getGoalType()!= Expansion)
        {
            CCLOG("Goal type still not implemented TODO");
            continue;
        }
        ExpansionGoal * expansionGoal = (ExpansionGoal*)(goal);
        
        //FIND AREA
        int minX = 479;
        int maxX = 0;
        int minY = 319;
        int maxY = 0;
        for(int x = 0; x < 480; x++)
        {
            for(int y = 1; y <= 320; y++)
            {
                int goalCode = expansionGoal->getColorZone();
                int valueAtMap = getValueAtGameplayMap(1,x,y);
                if(goalCode!=valueAtMap)
                {
                    continue;
                }

                // check limits
                if(minX>x)
                {
                    minX = x;
                }
                if(maxX<x)
                {
                    maxX = x;
                }
    
                if(minY>y)
                {
                    minY = y;
                }
                if(maxY<y)
                {
                    maxY = y;
                }
            }
        }
        int centerX = maxX - ((maxX - minX) / 2);
        int centerY = maxY - ((maxY - minY) / 2);
        expansionGoal->setCenterArea(centerX, centerY);
        
        int x = (int)(centerX * float(GameData::getInstance()->getResourcesWidth() / 480.0));
        int y = (int)(float((GameData::getInstance()->getResourcesHeight() - GameData::getInstance()->getResourcesMargin()) / 2.0) + (centerY * float(GameData::getInstance()->getResourcesMargin() / 320.0)));
        
        auto area = Sprite::create("gui/CheckpointArea.png");
        if(i == 0)
        {
            auto blink = Blink::create(2, 2);
            auto repeatBlink = RepeatForever::create(blink);
            area->setColor(Color3B::WHITE);
            area->runAction(repeatBlink);
        }
        else
        {
            area->setColor(Color3B::RED);
            area->setOpacity(0);
        }
        area->setPosition(x, y);
        area->setTag(400 + int(i));
        gameplayMap->addChild(area, 3);
    }

    //TIME PROGRESS BAR
    timeBorderBar = Sprite::create("gui/ProgressBarBorder.png");
    timeBorderBar->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    timeBorderBar->setPosition(6 * visibleSize.width / 21, 15 * visibleSize.height / 16);
    auto timeBorderSize = timeBorderBar->getContentSize();
    timeBorderBar->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(timeBorderBar, 3);
    auto barContent = Sprite::create("gui/ProgressBarContent.png");
    timeBar = ProgressTimer::create(barContent);
    timeBar->setType(ProgressTimer::Type::BAR);
    timeBar->setAnchorPoint(Vec2(0, 0));
    timeBar->setMidpoint(Vec2(0, 0));
    timeBar->setBarChangeRate(Vec2(1, 0));
    timeBar->setPosition(0, 0);
    timeBorderBar->addChild(timeBar, 3);
    
    auto degradateTime = Sprite::create("gui/GoalDegradate.png");
    degradateTime->setName("degradateTime");
    degradateTime->setPositionY(timeBar->getContentSize().height / 2);
    degradateTime->setVisible(false);
    degradateTime->setAnchorPoint(Vec2(0, 0.5));
    timeBorderBar->addChild(degradateTime, 2);

    //SET GOALS ON TIME PROGRESS BAR
    //float pixelPerStep = barContent->getTexture()->getPixelsWide() / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime();
    for (size_t i = 0; i < GameLevel::getInstance()->getGoals().size(); i++)
    {
        float posXaverage = (float)GameLevel::getInstance()->getGoals().at(i)->getAverageTime() / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime() * timeBorderBar->getContentSize().width;
        float posXcentered = (float)(GameLevel::getInstance()->getGoals().at(i)->getMinTime() + ((GameLevel::getInstance()->getGoals().at(i)->getMaxTime() - GameLevel::getInstance()->getGoals().at(i)->getMinTime()) / 2)) / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime() * timeBorderBar->getContentSize().width;
        
        auto goalMark1star = Sprite::create("gui/GoalMark1.png");
        goalMark1star->setPosition(posXcentered, (timeBorderBar->getContentSize().height / 2));
        float pixelPerStepMark = goalMark1star->getTexture()->getPixelsWide() / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime();
        float sizeInPixelsMark = (GameLevel::getInstance()->getGoals().at(i)->getMaxTime() - GameLevel::getInstance()->getGoals().at(i)->getMinTime()) * pixelPerStepMark;
        float sizeRatio = timeBorderBar->getContentSize().width/goalMark1star->getTexture()->getPixelsWide();
        goalMark1star->setScaleX(sizeRatio *sizeInPixelsMark / goalMark1star->getTexture()->getPixelsWide());
        timeBorderBar->addChild(goalMark1star, 1);
        
        auto goalMark2star = Sprite::create("gui/GoalMark2.png");
        goalMark2star->setPosition(posXaverage, (timeBorderBar->getContentSize().height / 2));
        pixelPerStepMark = goalMark2star->getTexture()->getPixelsWide() / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime();
        sizeInPixelsMark = (GameLevel::getInstance()->getGoals().at(i)->getDesviation2Star() * 2) * pixelPerStepMark;
        sizeRatio = timeBorderBar->getContentSize().width/goalMark2star->getTexture()->getPixelsWide();
        goalMark2star->setScaleX(sizeRatio * sizeInPixelsMark / goalMark2star->getTexture()->getPixelsWide());
        timeBorderBar->addChild(goalMark2star, 1);
        
        auto goalMark3star = Sprite::create("gui/GoalMark3.png");
        goalMark3star->setPosition(posXaverage, (timeBorderBar->getContentSize().height / 2));
        pixelPerStepMark = goalMark3star->getTexture()->getPixelsWide() / (float)GameLevel::getInstance()->getGoals().at(GameLevel::getInstance()->getGoals().size() - 1)->getMaxTime();
        sizeInPixelsMark = (GameLevel::getInstance()->getGoals().at(i)->getDesviation3Star() * 2) * pixelPerStepMark;
        sizeRatio = timeBorderBar->getContentSize().width/goalMark3star->getTexture()->getPixelsWide();
        goalMark3star->setScaleX(sizeRatio * sizeInPixelsMark / goalMark3star->getTexture()->getPixelsWide());
        timeBorderBar->addChild(goalMark3star, 1);
        
        /*auto goalHex = DrawNode::create();
        float R = (GameLevel::getInstance()->getGoals().at(i)->getDesviation3Star() * 2) * pixelPerStep / 2;
        Vec2 v[6];
        v[0]= Vec2(posXaverage - R, timeBorderSize.height);
        v[1]= Vec2(posXaverage, timeBorderSize.height + timeBorderSize.height / 10);
        v[2]= Vec2(posXaverage + R, timeBorderSize.height);
        v[3]= Vec2(posXaverage + R, 0);
        v[4]= Vec2(posXaverage, - timeBorderSize.height / 10);
        v[5]= Vec2(posXaverage - R, 0);
        timeBorderBar->addChild(goalHex, 2);
        goalHex->drawPolygon(v, 6, Color4F(Color4B(108, 185, 94, 240)), 1, Color4F::WHITE);//(v, 6, true, Color4F::WHITE);*/

        //auto goalNum = Sprite::create("gui/GoalIcon.png");
        //goalNum->setPosition(posXaverage, timeBorderBar->getContentSize().height / 2);
        /*goalNum->setScaleX(GameLevel::getInstance()->getGoals()[i]->getDesviation3Star() * (goalNum->getTexture()->getPixelsWide()
                           / (float)GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime() - 1));*/
        /*auto labelGoalNum = Label::createWithTTF(to_string(i + 1), "fonts/BebasNeue.otf", 30);
        labelGoalNum->setPosition(goalNum->getContentSize().width / 2, goalNum->getContentSize().height / 2);
        labelGoalNum->setColor(Color3B(216, 229, 235));
        goalNum->addChild(labelGoalNum, 2);*/
        //timeBorderBar->addChild(goalNum, 2);
    }

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIGameplayMap::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(UIGameplayMap::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(UIGameplayMap::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // set listener for tutorial
    _listenerTutorial = EventListenerTouchOneByOne::create();
    _listenerTutorial->setSwallowTouches(true);
    _listenerTutorial->onTouchBegan = CC_CALLBACK_2(UIGameplayMap::onTouchBeganTutorial, this);
    _listenerTutorial->onTouchEnded = CC_CALLBACK_2(UIGameplayMap::onTouchEndedTutorial, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_listenerTutorial,-1);

    if (GameData::getInstance()->getMusic() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/level.mp3", true);
    }
    
    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel();
    }
    GameData::getInstance()->setGameStarted(true);

    initializeAgents();

    const GameLevel::Levels & modifAttr = GameLevel::getInstance()->getModifiableAttr();
    
    auto graphicBackground = MenuItemImage::create("gui/GraphicBackground.jpg", "gui/GraphicBackground.jpg", CC_CALLBACK_1(UIGameplayMap::changeGraphicCallback, this));
    graphicBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    graphicBackground->setPosition(10.15 * visibleSize.width / 11, 0.55 * visibleSize.height / 7.5);
    graphicBackground->setName("graphicBackground");

    auto menuGraphic = Menu::create(graphicBackground, nullptr);
    menuGraphic->setPosition(Vec2::ZERO);
    menuGraphic->setName("menuGraphic");
    this->addChild(menuGraphic, 1);
    
    auto labelGraphic = Label::createWithTTF("AGENTS", "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    labelGraphic->setPosition(graphicBackground->getPositionX(), graphicBackground->getPositionY() + (graphicBackground->getBoundingBox().size.height / 2) + (labelGraphic->getBoundingBox().size.height / 2));
    labelGraphic->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    labelGraphic->setName("graphicLabel");
    this->addChild(labelGraphic);
    
    auto labelCounterGraphic = Label::createWithTTF("( 1 / 4 )", "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    labelCounterGraphic->setPosition(graphicBackground->getPositionX(), graphicBackground->getPositionY() - (graphicBackground->getBoundingBox().size.height / 2) - (labelCounterGraphic->getBoundingBox().size.height / 2));
    labelCounterGraphic->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    labelCounterGraphic->setName("graphicCounterLabel");
    this->addChild(labelCounterGraphic);
    
    ///////////////////////////////////////////////   WAVE NODE   //////////////////////////////////////////////////////
    auto populationNode = new WaveNode();
    populationNode->init();
    populationNode->glDrawMode = kDrawLines;
    populationNode->setReadyToDrawDynamicVerts(true);
    populationNode->setName("population");
    graphicBackground->addChild(populationNode, 1, 1);
    waveNodes.push_back(populationNode);
    
    auto woodNode = new WaveNode();
    woodNode->init();
    woodNode->glDrawMode = kDrawLines;
    woodNode->setReadyToDrawDynamicVerts(true);
    woodNode->setName("wood");
    waveNodes.push_back(woodNode);
    
    auto mineralNode = new WaveNode();
    mineralNode->init();
    mineralNode->glDrawMode = kDrawLines;
    mineralNode->setReadyToDrawDynamicVerts(true);
    mineralNode->setName("mineral");
    waveNodes.push_back(mineralNode);
    
    auto stoneNode = new WaveNode();
    stoneNode->init();
    stoneNode->glDrawMode = kDrawLines;
    stoneNode->setReadyToDrawDynamicVerts(true);
    stoneNode->setName("stone");
    waveNodes.push_back(stoneNode);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto attBackgroundTitle = Label::createWithTTF(LocalizedString::create("EVOLUTION_POINTS") + " - ", "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    attBackgroundTitle->setColor(Color3B(216, 229, 236));
    attBackgroundTitle->setAnchorPoint(Vec2(0, 0.5));
    attBackgroundTitle->setPosition(Vec2(4 * bottomFrame->getContentSize().width / 13,
                                         4 * bottomFrame->getContentSize().height / 6));
    bottomFrame->addChild(attBackgroundTitle);
    
    auto attBackgroundTitleCost = Label::createWithTTF(" [ " + LocalizedString::create("COST") + " ]", "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    attBackgroundTitleCost->setColor(Color3B(211, 197, 0));
    attBackgroundTitleCost->setAnchorPoint(Vec2(0, 0.5));
    attBackgroundTitleCost->setPosition(Vec2(attBackgroundTitle->getPositionX() + attBackgroundTitle->getContentSize().width,
                                         attBackgroundTitle->getPositionY()));
    bottomFrame->addChild(attBackgroundTitleCost);

    int k = 1;
    int tag = 0;
    Vector<MenuItem*> attributesButtons;
    for (size_t j = 0; j < modifAttr.size(); j++)
    {   
        if(modifAttr.at(j)==-1)
        {
            continue;
        }
        auto labelAttRight = Label::createWithTTF(string(LocalizedString::create(GameLevel::getInstance()->convertAttIntToString(modifAttr.at(j)).c_str())), "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
        labelAttRight->setColor(Color3B(216, 229, 236));
        bottomFrame->addChild(labelAttRight, 1, (int(j) + 1) * 1000);

        auto attNumLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), modifAttr.at(j))), "fonts/BebasNeue.otf", 35 * GameData::getInstance()->getRaConversion());
        attNumLabel->setColor(Color3B::BLACK);
        attNumLabel->setAnchorPoint(Vec2(0.5, 0.5));
        bottomFrame->addChild(attNumLabel, 1, (int(j) + 1) * 1100);
        
        auto costBackground = Sprite::create("gui/EvolutionPointsCost.png");
        bottomFrame->addChild(costBackground);
        
        auto minusAttButton = MenuItemImage::create("gui/MinusButtonSmall.png", "gui/MinusButtonSmallPressed.png", "gui/MinusButtonSmallPressed.png", CC_CALLBACK_1(UIGameplayMap::minusAttCallback, this));
        minusAttButton->setAnchorPoint(Vec2(0, 0.5));
        minusAttButton->setPosition(Vec2((3.8 + (j * 2.35)) * bottomFrame->getContentSize().width / 13, 0.8 * bottomFrame->getContentSize().height / 2));
        minusAttButton->setTag(int(j) + 10);
        minusAttButton->setEnabled(false);
        minusAttButton->setName("minus"+labelAttRight->getString());
        attributesButtons.pushBack(minusAttButton);

        auto plusAttButton = MenuItemImage::create("gui/PlusButtonSmall.png", "gui/PlusButtonSmallPressed.png", "gui/PlusButtonSmallPressed.png", CC_CALLBACK_1(UIGameplayMap::plusAttCallback, this));
        plusAttButton->setPosition(Vec2((3.8 + (j * 2.35) + 1.95) * bottomFrame->getContentSize().width / 13, 0.8 * bottomFrame->getContentSize().height / 2));
        plusAttButton->setTag(int(j) + 50);
        plusAttButton->setEnabled(false);
        plusAttButton->setName("plus"+labelAttRight->getString());
        attributesButtons.pushBack(plusAttButton);

        float posX = minusAttButton->getPosition().x + minusAttButton->getContentSize().width;
        float incX = ((plusAttButton->getPosition().x - (plusAttButton->getContentSize().width / 2)) - posX) / 6;
        int posY = 0.8 * bottomFrame->getContentSize().height / 2;

        for (int m = 0; m < 5; m++) {
            posX = posX + incX;
            auto blankAttribute = Sprite::create("gui/BlankAttributePointButtonSmall.png");
            auto filledAttribute = Sprite::create("gui/FilledAttributePointButtonSmall.png");
            if (m >= GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), modifAttr.at(j))) {
                blankAttribute->setPosition(Vec2(posX, posY));
                bottomFrame->addChild(blankAttribute, 1, tag);
            }
            else if (m < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), modifAttr.at(j))) {
                filledAttribute->setPosition(Vec2(posX, posY));
                bottomFrame->addChild(filledAttribute, 1, tag);
            }
            if (m == 2)
            {
                labelAttRight->setPosition(Vec2(posX, 0.2 * visibleSize.height / 7.5));
                attNumLabel->setPosition(plusAttButton->getPositionX(), labelAttRight->getPositionY());
                costBackground->setPosition(attNumLabel->getPositionX(), attNumLabel->getPositionY());
            }
            tag++;
        }

        k++;
    }

    Menu* attrMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attrMenu->setPosition(Vec2(0, 0));
    attrMenu->setName("attrMenu");
    bottomFrame->addChild(attrMenu, 1, 100000);

    //CHECK IF THERE IS ANY IN-GAME ACHIEVEMENT (achievement num 3 of the corresponding level)
    if (GameData::getInstance()->getAchievements(GameLevel::getInstance()->getNumLevel()).size() > 2 and GameData::getInstance()->getAchievements(GameLevel::getInstance()->getNumLevel()).at(2)->getGoalType() == "DISCOVER")
    {
        GameLevel::getInstance()->setInGameAchievement(GameData::getInstance()->getAchievements(GameLevel::getInstance()->getNumLevel()).at(2));
    }
    
    //INFLUENCE LABEL
    for (int i = 0; i < GameLevel::getInstance()->getModifiableAttr().size(); i++)
    {
        if (GameLevel::getInstance()->getModifiableAttr().at(i) == eInfluence)
        {
            int x = (int)(GameLevel::getInstance()->getAgents().at(0).front()->getPosition().getX() * GameData::getInstance()->getRowDrawAgentPrecalc());
            int y = (int)(GameData::getInstance()->getColumnOffsetDrawAgentPrecalc() + ((GameLevel::getInstance()->getAgents().at(0).front()->getPosition().getY()) * GameData::getInstance()->getColumnDrawAgentPrecalc()));
            auto labelInfluenced = Label::createWithTTF(to_string(Agent::_numInfluenced.at(0)), "fonts/BebasNeue.otf", 50);
            labelInfluenced->setPosition(Vec2(x, y));
            labelInfluenced->setName("labelInfluence");
            labelInfluenced->setOpacity(0);
            gameplayMap->addChild(labelInfluenced);
            i = GameLevel::getInstance()->getModifiableAttr().size();
        }
    }
    
    createTutorialGUI();
    _message = 0;

    createTimingThread();
    createNewLevelThread();
    
    gameplayMap->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

	auto mouseListener  = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(UIGameplayMap::onMouseScroll, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(mouseListener, 1);
        
    updateAttributesButtons();
    
    this->scheduleUpdate();
    return true;
}

bool UIGameplayMap::checkPowersClicked(void)
{
    for (size_t i = 0; i < powerButtons.size(); i++)
    {
        if (powerButtons.at(i)->getClicked() == true) {
            return true;
        }
    }
    return false;
}

bool UIGameplayMap::checkButtonPressed(Touch* touch)
{
    auto menu = (Menu*)this->getChildByName("bottomFrame")->getChildByName("attrMenu");
    Point touchLocation = menu->convertToNodeSpace(touch->getLocation());
    
    Vector<Node*> menuChildren = menu->getChildren();
    for (int i = 0; i < menuChildren.size(); i++)
    {
        Node *button = menuChildren.at(i);
        if(button->getBoundingBox().containsPoint(touchLocation))
        {
            return true;
        }
    }
    
    touchLocation = touch->getLocation();
    for (size_t i = 0; i < powerButtons.size(); i++)
    {
        if(powerButtons.at(i)->getIcon()->getBoundingBox().containsPoint(touchLocation))
        {
            return true;
        }
    }

    return false;
}

void UIGameplayMap::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted || checkPowersClicked())
    {
        return;
    }
    
    for (auto touch : touches)
    {
        _touches.pushBack(touch);
    }
    if (touches.size() == 1 and checkButtonPressed(touches.at(0)) == false)
    {
        if ((((clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC) < 0.3) and (abs(touches[0]->getLocation().distance(firstTouchLocation)) < 40))
        {
            firstTouchLocation = touches.at(0)->getLocation();
            changeSpotPosition();
        }
    }

    for (auto touch : touches)
    {
        Point touchLocation = this->convertTouchToNodeSpace(touch);
        moveBackground = false;
        for (size_t i = 0; i < powerButtons.size(); i++)
        {
            powerButtons.at(i)->onTouchesBegan(touchLocation);
        }
        if (checkPowersClicked() == false and selectSpriteForTouch(gameplayMap, touchLocation))
        {
            moveBackground = true;
        }
    }
}

void UIGameplayMap::changeSpotPosition()
{
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    float verticalMargin = gameplayMap->getContentSize().width / 1.5;
    if (verticalMargin > gameplayMap->getContentSize().height)
    {
        verticalMargin = gameplayMap->getContentSize().height;
    }
    
    Point nextDirection(gameplayMap->convertToNodeSpace(firstTouchLocation).x / float(gameplayMap->getContentSize().width / 480.0), (gameplayMap->convertToNodeSpace(firstTouchLocation).y - ((gameplayMap->getContentSize().height - verticalMargin) / 2)) / float(verticalMargin / 320.0));
    GameLevel::getInstance()->setAgentDirection(0, nextDirection);
        
    auto fadeFinger = FadeIn::create(1);
    fadeFinger->setTag(1);
        
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
        
    fingerSpot->setPosition(Vec2(gameplayMap->convertToNodeSpace(firstTouchLocation)));
    fingerSpot->setVisible(true);
    if(_tutorial && _tutorial->getCurrentMessage() && _tutorial->getCurrentMessage()->getPostCondition()=="spot")
    {
        _tutorial->getCurrentMessage()->postConditionAchieved();
    }
}

void UIGameplayMap::onTouchesMoved(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted)
    {
        return;
    }
    // ZOOM
    if (_touches.size() >= 2)
    {
        if (checkPowersClicked() == false)
        {
            // Get the two first touches
            Touch *touch1 = (Touch*)_touches.at(0);
            Touch *touch2 = (Touch*)_touches.at(1);
            
            // Get current and previous positions of the touches
            Point curPosTouch1 = Director::getInstance()->convertToGL(touch1->getLocationInView());
            Point curPosTouch2 = Director::getInstance()->convertToGL(touch2->getLocationInView());
            Point prevPosTouch1 = Director::getInstance()->convertToGL(touch1->getPreviousLocationInView());
            Point prevPosTouch2 = Director::getInstance()->convertToGL(touch2->getPreviousLocationInView());
            
            // Calculate current and previous positions of the layer relative the anchor point
            Point curPosLayer = curPosTouch1.getMidpoint(curPosTouch2);
            Point prevPosLayer = prevPosTouch1.getMidpoint(prevPosTouch2);
            
            // Calculate new scale
            float prevScaleX = gameplayMap->getScaleX();
            float curScaleX = gameplayMap->getScaleX() * curPosTouch1.getDistance(curPosTouch2) / prevPosTouch1.getDistance(prevPosTouch2) ;
            
            float prevScaleY = gameplayMap->getScaleY();
            float curScaleY = gameplayMap->getScaleY() * curPosTouch1.getDistance(curPosTouch2) / prevPosTouch1.getDistance(prevPosTouch2) ;
            
            float xScale = MIN( MAX( curScaleX, GameData::getInstance()->getRaWConversion()), 3.0);
            float yScale = MIN( MAX( curScaleY, GameData::getInstance()->getRaHConversion()), 3.0 );
            
            //si no canvia x i y (aixi seviten deformacions)
            if (xScale != gameplayMap->getScaleX() and yScale != gameplayMap->getScaleY())
            {
                gameplayMap->setScale(xScale, yScale);
            }
            
            if( this->getScaleX() != prevScaleX )
            {
                Point realCurPosLayer = gameplayMap->convertToNodeSpaceAR(curPosLayer);
                float deltaX = (realCurPosLayer.x) * (gameplayMap->getScaleX() - prevScaleX);
                float deltaY = (realCurPosLayer.y) * (gameplayMap->getScaleY() - prevScaleY);
                
                gameplayMap->setPosition(Vec2(gameplayMap->getPosition().x - deltaX, gameplayMap->getPosition().y - deltaY));
            }
            
            // If current and previous position of the multitouch's center aren't equal -> change position of the layer
            if (!prevPosLayer.equals(curPosLayer))
            {
                gameplayMap->setPosition(Vec2(gameplayMap->getPosition().x + curPosLayer.x - prevPosLayer.x,
                                      gameplayMap->getPosition().y + curPosLayer.y - prevPosLayer.y));
            }
            
            Point reLocate = gameplayMap->getPosition();

            checkBackgroundLimitsInTheScreen(reLocate);
            while (!moveBackgroundLeft) {
                reLocate.x -= 2.0;
                checkBackgroundLimitsInTheScreen(reLocate);
            }
            while (!moveBackgroundRight) {
                reLocate.x += 2.0;
                checkBackgroundLimitsInTheScreen(reLocate);
            }
            while (!moveBackgroundUp) {
                reLocate.y += 2.0;
                checkBackgroundLimitsInTheScreen(reLocate);
            }
            while (!moveBackgroundDown) {
                reLocate.y -= 2.0;
                checkBackgroundLimitsInTheScreen(reLocate);
            }
            gameplayMap->setPosition(reLocate);
        }
    }
    // PAN
    else if (touches.size() == 1)
    {
        for (size_t i = 0; i < powerButtons.size(); i++)
        {
            powerButtons.at(i)->onTouchesMoved(touches.at(0));
        }
        if (moveBackground) {
            Point touchLocation = this->convertTouchToNodeSpace(touches.at(0));

            Point oldTouchLocation = touches.at(0)->getPreviousLocationInView();
            oldTouchLocation = Director::getInstance()->convertToGL(oldTouchLocation);
            oldTouchLocation = convertToNodeSpace(oldTouchLocation);

            Point translation = touchLocation - oldTouchLocation;
            Point newPos = gameplayMap->getPosition() + translation;

            checkBackgroundLimitsInTheScreen(newPos);

            Point destPos = gameplayMap->getPosition();
            if (moveBackgroundLeft and translation.x > 0) {
                destPos.x = newPos.x;
            }
            if (moveBackgroundRight and translation.x < 0) {
                destPos.x = newPos.x;
            }
            if (moveBackgroundUp and translation.y < 0) {
                destPos.y = newPos.y;
            }
            if (moveBackgroundDown and translation.y > 0) {
                destPos.y = newPos.y;
            }
            gameplayMap->setPosition(destPos);
        }
    }
}

bool UIGameplayMap::onTouchBeganTutorial(Touch * touch, Event* event)
{    
    if(!_tutorial)
    {
        return false;
    }
    if (!_tutorial->getCurrentMessage())
    {
        return true;
    }
    
    Node * parent = this;
    Point touchLocation = parent->convertToNodeSpace(touch->getLocation());
    
    //SKIP
    auto tutorialWindow = parent->getChildByName("menuTutorialWindow")->getChildByName("tutorialWindow");
    auto skip = parent->getChildByName("menuTutorialWindow")->getChildByName("skipWindow");
    if(tutorialWindow->getBoundingBox().containsPoint(touchLocation) or skip->getBoundingBox().containsPoint(touchLocation))
    {
        return false;
    }
    
    //QUIT OR RETRY
    Node * quitRetry = parent->getChildByName("quitRetryMenu");
    if(quitRetry->getChildByName("quitButton")->getBoundingBox().containsPoint(touchLocation) or
       quitRetry->getChildByName("retryButton")->getBoundingBox().containsPoint(touchLocation))
    {
        return false;
    }
    
    string qrButton = "";
    if (quitRetry->getChildByName("quitButton")->getChildByName("confirmBackground") != nullptr)
    {
        Node * n = quitRetry->getChildByName("quitButton")->getChildByName("confirmBackground")->getChildByName("confirmMenu");
        
        //both buttons have same size
        Size buttonSize = n->getChildByName("confirmNo")->getContentSize();
        
        Vec2 posRNO = n->convertToWorldSpace(n->getChildByName("confirmNo")->getPosition());
        posRNO.x -= buttonSize.width / 2;
        posRNO.y -= buttonSize.height / 2;
        Rect rNo(posRNO, buttonSize);
        
        Vec2 posROK = n->convertToWorldSpace(n->getChildByName("confirmOk")->getPosition());
        posROK.x -= buttonSize.width / 2;
        posROK.y -= buttonSize.height / 2;
        Rect rOk(posROK, buttonSize);
        
        if (rNo.containsPoint(touchLocation) or rOk.containsPoint(touchLocation))
        {
            return false;
        }
    }
    if (quitRetry->getChildByName("retryButton")->getChildByName("confirmBackground") != nullptr)
    {
        Node * n = quitRetry->getChildByName("retryButton")->getChildByName("confirmBackground")->getChildByName("confirmMenu");
        
        //both buttons have same size
        Size buttonSize = n->getChildByName("confirmNo")->getContentSize();
        
        Vec2 posRNO = n->convertToWorldSpace(n->getChildByName("confirmNo")->getPosition());
        posRNO.x -= buttonSize.width / 2;
        posRNO.y -= buttonSize.height / 2;
        Rect rNo(posRNO, buttonSize);
        
        Vec2 posROK = n->convertToWorldSpace(n->getChildByName("confirmOk")->getPosition());
        posROK.x -= buttonSize.width / 2;
        posROK.y -= buttonSize.height / 2;
        Rect rOk(posROK, buttonSize);
        
        if (rNo.containsPoint(touchLocation) or rOk.containsPoint(touchLocation))
        {
            return false;
        }
        
    }
    
    if(_tutorial->getCurrentMessage()->getPostCondition() == "tap")
    {
        _tutorial->getCurrentMessage()->postConditionAchieved();
        return true;
    }
    if(_tutorial->getCurrentMessage()->getPostCondition() == "spot")
    {
        // XRC this is implemented in a different way than the standard onTouchesBegan. Should we unify both codes?
        if (((clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC) < 0.3)
        {
            firstTouchLocation = touchLocation;
            changeSpotPosition();
        }
        return true;
    }
    if (_tutorial->getCurrentMessage()->getPostCondition() != "tapButton")
    {
        return false;
    }
    
    std::string buttonName = _tutorial->getCurrentMessage()->getPostConditionButtonTap();
    parent = this;
    std::size_t pos;
    std::string delimiter = "/";
    std::string token;
    while ((pos = buttonName.find(delimiter)) != std::string::npos)
    {
        token = buttonName.substr(0, pos);
        parent = parent->getChildByName(token);
        buttonName.erase(0, pos + delimiter.length());
    }
    
    token = buttonName.substr(0, pos);
    
    //attribute translation
    if (token.substr(0, 4) == "plus")
    {
        token = "plus" + LocalizedString::create(token.substr(4).c_str());
    }
    else if (token.substr(0, 5) == "minus")
    {
        token = "minus" + LocalizedString::create(token.substr(5).c_str());
    }
    
    Node * button = parent->getChildByName(token);
    touchLocation = parent->convertToNodeSpace(touch->getLocation());
    if(button->getBoundingBox().containsPoint(touchLocation))
    {
        _tutorial->getCurrentMessage()->postConditionAchieved();
        return false;
    }

    return true;
}

void UIGameplayMap::onTouchEndedTutorial(Touch * touch, Event* event)
{
    timeFingerSpot = clock();
}

void UIGameplayMap::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted)
    {
        return;
    }
    Point touchLocation = this->convertTouchToNodeSpace(touches.at(0));
    for(size_t i = 0; i < powerButtons.size(); i++)
    {
        bool actioned = powerButtons.at(i)->onTouchesEnded(touchLocation);
        GameLevel::getInstance()->setPowersUsed(true);
        //ANIMACIO RESTA PUNTS
        if (actioned == true)
        {
            restaEvolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
            restaEvolutionPointsLabel->setString("-" + to_string(int(powerButtons.at(i)->getPower()->getCost())));
            auto mov = MoveTo::create(1.5, Vec2(evolutionPointsIcon->getContentSize().width / 2, - evolutionPointsIcon->getContentSize().height / 2));
            restaEvolutionPointsLabel->runAction(Spawn::create(mov, Sequence::create(FadeIn::create(0.5), FadeOut::create(1.0), NULL), NULL));
        }
        else
        {
            powerButtons.at(i)->disabled = true;
        }
    }
    moveBackground = false;
    _touches.clear();

    firstTouchLocation = touches.at(0)->getLocation();
    timeFingerSpot = clock();
}

void UIGameplayMap::onMouseScroll(Event* event)
{
    if (endGameWindowPainted)
    {
        return;
    }

    // ZOOM
    EventMouse* e = (EventMouse*)event;
    if(checkPowersClicked() == false and (!_tutorial or _tutorial->isFinished()))
    {
        // Get current and previous positions of the touches
        Point curPosTouch1 = Director::getInstance()->convertToGL(e->getLocationInView());
        Point prevPosTouch1 = Director::getInstance()->convertToGL(e->getPreviousLocationInView());
        
        // Calculate new scale
        float prevScaleX = gameplayMap->getScaleX();
        float curScaleX = gameplayMap->getScaleX() - (e->getScrollY() / 10) * GameData::getInstance()->getRaWConversion();
        
        float prevScaleY = gameplayMap->getScaleY();
        float curScaleY = gameplayMap->getScaleY() - (e->getScrollY() / 10) * GameData::getInstance()->getRaHConversion();
        
        gameplayMap->setScale(MIN( MAX( curScaleX, GameData::getInstance()->getRaWConversion()),
                                  3.0 * GameData::getInstance()->getRaWConversion()),
                              MIN( MAX( curScaleY, GameData::getInstance()->getRaHConversion()),
                                  3.0 * GameData::getInstance()->getRaHConversion()));
        
        if( this->getScaleX() != prevScaleX )
        {
            Point realCurPosLayer = gameplayMap->convertToNodeSpaceAR(curPosTouch1);
            float deltaX = (realCurPosLayer.x) * (gameplayMap->getScaleX() - prevScaleX);
            float deltaY = (realCurPosLayer.y) * (gameplayMap->getScaleY() - prevScaleY);
            
            gameplayMap->setPosition(Vec2(gameplayMap->getPosition().x - deltaX, gameplayMap->getPosition().y + deltaY));
        }
        
        Point reLocate = gameplayMap->getPosition();

        checkBackgroundLimitsInTheScreen(reLocate);
        while (!moveBackgroundLeft) {
            reLocate.x -= 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundRight) {
            reLocate.x += 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundUp) {
            reLocate.y += 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }
        while (!moveBackgroundDown) {
            reLocate.y -= 2.0;
            checkBackgroundLimitsInTheScreen(reLocate);
        }

        gameplayMap->setPosition(reLocate);
    }
}

void UIGameplayMap::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    _eventDispatcher->removeEventListener(_listenerTutorial);
    GameLevel::getInstance()->setFinishedGame(UserCancel);
    while (GameLevel::getInstance()->ended == false)
        ;
    //pthread_cancel(gameLevelThread);
    //pthread_cancel(timingThread);
    pthread_join(gameLevelThread, nullptr);
    pthread_join(timingThread, nullptr);
    GameLevel::getInstance()->play(false);
    GameData::getInstance()->setGameStarted(false);
    
    if (GameData::getInstance()->getFirstTimeLevelCompleted() == GameLevel::getInstance()->getNumLevel())
    {
        auto scene = UITransitionScene::createScene();
        auto transition = TransitionFade::create(0.2f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else
    {
        auto scene = UIProgressMap::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
}

void UIGameplayMap::pauseGame()
{
    ((MenuItemToggle*)this->getChildByName("timeMenu")->getChildByName("playToggle"))->setSelectedIndex(0);
    pauseDarkBackground->setVisible(true);
    GameLevel::getInstance()->play(false);
}

void UIGameplayMap::togglePlay(Ref* pSender)
{
    if (endGameWindowPainted)
    {
        return;
    }
    
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (firstPlayFF == true)
    {
        setAttributesToInitialAgents();
        auto l = ((Label*)pauseDarkBackground->getChildByName("pauseDarkLabel"));
        l->setTTFConfig(_ttfConfig("fonts/BebasNeue.otf", 150 * GameData::getInstance()->getRaConversion()));
        l->setString(string(LocalizedString::create("PAUSE")));
        timeBorderBar->getChildByName("degradateTime")->setVisible(true);
        firstPlayFF = false;
    }

    if(!GameLevel::getInstance()->isPlaying())
    {
        pauseDarkBackground->setVisible(false);
        GameLevel::getInstance()->play(true);
    }
    else
    {
        pauseGame();
    }
}

void UIGameplayMap::attributeSelectionCallback(Ref* pSender)
{
    MenuItem* lifeButton = (MenuItem*)pSender;
    lifeButton->setEnabled(false);
    agentColor = lifeButton->getTag();
    Menu* attributesMenu = (Menu*)lifeButton->getParent();
    for(int i = 0; i < attributesMenu->getChildren().size(); i++)
    {
        if (((MenuItem*)attributesMenu->getChildren().at(i))->getTag() != agentColor)
        {
            auto item = ((MenuItem*)attributesMenu->getChildren().at(i));
            item->setEnabled(true);
        }
    }
}

void UIGameplayMap::quitCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    MenuItem* p = (MenuItem*)pSender;
    p->setEnabled(false);
    auto confirmBackground = Sprite::create("gui/ConfirmBackground.png");
    confirmBackground->setName("confirmBackground");
    Vector<MenuItem*> confirmVec;
    MenuItem* confirmOk = MenuItemImage::create("gui/ConfirmOk.png", "gui/ConfirmOkPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    confirmOk->setName("confirmOk");
    MenuItem* confirmNo = MenuItemImage::create("gui/ConfirmNo.png", "gui/ConfirmNoPressed.png", CC_CALLBACK_1(UIGameplayMap::NoCallback, this));
    confirmNo->setName("confirmNo");
    auto separator = Sprite::create("gui/ConfirmSeparator.png");
    separator->setPosition(Vec2(3 * confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmOk->setPosition(Vec2(5 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmNo->setPosition(Vec2(7 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmVec.pushBack(confirmOk);
    confirmVec.pushBack(confirmNo);
    Menu* confirmMenu = Menu::createWithArray(confirmVec);
    confirmMenu->setName("confirmMenu");
    confirmMenu->setPosition(0, 0);
    auto labelConfirm = Label::createWithTTF(string(LocalizedString::create("QUIT_QUESTION")), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    labelConfirm->setPosition(Vec2(confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmBackground->addChild(separator);
    confirmBackground->addChild(confirmMenu, 10);
    confirmBackground->addChild(labelConfirm);
    confirmBackground->setAnchorPoint(Vec2(0, 0.5));
    confirmBackground->setPosition(Vec2(p->getContentSize().width - confirmBackground->getContentSize().width, p->getContentSize().height / 2));
    p->addChild(confirmBackground, -1);
    auto moveConfirm = MoveTo::create(0.4, Vec2(p->getContentSize().width, p->getContentSize().height / 2));
    auto ease = EaseBackOut::create(moveConfirm);
    confirmBackground->runAction(ease);
}

void UIGameplayMap::retryCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    MenuItem* p = (MenuItem*)pSender;
    p->setEnabled(false);
    auto confirmBackground = Sprite::create("gui/ConfirmBackground.png");
    confirmBackground->setName("confirmBackground");
    Vector<MenuItem*> confirmVec;
    auto confirmOk = MenuItemImage::create("gui/ConfirmOk.png", "gui/ConfirmOkPressed.png", CC_CALLBACK_1(UIGameplayMap::retryOkCallback, this));
    confirmOk->setName("confirmOk");
    auto confirmNo = MenuItemImage::create("gui/ConfirmNo.png", "gui/ConfirmNoPressed.png", CC_CALLBACK_1(UIGameplayMap::NoCallback, this));
    confirmNo->setName("confirmNo");
    auto separator = Sprite::create("gui/ConfirmSeparator.png");
    separator->setPosition(Vec2(3 * confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmOk->setPosition(Vec2(5 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmNo->setPosition(Vec2(7 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmVec.pushBack(confirmOk);
    confirmVec.pushBack(confirmNo);
    Menu* confirmMenu = Menu::createWithArray(confirmVec);
    confirmMenu->setName("confirmMenu");
    confirmMenu->setPosition(0, 0);
    auto labelConfirm = Label::createWithTTF(string(LocalizedString::create("RETRY_QUESTION")), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    labelConfirm->setPosition(Vec2(confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmBackground->addChild(separator);
    confirmBackground->addChild(confirmMenu);
    confirmBackground->addChild(labelConfirm);
    confirmBackground->setAnchorPoint(Vec2(0, 0.5));
    confirmBackground->setPosition(Vec2(p->getContentSize().width - confirmBackground->getContentSize().width, p->getContentSize().height / 2));
    p->addChild(confirmBackground, -1);
    auto moveConfirm = MoveTo::create(0.4, Vec2(p->getContentSize().width, p->getContentSize().height / 2));
    auto ease = EaseBackOut::create(moveConfirm);
    confirmBackground->runAction(ease);
}

void UIGameplayMap::retryOkCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    GameLevel::getInstance()->setFinishedGame(UserCancel);

    //pthread_cancel(gameLevelThread);
    //pthread_cancel(timingThread);
    pthread_join(gameLevelThread, nullptr);
    pthread_join(timingThread, nullptr);
    GameLevel::getInstance()->play(false);
    GameData::getInstance()->setGameStarted(false);
    string filename = "level" + to_string(GameLevel::getInstance()->getNumLevel());
    GameLevel::getInstance()->resetLevel();
    LevelLoader loader;
    loader.loadXmlFile(filename);
    
    _eventDispatcher->removeEventListener(_listenerTutorial);
    GameLevel::getInstance()->setAgentAttributesInitToCurrent();
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGameplayMap::NoCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    MenuItem* p = (MenuItem*)((MenuItem*)pSender)->getParent()->getParent();
    MenuItem* p2 = (MenuItem*)p->getParent();
    p2->setEnabled(true);
    auto moveConfirm = MoveTo::create(0.8, Vec2(p2->getContentSize().width - p->getContentSize().width,
                                               p2->getContentSize().height / 2));
    auto ease = EaseBackOut::create(moveConfirm);
    p->runAction(ease);
    p2->removeChild(p);
}

void UIGameplayMap::minusAttCallback(Ref* pSender)
{
    if (endGameWindowPainted)
    {
        return;
    }
    
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 10;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    // XRC TODO fix this
    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) <= 0)
    {
        return;
    }
    
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    
    GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i), GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) - 1);
    GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() - GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)));
    int oldCost = GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i));
    GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i), GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + 1);
    Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
    l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i))));

    auto filledAttribute = layout->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + (i * 5));
    auto blankAttribute = Sprite::create("gui/BlankAttributePointButtonSmall.png");
    blankAttribute->setPosition(filledAttribute->getPosition());
    layout->removeChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + (i * 5));
    layout->addChild(blankAttribute, 1, GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + (i * 5));
    
    //ANIMATION
    restaEvolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
    restaEvolutionPointsLabel->setString("-" + to_string(oldCost));
    auto mov = MoveTo::create(1.5, Vec2(evolutionPointsIcon->getContentSize().width / 2, - evolutionPointsIcon->getContentSize().height / 2));
    restaEvolutionPointsLabel->runAction(Spawn::create(mov, Sequence::create(FadeIn::create(0.5), FadeOut::create(1.0), NULL), NULL));
}

void UIGameplayMap::plusAttCallback(Ref* pSender)
{
    if (endGameWindowPainted)
    {
        return;
    }
    
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 50;
    Sprite* layout = (Sprite*)(pMenuItem->getParent()->getParent());

    // XRC TODO fix this
    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) >= 5 or GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) > GameLevel::getInstance()->getEvolutionPoints())
    {
        return;
    }
    
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }

    GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr()[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + 1);
    GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() - GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)));
    int oldCost = GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i));
    GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i), GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) + 1);
    Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
    l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i))));

    auto blankAttribute = layout->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) - 1) + (i * 5));
    auto filledAttribute = Sprite::create("gui/FilledAttributePointButtonSmall.png");
    filledAttribute->setPosition(blankAttribute->getPosition());
    layout->removeChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) - 1) + (i * 5));
    layout->addChild(filledAttribute, 1, (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) - 1) + (i * 5));
    
    //ANIMATION
    restaEvolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
    restaEvolutionPointsLabel->setString("-" + to_string(oldCost));
    auto mov = MoveTo::create(1.5, Vec2(evolutionPointsIcon->getContentSize().width / 2, - evolutionPointsIcon->getContentSize().height / 2));
    restaEvolutionPointsLabel->runAction(Spawn::create(mov, Sequence::create(FadeIn::create(0.5), FadeOut::create(1.0), NULL), NULL));
}

void UIGameplayMap::hideAchievementWindowCallback(Ref* pSender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto window = (MenuItemImage*)pSender;
    window->runAction(EaseBackIn::create(MoveTo::create(0.4, Vec2(visibleSize.width / 2, visibleSize.height + window->getContentSize().height))));
}

void UIGameplayMap::removeFingerSpot(Ref* pSender)
{
    if (endGameWindowPainted)
    {
        return;
    }
    
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    GameLevel::getInstance()->setAgentDirection(0, Point(-1, -1));
    fingerSpot->setVisible(false);
}

void UIGameplayMap::changeGraphicCallback(Ref* pSender)
{
    auto graphicBackground = (MenuItem*) pSender;
    auto graphicLabel = (Label*)this->getChildByName("graphicLabel");
    auto graphicCounterLabel = (Label*)this->getChildByName("graphicCounterLabel");
    string name;
    for (size_t i = 0; i < graphicBackground->getChildren().size(); i++)
    {
        if ((name = graphicBackground->getChildren().at(i)->getName()) != "")
        {
            i = graphicBackground->getChildren().size();
        }
    }
    if (name == "population")
    {
        graphicBackground->removeChild(waveNodes.at(0));
        graphicBackground->addChild(waveNodes.at(1), 1, 1);
        agentColor = 1;
        graphicLabel->setString("WOOD");
        graphicCounterLabel->setString("( 2 / 4 )");
    }
    else if (name == "wood")
    {
        graphicBackground->removeChild(waveNodes.at(1));
        graphicBackground->addChild(waveNodes.at(2), 1, 1);
        agentColor = 2;
        graphicLabel->setString("MINERAL");
        graphicCounterLabel->setString("( 3 / 4 )");
    }
    else if (name == "mineral")
    {
        graphicBackground->removeChild(waveNodes.at(2));
        graphicBackground->addChild(waveNodes.at(3), 1, 1);
        agentColor = 3;
        graphicLabel->setString("STONE");
        graphicCounterLabel->setString("( 4 / 4 )");
    }
    else if (name == "stone")
    {
        graphicBackground->removeChild(waveNodes.at(3));
        graphicBackground->addChild(waveNodes.at(0), 1, 1);
        agentColor = 0;
        graphicLabel->setString("AGENTS");
        graphicCounterLabel->setString("( 1 / 4 )");
    }
}

void UIGameplayMap::skipTutorial(Ref* pSender)
{
    auto window = (MenuItemToggle*)pSender;
    auto skipWindow = (MenuItem*)window->getParent()->getChildByName("skipWindow");
    if (skipWindow->isVisible())
    {
        skipWindow->runAction(FadeOut::create(0.3));
        skipWindow->setVisible(false);
        window->setSelectedIndex(0);
    }
    else
    {
        skipWindow->setVisible(true);
        skipWindow->runAction(FadeIn::create(0.2));
        window->setSelectedIndex(1);
    }
}

void UIGameplayMap::skipTutorialConfirm(Ref* pSender)
{
    _tutorial->removeCurrentMessage();
    this->getChildByName("tutorial")->setVisible(false);
    this->getChildByName("tutorialNext")->setVisible(false);
    this->getChildByName("tutorialBorder")->setVisible(false);
    this->getChildByName("tutorialImage")->stopAllActions();
    this->getChildByName("tutorialImage")->setVisible(false);
    _tutorial->removeAllMessages();
    _eventDispatcher->removeEventListener(_listenerTutorial);
    disableTutorialGUI();
    pauseGame();
}

void UIGameplayMap::createTimingThread(void)
{
    pthread_create(&timingThread, NULL, &UIGameplayMap::createTiming, this);
}

void* UIGameplayMap::createTiming(void* arg)
{
    UIGameplayMap* game = (UIGameplayMap*)arg;
    game->startTiming();
    //delete game;
    return nullptr;
}

void UIGameplayMap::startTiming(void)
{
    Timing::getInstance()->start();
    CCLOG("DONE TIMING");
}

void UIGameplayMap::createNewLevelThread(void)
{
    pthread_create(&gameLevelThread, NULL, &UIGameplayMap::createLevel, this);
}

void* UIGameplayMap::createLevel(void* arg)
{
    UIGameplayMap* game = (UIGameplayMap*)arg;
    game->playLevel();
    //delete game;
    return nullptr;
}

void UIGameplayMap::enableTutorialGUI()
{
    this->getChildByName("menuTutorialWindow")->setVisible(true);
}

void UIGameplayMap::disableTutorialGUI()
{
    this->getChildByName("menuTutorialWindow")->setVisible(false);
}


void UIGameplayMap::playLevel(void)
{
    bool launchTutorial = GameData::getInstance()->launchTutorial(GameLevel::getInstance()->getNumLevel());
    delete _tutorial;
    _tutorial = nullptr;

    disableTutorialGUI();

    if(launchTutorial)
    {
        _tutorial = new Tutorial();
        /** delete tutorial if level doesn't have messages **/
        if(!_tutorial->loadTutorial())
        {
            delete _tutorial;
            _tutorial = nullptr;
        }
        else
        {
            enableTutorialGUI();
        }
    }
    GameLevel::getInstance()->playLevel();
    CCLOG("DONE GAME LEVEL");
}

bool UIGameplayMap::selectSpriteForTouch(Node* sprite, Point touchLocation)
{
    return sprite->getBoundingBox().containsPoint(touchLocation);
}

void UIGameplayMap::moveGoalPopup(int index)
{
    Goal * goal = GameLevel::getInstance()->getGoals().at(index);
    if(goal->getGoalType() != Expansion)
    {
        CCLOG("goal not expansion, not implemented TODO");
        return;
    }

    auto area = gameplayMap->getChildByTag(400+index);
    area->stopAllActions();
    area->setVisible(true);
    area->setColor(Color3B(223, 211, 39));
    auto fadeOut = FadeOut::create(2.5);
    area->runAction(fadeOut);

    // if it's not the last goal, highlight the next one
    if(index!=(GameLevel::getInstance()->getGoals().size()-1))
    {
        auto nextArea = gameplayMap->getChildByTag(400+index+1);
        auto blink = Blink::create(2, 2);
        auto repeatBlink = RepeatForever::create(blink);
        nextArea->setColor(Color3B::WHITE);
        nextArea->runAction(repeatBlink);
        nextArea->setOpacity(255);
    }
}

float UIGameplayMap::sqrOfDistanceBetweenPoints(Point p1, Point p2)
{
    Point diff = p1 - p2;
    return diff.x * diff.x + diff.y * diff.y;
}

void UIGameplayMap::checkBackgroundLimitsInTheScreen(Point destPoint)
{
    Size winSize = Director::getInstance()->getVisibleSize();
    float gameplayMapBoundingBoxWidth = gameplayMap->getBoundingBox().size.width;
    float gameplayMapBoundingBoxHeight = gameplayMap->getBoundingBox().size.height;

    // LEFT-RIGHT
    if ((destPoint.x - gameplayMapBoundingBoxWidth / 2) > 0) {
        moveBackgroundLeft = false;
    }
    else {
        moveBackgroundLeft = true;
    }
    if ((destPoint.x + gameplayMapBoundingBoxWidth / 2) < winSize.width) {
        moveBackgroundRight = false;
    }
    else {
        moveBackgroundRight = true;
    }
    // UP-DOWN
    if ((destPoint.y - gameplayMapBoundingBoxHeight / 2) > 0) {
        moveBackgroundDown = false;
    }
    else {
        moveBackgroundDown = true;
    }
    if ((destPoint.y + gameplayMapBoundingBoxHeight / 2) < winSize.height) {
        moveBackgroundUp = false;
    }
    else {
        moveBackgroundUp = true;
    }
}

int UIGameplayMap::getValueAtGameplayMap(int rgb, int posx, int posy)
{
    Point loc(Point(posx, posy));
    loc.y = 320 - loc.y;
    return getValueAtGameplayMap(rgb, loc);
}

Color4B* UIGameplayMap::getExploitedMapTextureData(void)
{
    return exploitedMapTextureData;
}

bool UIGameplayMap::isInBoostResistanceArea(int posx, int posy)
{
    int i = 0;
    while(powerButtons.at(i)->getPower()->getId() != ResistanceBoost)
    {
        i++;
    }
    Vec2 center = ((UIAreaPower*)powerButtons.at(i))->getArea()->getPosition();
    float radius = center.y - ((UIAreaPower*)powerButtons.at(i))->getArea()->getBoundingBox().getMinY();
    return (abs(center.distance(Vec2(posx, posy))) <= radius);
}

void UIGameplayMap::restoreLand(void)
{
    int i = 0;
    while (powerButtons.at(i)->getPower()->getId() != RestoreLand) {
        i++;
    }
    float radius = 37.0; //((UIAreaPower*)powerButtons.at(i))->getScale();
    Point pos = ((UIAreaPower*)powerButtons.at(i))->getArea()->getPosition();
    Point posTransformed;
    posTransformed.x = pos.x / float(GameData::getInstance()->getResourcesWidth() / 480.0),
    posTransformed.y = ((pos.y - ((GameData::getInstance()->getResourcesHeight() - GameData::getInstance()->getResourcesMargin()) / 2)) / float(GameData::getInstance()->getResourcesMargin() / 320.0));
    for (int i = -37; i < 37; i++) {
        for (int j = -37; j < 37; j++) {
            float dist = sqrt((i * i) + (j * j));
            if (dist <= radius and posTransformed.x + i >= 0 and posTransformed.x + i < 480 and posTransformed.y + j >= 0 and posTransformed.y + j < 320) {
                GameLevel::getInstance()->setTimeExploited(posTransformed.x + i, posTransformed.y + j, 0);
                GameLevel::getInstance()->setDepleted(posTransformed.x + i, posTransformed.y + j, false);
                GameLevel::getInstance()->setEnvironmentAdaptation(posTransformed.x + i, posTransformed.y + j, false);
                //drawExploitedMap(Point(posTransformed.x + i, posTransformed.y + j), Color4B(127, 127, 127, 0), 0);
            }
        }
    }
}


int UIGameplayMap::getValueAtGameplayMap(int rgb, Point pt)
{
    unsigned char* pixel;
    int x = 3;

    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    pixel = dataGameplayMapHotSpot + ((int)pt.x + (int)pt.y * gameplayMapHotSpot->getWidth()) * x;

    switch (rgb) {
    case 0: {
        unsigned char r = *pixel;
        return (int)r;
        break;
    }
    case 1: {
        unsigned char g = *(pixel + 1);
        return (int)g;
        break;
    }
    case 2: {
        unsigned char b = *(pixel + 2);
        return (int)b;
        break;
    }
    case 3: {
        unsigned char a = *(pixel + 3);
        return (int)a;
        break;
    }
    default:
        break;
    }
    return 255;
}

void UIGameplayMap::initializeAgents(void)
{
    vector<list<Agent*> > agentsDomain = GameLevel::getInstance()->getAgents();
    for (size_t i = 0; i < agentsDomain.size(); i++)
    {
        for (list<Agent*>::iterator it = agentsDomain.at(i).begin(); it != agentsDomain.at(i).end(); it++)
        {
            Color3B c = GameData::getInstance()->getPlayerColor();
            Color4B color = Color4B(c.r, c.g, c.b, (*it)->getLife() * (255 / 175));
            drawAgent(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()), color, 0);
        }
    }
    agentsTexture->updateWithData(agentsTextureData, 0, 0, GameData::getInstance()->getResourcesWidth(), GameData::getInstance()->getResourcesHeight());
}

void UIGameplayMap::setAttributesToInitialAgents(void)
{
    GameLevel::getInstance()->setAttributesToInitialAgents();
}

void UIGameplayMap::createEndGameWindow(const LevelState & mode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("gui/EndedGameBackground.png");
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background, 9);

    auto window = Sprite::create("gui/EndedGameWindow.png");
    window->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    window->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    string title;
    string text;

    if (mode == Success) {
        //success
        title = LocalizedString::create("LEVEL_COMPLETED");
        text = LocalizedString::create("CONGRATULATIONS");
        int starCount = 0;
        int score = GameData::getInstance()->getLevelScore(GameLevel::getInstance()->getNumLevel());
        while (starCount < 3) {
            if (starCount < score) {
                auto starFull = Sprite::create("gui/StarFull.png");
                starFull->setPosition(((starCount * 3) + 10) * window->getContentSize().width / 18,
                    5 * window->getContentSize().height / 10);
                window->addChild(starFull);
            }
            else {
                auto starEmpty = Sprite::create("gui/StarEmpty.png");
                starEmpty->setPosition(((starCount * 3) + 10) * window->getContentSize().width / 18,
                    5 * window->getContentSize().height / 10);
                window->addChild(starEmpty);
            }
            starCount++;
        }
        auto titleLabel = Label::createWithTTF(title, "fonts/BebasNeue.otf", 120 * GameData::getInstance()->getRaConversion());
        titleLabel->setColor(Color3B(255, 255, 255));
        titleLabel->setPosition(5 * window->getContentSize().width / 18, 5 * window->getContentSize().height / 10);
        window->addChild(titleLabel);
        // turn off tutorial for successful levels
        GameData::getInstance()->setTutorial(GameLevel::getInstance()->getNumLevel(), false);
    }
    else {
        //game over
        title = LocalizedString::create("GAME_OVER");

        if (mode == GoalFailBefore) {
            text = LocalizedString::create("GOAL_FAIL_BEFORE");
        }
        else if (mode == GoalFailAfter) {
            text = LocalizedString::create("GOAL_FAIL_AFTER");
        }
        else if (mode == NoAgentsLeft) {
            text = LocalizedString::create("ALL_AGENTS_DIED");
        }

        auto titleLabel = Label::createWithTTF(title, "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
        titleLabel->setColor(Color3B(255, 255, 255));
        titleLabel->setPosition(9 * window->getContentSize().width / 18, 4 * window->getContentSize().height / 10);
        window->addChild(titleLabel);

        auto textLabel = Label::createWithTTF(text, "fonts/arial_rounded_mt_bold.ttf", 40 * GameData::getInstance()->getRaConversion());
        textLabel->setPosition(9 * window->getContentSize().width / 18, 6 * window->getContentSize().height / 10);
        textLabel->setAlignment(TextHAlignment::CENTER);
        window->addChild(textLabel);
    }

    string space = " ";
    string lvl = LocalizedString::create("LEVEL") + space + to_string(GameLevel::getInstance()->getNumLevel());
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 120 * GameData::getInstance()->getRaConversion());
    levelLabel->setColor(Color3B(85, 108, 117));
    levelLabel->setPosition(Vec2(4 * window->getContentSize().width / 18, 8.5 * window->getContentSize().height / 10));
    window->addChild(levelLabel);

    auto continueButton = MenuItemImage::create("gui/ProgressMapPlayButton.png", "gui/ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    continueButton->setPosition(14 * window->getContentSize().width / 18, 1.5 * window->getContentSize().height / 10);
    auto continueLabel = Label::createWithTTF(LocalizedString::create("CONTINUE"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    continueLabel->setColor(Color3B(205, 202, 207));
    continueLabel->setPosition(continueButton->getContentSize().width / 2, continueButton->getContentSize().height / 2);
    continueButton->addChild(continueLabel);
    auto continueMenu = Menu::createWithItem(continueButton);
    continueMenu->setPosition(0, 0);
    window->addChild(continueMenu);

    auto retryButton = MenuItemImage::create("gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::retryOkCallback, this));
    retryButton->setPosition(4 * window->getContentSize().width / 18, 1.5 * window->getContentSize().height / 10);
    auto retryLabel = Label::createWithTTF(LocalizedString::create("RETRY"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    retryLabel->setColor(Color3B(205, 202, 207));
    retryLabel->setPosition(retryButton->getContentSize().width / 2, retryButton->getContentSize().height / 2);
    retryButton->addChild(retryLabel);
    auto retryMenu = Menu::createWithItem(retryButton);
    retryMenu->setPosition(0, 0);
    window->addChild(retryMenu);
    
    if (GameData::getInstance()->getFirstTimeLevelCompleted() == GameLevel::getInstance()->getNumLevel())
    {
        retryButton->setVisible(false);
        continueButton->setPositionX(window->getContentSize().width / 2);
    }
    
    //delete wave nodes
    this->getChildByName("menuGraphic")->getChildByName("graphicBackground")->removeAllChildren();
    ((MenuItem*)this->getChildByName("timeMenu")->getChildByName("playToggle"))->setEnabled(false);
    
    this->addChild(window, 10);
}

void UIGameplayMap::createAchievementWindow(void)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int numCompletedAchievements = (int)GameLevel::getInstance()->getCompletedAchievements().size();
    auto window = MenuItemImage::create("gui/AchievementsBackground.png", "gui/AchievementsBackground.png",
                                        CC_CALLBACK_1(UIGameplayMap::hideAchievementWindowCallback, this));
    window->setPosition(Vec2(visibleSize.width /2, visibleSize.height + window->getContentSize().height));
    window->setScale(GameData::getInstance()->getRaWConversion() * 0.6,
                     GameData::getInstance()->getRaHConversion() * (0.3 + (0.1 * numCompletedAchievements)));
    window->setName("achievementWindow");
    
    string s = "";
    if (numCompletedAchievements > 1)
    {
        s = "S";
    }
    auto titleLabel = Label::createWithTTF("YOU HAVE UNLOCKED " + to_string(numCompletedAchievements) + " ACHIEVEMENT" + s,
                                           "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    titleLabel->setColor(Color3B(255, 255, 255));
    titleLabel->setAnchorPoint(Vec2(0.5, 0));
    titleLabel->setPosition(Vec2(window->getContentSize().width / 2, window->getContentSize().height / 2 - titleLabel->getContentSize().height * 1.1));
    window->addChild(titleLabel);
    
    for (int i = 0; i < numCompletedAchievements; i++)
    {
        string titleAch = LocalizedString::create(("TITLE_LVL" + GameLevel::getInstance()->getCompletedAchievements().at(i)).c_str(), "achievements");
        string descrAch = LocalizedString::create(("DESCR_LVL" + GameLevel::getInstance()->getCompletedAchievements().at(i)).c_str(), "achievements");
        auto labelAch = Label::createWithTTF(titleAch + ": " + descrAch, "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
        labelAch->setColor(Color3B(85, 108, 117));
        labelAch->setPosition(Vec2(window->getContentSize().width / 2,
                                   (titleLabel->getPositionY() - titleLabel->getContentSize().height) - (labelAch->getContentSize().height * 1.2 * i)));
        window->addChild(labelAch);
    }
    
    auto menuWindow = Menu::create(window, NULL);
    menuWindow->setPosition(Vec2(0, 0));
    this->addChild(menuWindow, 10);
    window->runAction(EaseBackOut::create(MoveTo::create(1, Vec2(visibleSize.width /2, visibleSize.height))));
}

void UIGameplayMap::createInGameAchievementWindow(Achievement * ach)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto window = MenuItemImage::create("gui/AchievementsBackground.png", "gui/AchievementsBackground.png",
                                        CC_CALLBACK_1(UIGameplayMap::hideAchievementWindowCallback, this));
    window->setPosition(Vec2(visibleSize.width /2, visibleSize.height + window->getContentSize().height));
    window->setScale(GameData::getInstance()->getRaWConversion() * 0.6,
                     GameData::getInstance()->getRaHConversion() * 0.4);
    window->setName("achievementWindow");
    
    auto titleLabel = Label::createWithTTF("YOU HAVE UNLOCKED 1 ACHIEVEMENT", "fonts/BebasNeue.otf",
                                           100 * GameData::getInstance()->getRaConversion());
    titleLabel->setColor(Color3B(255, 255, 255));
    titleLabel->setAnchorPoint(Vec2(0.5, 0));
    titleLabel->setPosition(Vec2(window->getContentSize().width / 2, window->getContentSize().height / 2 - titleLabel->getContentSize().height * 1.1));
    window->addChild(titleLabel);
    
    string key = to_string(ach->getLevel())+"_"+ach->getGoalType();
    
        string titleAch = LocalizedString::create(("TITLE_LVL" + key).c_str(), "achievements");
        string descrAch = LocalizedString::create(("DESCR_LVL" + key).c_str(), "achievements");
        auto labelAch = Label::createWithTTF(titleAch + ": " + descrAch, "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
        labelAch->setColor(Color3B(85, 108, 117));
        labelAch->setPosition(Vec2(window->getContentSize().width / 2,
                                   (titleLabel->getPositionY() - titleLabel->getContentSize().height) - (labelAch->getContentSize().height * 1.2)));
        window->addChild(labelAch);
    
    auto menuWindow = Menu::create(window, NULL);
    menuWindow->setPosition(Vec2(0, 0));
    this->addChild(menuWindow, 10);
    window->runAction(EaseBackOut::create(MoveTo::create(1, Vec2(visibleSize.width /2, visibleSize.height))));
}

void UIGameplayMap::updateAgents(void)
{

    pthread_mutex_lock(&gameLevelMutex);
    vector<list<Agent*> > agentsDomain = GameLevel::getInstance()->getAgents();

    Color4B transparent = Color4B::WHITE;
    transparent.a = 0;
    for (size_t i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++)
    {
        drawAgent(GameLevel::getInstance()->getDeletedAgents().at(i), transparent);
    }

    Color3B agentColorPlayer = GameData::getInstance()->getPlayerColor();
    for (int i = agentsDomain.size() - 1; i >= 0 ; i--)
    {
        int resourcesPainted = 0;
        for (list<Agent*>::iterator it = agentsDomain.at(i).begin(); it != agentsDomain.at(i).end(); ++it)
        {
            Color4B color;
            switch (agentColor) {
            //wood
            case 1:
                color = Color4B(0, 249, 105, 255);
                break;
            //mineral
            case 2:
                color = Color4B(229, 232, 5, 255);
                break;
            //stone
            case 3:
                color = Color4B(225, 144, 57, 255);
                break;
            //normal
            default:
                switch ((*it)->getType()) {
                case 1:
                    color = Color4B(0, 248, 251, (*it)->getLife() * (255 / 175));
                    break;
                case 2:
                    color = Color4B(210, 214, 47, (*it)->getLife() * (255 / 175));
                    break;
                case 3:
                    color = Color4B(68, 165, 195, (*it)->getLife() * (255 / 175));
                    break;
                default:
                    color = Color4B(agentColorPlayer.r, agentColorPlayer.g, agentColorPlayer.b, (*it)->getLife() * (255 / 175));
                    break;
                }
                break;
            }
            
            //check num_agents painted in accordance with num_resources if not painting population
            if (agentColor > 0)
            {
                if (resourcesPainted < Agent::_resourcesPool.at(i).at(agentColor - 1))
                {
                    resourcesPainted++;
                    Color4B colorBorder;
                    switch ((*it)->getType()) {
                        case 1:
                            colorBorder = Color4B(0, 248, 251, 255);
                            break;
                        case 2:
                            colorBorder = Color4B(210, 214, 47, 255);
                            break;
                        case 3:
                            colorBorder = Color4B(68, 165, 195, 255);
                            break;
                        default:
                            colorBorder = Color4B(agentColorPlayer.r, agentColorPlayer.g, agentColorPlayer.b, 255);
                            break;
                    }
                    drawAgent(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()), color, 1, colorBorder);
                }
                else
                {
                    color = transparent;
                    drawAgent(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()), color);
                }

            }
            else
            {
                drawAgent(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()), color);
  
            }
            if (GameLevel::getInstance()->getDepleted((*it)->getPosition().getX(), (*it)->getPosition().getY()) == true) {
                drawExploitedMap(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()), Color4B(0,0,0,0));
            }
            /*if (GameLevel::getInstance()->getEnvironmentAdaptation((*it)->getPosition().getX(), (*it)->getPosition().getY()) == true) {
                drawExploitedMap(Point((*it)->getPosition().getX(), (*it)->getPosition().getY()),
                    Color4B(0, 0, 0, 50));
            }*/
        }
    }

    agentsTexture->updateWithData(agentsTextureData, 0, 0, GameData::getInstance()->getResourcesWidth(), GameData::getInstance()->getResourcesHeight());
    if (resourcesMap) {
        //restore map
        for (int i = 0; i < GameLevel::getInstance()->getRestored().size(); i++)
        {
            drawExploitedMap(Point(GameLevel::getInstance()->getRestored().at(i).x, GameLevel::getInstance()->getRestored().at(i).y), Color4B(255,255,255,255));
        }
        GameLevel::getInstance()->clearRestored();
        exploitedMapTexture->updateWithData(exploitedMapTextureData, 0, 0, GameData::getInstance()->getResourcesWidth(), GameData::getInstance()->getResourcesHeight());
    }
    //pthread_mutex_unlock(&gameLevelMutex);
}

void UIGameplayMap::drawAgent(Point pos, Color4B colour, int geometry, Color4B colorBorder)
{
    int x = (int)(pos.x * GameData::getInstance()->getRowDrawAgentPrecalc());
    int y = (int)(GameData::getInstance()->getColumnOffsetDrawAgentPrecalc() + ((pos.y) * GameData::getInstance()->getColumnDrawAgentPrecalc()));
    int position = x + ((GameData::getInstance()->getResourcesHeight() - y) * GameData::getInstance()->getResourcesWidth());

    switch (geometry) {
    case 1: {
        int k = -GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize();
        while (k <= GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize()) {
            for (int j = -GameLevel::getInstance()->getAgentPixelSize(); j < GameLevel::getInstance()->getAgentPixelSize() + 1; j++) {
                agentsTextureData[position + j + k] = colorBorder;
            }
            k += GameData::getInstance()->getResourcesWidth();
        }
        
        k = -GameData::getInstance()->getResourcesWidth() * (GameLevel::getInstance()->getAgentPixelSize() / 2);
        while (k <= GameData::getInstance()->getResourcesWidth() * (GameLevel::getInstance()->getAgentPixelSize() / 2)) {
            for (int j = -(GameLevel::getInstance()->getAgentPixelSize()/2); j < (GameLevel::getInstance()->getAgentPixelSize() /2) + 1; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            k += GameData::getInstance()->getResourcesWidth();
        }
        break;
    }
    case 2: {
        int k = -GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize();
        int times = 0;
        while (k <= GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize()) {
            for (int j = times; j < abs(times) - 1; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            times--;
            k += GameData::getInstance()->getResourcesWidth();
        }
        break;
    }
    default:
        int k = -GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize();
        while (k <= GameData::getInstance()->getResourcesWidth() * GameLevel::getInstance()->getAgentPixelSize()) {
            for (int j = -GameLevel::getInstance()->getAgentPixelSize(); j < GameLevel::getInstance()->getAgentPixelSize() + 1; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            k += GameData::getInstance()->getResourcesWidth();
        }
        break;
    }
}

void UIGameplayMap::drawExploitedMap(Point pos, Color4B colour, int geometry)
{
    /*int x = (int)(pos.x * float(GameData::getInstance()->getResourcesWidth() / 480.0));
    int y = (int)(float((GameData::getInstance()->getResourcesHeight() - GameData::getInstance()->getResourcesMargin()) / 2.0) + ((pos.y) * float(GameData::getInstance()->getResourcesMargin() / 320.0)));
    int position = x + ((GameData::getInstance()->getResourcesHeight() - y) * GameData::getInstance()->getResourcesWidth());*/
    int x = (int)(pos.x * GameData::getInstance()->getRowDrawAgentPrecalc());
    int y = (int)(GameData::getInstance()->getColumnOffsetDrawAgentPrecalc() + ((pos.y) * GameData::getInstance()->getColumnDrawAgentPrecalc()));
    int position = x + ((GameData::getInstance()->getResourcesHeight() - y) * GameData::getInstance()->getResourcesWidth());    
    switch (geometry) {
    default:
        int k = -GameData::getInstance()->getResourcesWidth()* GameLevel::getInstance()->getAgentPixelSize();
        while (k <= GameData::getInstance()->getResourcesWidth()* GameLevel::getInstance()->getAgentPixelSize()) {
            for (int j = - GameLevel::getInstance()->getAgentPixelSize(); j < GameLevel::getInstance()->getAgentPixelSize() + 1; j++) {
                if (colour.r == 0)
                {
                    if (Color3B(exploitedMapTextureData[position + j + k]) == Color3B::BLACK)
                    {
                        exploitedMapTextureData[position + j + k].a = 1;
                    }
                    else
                    {
                        exploitedMapTextureData[position + j + k].a = 0;
                    }
                }
                else
                {
                    if (exploitedMapTextureData[position + j + k].a != 0)
                    {
                        exploitedMapTextureData[position + j + k].a = 1;
                    }
                    else
                    {
                        exploitedMapTextureData[position + j + k].a = 255;
                    }
                }
            }
            k += GameData::getInstance()->getResourcesWidth();
        }
        break;
    }
}

void UIGameplayMap::createTutorialGUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // TUTORIAL MESSAGES
    auto messageLabel = Label::createWithTTF("no message", "fonts/arial_rounded_mt_bold.ttf", 40 * GameData::getInstance()->getRaConversion());
    messageLabel->setName("tutorial");
    messageLabel->setColor(Color3B(230, 230, 230));
    messageLabel->enableShadow();
    messageLabel->setMaxLineWidth(300);
    messageLabel->setVisible(false);
    messageLabel->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    auto messageNextLabel = Label::createWithTTF("", "fonts/arial_rounded_mt_bold.ttf", 40 * GameData::getInstance()->getRaConversion());
    messageNextLabel->setName("tutorialNext");
    messageNextLabel->setColor(Color3B(210, 210, 210));
    messageNextLabel->setMaxLineWidth(300);
    messageNextLabel->setVisible(false);
    messageNextLabel->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    auto labelBorder = DrawNode::create();
    labelBorder->setName("tutorialBorder");
    labelBorder->setVisible(false);

    auto tutorialWindow = MenuItemToggle::createWithCallback(CC_CALLBACK_1(UIGameplayMap::skipTutorial, this), MenuItemImage::create("gui/TutorialWindow.png", "gui/TutorialWindow.png"), MenuItemImage::create("gui/TutorialWindowPressed.png", "gui/TutorialWindowPressed.png"), nullptr);
    tutorialWindow->setAnchorPoint(Vec2(0, 0.5));
    tutorialWindow->setPosition(Vec2(5 * visibleSize.width / 204, 125 * visibleSize.height / 155));
    tutorialWindow->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    tutorialWindow->setName("tutorialWindow");
    
    auto skipWindow = MenuItemImage::create("gui/TutorialWindowSkip.png", "gui/TutorialWindowSkip.png", CC_CALLBACK_1(UIGameplayMap::skipTutorialConfirm, this));
    skipWindow->setName("skipWindow");
    skipWindow->setAnchorPoint(Vec2(0, 0.5));
    skipWindow->setPosition(Vec2(tutorialWindow->getPositionX() + tutorialWindow->getBoundingBox().size.width / 1.05, tutorialWindow->getPositionY() + skipWindow->getBoundingBox().size.height / 1.5));
    skipWindow->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    skipWindow->setOpacity(0);
    skipWindow->setVisible(false);
    
    auto skipLabel = Label::createWithTTF("SKIP", "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    skipLabel->setPosition(Vec2(skipWindow->getContentSize().width / 2, skipWindow->getContentSize().height / 2));
    skipLabel->setColor(Color3B(139, 146, 154));
    skipWindow->addChild(skipLabel);
    
    auto menuTutorialWindow = Menu::create(tutorialWindow, skipWindow, nullptr);
    menuTutorialWindow->setName("menuTutorialWindow");
    menuTutorialWindow->setPosition(Vec2::ZERO);
    menuTutorialWindow->setVisible(false);
    
    auto tutorialTitle = Label::createWithTTF("TUTORIAL", "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    tutorialTitle->setName("tutorialTitle");
    tutorialTitle->setColor(Color3B(139, 146, 154));
    tutorialTitle->setPosition(Vec2(3.3 * tutorialWindow->getContentSize().width / 7, 2.3 * tutorialWindow->getContentSize().height / 5));
    tutorialWindow->addChild(tutorialTitle, 5);
    
    auto tutorialImage = Sprite::create();
    tutorialImage->setName("tutorialImage");
    tutorialImage->setVisible(false);
    tutorialImage->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    // add first the border to draw it first
    this->addChild(labelBorder);
    this->addChild(messageLabel);
    this->addChild(messageNextLabel);
    this->addChild(menuTutorialWindow);
    this->addChild(tutorialImage, 100);
}

void UIGameplayMap::update(float delta)
{
    if (GameLevel::getInstance()->getFinishedGame() == Running) {
        if (GameLevel::getInstance()->paint == true and GameLevel::getInstance()->ended == false)
        {
            if(_tutorial)
            {
                if(_tutorial->checkNextMessage())
                {
                    setMessage(_tutorial->getCurrentMessage());
                }
                else if(_tutorial->getCurrentMessage() && _tutorial->getCurrentMessage()->meetsPostCondition())
                {
                    _tutorial->removeCurrentMessage();
                    this->getChildByName("tutorial")->setVisible(false);
                    this->getChildByName("tutorialNext")->setVisible(false);
                    this->getChildByName("tutorialBorder")->setVisible(false);
                    this->getChildByName("tutorialImage")->stopAllActions();
                    this->getChildByName("tutorialImage")->setVisible(false);

                    if(_tutorial->isFinished())
                    {
                        _eventDispatcher->removeEventListener(_listenerTutorial);
                        disableTutorialGUI();
                        pauseGame();
                    }
                }
            }
       
            updateAgents();
            
            if (gameplayMap->getChildByName("labelInfluence") != nullptr and Agent::_numInfluenced.at(0) > 0)
            {
                int x = (int)(GameLevel::getInstance()->getAgents().at(0).front()->getPosition().getX() * GameData::getInstance()->getRowDrawAgentPrecalc());
                int y = (int)(GameData::getInstance()->getColumnOffsetDrawAgentPrecalc() + ((GameLevel::getInstance()->getAgents().at(0).front()->getPosition().getY()) * GameData::getInstance()->getColumnDrawAgentPrecalc()));
                auto labelInfluenced = (Label*)gameplayMap->getChildByName("labelInfluence");
                labelInfluenced->setString(to_string(Agent::_numInfluenced.at(0)));
                labelInfluenced->setPosition(Vec2(x, y));
                labelInfluenced->runAction(Sequence::create(FadeIn::create(0.1), MoveBy::create(0.2, Vec2(0, 5.0)), FadeOut::create(0.1), NULL));
            }
            // TODO everything stopped if _message?
            updateWave(0, int(GameLevel::getInstance()->getAgents().at(0).size()), GameLevel::getInstance()->getMaxAgents().at(0), Color4B(179, 205, 221, 255));
            updateWave(1, Agent::_resourcesPool.at(0).at(Wood), 2000, Color4B(0, 249, 105, 255));
            updateWave(2, Agent::_resourcesPool.at(0).at(Mineral), 2000, Color4B(229, 232, 5, 255));
            updateWave(3, Agent::_resourcesPool.at(0).at(Stone), 2000, Color4B(225, 144, 57, 255));
            
            pthread_mutex_unlock(&gameLevelMutex);
        }
        if (GameLevel::getInstance()->getGoals().empty() == false)
        {
            float value = std::min(100.0f, Timing::getInstance()->getTimeStep()/float(GameLevel::getInstance()->getGoals().back()->getMaxTime())*100.0f);
            // min with 100 and percentage because Timing thread is faster than gamelevel thread (checking goals) and this thread (painting). it will add some diff to _timeStep before checking for fail
            timeBar->setPercentage(value);
            timeBorderBar->getChildByName("degradateTime")->setPositionX(timeBorderBar->getContentSize().width * (value/100.0f));
        }

        for (size_t i = 0; i < powerButtons.size(); i++) {
            powerButtons.at(i)->update(delta);
        }

        if (std::atoi(evolutionPointsLabel->getString().c_str()) != GameLevel::getInstance()->getEvolutionPoints())
        {
            evolutionPointsLabel->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));
            updateAttributesButtons();
        }
        if (drawInGameAchievementWindow)
        {
            createInGameAchievementWindow(GameLevel::getInstance()->getInGameAchievement());
            drawInGameAchievementWindow = false;
            GameLevel::getInstance()->setInGameAchievement(nullptr);
        }
    }
    else if (GameLevel::getInstance()->getFinishedGame() != Running and endGameWindowPainted == false and GameLevel::getInstance()->ended == true)
    {
        updateAgents();
        if (GameLevel::getInstance()->getGoals().empty() == false)
        {
            float value = std::min(100.0f, Timing::getInstance()->getTimeStep()/float(GameLevel::getInstance()->getGoals().back()->getMaxTime())*100.0f);
            timeBar->setPercentage(value);
        }

        for (size_t i = 0; i < powerButtons.size(); i++) {
            powerButtons.at(i)->update(delta);
        }
        pthread_mutex_unlock(&gameLevelMutex);


        evolutionPointsLabel->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));

        createEndGameWindow(GameLevel::getInstance()->getFinishedGame());
        
        //HAS COMPLETED ANY ACHIEVEMENT
        if (GameLevel::getInstance()->getCompletedAchievements().size() > 0)
        {
            createAchievementWindow();
        }
        
        endGameWindowPainted = true;
    }
}

void UIGameplayMap::setMessage( const Message * message )
{
    restoreGameplayMap();
    pauseGame();

    _message = message;

    Label * label = (Label*)(this->getChildByName("tutorial"));
    Label * nextLabel = (Label*)(this->getChildByName("tutorialNext"));
    Sprite* image = (Sprite*)(this->getChildByName("tutorialImage"));

    label->setString(_message->text());
    Size visibleSize = Director::getInstance()->getVisibleSize();
    label->setMaxLineWidth(_message->lineWidth()*visibleSize.width);
    nextLabel->setMaxLineWidth(_message->lineWidth()*visibleSize.width);
    Vec2 position = Vec2(visibleSize.width*_message->pos().x, visibleSize.height*_message->pos().y);
    label->setPosition(position);

    auto labelBorder = (DrawNode*)(this->getChildByName("tutorialBorder"));
    labelBorder->clear();
    if(_message->text()!="")
    {
        // condition to close the message
        if(message->getPostCondition()=="tapButton")
        {
            nextLabel->setString(LocalizedString::create("NEXT_MESSAGE_BUTTON", "tutorial"));
        }
        else if(message->getPostCondition()=="spot")
        {
            nextLabel->setString(LocalizedString::create("NEXT_MESSAGE_SPOT", "tutorial"));
        }
        else
        {
            nextLabel->setString(LocalizedString::create("NEXT_MESSAGE_TAP", "tutorial"));
        }

        // if message shows an image in a particular spot
        if(message->getSpot()!=nullptr)
        {
            const Spot * spot = message->getSpot();
            image->setTexture(spot->_image+".png");
            image->setPosition(Vec2(visibleSize.width*spot->_centerX, visibleSize.height*spot->_centerY));
            image->setVisible(true);
            image->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.9, 70), FadeTo::create(1, 255), nullptr)));
        }
        
        const Rect & contents = label->getBoundingBox();
        const Rect & ownContents = nextLabel->getBoundingBox();
        nextLabel->setPosition(Vec2(contents.getMaxX()-(ownContents.size.width/2), contents.getMinY()-20-(ownContents.size.height/2)));
        
        label->setVisible(true);    
        labelBorder->setVisible(true);   
        nextLabel->setVisible(true);

    }

    Vec2 origin(label->getBoundingBox().origin - Vec2(20.0f, 20.0f));
    Vec2 end(label->getBoundingBox().origin + label->getBoundingBox().size + Vec2(20.0f, 20.0f));
    labelBorder->drawSolidRect(origin, end, Color4F(0.07f, 0.36f, 0.52f, 0.2f));
    labelBorder->drawRect(origin, end, Color4F(0.71f, 0.83f, 0.89f, 1.0f));

    auto pauseDarkBackground = this->getChildByName("pauseDarkBackground");
    pauseDarkBackground->setVisible(false);
}

void UIGameplayMap::updateWave(int index, int variable, int maxVariable, Color4B color)
{
    auto graphicBackground = (MenuItem*)this->getChildByName("menuGraphic")->getChildByName("graphicBackground");
    
    float height = float(variable)/float(maxVariable) * graphicBackground->getContentSize().height * GameData::getInstance()->getRaHConversion();

    // Space the verticies out evenly across the screen for the wave.
    float vertexHorizontalSpacing = graphicBackground->getContentSize().width * GameData::getInstance()->getRaWConversion()/ float(GameLevel::getInstance()->getGoals().back()->getMaxTime());
    
    // Used to increment to the next vertexX position.
    float currentWaveVertX = this->convertToWorldSpace(graphicBackground->getPosition()).x - (graphicBackground->getContentSize().width * GameData::getInstance()->getRaWConversion() / 2);

    WavePoint w;
    w.x = currentWaveVertX + vertexHorizontalSpacing * Timing::getInstance()->getTimeStep();
    w.y = height + this->convertToWorldSpace(graphicBackground->getPosition()).y - (graphicBackground->getContentSize().height * GameData::getInstance()->getRaHConversion() / 2);
    w.z = 0;
    waveNodes.at(index)->addToDynamicVerts3D(w, color);
}

void UIGameplayMap::restoreGameplayMap(void)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    gameplayMap->runAction(Spawn::create(ScaleTo::create(0.3, GameData::getInstance()->getRaWConversion(),
                                                         GameData::getInstance()->getRaHConversion()),
                                         MoveTo::create(0.3, Vec2(visibleSize.width / 2, visibleSize.height / 2)),
                                         NULL));
}

void UIGameplayMap::updateAttributesButtons(void)
{
    Menu* attributesMenu = (Menu*)this->getChildByName("bottomFrame")->getChildByTag(100000);
    for (size_t i = 0; i < GameLevel::getInstance()->getModifiableAttr().size(); i++)
    {
        if(GameLevel::getInstance()->getModifiableAttr().at(i)==-1)
        {
            continue;
        }
        MenuItem* minus = (MenuItem*) attributesMenu->getChildByTag(int(i) + 10);
        MenuItem* plus = (MenuItem*) attributesMenu->getChildByTag(int(i) + 50);
        if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) > GameLevel::getInstance()->getEvolutionPoints())
        {
            plus->setEnabled(false);
            minus->setEnabled(false);
        }
        else
        {
            plus->setEnabled(true);
            minus->setEnabled(true);
        }
        if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) >= 5)
        {
            plus->setEnabled(false);
        }
        else if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), GameLevel::getInstance()->getModifiableAttr().at(i)) <= 0)
        {
            minus->setEnabled(false);
        }
    }
}

