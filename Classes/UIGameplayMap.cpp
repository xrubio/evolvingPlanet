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
#include "UIMultiplierPower.h"
#include "UIAreaPower.h"
#include "CollectionGoal.h"

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

    string map = GameLevel::getInstance()->getMapFilename();
    //fer DEFINES
    string ext = ".png";
    string background = "Background";
    string hotSpotsBase = "HotSpotsBase";
    string resources = "Resources";

    //Set background gameplay map and all its functionalities
    gameplayMap = Sprite::create(map + background + ext);
    gameplayMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
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

    //RESOURCES MAP (IF ANY)
    gameplayMapResources = new Image();
    if (gameplayMapResources->initWithImageFile(map + resources + ext)) {
        x = 3;
        if (gameplayMapResources->hasAlpha()) {
            x = 4;
        }
        dataGameplayMapResources = new unsigned char[gameplayMapResources->getDataLen() * x];
        dataGameplayMapResources = gameplayMapResources->getData();
    }

    GameLevel::getInstance()->setUIGameplayMap(this);

    //INITIALIZE TEXTUREDATA
    Color4B white;
    white.r = 255;
    white.g = 255;
    white.b = 255;
    white.a = 0;
    for (int i = 0; i < 2048 * 1536; i++) {
        m_TextureData[i] = white; // i is an index running from 0 to w*h-1
    }

    Size contentSize;
    contentSize.width = 2048;
    contentSize.height = 1536;

    m_Texture = new Texture2D;
    m_Texture->initWithData(m_TextureData, 2048 * 1536, Texture2D::PixelFormat::RGBA8888, 2048, 1536, contentSize);
    m_Sprite = Sprite::createWithTexture(m_Texture);
    m_Sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    gameplayMap->addChild(m_Sprite, 1);

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

    //FER DINAMIC
    if (GameLevel::getInstance()->getNumLevel() == 2) {
        collect1PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(0))->getCurrentAmount()),
                                                          "Arial", 65);
        collect1PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect1PointsLabel, 1);
        collect2PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(1))->getCurrentAmount()),
                                                          "Arial", 65);
        collect2PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3 + 400,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect2PointsLabel, 1);
        collect3PointsLabel = Label::createWithSystemFont(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(2))->getCurrentAmount()),
                                                          "Arial", 65);
        collect3PointsLabel->setPosition(origin.x + agentsButton->getContentSize().width * 3 + 800,
                                         visibleSize.height - (agentsButton->getContentSize().height));
        this->addChild(collect3PointsLabel, 1);
    }

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
    vector<Power*> pws = GameLevel::getInstance()->getPowers();
    for (int i = 0; i < pws.size(); i++) {
        Vec2 pos;
        if (i == 0) {
            pos.x = goalsButton->getPosition().x + (3 * goalsButton->getContentSize().width / 2);
            pos.y = goalsButton->getPosition().y;
        } else {
            pos.x = powerButtons.at(i - 1)->getIcon()->getPosition().x + powerButtons.at(i - 1)->getIcon()->getContentSize().width;
            pos.y = powerButtons.at(i - 1)->getIcon()->getPosition().y;
        }

        if (pws.at(i)->getType() == "Multiplier") {
            powerButtons.push_back(new UIMultiplierPower(pws.at(i)));
            powerButtons.at(i)->setPosition(pos.x, pos.y);
            this->addChild(powerButtons.at(i)->getIcon(), 3);
        } else if (pws.at(i)->getType() == "Area") {
            powerButtons.push_back(new UIAreaPower(pws.at(i)));
            powerButtons.at(i)->setPosition(pos.x, pos.y);
            this->addChild(powerButtons.at(i)->getIcon(), 3);
            gameplayMap->addChild(((UIAreaPower*)powerButtons.at(i))->getArea(), 3);
        }
    }

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIGameplayMap::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(UIGameplayMap::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(UIGameplayMap::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    if (GameData::getInstance()->getMusic() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("driver2.mp3", true);
    }

    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->createLevel();
    }
    GameData::getInstance()->setGameStarted(true);

    initializeAgents();
    createNewLevelThread();

    this->scheduleUpdate();
    //this->schedule(schedule_selector(UIGameplayMap::update), 1);

    return true;
}

bool UIGameplayMap::checkPowersClicked(void)
{
    for (int i = 0; i < powerButtons.size(); i++) {
        if (powerButtons.at(i)->getClicked() == true) {
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
                firstTouchLocation = touches.at(0)->getLocation();
            }

            for (auto touch : touches) {
                Point touchLocation = this->convertTouchToNodeSpace(touch);
                moveBackground = false;
                for (int i = 0; i < powerButtons.size(); i++) {
                    powerButtons.at(i)->onTouchesBegan(touchLocation);
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
                gameplayMap->Node::setPosition(destPos);
            }
        }
    }
}

void UIGameplayMap::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    if (endGameWindowPainted == false) {
        Point touchLocation = this->convertTouchToNodeSpace(touches.at(0));
        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons.at(i)->onTouchesEnded(touchLocation);
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

    GameLevel::getInstance()->setTimeSpeed(1.26);
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
    while (powerButtons.at(i)->getPower()->getName() != "ResistanceBoost") {
        i++;
    }
    return selectSpriteForTouch(((UIAreaPower*)powerButtons.at(i))->getArea(), Point(posx, posy));
}

