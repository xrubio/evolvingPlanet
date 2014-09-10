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
#include "SimpleAudioEngine.h"

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

    Director::getInstance()->getTextureCache()->addImage("Agent.png");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //Set background gameplay map and all its functionalities
    gameplayMap = Sprite::create("GameLevelMapBackground.png");
    gameplayMap->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                  visibleSize.height / 2 + origin.y));
    this->addChild(gameplayMap, 0);

    gameplayMapHotSpot = new Image();
    gameplayMapHotSpot->initWithImageFile("GameLevelMapHotSpotsBase.png");
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    dataGameplayMapHotSpot = new unsigned char[gameplayMapHotSpot->getDataLen() * x];
    dataGameplayMapHotSpot = gameplayMapHotSpot->getData();

    Vector<MenuItem*> menuButtons;
    backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuBackCallback, this));
    backButton->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                                 origin.y + (backButton->getContentSize().height / 2)));
    menuButtons.pushBack(backButton);

    MenuItem* goalsButton = MenuItemImage::create(
        "GoalsButton.png", "GoalsButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuGoalsCallback, this));
    goalsButton->setPosition(Vec2(backButton->getPosition().x - visibleSize.width + goalsButton->getContentSize().width,
                                  backButton->getPosition().y));
    menuButtons.pushBack(goalsButton);

    MenuItem* agentsButton = MenuItemImage::create(
        "AgentsButton.png", "AgentsButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::menuAgentsCallback, this));
    agentsButton->setPosition(Vec2(backButton->getPosition().x - visibleSize.width + agentsButton->getContentSize().width,
                                   visibleSize.height - (agentsButton->getContentSize().height / 2)));
    menuButtons.pushBack(agentsButton);

    Menu* menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 10);

    evolutionPointsLabel = Label::createWithSystemFont(string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
                                                       + ": " + to_string(GameLevel::getInstance()->getEvolutionPoints()),
                                                       "Arial", 65);
    evolutionPointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3,
                                      visibleSize.height - (agentsButton->getContentSize().height / 2));
    this->addChild(evolutionPointsLabel, 1);

    Vector<MenuItem*> timeButtons;
    MenuItem* fastForwardButton = MenuItemImage::create(
        "FastForwardButton.png", "FastForwardButtonPressed.png", "FastForwardButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::fastForwardCallback, this));
    fastForwardButton->setPosition(Vec2(origin.x + visibleSize.width - (fastForwardButton->getContentSize().width / 2),
                                        origin.y + visibleSize.height - (fastForwardButton->getContentSize().height / 2)));
    timeButtons.pushBack(fastForwardButton);

    MenuItem* playButton = MenuItemImage::create(
        "PlayButton.png", "PlayButtonPressed.png", "PlayButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::playCallback, this));
    playButton->setPosition(Vec2(fastForwardButton->getPosition().x - fastForwardButton->getContentSize().width,
                                 fastForwardButton->getPosition().y));
    playButton->setEnabled(false);
    timeButtons.pushBack(playButton);

    MenuItem* pauseButton = MenuItemImage::create(
        "PauseButton.png", "PauseButtonPressed.png", "PauseButtonPressed.png", CC_CALLBACK_1(UIGameplayMap::pauseCallback, this));
    pauseButton->setPosition(Vec2(playButton->getPosition().x - playButton->getContentSize().width,
                                  playButton->getPosition().y));
    timeButtons.pushBack(pauseButton);

    Menu* timeMenu = Menu::createWithArray(timeButtons);
    timeMenu->setPosition(Vec2(0, 0));
    this->addChild(timeMenu, 2);

    timeSteps = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getTimeSteps()), "Arial Rounded MT Bold", 70);
    timeSteps->setPosition(Vec2(pauseButton->getPosition().x - pauseButton->getContentSize().width * 2,
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
    power1Button = Sprite::create("BoostReproductionButton.png");
    power1Button->setPosition(Vec2(goalsButton->getPosition().x + (3 * goalsButton->getContentSize().width / 2),
                                   goalsButton->getPosition().y));
    this->addChild(power1Button, 3);

    power2Button = Sprite::create("BoostResistanceButton.png");
    power2Button->setPosition(Vec2(power1Button->getPosition().x + power1Button->getContentSize().width,
                                   power1Button->getPosition().y));

    /*DrawPoint* area = DrawPoint::create();
    area->appendPoint(power2Button->getPosition(), 0, 0, 1);
    power2Button->addChild(area);*/
    this->addChild(power2Button, 3);

    cooldownPower1 = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getCooldownPower1()),
                                                 "Arial Rounded MT Bold", 60);
    cooldownPower1->setColor(Color3B::MAGENTA);
    cooldownPower1->setPosition(Vec2(goalsButton->getPosition().x + (3 * goalsButton->getContentSize().width / 2),
                                     goalsButton->getPosition().y));
    cooldownPower1->setVisible(false);
    cooldownPower2 = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getCooldownPower2()),
                                                 "Arial Rounded MT Bold", 60);
    cooldownPower2->setColor(Color3B::BLUE);
    cooldownPower2->setPosition(Vec2(power1Button->getPosition().x + power1Button->getContentSize().width,
                                     power1Button->getPosition().y));
    cooldownPower2->setVisible(false);
    areaPower2 = Sprite::create("BoostResistanceArea.png");
    areaPower2->setOpacity(100);
    areaPower2->setVisible(false);
    this->addChild(cooldownPower1, 4);
    this->addChild(cooldownPower2, 4);
    //this->addChild(areaPower2, 4);
    gameplayMap->addChild(areaPower2);

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIGameplayMap::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(UIGameplayMap::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(UIGameplayMap::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //agents = DrawPoint::create();
    //gameplayMap->addChild(agents, 1);

    initializeAgents();
    GameLevel::getInstance()->setUIGameplayMap(this);
    createNewLevelThread();

    this->scheduleUpdate();
    //this->schedule(schedule_selector(UIGameplayMap::update), 0.01);

    if (GameData::getInstance()->getMusic() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("driver2.mp3", true);
    }

    return true;
}

void UIGameplayMap::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        if (movePower2 == false) {
            for (auto touch : touches) {
                _touches.pushBack(touch);
            }
            if (touches.size() == 1) {
                firstTouchLocation = touches.at(0)->getLocation();
            }

            for (auto touch : touches) {
                Point touchLocation = this->convertTouchToNodeSpace(touch);
                movePower1 = false;
                moveBackground = false;
                if (GameLevel::getInstance()->getCooldownPower1() == 0 and selectSpriteForTouch(power1Button, touchLocation)) {
                    movePower1 = true;
                    power1Button->setScale(1.25);
                } else if (GameLevel::getInstance()->getCooldownPower2() == 0 and selectSpriteForTouch(power2Button, touchLocation)) {
                    movePower2 = true;
                    areaPower2->setPosition(gameplayMap->convertToNodeSpace(power2Button->getPosition()));
                    areaPower2->setVisible(true);
                    /*DrawPoint* area = (DrawPoint*)power2Button->getChildren().at(0);
            area->erasePoint(0);
            area->appendPoint(touchLocation, 1, 0, 0);
            area->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);*/
                } else if (selectSpriteForTouch(gameplayMap, touchLocation)) {
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
            if (movePower2 == false) {
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
                gameplayMap->Node::setPosition(destPos);
            } else if (movePower2) {
                Point touchArea = gameplayMap->convertToNodeSpace(Director::getInstance()->convertToGL(touches.at(0)->getLocationInView()));
                Point touch = Director::getInstance()->convertToGL(touches.at(0)->getLocationInView());
                if (touches.at(0)) {
                    //power2Button->setPosition(touch);
                    power2Button->setColor(Color3B::GRAY);
                    areaPower2->setPosition(touchArea);
                    /*DrawPoint* area = (DrawPoint*)power2Button->getChildren().at(0);
                area->erasePoint(0);
                area->appendPoint(touchArea, 1, 0, 0);
                area->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);*/
                }
            }
        }
    }
}

void UIGameplayMap::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        Point touchLocation = this->convertTouchToNodeSpace(touches.at(0));
        power1Button->setScale(1);
        if (selectSpriteForTouch(power1Button, touchLocation) and movePower1) {
            //Activar boost reproduction un cop s'ha tocat i soltat a sobre la imatge que toca
            power1Button->setColor(Color3B::GREEN);
            GameLevel::getInstance()->setPower1Active(5);
            cooldownPower1->setVisible(true);
        } else if (movePower2) {
            Point touchArea = gameplayMap->convertToNodeSpace(power2Button->getPosition());
            /*DrawPoint* area = (DrawPoint*)power2Button->getChildren().at(0);
        area->erasePoint(0);
        area->appendPoint(touchArea, 1, 0, 0);
        area->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);*/
            //areaPower2->setPosition(touchArea);
            power2Button->setColor(Color3B::WHITE);
            if (selectSpriteForTouch(power2Button, touchLocation) == false) {
                GameLevel::getInstance()->setPower2Active(10);
                cooldownPower2->setVisible(true);
            }
            movePower2 = false;

            /*auto action = MoveTo::create(0.5, Vec2(power1Button->getPosition().x + power1Button->getContentSize().width,
                                               power1Button->getPosition().y));
        power2Button->runAction(action);*/
        }
        moveBackground = false;
        _touches.clear();
    }
}

