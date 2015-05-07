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
#include "UIMultiplierPower.h"
#include "UIAreaPower.h"
#include "CollectionGoal.h"
#include "ExpansionGoal.h"
#include "Timing.h"
#include "LevelLoader.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIGameplayMap::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGameplayMap::create();
    scene->addChild(layer);
    return scene;
}

bool UIGameplayMap::init()
{
    if (!Layer::init()) {
        return false;
    }

    //this->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    //Director::getInstance()->getTextureCache()->addImage("Agent.png");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Size contentSize;
    contentSize.width = 2048;
    contentSize.height = 1536;

    string map = GameLevel::getInstance()->getMapFilename();
    //fer DEFINES
    string ext = ".png";
    string background = "Background";
    string hotSpotsBase = "HotSpotsBase";
    string resources = "Resources";
    string forest = "Forest";

    //Set background gameplay map and all its functionalities
    gameplayMap = Sprite::create(map + background + ".jpg");
    gameplayMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(gameplayMap, 0);

    //FINGER SPOT
    fingerSpot = MenuItemImage::create("FingerSpot.png", "FingerSpot.png", CC_CALLBACK_1(UIGameplayMap::removeFingerSpot, this));
    fingerSpot->setVisible(false);
    fingerSpot->setAnchorPoint(Vec2(0.5, 0));
    Menu* mFinger = Menu::createWithItem(fingerSpot);
    mFinger->setPosition(0, 0);
    gameplayMap->addChild(mFinger, 3);
    /*fingerSpotArea = Sprite::create("FingerSpotArea.png");
    fingerSpotArea->setVisible(false);
    gameplayMap->addChild(fingerSpotArea, 2);*/

    //FRAMES
    auto topFrame = Sprite::create("FrameTop.png");
    topFrame->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    topFrame->setPosition(visibleSize.width / 2, visibleSize.height - (topFrame->getBoundingBox().size.height / 2));
    this->addChild(topFrame, 1);
    auto bottomFrame = Sprite::create("FrameBottom.png");
    bottomFrame->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    bottomFrame->setPosition(visibleSize.width / 2, bottomFrame->getBoundingBox().size.height / 2);
    this->addChild(bottomFrame, 1);

    string space = " ";
    string lvl = LocalizedString::create("LEVEL")->getCString() + space + to_string(GameLevel::getInstance()->getNumLevel());
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 136);
    levelLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    levelLabel->setColor(Color3B(139, 146, 154));
    levelLabel->setAnchorPoint(Vec2(0, 0.5));
    levelLabel->setPosition(Vec2(13 * visibleSize.width / 204, 145 * visibleSize.height / 155));
    this->addChild(levelLabel, 5);

    //QUIT / RETRY
    Vector<MenuItem*> quitRetryVec;
    MenuItem* quitButton = MenuItemImage::create("Quit.png", "QuitPressed.png", CC_CALLBACK_1(UIGameplayMap::quitCallback, this));
    quitButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    quitButton->setPosition(Vec2(quitButton->getBoundingBox().size.width / 2,
        visibleSize.height - (quitButton->getBoundingBox().size.height / 2)));
    quitRetryVec.pushBack(quitButton);

    MenuItem* retryButton = MenuItemImage::create("Repeat.png", "RepeatPressed.png", CC_CALLBACK_1(UIGameplayMap::retryCallback, this));
    retryButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    retryButton->setPosition(Vec2(retryButton->getBoundingBox().size.width / 2,
        quitButton->getPositionY() - quitButton->getBoundingBox().size.height));
    quitRetryVec.pushBack(retryButton);

    Menu* quitRetryMenu = Menu::createWithArray(quitRetryVec);
    quitRetryMenu->setPosition(0, 0);
    this->addChild(quitRetryMenu, 10);

    //HOTSPOT
    gameplayMapHotSpot = new Image();
    gameplayMapHotSpot->initWithImageFile(map + hotSpotsBase + ext);
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    dataGameplayMapHotSpot = new unsigned char[gameplayMapHotSpot->getDataLen() * x];
    dataGameplayMapHotSpot = gameplayMapHotSpot->getData();

    //RESOURCES MAP (IF ANY)
    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        if (GameLevel::getInstance()->getGoals()[i]->getGoalType() == "Collection") {
            resourcesMap = true;
        }
    }
    if (resourcesMap) {
        gameplayMapResources = new Image();
        gameplayMapResources->initWithImageFile(map + resources + ext);
        x = 3;
        if (gameplayMapResources->hasAlpha()) {
            x = 4;
        }
        dataGameplayMapResources = new unsigned char[gameplayMapResources->getDataLen() * x];
        dataGameplayMapResources = gameplayMapResources->getData();

        exploitedMapTexture = new Texture2D;
        Image* im = new Image();
        im->initWithImageFile(map + forest + ext);
        //4 = alpha
        unsigned char* data = new unsigned char[im->getDataLen() * 4];
        data = im->getData();

        exploitedMapTexture->initWithData(exploitedMapTextureData, 2048 * 1536, Texture2D::PixelFormat::RGBA8888, 2048, 1536, contentSize);
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
    agentsTexture->initWithData(agentsTextureData, 2048 * 1536, Texture2D::PixelFormat::RGBA8888,
        2048, 1536, contentSize);
    agentsSprite = Sprite::createWithTexture(agentsTexture);
    agentsSprite->setPosition(Vec2(gameplayMap->getBoundingBox().size.width / 2, gameplayMap->getBoundingBox().size.height / 2));
    gameplayMap->addChild(agentsSprite, 2);
    agentsSprite->setCascadeOpacityEnabled(true);

    //EVOLUTION POINTS
    //string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
    auto evolutionPointsIcon = Sprite::create("EvolutionPoints.png");
    evolutionPointsIcon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    evolutionPointsIcon->setPosition(159 * visibleSize.width / 204, 11 * visibleSize.height / 155);
    evolutionPointsLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getEvolutionPoints()),
        "fonts/BebasNeue.otf", 80);
    evolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
    evolutionPointsLabel->setColor(Color3B(216, 229, 235));
    evolutionPointsIcon->addChild(evolutionPointsLabel, 2);
    this->addChild(evolutionPointsIcon, 1);

    auto evolutionPointsStringLabel = Label::createWithTTF(string(LocalizedString::create("EVOLUTION_POINTS")->getCString()),
        "fonts/BebasNeue.otf", 40);
    evolutionPointsStringLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    evolutionPointsStringLabel->setColor(Color3B(216, 229, 235));
    evolutionPointsStringLabel->setPosition(evolutionPointsIcon->getPosition().x + (20 * visibleSize.width / 204),
        evolutionPointsIcon->getPosition().y);
    this->addChild(evolutionPointsStringLabel, 3);

    //FER DINAMIC
    if (GameLevel::getInstance()->getNumLevel() == 2) {
        collect1PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()),
            "Arial", 65);
        collect1PointsLabel->setPosition(visibleSize.width - 80, visibleSize.height - 300);
        this->addChild(collect1PointsLabel, 1);
    }

    //TIME BUTTONS
    Vector<MenuItem*> timeButtons;
    MenuItem* fastForwardButton = MenuItemImage::create(
        "FastForwardButton.png", "FastForwardButtonPressed.png", "FastForwardButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::fastForwardCallback, this));
    fastForwardButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    fastForwardButton->setPosition(Vec2(193 * visibleSize.width / 204, 145 * visibleSize.height / 155));
    timeButtons.pushBack(fastForwardButton);

    MenuItem* playButton = MenuItemImage::create(
        "PlayButton.png", "PlayButtonPressed.png", "PlayButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::playCallback, this));
    playButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    playButton->setPosition(Vec2(fastForwardButton->getPosition().x - (14 * visibleSize.width / 204),
        fastForwardButton->getPosition().y));
    playButton->setEnabled(false);
    timeButtons.pushBack(playButton);

    MenuItem* pauseButton = MenuItemImage::create(
        "PauseButton.png", "PauseButtonPressed.png", "PauseButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::pauseCallback, this));
    pauseButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    pauseButton->setPosition(Vec2(playButton->getPosition().x - (14 * visibleSize.width / 204),
        playButton->getPosition().y));
    timeButtons.pushBack(pauseButton);

    Menu* timeMenu = Menu::createWithArray(timeButtons);
    timeMenu->setPosition(Vec2(0, 0));
    this->addChild(timeMenu, 2);

    timeSteps = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getTimeSteps()), "Arial Rounded MT Bold", 70);
    timeSteps->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    timeSteps->setPosition(Vec2(pauseButton->getPosition().x - pauseButton->getBoundingBox().size.width * 1.5,
        pauseButton->getPosition().y));
    this->addChild(timeSteps, 2);

    //Powers
    vector<Power*> pws = GameLevel::getInstance()->getPowers();
    for (int i = 0; i < pws.size(); i++) {
        Vec2 pos;
        if (i == 0) {
            pos.x = 14 * visibleSize.width / 204;
            pos.y = 11 * visibleSize.height / 155;
        }
        else {
            pos.x = powerButtons[i - 1]->getIcon()->getPosition().x + (4 * visibleSize.width / 204) + powerButtons[i - 1]->getIcon()->getBoundingBox().size.width;
            pos.y = powerButtons[i - 1]->getIcon()->getPosition().y;
        }

        if (pws[i]->getType() == "Multiplier") {
            powerButtons.push_back(new UIMultiplierPower(pws[i]));
            powerButtons[i]->setPosition(pos.x, pos.y);
            this->addChild(powerButtons[i]->getIcon(), 3);
        }
        else if (pws[i]->getType() == "Area") {
            powerButtons.push_back(new UIAreaPower(pws[i]));
            powerButtons[i]->setPosition(pos.x, pos.y);
            this->addChild(powerButtons[i]->getIcon(), 3);
            gameplayMap->addChild(((UIAreaPower*)powerButtons[i])->getArea(), 3);
        }
    }

    //SET GOALS ON MAP
    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        //Set Checkpoint Area
        if (GameLevel::getInstance()->getGoals()[i]->getGoalType() == "Expansion") {
            //FIND AREA
            int minX = 479;
            int maxX = 0;
            int minY = 319;
            int maxY = 0;
            for (int x = 0; x < 480; x++) {
                for (int y = 0; y < 320; y++) {
                    if (getValueAtGameplayMap(1, x, y, 0) == ((ExpansionGoal*)GameLevel::getInstance()->getGoals()[i])->getColorZone()) {
                        if (minX > x)
                            minX = x;
                        if (maxX < x)
                            maxX = x;
                        if (minY > y)
                            minY = y;
                        if (maxY < y)
                            maxY = y;
                    }
                }
            }
            ((ExpansionGoal*)GameLevel::getInstance()->getGoals()[i])->setCenterArea(maxX - ((maxX - minX) / 2), maxY - ((maxY - minY) / 2));
            int x = (int)((maxX - ((maxX - minX) / 2)) * float(2048.0 / 480.0));
            int y = (int)(float((1536.0 - 1365.0) / 2.0) + ((maxY - ((maxY - minY) / 2)) * float(1365.0 / 320.0)));
            auto area = Sprite::create("CheckpointArea.png");
            if (i == 0) {
                area->setColor(Color3B::BLUE);
                auto blink = Blink::create(2, 2);
                auto repeatBlink = RepeatForever::create(blink);
                area->setColor(Color3B::BLUE);
                area->runAction(repeatBlink);

                //GOAL EXPANSION DISTANCE
                distanceLabel = Label::createWithTTF(" ", "fonts/BebasNeue.otf", 40);
                distanceLabel->setColor(Color3B(216, 229, 235));
                distanceLabel->setPosition(visibleSize.width - 80, visibleSize.height - 300);
                //this->addChild(distanceLabel);
            }
            else {
                area->setColor(Color3B::RED);
            }
            area->setPosition(x, y);
            area->setTag(400 + i);
            gameplayMap->addChild(area, 3);
        }
    }

    //TIME PROGRESS BAR
    timeBorderBar = Sprite::create("ProgressBarBorder.png");
    timeBorderBar->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    timeBorderBar->setPosition(102 * visibleSize.width / 204, 145 * visibleSize.height / 155);
    this->addChild(timeBorderBar);
    auto barContent = Sprite::create("ProgressBarContent.png");
    timeBar = ProgressTimer::create(barContent);
    timeBar->setType(ProgressTimer::Type::BAR);
    timeBar->setAnchorPoint(Vec2(0, 0));
    timeBar->setMidpoint(Vec2(0, 0));
    timeBar->setBarChangeRate(Vec2(1, 0));
    timeBar->setPosition(0, 0);
    timeBorderBar->addChild(timeBar, 3);
    auto labelGoals = Label::createWithTTF(string(LocalizedString::create("GOALS")->getCString()), "fonts/BebasNeue.otf", 48);
    labelGoals->setColor(Color3B(139, 146, 154));
    labelGoals->setAnchorPoint(Vec2(0, 0.5));
    labelGoals->setPosition(0, timeBorderBar->getContentSize().height + (3 * visibleSize.height / 155));
    timeBorderBar->addChild(labelGoals, 2);

    //SET GOALS ON TIME PROGRESS BAR
    float pixelPerStep = barContent->getTexture()->getPixelsWide()
        / (float)GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime();
    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        float posXaverage = (float)GameLevel::getInstance()->getGoals()[i]->getAverageTime() / (float)GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime() * timeBorderBar->getContentSize().width;
        auto goalMark = Sprite::create("GoalMark.png");
        float posXcentered = (float)(GameLevel::getInstance()->getGoals()[i]->getMinTime() + ((GameLevel::getInstance()->getGoals()[i]->getMaxTime() - GameLevel::getInstance()->getGoals()[i]->getMinTime()) / 2)) / (float)GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime() * timeBorderBar->getContentSize().width;
        goalMark->setPosition(posXcentered, (timeBorderBar->getContentSize().height / 2) + (timeBorderBar->getContentSize().height / 35));
        //SET SCALE GOAL MARK
        goalMark->setScaleX((GameLevel::getInstance()->getGoals()[i]->getMaxTime() - GameLevel::getInstance()->getGoals()[i]->getMinTime()) * pixelPerStep);
        timeBorderBar->addChild(goalMark, 1);
        auto goalNum = Sprite::create("GoalNum.png");
        goalNum->setPosition(posXaverage, timeBorderBar->getContentSize().height + (3 * visibleSize.height / 155));
        auto labelGoalNum = Label::createWithTTF(to_string(i + 1), "fonts/BebasNeue.otf", 30);
        labelGoalNum->setPosition(goalNum->getContentSize().width / 2, goalNum->getContentSize().height / 2);
        labelGoalNum->setColor(Color3B(216, 229, 235));
        goalNum->addChild(labelGoalNum, 2);
        timeBorderBar->addChild(goalNum, 2);
    }

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIGameplayMap::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(UIGameplayMap::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(UIGameplayMap::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    if (GameData::getInstance()->getMusic() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("driver2.mp3", true);
    }

    time_t wait;
    wait = clock();
    /*while ((clock() / CLOCKS_PER_SEC) - (wait / CLOCKS_PER_SEC) < 0.2)
        ;*/
    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel();
    }
    GameData::getInstance()->setGameStarted(true);

    initializeAgents();

    std::map<string, int> atts = GameLevel::getInstance()->getAgentAttributes(0);
    int i = 0;
    for (std::map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        //si el cost de l'atribut es diferent de 0, es modificable
        if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), it->first) != 0) {
            keys.push_back(it->first);
        }
        i++;
    }

    //AGENT TYPE BUTTONS
    Vector<MenuItem*> agentTypeSelectorVec;
    for (int i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
        MenuItemImage* agentTypeButton;
        //NOMES UN TIPUS DAGENT (PROVISIONAL)
        if (i == 0) {
            agentTypeButton = MenuItemImage::create("AgentTypeButton.png", "AgentTypeButtonPressed.png",
                "AgentTypeButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::agentTypeCallback, this));
            agentTypeButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
            agentTypeButton->setPosition(Vec2((61 * visibleSize.width / 204) + (agentTypeButton->getContentSize().width * i),
                5 * visibleSize.height / 155));
            auto agentTypeButtonLabel = Label::createWithTTF("A-BOTS", "fonts/BebasNeue.otf", 30);
            agentTypeButtonLabel->setPosition(agentTypeButton->getContentSize().width / 2, agentTypeButton->getContentSize().height / 2);
            agentTypeButton->addChild(agentTypeButtonLabel);
            agentTypeSelectorVec.pushBack(agentTypeButton);
        }
        if (i == 0) {
            agentTypeButton->setEnabled(false);
            auto attColorsBackground = Sprite::create("AttributeColorsBackground.png");
            attColorsBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
            attColorsBackground->setAnchorPoint(Vec2(0, 0));
            attColorsBackground->setPosition(Vec2((48.9 * visibleSize.width / 204), (9.5 * visibleSize.height / 155)));
            this->addChild(attColorsBackground, 5);

            auto attBackground = Sprite::create("AgentAttributesBackground.png");
            attBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
            attBackground->setAnchorPoint(Vec2(0, 0));
            attBackground->setPosition(Vec2(visibleSize.width - attBackground->getBoundingBox().size.width,
                (18.5 * visibleSize.height / 155)));
            this->addChild(attBackground, 5);
            auto arrowRetract = MenuItemImage::create("ArrowRetract.png", "ArrowRetractPressed.png",
                CC_CALLBACK_1(UIGameplayMap::moveAttCallback, this));
            arrowRetract->setAnchorPoint(Vec2(1, 0));
            arrowRetract->setPosition(0, 0);
            Menu* arrowMenu = Menu::createWithItem(arrowRetract);
            arrowMenu->setPosition(0, 0);
            attBackground->addChild(arrowMenu);

            Vector<MenuItem*> agentLabelsSelectorVec;
            auto labelLife = MenuItemLabel::create(Label::createWithTTF(string(LocalizedString::create("LIFE")->getCString()),
                                                       "fonts/BebasNeue.otf", 40),
                CC_CALLBACK_1(UIGameplayMap::attributeSelectionCallback, this));
            labelLife->setPosition(3.4 * attColorsBackground->getContentSize().width / 24, attColorsBackground->getContentSize().height / 2);
            labelLife->setEnabled(false);
            labelLife->setTag(0);
            agentLabelsSelectorVec.pushBack(labelLife);

            auto attColorSel = Sprite::create("AttributeColorIndicator.png");
            attColorSel->setAnchorPoint(Vec2(0.5, 0));
            attColorSel->setPosition(labelLife->getPositionX(), 0);
            Color3B color;
            switch (i) {
            case 1:
                color = Color3B(0, 248, 251);
                break;
            case 2:
                color = Color3B(237, 184, 0);
                break;
            case 3:
                color = Color3B(246, 9, 255);
                break;
            default:
                color = Color3B(255, 4, 4);
                break;
            }
            attColorSel->setColor(color);
            attColorsBackground->addChild(attColorSel, 5);

            int k = 1;
            int tag = 0;
            Vector<MenuItem*> attributesButtons;
            for (int j = 0; j < keys.size(); j++) {
                auto labelAtt = MenuItemLabel::create(Label::createWithTTF(string(LocalizedString::create(keys[j].c_str())->getCString()),
                                                          "fonts/BebasNeue.otf", 40),
                    CC_CALLBACK_1(UIGameplayMap::attributeSelectionCallback, this));
                labelAtt->setPosition((3.5 + (k * 5.8)) * attColorsBackground->getContentSize().width / 24,
                    attColorsBackground->getContentSize().height / 2);
                labelAtt->setTag(j + 1);
                agentLabelsSelectorVec.pushBack(labelAtt);

                auto attColorSelector = Sprite::create("AttributeColorIndicator.png");
                attColorSelector->setAnchorPoint(Vec2(0.5, 0));
                attColorSelector->setPosition(labelAtt->getPositionX(), 0);
                Color3B color2;
                switch (j + 1) {
                case 1:
                    color2 = Color3B(212, 105, 11);
                    break;
                case 2:
                    color2 = Color3B(5, 5, 117);
                    break;
                case 3:
                    color2 = Color3B(115, 8, 214);
                    break;
                }
                attColorSelector->setColor(color2);
                attColorsBackground->addChild(attColorSelector, 5);

                auto labelAttRight = Label::createWithTTF(string(LocalizedString::create(keys[j].c_str())->getCString()) + " - ",
                    "fonts/BebasNeue.otf", 35);
                labelAttRight->setColor(Color3B(216, 229, 236));
                labelAttRight->setAnchorPoint(Vec2(0, 0.5));
                labelAttRight->setPosition(Vec2(2 * attBackground->getContentSize().width / 12,
                    (12 - (4 * (k - 1))) * attBackground->getContentSize().height / 14));
                attBackground->addChild(labelAttRight, 1, (j + 1) * 1000);

                auto attNumLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[j])), "fonts/BebasNeue.otf", 40);
                attNumLabel->setColor(Color3B(216, 229, 236));
                attNumLabel->setAnchorPoint(Vec2(1, 0.5));
                attNumLabel->setPosition(labelAttRight->getPositionX() + labelAttRight->getContentSize().width + attNumLabel->getContentSize().width * 2, labelAttRight->getPositionY());
                attBackground->addChild(attNumLabel, 1, (j + 1) * 1100);

                auto minusAttButton = MenuItemImage::create(
                    "MinusButtonSmall.png", "MinusButtonSmallPressed.png", CC_CALLBACK_1(UIGameplayMap::minusAttCallback, this));
                minusAttButton->setAnchorPoint(Vec2(0, 0.5));
                minusAttButton->setPosition(Vec2(2 * attBackground->getContentSize().width / 12,
                    (10 - (4 * (k - 1))) * attBackground->getContentSize().height / 14));
                minusAttButton->setTag(j + 10);
                attributesButtons.pushBack(minusAttButton);

                auto plusAttButton = MenuItemImage::create(
                    "PlusButtonSmall.png", "PlusButtonSmallPressed.png", CC_CALLBACK_1(UIGameplayMap::plusAttCallback, this));
                plusAttButton->setPosition(Vec2(10 * attBackground->getContentSize().width / 12,
                    (10 - (4 * (k - 1))) * attBackground->getContentSize().height / 14));
                plusAttButton->setTag(j + 50);
                attributesButtons.pushBack(plusAttButton);

                float posX = minusAttButton->getPosition().x + minusAttButton->getContentSize().width;
                float incX = ((plusAttButton->getPosition().x - (plusAttButton->getContentSize().width / 2)) - posX) / 6;
                int posY = (10 - (4 * (k - 1))) * attBackground->getContentSize().height / 14;

                for (int m = 0; m < 5; m++) {
                    posX = posX + incX;
                    auto blankAttribute = Sprite::create("BlankAttributePointButtonSmall.png");
                    auto filledAttribute = Sprite::create("FilledAttributePointButtonSmall.png");
                    if (m >= GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[j])) {
                        blankAttribute->setPosition(Vec2(posX, posY));
                        attBackground->addChild(blankAttribute, 1, tag);
                    }
                    else if (m < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[j])) {
                        filledAttribute->setPosition(Vec2(posX, posY));
                        attBackground->addChild(filledAttribute, 1, tag);
                    }
                    tag++;
                }

                k++;
            }
            Menu* agentLabelsSelectorMenu = Menu::createWithArray(agentLabelsSelectorVec);
            agentLabelsSelectorMenu->setPosition(0, 0);
            attColorsBackground->addChild(agentLabelsSelectorMenu, 5);

            Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
            attributesMenu->setPosition(Vec2(0, 0));
            attBackground->addChild(attributesMenu, 1, 100000);
        }
    }
    Menu* menuAgentTypeSelector = Menu::createWithArray(agentTypeSelectorVec);
    menuAgentTypeSelector->setPosition(0, 0);
    this->addChild(menuAgentTypeSelector, 5);

    createTimingThread();
    wait = clock();
    while ((clock() / CLOCKS_PER_SEC) - (wait / CLOCKS_PER_SEC) < 0.1)
        ;
    createNewLevelThread();
    gameplayMap->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    this->scheduleUpdate();
    //this->schedule(schedule_selector(UIGameplayMap::update), 1.3);

    return true;
}

