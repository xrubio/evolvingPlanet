//
//  UIGameplayMap.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIGameplayMap.h"
#include "UIGoals.h"
#include "UIAgents.h"
#include "GameData.h"
#include "LocalizedString.h"
#include "UIMultiplierPower.h"
#include "UIAreaPower.h"
#include "CollectionGoal.h"
#include "ExpansionGoal.h"
#include "Timing.h"

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

    //Director::getInstance()->getTextureCache()->addImage("Agent.png");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    string map = GameLevel::getInstance()->getMapFilename();
    cout << "MAPA " << map << endl;
    //fer DEFINES
    string ext = ".png";
    string background = "Background";
    string hotSpotsBase = "HotSpotsBase";
    string resources = "Resources";

    /*auto ff = MenuItemImage::create("Level0FFFocus.png", "Level0FFFocus.png");
    ff->setOpacity(160);
    ff->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    auto menuTutorial = Menu::create(ff, NULL);
    menuTutorial->setPosition(0, 0);
    this->addChild(menuTutorial, 50);*/

    //Set background gameplay map and all its functionalities
    gameplayMap = Sprite::create(map + background + ext);
    gameplayMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(gameplayMap, 0);

    //FRAMES
    auto topFrame = Sprite::create("FrameTop.png");
    topFrame->setPosition(visibleSize.width / 2, visibleSize.height - (topFrame->getContentSize().height / 2));
    this->addChild(topFrame, 1);
    auto bottomFrame = Sprite::create("FrameBottom.png");
    bottomFrame->setPosition(visibleSize.width / 2, bottomFrame->getContentSize().height / 2);
    this->addChild(bottomFrame, 1);

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
    bool resourcesMap = false;
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
    }

    GameLevel::getInstance()->setUIGameplayMap(this);

    //INITIALIZE AGENTS AND EXPLOITED MAP TEXTUREDATA
    Color4B white;
    white.r = 127;
    white.g = 127;
    white.b = 127;
    white.a = 0;
    for (int i = 0; i < 2048 * 1536; i++) {
        agentsTextureData[i] = white; // i is an index running from 0 to w*h-1
        exploitedMapTextureData[i] = white; // i is an index running from 0 to w*h-1
    }

    Size contentSize;
    contentSize.width = 2048;
    contentSize.height = 1536;

    agentsTexture = new Texture2D;
    agentsTexture->initWithData(agentsTextureData, 2048 * 1536, Texture2D::PixelFormat::RGBA8888, 2048, 1536, contentSize);
    agentsSprite = Sprite::createWithTexture(agentsTexture);
    agentsSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    gameplayMap->addChild(agentsSprite, 2);
    agentsSprite->setCascadeOpacityEnabled(true);

    exploitedMapTexture = new Texture2D;
    exploitedMapTexture->initWithData(exploitedMapTextureData, 2048 * 1536, Texture2D::PixelFormat::RGBA8888, 2048, 1536, contentSize);
    exploitedMapSprite = Sprite::createWithTexture(exploitedMapTexture);
    exploitedMapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    gameplayMap->addChild(exploitedMapSprite, 1);

    Vector<MenuItem*> menuButtons;
    backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    backButton->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                                 origin.y + (backButton->getContentSize().height / 2)));
    menuButtons.pushBack(backButton);

    MenuItem* agentsButton = MenuItemImage::create(
        "AgentsButton.png", "AgentsButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuAgentsCallback, this));
    agentsButton->setPosition(Vec2(backButton->getPosition().x - visibleSize.width + agentsButton->getContentSize().width,
                                   visibleSize.height - (agentsButton->getContentSize().height / 2)));
    menuButtons.pushBack(agentsButton);

    MenuItem* goalsButton = MenuItemImage::create(
        "GoalsButton.png", "GoalsButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuGoalsCallback, this));
    goalsButton->setPosition(Vec2(backButton->getPosition().x - visibleSize.width + goalsButton->getContentSize().width,
                                  visibleSize.height - agentsButton->getContentSize().height - (goalsButton->getContentSize().height / 2)));
    menuButtons.pushBack(goalsButton);

    Menu* menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 10);

    //EVOLUTION POINTS
    //string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
    auto evolutionPointsIcon = Sprite::create("EvolutionPoints.png");
    evolutionPointsIcon->setPosition(146 * visibleSize.width / 204, 18 * visibleSize.height / 155);
    evolutionPointsLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getEvolutionPoints()),
                                                "fonts/BebasNeue.otf", 80);
    evolutionPointsLabel->setPosition(evolutionPointsIcon->getContentSize().width / 2, evolutionPointsIcon->getContentSize().height / 2);
    evolutionPointsLabel->setColor(Color3B(216, 229, 235));
    evolutionPointsIcon->addChild(evolutionPointsLabel, 2);
    this->addChild(evolutionPointsIcon, 1);

    auto evolutionPointsStringLabel = Label::createWithTTF(string(LocalizedString::create("EVOLUTION_POINTS")->getCString()),
                                                           "fonts/BebasNeue.otf", 40);
    evolutionPointsStringLabel->setColor(Color3B(216, 229, 235));
    evolutionPointsStringLabel->setPosition(evolutionPointsIcon->getPosition().x + (20 * visibleSize.width / 204),
                                            evolutionPointsIcon->getPosition().y);
    this->addChild(evolutionPointsStringLabel, 3);

    //FER DINAMIC
    if (GameLevel::getInstance()->getNumLevel() == 2) {
        collect1PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[0])->getCurrentAmount()),
                                                          "Arial", 65);
        collect1PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect1PointsLabel, 1);
        collect2PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()),
                                                          "Arial", 65);
        collect2PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3 + 400,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect2PointsLabel, 1);
        collect3PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[2])->getCurrentAmount()),
                                                          "Arial", 65);
        collect3PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3 + 800,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect3PointsLabel, 1);
    }

    //TIME BUTTONS
    Vector<MenuItem*> timeButtons;
    MenuItem* fastForwardButton = MenuItemImage::create(
        "FastForwardButton.png", "FastForwardButtonPressed.png", "FastForwardButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::fastForwardCallback, this));
    fastForwardButton->setPosition(Vec2(190 * visibleSize.width / 204, 145 * visibleSize.height / 155));
    timeButtons.pushBack(fastForwardButton);

    MenuItem* playButton = MenuItemImage::create(
        "PlayButton.png", "PlayButtonPressed.png", "PlayButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::playCallback, this));
    playButton->setPosition(Vec2(fastForwardButton->getPosition().x - (12 * visibleSize.width / 204),
                                 fastForwardButton->getPosition().y));
    playButton->setEnabled(false);
    timeButtons.pushBack(playButton);

    MenuItem* pauseButton = MenuItemImage::create(
        "PauseButton.png", "PauseButtonPressed.png", "PauseButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::pauseCallback, this));
    pauseButton->setPosition(Vec2(playButton->getPosition().x - (12 * visibleSize.width / 204),
                                  playButton->getPosition().y));
    timeButtons.pushBack(pauseButton);

    Menu* timeMenu = Menu::createWithArray(timeButtons);
    timeMenu->setPosition(Vec2(0, 0));
    this->addChild(timeMenu, 2);

    timeSteps = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getTimeSteps()), "Arial Rounded MT Bold", 70);
    timeSteps->setPosition(Vec2(pauseButton->getPosition().x - pauseButton->getContentSize().width * 1.5,
                                pauseButton->getPosition().y));
    this->addChild(timeSteps, 2);

    //Attribute values
    Vector<MenuItem*> attributesButtons;
    MenuItem* lifeButton = MenuItemImage::create(
        "LifeButton.png", "LifeButtonPressed.png", "LifeButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::lifeCallback, this));
    lifeButton->setScale(1.5);
    lifeButton->setPosition(Vec2(origin.x + visibleSize.width - (lifeButton->getBoundingBox().size.width / 2),
                                 origin.y + visibleSize.height / 3));
    lifeButton->setEnabled(false);
    attributesButtons.pushBack(lifeButton);

    MenuItem* reproductionButton = MenuItemImage::create(
        "ReproductionButton.png", "ReproductionButtonPressed.png", "ReproductionButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::reproductionCallback, this));
    reproductionButton->setScale(1.5);
    reproductionButton->setPosition(Vec2(lifeButton->getPosition().x,
                                         lifeButton->getPosition().y - lifeButton->getContentSize().height * 2));
    attributesButtons.pushBack(reproductionButton);

    MenuItem* mobilityButton = MenuItemImage::create(
        "MobilityButton.png", "MobilityButtonPressed.png", "MobilityButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::mobilityCallback, this));
    mobilityButton->setScale(1.5);
    mobilityButton->setPosition(Vec2(lifeButton->getPosition().x,
                                     reproductionButton->getPosition().y - reproductionButton->getContentSize().height * 2));
    attributesButtons.pushBack(mobilityButton);

    MenuItem* resistanceButton = MenuItemImage::create(
        "ResistanceButton.png", "ResistanceButtonPressed.png", "ResistanceButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::resistanceCallback, this));
    resistanceButton->setScale(1.5);
    resistanceButton->setPosition(Vec2(lifeButton->getPosition().x,
                                       mobilityButton->getPosition().y - mobilityButton->getContentSize().height * 2));
    attributesButtons.pushBack(resistanceButton);

    Menu* attributesMenu = Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    this->addChild(attributesMenu, 2);

    //Powers
    vector<Power*> pws = GameLevel::getInstance()->getPowers();
    for (int i = 0; i < pws.size(); i++) {
        Vec2 pos;
        if (i == 0) {
            pos.x = 13 * visibleSize.width / 204;
            pos.y = 18 * visibleSize.height / 155;
        } else {
            pos.x = powerButtons[i - 1]->getIcon()->getPosition().x + (4 * visibleSize.width / 204) + powerButtons[i - 1]->getIcon()->getContentSize().width;
            pos.y = powerButtons[i - 1]->getIcon()->getPosition().y;
        }

        if (pws[i]->getType() == "Multiplier") {
            powerButtons.push_back(new UIMultiplierPower(pws[i]));
            powerButtons[i]->setPosition(pos.x, pos.y);
            this->addChild(powerButtons[i]->getIcon(), 3);
        } else if (pws[i]->getType() == "Area") {
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
                    if (getValueAtGameplayMapHotSpot(1, x, y) == ((ExpansionGoal*)GameLevel::getInstance()->getGoals()[i])->getColorZone()) {
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
                this->addChild(distanceLabel);
            } else {
                area->setColor(Color3B::RED);
            }
            area->setPosition(x, y);
            area->setTag(400 + i);
            gameplayMap->addChild(area, 3);
        }
    }

    //TIME PROGRESS BAR
    timeBorderBar = Sprite::create("ProgressBarBorder.png");
    timeBorderBar->setPosition(102 * visibleSize.width / 204, 145 * visibleSize.height / 155);
    this->addChild(timeBorderBar);
    timeBar = ProgressTimer::create(Sprite::create("ProgressBarContent.png"));
    timeBar->setType(ProgressTimer::Type::BAR);
    timeBar->setAnchorPoint(Vec2(0, 0));
    timeBar->setMidpoint(Vec2(0, 0));
    timeBar->setBarChangeRate(Vec2(1, 0));
    timeBar->setPosition(0, 0);
    timeBorderBar->addChild(timeBar);
    auto labelGoals = Label::createWithTTF(string(LocalizedString::create("GOALS")->getCString()), "fonts/BebasNeue.otf", 48);
    labelGoals->setColor(Color3B(139, 146, 154));
    labelGoals->setAnchorPoint(Vec2(0, 0.5));
    labelGoals->setPosition(0, timeBorderBar->getContentSize().height + (3 * visibleSize.height / 155));
    timeBorderBar->addChild(labelGoals, 2);

    //SET GOALS ON TIME PROGRESS BAR
    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        float posX = (float)GameLevel::getInstance()->getGoals()[i]->getAverageTime() / (float)GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime() * timeBorderBar->getContentSize().width;
        auto goalMark = Sprite::create("GoalMark.png");
        goalMark->setPosition(posX, timeBorderBar->getContentSize().height / 2);
        timeBorderBar->addChild(goalMark, 2);
        auto goalNum = Sprite::create("GoalNum.png");
        goalNum->setPosition(posX, timeBorderBar->getContentSize().height + (3 * visibleSize.height / 155));
        auto labelGoalNum = Label::createWithTTF(to_string(i + 1), "fonts/BebasNeue.otf", 30);
        labelGoalNum->setPosition(goalNum->getContentSize().width / 2, goalNum->getContentSize().height / 2);
        labelGoalNum->setColor(Color3B(216, 229, 235));
        goalNum->addChild(labelGoalNum, 2);
        timeBorderBar->addChild(goalNum, 2);
    }

    /*goalPopup = Sprite::create("GoalPopup.png");
    goalPopup->setOpacity(127);
    goalPopup->setAnchorPoint(Vec2(1, 1));
    goalPopup->setPosition(visibleSize.width - timeBorderBar->getBoundingBox().size.width + (GameLevel::getInstance()->getGoals()[0]->getAverageTime() * (timeBorderBar->getBoundingBox().size.width / GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime())),
                           timeBorderBar->getPosition().y - timeBorderBar->getBoundingBox().size.height / 2);
    this->addChild(goalPopup);*/

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
    while ((clock() / CLOCKS_PER_SEC) - (wait / CLOCKS_PER_SEC) < 1)
        ;
    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel();
    }
    GameData::getInstance()->setGameStarted(true);

    initializeAgents();
    createTimingThread();
    wait = clock();
    while ((clock() / CLOCKS_PER_SEC) - (wait / CLOCKS_PER_SEC) < 1)
        ;
    createNewLevelThread();

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
                firstTouchLocation = touches[0]->getLocation();
                timeFingerSpot = clock();
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
                gameplayMap->setScale(zoomScale);

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
        if ((clock() - float(timeFingerSpot)) / CLOCKS_PER_SEC > 1.5 and abs(touches[0]->getLocation().distance(firstTouchLocation)) < 20) {
            GameLevel::getInstance()->setFingerSpot(Point(touches[0]->getLocation().x / float(2048.0 / 480.0),
                                                          (touches[0]->getLocation().y - ((1536 - 1365) / 2)) / float(1365.0 / 320.0)));
        }
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

void UIGameplayMap::menuAgentsCallback(Ref* pSender)
{
    GameLevel::getInstance()->setTimeSpeedBeforePause(GameLevel::getInstance()->getTimeSpeed());
    GameLevel::getInstance()->setTimeSpeed(0);
    auto scene = UIAgents::createScene();
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

void UIGameplayMap::lifeCallback(Ref* pSender)
{
    MenuItem* lifeButton = (MenuItem*)pSender;
    lifeButton->setEnabled(false);
    Menu* attributesMenu = (Menu*)lifeButton->getParent();
    MenuItem* reproductionButton = (MenuItem*)attributesMenu->getChildren().at(1);
    MenuItem* mobilityButton = (MenuItem*)attributesMenu->getChildren().at(2);
    MenuItem* resistanceButton = (MenuItem*)attributesMenu->getChildren().at(3);

    reproductionButton->setEnabled(true);
    mobilityButton->setEnabled(true);
    resistanceButton->setEnabled(true);
    agentColor = 0;
}

void UIGameplayMap::reproductionCallback(Ref* pSender)
{
    MenuItem* reproductionButton = (MenuItem*)pSender;
    reproductionButton->setEnabled(false);
    Menu* attributesMenu = (Menu*)reproductionButton->getParent();
    MenuItem* lifeButton = (MenuItem*)attributesMenu->getChildren().at(0);
    MenuItem* mobilityButton = (MenuItem*)attributesMenu->getChildren().at(2);
    MenuItem* resistanceButton = (MenuItem*)attributesMenu->getChildren().at(3);

    lifeButton->setEnabled(true);
    mobilityButton->setEnabled(true);
    resistanceButton->setEnabled(true);
    agentColor = 2;
}

void UIGameplayMap::mobilityCallback(Ref* pSender)
{
    MenuItem* mobilityButton = (MenuItem*)pSender;
    mobilityButton->setEnabled(false);
    Menu* attributesMenu = (Menu*)mobilityButton->getParent();
    MenuItem* lifeButton = (MenuItem*)attributesMenu->getChildren().at(0);
    MenuItem* reproductionButton = (MenuItem*)attributesMenu->getChildren().at(1);
    MenuItem* resistanceButton = (MenuItem*)attributesMenu->getChildren().at(3);

    lifeButton->setEnabled(true);
    reproductionButton->setEnabled(true);
    resistanceButton->setEnabled(true);
    agentColor = 1;
}

void UIGameplayMap::resistanceCallback(Ref* pSender)
{
    MenuItem* resistanceButton = (MenuItem*)pSender;
    resistanceButton->setEnabled(false);
    Menu* attributesMenu = (Menu*)resistanceButton->getParent();
    MenuItem* lifeButton = (MenuItem*)attributesMenu->getChildren().at(0);
    MenuItem* reproductionButton = (MenuItem*)attributesMenu->getChildren().at(1);
    MenuItem* mobilityButton = (MenuItem*)attributesMenu->getChildren().at(2);

    lifeButton->setEnabled(true);
    reproductionButton->setEnabled(true);
    mobilityButton->setEnabled(true);
    agentColor = 3;
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
    }
}

bool UIGameplayMap::selectSpriteForTouch(Sprite* sprite, Point touchLocation)
{
    if (sprite->getBoundingBox().containsPoint(touchLocation)) {
        return true;
    } else {
        return false;
    }
}

void UIGameplayMap::moveGoalPopup(int index)
{
    /*goalPopup->runAction(MoveTo::create(1.5, Vec2(Director::getInstance()->getVisibleSize().width - timeBorderBar->getBoundingBox().size.width + (GameLevel::getInstance()->getGoals()[index]->getAverageTime() * (timeBorderBar->getBoundingBox().size.width / GameLevel::getInstance()->getGoals()[GameLevel::getInstance()->getGoals().size() - 1]->getMaxTime())),
                                                  timeBorderBar->getPosition().y - timeBorderBar->getBoundingBox().size.height / 2)));*/

    if (GameLevel::getInstance()->getGoals()[index]->getGoalType() == "Expansion") {
        auto area = gameplayMap->getChildByTag(400 + index - 1);
        area->stopAllActions();
        area->setVisible(true);
        area->setColor(Color3B::GREEN);
        if (index < GameLevel::getInstance()->getGoals().size()) {
            auto nextArea = gameplayMap->getChildByTag(400 + index);
            auto blink = Blink::create(2, 2);
            auto repeatBlink = RepeatForever::create(blink);
            nextArea->setColor(Color3B::BLUE);
            nextArea->runAction(repeatBlink);
        }
    }
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
    } else {
        moveBackgroundLeft = true;
    }
    if ((destPoint.x + gameplayMapBoundingBoxWidth / 2) < winSize.width) {
        moveBackgroundRight = false;
    } else {
        moveBackgroundRight = true;
    }
    // UP-DOWN
    if ((destPoint.y - gameplayMapBoundingBoxHeight / 2) > 0) {
        moveBackgroundDown = false;
    } else {
        moveBackgroundDown = true;
    }
    if ((destPoint.y + gameplayMapBoundingBoxHeight / 2) < winSize.height) {
        moveBackgroundUp = false;
    } else {
        moveBackgroundUp = true;
    }
}

int UIGameplayMap::getValueAtGameplayMapHotSpot(int rgb, int posx, int posy)
{
    Point loc(Point(posx, posy));
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //loc.y = visibleSize.height - loc.y;
    loc.y = 320 - loc.y;
    return getValueAtGameplayMapHotSpot(rgb, loc);
}

int UIGameplayMap::getValueAtGameplayMapResources(int rgb, int posx, int posy)
{
    Point loc(Point(posx, posy));
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //loc.y = visibleSize.height - loc.y;
    loc.y = 320 - loc.y;
    return getValueAtGameplayMapResources(rgb, loc);
}

bool UIGameplayMap::isInBoostResistanceArea(int posx, int posy)
{
    int i = 0;
    while (powerButtons[i]->getPower()->getNameInt() != 1) {
        i++;
    }
    return selectSpriteForTouch(((UIAreaPower*)powerButtons[i])->getArea(), Point(posx, posy));
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

int UIGameplayMap::getValueAtGameplayMapHotSpot(int rgb, Point pt)
{
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    unsigned char* pixel = dataGameplayMapHotSpot + ((int)pt.x + (int)pt.y * gameplayMapHotSpot->getWidth()) * x;
    // You can see/change pixels' RGBA value(0-255) here !

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

int UIGameplayMap::getValueAtGameplayMapResources(int rgb, Point pt)
{
    int x = 3;
    if (gameplayMapResources->hasAlpha()) {
        x = 4;
    }
    unsigned char* pixel = dataGameplayMapResources + ((int)pt.x + (int)pt.y * gameplayMapResources->getWidth()) * x;
    // You can see/change pixels' RGBA value(0-255) here !

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
    backButton->setVisible(false);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("EndedGameBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setOpacity(90);
    this->addChild(background, 9);

    auto window = Sprite::create("EndedGameWindow.png");
    window->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    string title;
    string text;

    if (mode == 1) {
        //success
        title = LocalizedString::create("LEVEL_COMPLETED")->getCString();
        text = LocalizedString::create("CONGRATULATIONS")->getCString();
        int starCount = 1;
        int score = GameData::getInstance()->getLevelScore(GameLevel::getInstance()->getNumLevel());
        while (starCount < 4) {
            if (starCount <= score) {
                auto starFull = Sprite::create("StarFull.png");
                starFull->setPosition(starCount * window->getPosition().x / 4, 5.5 * window->getContentSize().height / 8);
                window->addChild(starFull);
            } else {
                auto starEmpty = Sprite::create("StarEmpty.png");
                starEmpty->setPosition(starCount * window->getPosition().x / 4, 5.5 * window->getContentSize().height / 8);
                window->addChild(starEmpty);
            }
            starCount++;
        }
    } else {
        //game over
        title = LocalizedString::create("GAME_OVER")->getCString();
        auto gameOver = Sprite::create("GameOverIcon.png");
        gameOver->setScale(0.5, 0.5);
        gameOver->setPosition(window->getPosition().x / 2, 5.5 * window->getContentSize().height / 8);
        window->addChild(gameOver);

        if (mode == 2) {
            text = LocalizedString::create("GOAL_NO_COMPLETED")->getCString();
        } else if (mode == 3) {
            text = LocalizedString::create("ALL_AGENTS_DIED")->getCString();
        }
    }

    auto titleLabel = Label::createWithSystemFont(title, "Arial Rounded MT Bold", 100);
    titleLabel->setPosition(window->getPosition().x / 2, window->getContentSize().height - (titleLabel->getContentSize().height / 2));

    auto textLabel = Label::createWithSystemFont(text, "Arial Rounded MT Bold", 50);
    textLabel->setPosition(titleLabel->getPosition().x, window->getContentSize().height / 2);

    auto continueButton = MenuItemImage::create(
        "MainButtonBackground.png", "MainButtonBackgroundPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    continueButton->setPosition(titleLabel->getPosition().x, window->getContentSize().height / 4);

    auto continueLabel = LabelTTF::create(LocalizedString::create("CONTINUE")->getCString(), "Calibri", 50);
    continueLabel->setPosition(continueButton->getContentSize().width / 2, continueButton->getContentSize().height / 2);
    continueButton->addChild(continueLabel);

    auto continueMenu = Menu::createWithItem(continueButton);
    continueMenu->setPosition(0, 0);

    window->addChild(titleLabel);
    window->addChild(textLabel);
    window->addChild(continueMenu);

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
    for (int i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++) {
        drawAgent(GameLevel::getInstance()->getDeletedAgents()[i], white);
    }

    for (int i = 0; i < agentsDomain.size(); i++) {
        for (list<Agent*>::iterator it = agentsDomain[i].begin(); it != agentsDomain[i].end(); ++it) {
            Color4B color;
            switch (agentColor) {
            case 1:
                color = Color4B(212, 105, 11, (*it)->getValOfAttribute(keys[i]) * (255 / 10));
                break;
            case 2:
                color = Color4B(5, 5, 117, (*it)->getValOfAttribute(keys[1]) * (255 / 10));
                break;
            case 3:
                color = Color4B(115, 8, 214, (*it)->getValOfAttribute(keys[2]) * (255 / 10));
                break;
            default:
                switch ((*it)->getType()) {
                case 1:
                    color = Color4B(0, 248, 251, (*it)->getLife() * (255 / 100));
                    break;
                case 2:
                    color = Color4B(237, 184, 0, (*it)->getLife() * (255 / 100));
                    break;
                case 3:
                    color = Color4B(246, 9, 255, (*it)->getLife() * (255 / 100));
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
    exploitedMapTexture->updateWithData(exploitedMapTextureData, 0, 0, 2048, 1536);

    /*vector<int> null;
    vector<Point> p;
    GameLevel::getInstance()->setDeletedAgents(p);
    GameLevel::getInstance()->setAddedAgents(0);*/
}

void UIGameplayMap::drawAgent(Point pos, Color4B colour, int geometry)
{
    int x = (int)(pos.x * float(2048.0 / 480.0));
    int y = (int)(float((1536.0 - 1365.0) / 2.0) + ((pos.y) * float(1365.0 / 320.0)));
    int position = x + ((1536.0 - y) * 2048.0);

    switch (geometry) {
    case 1: {
        int k = -4096;
        while (k <= 4096) {
            int i = abs(k / 2048);
            for (int j = -2 + i; j < 3 - i; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            k += 2048;
        }
        break;
    }
    case 2: {
        int k = -4096;
        int times = 0;
        while (k <= 4096) {
            for (int j = times; j < abs(times) - 1; j++) {
                agentsTextureData[position + j + k] = colour;
            }
            times--;
            k += 2048;
        }
        break;
    }
    default:
        int k = -4096;
        while (k <= 4096) {
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
                exploitedMapTextureData[position + j + k] = colour;
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
            clock_t beforeTime = clock();
            updateAgents();
            timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
            timeBar->setPercentage(float(GameLevel::getInstance()->getTimeSteps()) / float(GameLevel::getInstance()->getGoals().back()->getMaxTime()) * 100.0);
            if (GameLevel::getInstance()->getNumLevel() == 2) {
                collect1PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[0])->getCurrentAmount()));
                collect2PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()));
                collect3PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[2])->getCurrentAmount()));
            }
            int i = 0;
            while (GameLevel::getInstance()->getGoals()[i]->getCompleted() == true) {
                i++;
            }
            if (GameLevel::getInstance()->getGoals()[i]->getGoalType() == "Expansion") {
                distanceLabel->setString(to_string(((ExpansionGoal*)GameLevel::getInstance()->getGoals()[i])->getMinDistanceToGoal()));
            }
            play = true;
            //cout << "Pintat: " << ((float)clock() / CLOCKS_PER_SEC) - ((float)beforeTime / CLOCKS_PER_SEC) << endl;
        }

        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons[i]->update(delta);
        }

        evolutionPointsLabel->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));
    } else if (GameLevel::getInstance()->getFinishedGame() > 0 and endGameWindowPainted == false) {
        //DARRER PINTAT
        play = false;
        updateAgents();
        timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
        timeBar->setPercentage(float(GameLevel::getInstance()->getTimeSteps()) / float(GameLevel::getInstance()->getGoals().back()->getMaxTime()) * 100.0);
        if (GameLevel::getInstance()->getNumLevel() == 2) {
            collect1PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[0])->getCurrentAmount()));
            collect2PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[1])->getCurrentAmount()));
            collect3PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals()[2])->getCurrentAmount()));
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