void UIGameplayMap::menuBackCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    GameData::getInstance()->setGameStarted(false);
    GameLevel::getInstance()->setFinishedGame(4);
    while (GameLevel::getInstance()->paint == false)
        ;
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

    GameLevel::getInstance()->setTimeSpeed(1.2555);
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
    agentColor = 1;
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
    agentColor = 2;
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

void UIGameplayMap::createNewLevelThread(void)
{
    pthread_t thread;
    pthread_create(&thread, NULL, &UIGameplayMap::createLevel, this);
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
    pthread_mutex_lock(&mutex);
    GameLevel::getInstance()->playLevel();
    pthread_mutex_unlock(&mutex);
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

int UIGameplayMap::getValueAtGameplayMapHotSpot(int posx, int posy)
{
    Point loc(Point(posx, posy));
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //loc.y = visibleSize.height - loc.y;
    loc.y = 200 - loc.y;
    return getValueAtGameplayMapHotSpot(loc);
}

bool UIGameplayMap::isInBoostResistanceArea(int posx, int posy)
{
    return selectSpriteForTouch(areaPower2, Point(posx, posy));
}

int UIGameplayMap::getValueAtGameplayMapHotSpot(Point pt)
{
    int retValue = 255;
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    unsigned char* pixel = dataGameplayMapHotSpot + ((int)pt.x + (int)pt.y * gameplayMapHotSpot->getWidth()) * x;
    // You can see/change pixels' RGBA value(0-255) here !
    unsigned char r = *pixel;
    retValue = (int)r;

    return retValue;
}

void UIGameplayMap::initializeAgents(void)
{
    vector<Agent*> agentsDomain = GameLevel::getInstance()->getAgents();

    /* for (int i = 0; i < agentsDomain.size(); i++) {
        agents->appendPoint(agentsDomain.at(i)->getPosition()->getX(), agentsDomain.at(i)->getPosition()->getY(), 0, 1, 1);
    }*/

    for (int i = 0; i < agentsDomain.size(); i++) {
        Sprite* s = Sprite::create("Agent.png");
        s->setColor(Color3B(128, 4, 4));
        s->setOpacity(100);
        s->setPosition(agentsDomain.at(i)->getPosition()->getX() * float(2048 / 200), agentsDomain.at(i)->getPosition()->getY() * float(1536 / 200));
        gameplayMap->addChild(s, 1, agentsDomain.at(i)->getId());
    }
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
        auto starFull = Sprite::create("StarFull.png");
        starFull->setPosition(window->getPosition().x / 4, 5.5 * window->getContentSize().height / 8);
        window->addChild(starFull);
        starCount++;
        while (starCount < 4) {
            auto starEmpty = Sprite::create("StarEmpty.png");
            starEmpty->setPosition(starCount * window->getPosition().x / 4, 5.5 * window->getContentSize().height / 8);
            window->addChild(starEmpty);
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

void UIGameplayMap::updateAgents(vector<Agent*> agentsDomain)
{
    //gameplayMap->removeAllChildren();
    //agentsSprite.clear();
    /*for (int i = 0; i < agentsDomain.size(); i++) {
        Sprite* s = Sprite::create("Agent.png");
        s->setPosition(agentsDomain.at(i)->getPosition()->getX(), agentsDomain.at(i)->getPosition()->getY());
        gameplayMap->addChild(s, 1);
        //agentsSprite.push_back(s);
    }*/
    /*agents->points->clear();
    for (int i = 0; i < agentsDomain.size(); i++) {
        Point p(agentsDomain.at(i)->getPosition()->getX(), agentsDomain.at(i)->getPosition()->getY());
        gameplayMap->convertToWorldSpace(p);
        agents->appendPoint(p, 0, 1, 1);
    }*/

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++) {
        gameplayMap->removeChildByTag(GameLevel::getInstance()->getDeletedAgents().at(i));
    }

    Vector<Node*> ags = gameplayMap->getChildren();
    for (int i = 1; i < ags.size(); i++) {
        Sprite* s = (Sprite*)ags.at(i);
        int id = ags.at(i)->getTag();
        bool found = false;
        int j = 0;
        while (j < agentsDomain.size() and found == false) {
            if (agentsDomain.at(j)->getId() != id) {
                j++;
            } else {
                found = true;
            }
        }
        if (found == true) {
            switch (agentColor) {
            case 1:
                s->setColor(Color3B(5, 5, 117));
                s->setOpacity(agentsDomain.at(j)->getValOfAttribute("att1") * (255 / 10));
                break;
            case 2:
                s->setColor(Color3B(212, 105, 11));
                s->setOpacity(agentsDomain.at(j)->getValOfAttribute("att2") * (255 / 10));

                break;
            case 3:
                s->setColor(Color3B(115, 8, 214));
                s->setOpacity(agentsDomain.at(j)->getValOfAttribute("att3") * (255 / 10));
                break;

            default:
                s->setColor(Color3B(128, 4, 4));
                s->setOpacity(agentsDomain.at(j)->getLife() * (255 / 100));
                break;
            }
        }
    }

    unsigned long sizeAgents = agentsDomain.size() - 1;
    for (int i = 0; i < GameLevel::getInstance()->getAddedAgents(); i++) {
        Sprite* s = Sprite::create("Agent.png");
        s->setPosition(agentsDomain.at(sizeAgents)->getPosition()->getX() * float(2048 / 200),
                       agentsDomain.at(sizeAgents)->getPosition()->getY() * float(1536 / 200));
        switch (agentColor) {
        case 1:
            s->setColor(Color3B(5, 5, 117));
            s->setOpacity(agentsDomain.at(sizeAgents)->getValOfAttribute("att1") * (255 / 10));
            break;
        case 2:
            s->setColor(Color3B(212, 105, 11));
            s->setOpacity(agentsDomain.at(sizeAgents)->getValOfAttribute("att2") * (255 / 10));

            break;
        case 3:
            s->setColor(Color3B(115, 8, 214));
            s->setOpacity(agentsDomain.at(sizeAgents)->getValOfAttribute("att3") * (255 / 10));
            break;

        default:
            s->setColor(Color3B(128, 4, 4));
            s->setOpacity(agentsDomain.at(sizeAgents)->getLife() * (255 / 100));
            break;
        }
        gameplayMap->addChild(s, 1, agentsDomain.at(sizeAgents)->getId());
        sizeAgents--;
    }
    /*for (int i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++) {
        agents->erasePoint(GameLevel::getInstance()->getDeletedAgents().at(i) - i);
    }

    unsigned long sizeAgents = agentsDomain.size() - 1;
    for (int i = 0; i < GameLevel::getInstance()->getAddedAgents(); i++) {
        Point p(agentsDomain.at(sizeAgents)->getPosition()->getX(),
                agentsDomain.at(sizeAgents)->getPosition()->getY());
        gameplayMap->convertToWorldSpace(p);
        agents->appendPoint(p, 0, 1, 1);
        sizeAgents--;
    }*/

    vector<int> null;
    GameLevel::getInstance()->setDeletedAgents(null);
    GameLevel::getInstance()->setAddedAgents(0);
}

void UIGameplayMap::update(float delta)
{
    /* while (agents->points->size() < game->getAgents().size()) {
        Point p(game->getAgents().at(agents->points->size())->getPosition()->getX(),
                game->getAgents().at(agents->points->size())->getPosition()->getY());
        gameplayMap->convertToWorldSpace(p);
        agents->appendPoint(p, 1, 1, 1);
    }
     if (agents->points->size() == game->getAgents().size()) {
     agents->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);
     }*/

    /*while (GameLevel::getInstance()->paint == false)
        ;*/
    if (GameLevel::getInstance()->paint == true) {
        play = false;
        updateAgents(GameLevel::getInstance()->getAgents());
        timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
        play = true;
    }
    //agents->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);
    /*
    while (agentsSprite.size() < game->getAgents().size()) {
        Sprite* s = Sprite::create("Agent.png");
        s->setPosition(game->getAgents().at(agentsSprite.size())->getPosition()->getX(),
                       game->getAgents().at(agentsSprite.size())->getPosition()->getY());
        gameplayMap->addChild(s, 1, game->getAgents().at(agentsSprite.size())->getId());
        agentsSprite.push_back(s);
    }
    */
    if (GameLevel::getInstance()->getPower1Active() == 0) {
        power1Button->setColor(Color3B::WHITE);
    }
    if (GameLevel::getInstance()->getCooldownPower1() > 0) {
        cooldownPower1->setVisible(true);
        cooldownPower1->setString(to_string(GameLevel::getInstance()->getCooldownPower1()));
    } else {
        cooldownPower1->setVisible(false);
    }
    if (movePower2 == false and GameLevel::getInstance()->getPower2Active() == 0) {
        areaPower2->setVisible(false);
    }
    /*if (GameLevel::getInstance()->getPower2Active() > 0) {
        DrawPoint* area = (DrawPoint*)power2Button->getChildren().at(0);
        area->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);
    }*/
    if (GameLevel::getInstance()->getCooldownPower2() > 0) {
        cooldownPower2->setVisible(true);
        cooldownPower2->setString(to_string(GameLevel::getInstance()->getCooldownPower2()));
    } else {
        cooldownPower2->setVisible(false);
    }
    evolutionPointsLabel->setString(string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
                                    + ": " + to_string(GameLevel::getInstance()->getEvolutionPoints()));

    if (GameLevel::getInstance()->getFinishedGame() > 0 and endGameWindowPainted == false) {
        createEndGameWindow(GameLevel::getInstance()->getFinishedGame());
        endGameWindowPainted = true;
    }
}