bool UIGameplayMap::checkPowersClicked(void)
{
    for (int i = 0; i < powerButtons.size(); i++) {
        if (powerButtons[i]->getClicked() == true) {
            return true;
        }
    }
    return false;
}

void UIGameplayMap::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        if (checkPowersClicked() == false) {
            for (auto touch : touches) {
                _touches.pushBack(touch);
            }
            if (touches.size() == 1) {
                if ((clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC < 0.2 and abs(touches[0]->getLocation().distance(firstTouchLocation)) < 40) {
                    //PASAR TIPUS D'AGENT SELECCIONAT AL MOMENT
                    GameLevel::getInstance()->setAgentDirection(0, Point(firstTouchLocation.x / float(2048.0 / 480.0),
                                                                       (firstTouchLocation.y - ((1536 - 1365) / 2)) / float(1365.0 / 320.0)));
                    firstTouchLocation = touches[0]->getLocation();
                    auto fadeFinger = FadeIn::create(1);
                    fadeFinger->setTag(1);
                    fingerSpot->setPosition(Vec2(gameplayMap->convertToNodeSpace(firstTouchLocation)));
                    fingerSpot->setVisible(true);
                }
            }

            for (auto touch : touches) {
                Point touchLocation = this->convertTouchToNodeSpace(touch);
                moveBackground = false;
                for (int i = 0; i < powerButtons.size(); i++) {
                    powerButtons[i]->onTouchesBegan(touchLocation);
                }
                if (checkPowersClicked() == false and selectSpriteForTouch(gameplayMap, touchLocation)) {
                    moveBackground = true;
                }
            }
        }
    }
}

