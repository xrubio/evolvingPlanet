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
    gameplayMapHotSpot->initWithImageFile("GameLevelMapHotSpots.png");
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    dataGameplayMapHotSpot = new unsigned char[gameplayMapHotSpot->getDataLen() * x];
    dataGameplayMapHotSpot = gameplayMapHotSpot->getData();

    Vector<MenuItem*> menuButtons;

    MenuItem* backButton = MenuItemImage::create(
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
    this->addChild(menu, 2);

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

    return true;
}

void UIGameplayMap::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    for (auto touch : touches) {
        _touches.pushBack(touch);
    }
    if (touches.size() == 1) {
        firstTouchLocation = touches.at(0)->getLocation();
    }

    for (auto touch : touches) {
        Point touchLocation = this->convertTouchToNodeSpace(touch);
        movePower1 = false;
        movePower2 = false;
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

void UIGameplayMap::onTouchesMoved(const vector<Touch*>& touches, Event* event)
{
    // ZOOM
    if (touches.size() == 2) {
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
                power2Button->setPosition(touch);
                areaPower2->setPosition(touchArea);
                /*DrawPoint* area = (DrawPoint*)power2Button->getChildren().at(0);
                area->erasePoint(0);
                area->appendPoint(touchArea, 1, 0, 0);
                area->draw(Director::getInstance()->getRenderer(), gameplayMap->getNodeToWorldTransform(), false);*/
            }
        }
    }
}

void UIGameplayMap::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
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
        areaPower2->setPosition(touchArea);
        GameLevel::getInstance()->setPower2Active(10);
        movePower2 = false;
        cooldownPower2->setVisible(true);
        auto action = MoveTo::create(0.5, Vec2(power1Button->getPosition().x + power1Button->getContentSize().width,
                                               power1Button->getPosition().y));
        power2Button->runAction(action);
    }
    moveBackground = false;
    _touches.clear();
}

void UIGameplayMap::menuBackCallback(Ref* pSender)
{
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

    GameLevel::getInstance()->setTimeSpeed(1.0);
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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    loc.y = visibleSize.height - loc.y;
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
        s->setPosition(agentsDomain.at(i)->getPosition()->getX(), agentsDomain.at(i)->getPosition()->getY());
        gameplayMap->addChild(s, 1, agentsDomain.at(i)->getId());
    }
    play = true;
}

void UIGameplayMap::addAgent(Agent* ag)
{
    auto s = Sprite::create("Agent.png");
    s->setPosition(ag->getPosition()->getX(), ag->getPosition()->getY());
    gameplayMap->addChild(s, 1, ag->getId());
    /*Point p(ag->getPosition()->getX(), ag->getPosition()->getY());
    gameplayMap->convertToWorldSpace(p);
    agents->appendPoint(p, 0, 1, 1);*/
}

void UIGameplayMap::deleteAgent(int tag, int i)
{
    //error al esborrar, exc_bad_acces quan es crida des del domini
    gameplayMap->removeChildByTag(tag);
    //agents->erasePoint(i);
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

    unsigned long sizeAgents = agentsDomain.size() - 1;
    for (int i = 0; i < GameLevel::getInstance()->getAddedAgents(); i++) {
        Sprite* s = Sprite::create("Agent.png");
        s->setPosition(agentsDomain.at(sizeAgents)->getPosition()->getX(),
                       agentsDomain.at(sizeAgents)->getPosition()->getY());
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
}