int UIGameplayMap::getValueAtGameplayMapHotSpot(int rgb, Point pt)
{
    int x = 3;
    if (gameplayMapHotSpot->hasAlpha()) {
        x = 4;
    }
    unsigned char* pixel = dataGameplayMapHotSpot + ((int)pt.x + (int)pt.y * gameplayMapHotSpot->getWidth()) * x;
    // You can see/change pixels' RGBA value(0-255) here !

    if (rgb == 0) {
        unsigned char r = *pixel;
        return (int)r;
    } else if (rgb == 1) {
        unsigned char g = *(pixel + 1);
        return (int)g;
    } else if (rgb == 2) {
        unsigned char b = *(pixel + 2);
        return (int)b;
    } else if (rgb == 3) {
        unsigned char a = *(pixel + 3);
        return (int)a;
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

    if (rgb == 0) {
        unsigned char r = *pixel;
        return (int)r;
    } else if (rgb == 1) {
        unsigned char g = *(pixel + 1);
        return (int)g;
    } else if (rgb == 2) {
        unsigned char b = *(pixel + 2);
        return (int)b;
    } else if (rgb == 3) {
        unsigned char a = *(pixel + 3);
        return (int)a;
    }
    return 255;
}

void UIGameplayMap::initializeAgents(void)
{
    vector<vector<Agent*> > agentsDomain = GameLevel::getInstance()->getAgents();
    for (int i = 0; i < agentsDomain.size(); i++) {
        for (int j = 0; j < agentsDomain.at(i).size(); j++) {
            Color4B color = Color4B(255, 4, 4, agentsDomain.at(i).at(j)->getLife() * (255 / 100));
            drawAgent(Point(agentsDomain.at(i).at(j)->getPosition()->getX(), agentsDomain.at(i).at(j)->getPosition()->getY()),
                      color);
        }
    }
    m_Texture->updateWithData(m_TextureData, 0, 0, 2048, 1536);
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
    vector<vector<Agent*> > agentsDomain = GameLevel::getInstance()->getAgents();
    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes();
    vector<string> keys;
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        keys.push_back(it->first);
        i++;
    }

    Color4B white = Color4B::WHITE;
    white.a = 0;
    for (int i = 0; i < GameLevel::getInstance()->getDeletedAgents().size(); i++) {
        drawAgent(GameLevel::getInstance()->getDeletedAgents().at(i), white);
    }

    for (int i = 0; i < agentsDomain.size(); i++) {
        for (int j = 0; j < agentsDomain.at(i).size(); j++) {
            Color4B color;
            switch (agentColor) {
            case 1:
                color = Color4B(212, 105, 11, agentsDomain.at(i).at(j)->getValOfAttribute(keys.at(0)) * (255 / 10));
                break;
            case 2:
                color = Color4B(5, 5, 117, agentsDomain.at(i).at(j)->getValOfAttribute(keys.at(1)) * (255 / 10));
                break;
            case 3:
                color = Color4B(115, 8, 214, agentsDomain.at(i).at(j)->getValOfAttribute(keys.at(2)) * (255 / 10));
                break;
            default:
                color = Color4B(255, 4, 4, agentsDomain.at(i).at(j)->getLife() * (255 / 100));
                break;
            }

            drawAgent(Point(agentsDomain.at(i).at(j)->getPosition()->getX(), agentsDomain.at(i).at(j)->getPosition()->getY()),
                      color);
        }
    }

    m_Texture->updateWithData(m_TextureData, 0, 0, 2048, 1536);

    vector<int> null;
    vector<Point> p;
    GameLevel::getInstance()->setDeletedAgents(p);
    GameLevel::getInstance()->setAddedAgents(0);
}

void UIGameplayMap::drawAgent(Point pos, Color4B colour, int geometry)
{
    int x = (int)(pos.x * float(2048 / 480));
    int y = (int)(float((1536 - 1365) / 2) + ((pos.y) * float(1365 / 320)));
    int position = x + ((1536 - y) * 2048);

    switch (geometry) {
    default:
        int k = -4096;
        while (k <= 4096) {
            for (int j = -2; j < 3; j++) {
                m_TextureData[position + j + k] = colour;
            }
            k += 2048;
        }
        break;
    }
}

void UIGameplayMap::update(float delta)
{
    if (GameLevel::getInstance()->getFinishedGame() > 0 and endGameWindowPainted == false) {
        createEndGameWindow(GameLevel::getInstance()->getFinishedGame());
        endGameWindowPainted = true;
    } else {
        if (GameLevel::getInstance()->paint == true) {
            play = false;
            updateAgents();
            timeSteps->setString(to_string(GameLevel::getInstance()->getTimeSteps()));
            if (GameLevel::getInstance()->getNumLevel() == 2) {
                collect1PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(0))->getCurrentAmount()));
                collect2PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(1))->getCurrentAmount()));
                collect3PointsLabel->setString(to_string(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(2))->getCurrentAmount()));
            }
            play = true;
        }

        for (int i = 0; i < powerButtons.size(); i++) {
            powerButtons.at(i)->update();
        }

        evolutionPointsLabel->setString(string(LocalizedString::create("EVOLUTION_POINTS")->getCString())
                                        + ": " + to_string(GameLevel::getInstance()->getEvolutionPoints()));
    }
}