void UIGameplayMap::onTouchesMoved(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        // ZOOM
        if (touches.size() == 2) {
            if (checkPowersClicked() == false) {
                for (auto touch : touches) {
                    pinchZoomWithMovedTouch(touch);
                }
                gameplayMap->setScale(zoomScale * GameData::getInstance()->getRaWConversion(),
                    zoomScale * GameData::getInstance()->getRaHConversion());

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
        else if (touches.size() == 1) {
            for (int i = 0; i < powerButtons.size(); i++) {
                powerButtons[i]->onTouchesMoved(touches[0]);
            }
            if (moveBackground) {
                Point touchLocation = this->convertTouchToNodeSpace(touches[0]);

                Point oldTouchLocation = touches[0]->getPreviousLocationInView();
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
                gameplayMap->Node::setPosition(destPos);
            }
        }
    }
}

void UIGameplayMap::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        Point touchLocation = this->convertTouchToNodeSpace(touches[0]);
        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons[i]->onTouchesEnded(touchLocation);
        }
        moveBackground = false;
        _touches.clear();
        //cout << (clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC << " " << touches[0]->getLocation().x << " " << firstTouchLocation.x << endl;
        /*if ((clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC > 1 and abs(touches[0]->getLocation().distance(firstTouchLocation)) < 20) {
            GameLevel::getInstance()->setFingerSpot(Point(firstTouchLocation.x / float(2048.0 / 480.0),
                                                          (firstTouchLocation.y - ((1536 - 1365) / 2)) / float(1365.0 / 320.0)));
        } else if (GameLevel::getInstance()->getFingerSpot().x == -1) {
            fingerSpot->stopAllActionsByTag(1);
            fingerSpot->setVisible(false);
        }*/
        firstTouchLocation = touches[0]->getLocation();
        timeFingerSpot = clock();
    }
}

void UIGameplayMap::menuBackCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    GameLevel::getInstance()->setFinishedGame(4);
    while (GameLevel::getInstance()->ended == false)
        ;
    pthread_cancel(gameLevelThread);
    pthread_cancel(timingThread);
    pthread_join(gameLevelThread, nullptr);
    pthread_join(timingThread, nullptr);
    GameData::getInstance()->setGameStarted(false);
    auto scene = UIProgressMap::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIGameplayMap::menuGoalsCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeedBeforePause(GameLevel::getInstance()->getTimeSpeed());
    GameLevel::getInstance()->setTimeSpeed(0);
    auto scene = UIGoals::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->pushScene(transition);
}

void UIGameplayMap::pauseCallback(Ref* pSender)
{
    MenuItem* pauseButton = (MenuItem*)pSender;
    pauseButton->setEnabled(false);
    Menu* timeMenu = (Menu*)pauseButton->getParent();
    MenuItem* fastForwardButton = (MenuItem*)timeMenu->getChildren().at(0);
    MenuItem* playButton = (MenuItem*)timeMenu->getChildren().at(1);
    fastForwardButton->setEnabled(true);
    playButton->setEnabled(true);

    GameLevel::getInstance()->setTimeSpeed(0);
}

void UIGameplayMap::playCallback(Ref* pSender)
{
    MenuItem* playButton = (MenuItem*)pSender;
    playButton->setEnabled(false);
    Menu* timeMenu = (Menu*)playButton->getParent();
    MenuItem* fastForwardButton = (MenuItem*)timeMenu->getChildren().at(0);
    MenuItem* pauseButton = (MenuItem*)timeMenu->getChildren().at(2);
    fastForwardButton->setEnabled(true);
    pauseButton->setEnabled(true);

    GameLevel::getInstance()->setTimeSpeed(2.5);
}

void UIGameplayMap::fastForwardCallback(Ref* pSender)
{
    MenuItem* fastForwardButton = (MenuItem*)pSender;
    fastForwardButton->setEnabled(false);
    Menu* timeMenu = (Menu*)fastForwardButton->getParent();
    MenuItem* playButton = (MenuItem*)timeMenu->getChildren().at(1);
    MenuItem* pauseButton = (MenuItem*)timeMenu->getChildren().at(2);
    playButton->setEnabled(true);
    pauseButton->setEnabled(true);

    GameLevel::getInstance()->setTimeSpeed(1.265);
}

void UIGameplayMap::attributeSelectionCallback(Ref* pSender)
{
    MenuItem* lifeButton = (MenuItem*)pSender;
    lifeButton->setEnabled(false);
    agentColor = lifeButton->getTag();
    Menu* attributesMenu = (Menu*)lifeButton->getParent();
    for (int i = 0; i < attributesMenu->getChildren().size(); i++) {
        if (((MenuItem*)attributesMenu->getChildren().at(i))->getTag() != agentColor) {
            auto item = ((MenuItem*)attributesMenu->getChildren().at(i));
            item->setEnabled(true);
        }
    }
}

void UIGameplayMap::quitCallback(Ref* pSender)
{
    MenuItem* p = (MenuItem*)pSender;
    p->setEnabled(false);
    auto confirmBackground = Sprite::create("ConfirmBackground.png");
    Vector<MenuItem*> confirmVec;
    auto confirmOk = MenuItemImage::create("ConfirmOk.png", "ConfirmOkPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    auto confirmNo = MenuItemImage::create("ConfirmNo.png", "ConfirmNoPressed.png", CC_CALLBACK_1(UIGameplayMap::NoCallback, this));
    auto separator = Sprite::create("ConfirmSeparator.png");
    separator->setPosition(Vec2(3 * confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmOk->setPosition(Vec2(5 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmNo->setPosition(Vec2(7 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmVec.pushBack(confirmOk);
    confirmVec.pushBack(confirmNo);
    Menu* confirmMenu = Menu::createWithArray(confirmVec);
    confirmMenu->setPosition(0, 0);
    auto labelConfirm = Label::createWithTTF(string(LocalizedString::create("QUIT_QUESTION")->getCString()), "fonts/BebasNeue.otf", 50);
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

void UIGameplayMap::retryCallback(Ref* pSender)
{
    MenuItem* p = (MenuItem*)pSender;
    p->setEnabled(false);
    auto confirmBackground = Sprite::create("ConfirmBackground.png");
    Vector<MenuItem*> confirmVec;
    auto confirmOk = MenuItemImage::create("ConfirmOk.png", "ConfirmOkPressed.png", CC_CALLBACK_1(UIGameplayMap::retryOkCallback, this));
    auto confirmNo = MenuItemImage::create("ConfirmNo.png", "ConfirmNoPressed.png", CC_CALLBACK_1(UIGameplayMap::NoCallback, this));
    auto separator = Sprite::create("ConfirmSeparator.png");
    separator->setPosition(Vec2(3 * confirmBackground->getContentSize().width / 4, confirmBackground->getContentSize().height / 2));
    confirmOk->setPosition(Vec2(5 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmNo->setPosition(Vec2(7 * confirmBackground->getContentSize().width / 8, confirmBackground->getContentSize().height / 2));
    confirmVec.pushBack(confirmOk);
    confirmVec.pushBack(confirmNo);
    Menu* confirmMenu = Menu::createWithArray(confirmVec);
    confirmMenu->setPosition(0, 0);
    auto labelConfirm = Label::createWithTTF(string(LocalizedString::create("RETRY_QUESTION")->getCString()), "fonts/BebasNeue.otf", 40);
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
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    GameLevel::getInstance()->setFinishedGame(4);
    /*while (GameLevel::getInstance()->ended == false)
        ;*/
    pthread_cancel(gameLevelThread);
    pthread_cancel(timingThread);
    pthread_join(gameLevelThread, nullptr);
    pthread_join(timingThread, nullptr);
    GameData::getInstance()->setGameStarted(false);
    string filename = "level" + to_string(GameLevel::getInstance()->getNumLevel());
    GameLevel::getInstance()->resetLevel();
    LevelLoader loader;
    loader.loadXmlFile(filename);
    auto scene = UIGoals::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGameplayMap::NoCallback(Ref* pSender)
{
    MenuItem* p = (MenuItem*)((MenuItem*)pSender)->getParent()->getParent();
    MenuItem* p2 = (MenuItem*)p->getParent();
    p2->setEnabled(true);
    auto moveConfirm = MoveTo::create(0.8, Vec2(p2->getContentSize().width - p->getContentSize().width,
                                               p2->getContentSize().height / 2));
    auto ease = EaseBackOut::create(moveConfirm);
    p->runAction(ease);
    p2->removeChild(p);
}

void UIGameplayMap::agentTypeCallback(Ref* pSender)
{
    MenuItem* p = (MenuItem*)pSender;
    p->setEnabled(false);
    GameLevel::getInstance()->setCurrentAgentType(0);
}

void UIGameplayMap::moveAttCallback(Ref* pSender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    MenuItem* p = (MenuItem*)pSender;
    Sprite* background = (Sprite*)p->getParent()->getParent();
    if (p->getParent()->getParent()->getPositionX() < visibleSize.width) {
        //VISIBLE
        auto move = MoveTo::create(0.5, Vec2(visibleSize.width, background->getPositionY()));
        auto ease = EaseBackInOut::create(move);
        background->runAction(ease);
        background->removeChild(p, true);
        auto arrowExpand = MenuItemImage::create("ArrowExpand.png", "ArrowExpandPressed.png",
            CC_CALLBACK_1(UIGameplayMap::moveAttCallback, this));
        arrowExpand->setAnchorPoint(Vec2(1, 0));
        arrowExpand->setPosition(0, 0);
        Menu* arrowMenu = Menu::createWithItem(arrowExpand);
        arrowMenu->setPosition(0, 0);
        background->addChild(arrowMenu);
    }
    else {
        //INVISIBLE
        auto move = MoveTo::create(0.5, Vec2(visibleSize.width - background->getBoundingBox().size.width, background->getPositionY()));
        auto ease = EaseBackInOut::create(move);
        background->runAction(ease);
        background->removeChild(p, true);
        auto arrowRetract = MenuItemImage::create("ArrowRetract.png", "ArrowRetractPressed.png",
            CC_CALLBACK_1(UIGameplayMap::moveAttCallback, this));
        arrowRetract->setAnchorPoint(Vec2(1, 0));
        arrowRetract->setPosition(0, 0);
        Menu* arrowMenu = Menu::createWithItem(arrowRetract);
        arrowMenu->setPosition(0, 0);
        background->addChild(arrowMenu);
    }
}

void UIGameplayMap::minusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 10;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) > 0) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto filledAttribute = layout->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
        auto blankAttribute = Sprite::create("BlankAttributePointButtonSmall.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        layout->removeChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
        layout->addChild(blankAttribute, 1, GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
    }
}

void UIGameplayMap::plusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 50;
    Sprite* layout = (Sprite*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) < 5) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto blankAttribute = layout->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
        auto filledAttribute = Sprite::create("FilledAttributePointButtonSmall.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        layout->removeChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
        layout->addChild(filledAttribute, 1, (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
    }
}

void UIGameplayMap::removeFingerSpot(Ref* pSender)
{
    GameLevel::getInstance()->setAgentDirection(0, Point(-1, -1));
    fingerSpot->setVisible(false);
    //fingerSpotArea->setVisible(false);
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
    pthread_mutex_lock(&timingMutex);
    Timing::getInstance()->start();
    cout << "DONE TIMING" << endl;
    pthread_mutex_unlock(&timingMutex);
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

void UIGameplayMap::playLevel(void)
{
    pthread_mutex_lock(&gameLevelMutex);
    GameLevel::getInstance()->playLevel();
    cout << "DONE GAME LEVEL" << endl;
    pthread_mutex_unlock(&gameLevelMutex);
}

void UIGameplayMap::pinchZoomWithMovedTouch(Touch* movedTouch)
{
    float minDistSqr = FLT_MAX;
    Touch* nearestTouch = nullptr;
    Point newLocation = movedTouch->getLocationInView();
    for (auto touch : _touches) {
        if (touch != movedTouch) {
            float distSqr = sqrOfDistanceBetweenPoints(touch->getLocationInView(), newLocation);
            if (distSqr < minDistSqr) {
                minDistSqr = distSqr;
                nearestTouch = touch;
            }
        }
    }
    if (nearestTouch) {
        float prevDistSqr = sqrOfDistanceBetweenPoints(nearestTouch->getLocationInView(),
            movedTouch->getPreviousLocationInView());
        float pinchDiff = sqrtf(minDistSqr) - sqrtf(prevDistSqr);
        zoomScale += pinchDiff * 0.005; // kPinchZoomCoeff is constant = 1.0 /
        // 200.0f Adjust it for your needs
        if (zoomScale < 1) {
            zoomScale = 1;
        }
        if (zoomScale > 3) {
            zoomScale = 3;
        }
        cout << zoomScale << endl;
    }
}

bool UIGameplayMap::selectSpriteForTouch(Sprite* sprite, Point touchLocation)
{
    if (sprite->getBoundingBox().containsPoint(touchLocation)) {
        return true;
    }
    else {
        return false;
    }
}

void UIGameplayMap::moveGoalPopup(int index)
{
    /*goalPopup->runAction(MoveTo::create(1.5, Vec2(Director::getInstance()->getVisibleSize().width - timeBorderBar->getBoundingBox().size.width + (GameLevel::getInstance()->getGoals()[index]->getAverageTime() * (timeBorderBar->getBoundingBox().size.width / GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime())),
                                                  timeBorderBar->getPosition().y - timeBorderBar->getBoundingBox().size.height / 2)));*/

    if (GameLevel::getInstance()->getGoals()[index - 1]->getGoalType() == "Expansion") {
        auto area = gameplayMap->getChildByTag(400 + index - 1);
        area->stopAllActions();
        area->setVisible(true);
        area->setColor(Color3B::GREEN);
    }
    if (index < GameLevel::getInstance()->getGoals().size() and GameLevel::getInstance()->getGoals()[index]->getGoalType() == "Expansion") {
        auto nextArea = gameplayMap->getChildByTag(400 + index);
        auto blink = Blink::create(2, 2);
        auto repeatBlink = RepeatForever::create(blink);
        nextArea->setColor(Color3B::BLUE);
        nextArea->runAction(repeatBlink);
    }
}

float UIGameplayMap::getTimeProgressBar(void)
{
    return timeProgressBar;
}

void UIGameplayMap::setTimeProgressBar(float t)
{
    timeProgressBar = t;
}

float UIGameplayMap::sqrOfDistanceBetweenPoints(Point p1, Point p2)
{
    Point diff = p1 - p2;
    return diff.x * diff.x + diff.y * diff.y;
}

void UIGameplayMap::checkBackgroundLimitsInTheScreen(Point destPoint)
{
    Size winSize = Director::getInstance()->getWinSize();
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

int UIGameplayMap::getValueAtGameplayMap(int rgb, int posx, int posy, int map)
{
    Point loc(Point(posx, posy));
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //loc.y = visibleSize.height - loc.y;
    loc.y = 320 - loc.y;
    return getValueAtGameplayMap(rgb, loc, map);
}

bool UIGameplayMap::isInBoostResistanceArea(int posx, int posy)
{
    int i = 0;
    while (powerButtons[i]->getPower()->getNameInt() != 1) {
        i++;
    }
    Vec2 center = ((UIAreaPower*)powerButtons[i])->getArea()->getPosition();
    float radius = center.y - ((UIAreaPower*)powerButtons[i])->getArea()->getBoundingBox().getMinY();
    return (abs(center.distance(Vec2(posx, posy))) <= radius);
}

void UIGameplayMap::restoreLand(void)
{
    int i = 0;
    while (powerButtons[i]->getPower()->getNameInt() != 3) {
        i++;
    }
    float radius = 37.0; //((UIAreaPower*)powerButtons.at(i))->getScale();
    Point pos = ((UIAreaPower*)powerButtons[i])->getArea()->getPosition();
    Point posTransformed;
    posTransformed.x = pos.x / float(2048.0 / 480.0),
    posTransformed.y = ((pos.y - ((1536 - 1365) / 2)) / float(1365.0 / 320.0));
    for (int i = -37; i < 37; i++) {
        for (int j = -37; j < 37; j++) {
            float dist = sqrt((i * i) + (j * j));
            if (dist <= radius and posTransformed.x + i >= 0 and posTransformed.x + i < 480 and posTransformed.y + j >= 0 and posTransformed.y + j < 320) {
                GameLevel::getInstance()->setTimeExploited(posTransformed.x + i, posTransformed.y + j, 0);
                GameLevel::getInstance()->setDepleted(posTransformed.x + i, posTransformed.y + j, false);
                GameLevel::getInstance()->setEnvironmentAdaptation(posTransformed.x + i, posTransformed.y + j, false);
                drawExploitedMap(Point(posTransformed.x + i, posTransformed.y + j), Color4B(127, 127, 127, 0), 0);
            }
        }
    }
}

//map = 0 -> hotspot, map = 1 -> resources
int UIGameplayMap::getValueAtGameplayMap(int rgb, Point pt, int map)
{
    unsigned char* pixel;
    int x = 3;

    switch (map) {
    case 1:
        if (gameplayMapResources->hasAlpha()) {
            x = 4;
        }
        pixel = dataGameplayMapResources + ((int)pt.x + (int)pt.y * gameplayMapResources->getWidth()) * x;
        break;

    default:
        if (gameplayMapHotSpot->hasAlpha()) {
            x = 4;
        }
        pixel = dataGameplayMapHotSpot + ((int)pt.x + (int)pt.y * gameplayMapHotSpot->getWidth()) * x;
        break;
    }

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
    for (int i = 0; i < agentsDomain.size(); i++) {
        for (list<Agent*>::iterator it = agentsDomain[i].begin(); it != agentsDomain[i].end(); ++it) {
            Color4B color = Color4B(255, 4, 4, (*it)->getLife() * (255 / 100));
            drawAgent(Point((*it)->getPosition()->getX(), (*it)->getPosition()->getY()),
                color, 0);
        }
    }
    agentsTexture->updateWithData(agentsTextureData, 0, 0, 2048, 1536);
    play = true;
}

void UIGameplayMap::createEndGameWindow(int mode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("EndedGameBackground.png");
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background, 9);

    auto window = Sprite::create("EndedGameWindow.png");
    window->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    window->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    string title;
    string text;

    if (mode == 1) {
        //success
        title = LocalizedString::create("LEVEL_COMPLETED")->getCString();
        text = LocalizedString::create("CONGRATULATIONS")->getCString();
        int starCount = 0;
        int score = GameData::getInstance()->getLevelScore(GameLevel::getInstance()->getNumLevel());
        while (starCount < 3) {
            if (starCount < score) {
                auto starFull = Sprite::create("StarFull.png");
                starFull->setPosition(((starCount * 3) + 10) * window->getContentSize().width / 18,
                    5 * window->getContentSize().height / 10);
                window->addChild(starFull);
            }
            else {
                auto starEmpty = Sprite::create("StarEmpty.png");
                starEmpty->setPosition(((starCount * 3) + 10) * window->getContentSize().width / 18,
                    5 * window->getContentSize().height / 10);
                window->addChild(starEmpty);
            }
            starCount++;
        }
        auto titleLabel = Label::createWithTTF(title, "fonts/BebasNeue.otf", 120);
        titleLabel->setColor(Color3B(255, 255, 255));
        titleLabel->setPosition(5 * window->getContentSize().width / 18, 5 * window->getContentSize().height / 10);
        window->addChild(titleLabel);
    }
    else {
        //game over
        title = LocalizedString::create("GAME_OVER")->getCString();

        if (mode == 2) {
            text = LocalizedString::create("GOAL_NO_COMPLETED")->getCString();
        }
        else if (mode == 3) {
            text = LocalizedString::create("ALL_AGENTS_DIED")->getCString();
        }

        auto titleLabel = Label::createWithTTF(title, "fonts/BebasNeue.otf", 120);
        titleLabel->setColor(Color3B(255, 255, 255));
        titleLabel->setPosition(9 * window->getContentSize().width / 18, 4 * window->getContentSize().height / 10);
        window->addChild(titleLabel);

        auto textLabel = Label::createWithSystemFont(text, "Corbel", 30);
        textLabel->setPosition(9 * window->getContentSize().width / 18, 6 * window->getContentSize().height / 10);
        window->addChild(textLabel);
    }

    string space = " ";
    string lvl = LocalizedString::create("LEVEL")->getCString() + space + to_string(GameLevel::getInstance()->getNumLevel());
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 100);
    levelLabel->setColor(Color3B(85, 108, 117));
    levelLabel->setPosition(Vec2(4 * window->getContentSize().width / 18, 8.5 * window->getContentSize().height / 10));
    window->addChild(levelLabel);

    auto continueButton = MenuItemImage::create(
        "ProgressMapPlayButton.png", "ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    continueButton->setPosition(14 * window->getContentSize().width / 18, 1.5 * window->getContentSize().height / 10);
    auto continueLabel = Label::createWithTTF(LocalizedString::create("CONTINUE")->getCString(), "fonts/BebasNeue.otf", 50);
    continueLabel->setColor(Color3B(205, 202, 207));
    continueLabel->setPosition(continueButton->getContentSize().width / 2, continueButton->getContentSize().height / 2);
    continueButton->addChild(continueLabel);
    auto continueMenu = Menu::createWithItem(continueButton);
    continueMenu->setPosition(0, 0);
    window->addChild(continueMenu);

    auto retryButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::retryOkCallback, this));
    retryButton->setPosition(4 * window->getContentSize().width / 18, 1.5 * window->getContentSize().height / 10);
    auto retryLabel = Label::createWithTTF(LocalizedString::create("RETRY")->getCString(), "fonts/BebasNeue.otf", 50);
    retryLabel->setColor(Color3B(205, 202, 207));
    retryLabel->setPosition(retryButton->getContentSize().width / 2, retryButton->getContentSize().height / 2);
    retryButton->addChild(retryLabel);
    auto retryMenu = Menu::createWithItem(retryButton);
    retryMenu->setPosition(0, 0);
    window->addChild(retryMenu);

    this->addChild(window, 10);
}

void UIGameplayMap::updateAgents(void)
{
    vector<list<Agent*> > agentsDomain = GameLevel::getInstance()->getAgents();
    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes(0);
    vector<string> keys;
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        keys.push_back(it->first);
        i++;
    }

    Color4B white = Color4B::WHITE;
    white.a = 0;
    /*for (int i = 0; i < (int)Director::getInstance()->getVisibleSize().width * (int)Director::getInstance()->getVisibleSize().height; i++) {
        agentsTextureData[i] = white;
    }*/

    for (int i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++) {
        drawAgent(GameLevel::getInstance()->getDeletedAgents()[i], white);
    }

    for (int i = 0; i < agentsDomain.size(); i++) {
        for (list<Agent*>::iterator it = agentsDomain[i].begin(); it != agentsDomain[i].end(); ++it) {
            Color4B color;
            switch (agentColor) {
            case 1:
                color = Color4B(212, 105, 11, (*it)->getValOfAttribute(keys[0]) * (255 / 5));
                break;
            case 2:
                color = Color4B(5, 5, 117, (*it)->getValOfAttribute(keys[1]) * (255 / 5));
                break;
            case 3:
                color = Color4B(115, 8, 214, (*it)->getValOfAttribute(keys[2]) * (255 / 5));
                break;
            default:
                switch ((*it)->getType()) {
                case 1:
                    color = Color4B(0, 248, 251, (*it)->getLife() * (255 / 100));
                    break;
                case 2:
                    color = Color4B(210, 214, 47, (*it)->getLife() * (255 / 100));
                    break;
                case 3:
                    color = Color4B(68, 165, 195, (*it)->getLife() * (255 / 100));
                    break;
                default:
                    color = Color4B(255, 4, 4, (*it)->getLife() * (255 / 100));
                    break;
                }
                break;
            }

            drawAgent(Point((*it)->getPosition()->getX(), (*it)->getPosition()->getY()), color, 0);
            if (GameLevel::getInstance()->getDepleted((*it)->getPosition()->getX(), (*it)->getPosition()->getY()) == true) {
                drawExploitedMap(Point((*it)->getPosition()->getX(), (*it)->getPosition()->getY()),
                    Color4B(100, 100, 100, 100));
            }
            if (GameLevel::getInstance()->getEnvironmentAdaptation((*it)->getPosition()->getX(), (*it)->getPosition()->getY()) == true) {
                drawExploitedMap(Point((*it)->getPosition()->getX(), (*it)->getPosition()->getY()),
                    Color4B(0, 0, 0, 50));
            }
        }
    }

    agentsTexture->updateWithData(agentsTextureData, 0, 0, 2048, 1536);
    if (resourcesMap) {
        exploitedMapTexture->updateWithData(exploitedMapTextureData, 0, 0, 2048, 1536);
    }

    /*vector<int> null;
    vector<Point> p;
    GameLevel::getInstance()->setDeletedAgents(p);
    GameLevel::getInstance()->setAddedAgents(0);*/
}

void UIGameplayMap::drawAgent(Point pos, Color4B colour, int geometry)
{
    int x = (int)(pos.x * GameData::getInstance()->getRowDrawAgentPrecalc());
    int y = (int)(GameData::getInstance()->getColumnOffsetDrawAgentPrecalc() + ((pos.y) * GameData::getInstance()->getColumnDrawAgentPrecalc()));
    int position = x + ((1536.0 - y) * 2048.0);

    switch (geometry) {
    case 1: {
        int k = -2048 * 2;
        while (k <= 2048 * 2) {
            int i = abs(k / 2048);
            for (int j = -2 + i; j < 3 - i; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            k += 2048;
        }
        break;
    }
    case 2: {
        int k = -2048 * 2;
        int times = 0;
        while (k <= 2048 * 2) {
            for (int j = times; j < abs(times) - 1; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            times--;
            k += 2048;
        }
        break;
    }
    default:
        int k = -2048 * 2;
        while (k <= 2048 * 2) {
            for (int j = -2; j < 3; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            k += 2048;
        }
        break;
    }
}

void UIGameplayMap::drawExploitedMap(Point pos, Color4B colour, int geometry)
{
    int x = (int)(pos.x * float(2048.0 / 480.0));
    int y = (int)(float((1536.0 - 1365.0) / 2.0) + ((pos.y) * float(1365.0 / 320.0)));
    int position = x + ((1536.0 - y) * 2048.0);
    switch (geometry) {
    default:
        int k = -4096;
        while (k <= 4096) {
            for (int j = -2; j < 3; j++) {
                //exploitedMapTextureData[position + j + k] = colour;
                exploitedMapTextureData[position + j + k] = Color4B(0, 0, 0, 0);
            }
            k += 2048;
        }
        break;
    }
}

void UIGameplayMap::update(float delta)
{
    if (GameLevel::getInstance()->getFinishedGame() == 0) {
        if (GameLevel::getInstance()->paint == true and GameLevel::getInstance()->ended == false) {
            play = false;
            //clock_t beforeTime = clock();
            updateAgents();
            timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
            if (GameLevel::getInstance()->getNumLevel() == 2) {
                collect1PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()));
            }
            int i = 0;
            while (i < GameLevel::getInstance()->getGoals().size() and GameLevel::getInstance()->getGoals()[i]->getCompleted() == true) {
                i++;
            }
            if (i < GameLevel::getInstance()->getGoals().size() and GameLevel::getInstance()->getGoals()[i]->getGoalType() == "Expansion") {
                //distanceLabel->setString(to_string(((ExpansionGoal*)GameLevel::getInstance()->getGoals()[i])->getMinDistanceToGoal()));
            }
            play = true;
            //cout << "Pintat: " << ((float)clock() / CLOCKS_PER_SEC) - ((float)beforeTime / CLOCKS_PER_SEC) << endl;
        }
        if (GameLevel::getInstance()->getGoals().empty() == false) {
            timeBar->setPercentage(float(timeProgressBar) / float(GameLevel::getInstance()->getGoals().back()->getMaxTime()) * 100.0);
        }
        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons[i]->update(delta);
        }

        evolutionPointsLabel->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));
    }
    else if (GameLevel::getInstance()->getFinishedGame() > 0 and endGameWindowPainted == false) {
        //DARRER PINTAT
        play = false;
        updateAgents();
        //timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
        if (GameLevel::getInstance()->getGoals().empty() == false) {
            timeBar->setPercentage(float(GameLevel::getInstance()->getTimeSteps()) / float(GameLevel::getInstance()->getGoals().back()->getMaxTime()) * 100.0);
        }
        if (GameLevel::getInstance()->getNumLevel() == 2) {
            collect1PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()));
        }
        play = true;

        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons[i]->update(delta);
        }

        evolutionPointsLabel->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));

        createEndGameWindow(GameLevel::getInstance()->getFinishedGame());
        endGameWindowPainted = true;
    }
}
