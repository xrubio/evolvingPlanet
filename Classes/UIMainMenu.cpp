//
//  UIMainMenu.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIMainMenu.h"
#include "UICredits.h"
#include "UIConfiguration.h"
#include "UIProgressMap.h"
#include "UIAchievements.h"
#include "LocalizedString.h"
#include "GameData.h"

#include <audio/include/SimpleAudioEngine.h>
#include <iostream>

USING_NS_CC;

Scene* UIMainMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = UIMainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool UIMainMenu::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Director::getInstance()->getTextureCache()->addImage("ProgressMap0Background.png");

    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);

    auto title = Sprite::create("MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
        (12 * visibleSize.height / 18)));
    title->setOpacity(0);
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto fadeTitle = FadeIn::create(2.0);
    auto titleDelay = DelayTime::create(4.0);
    auto titleSeq = Sequence::create(titleDelay, fadeTitle, NULL);
    title->runAction(titleSeq);
    this->addChild(title, 5, 0);

    auto planet1 = Sprite::create("MainMenuBackgroundPlanet1.png");
    planet1->setPosition(Vec2((3 * visibleSize.width / 25),
        (7 * visibleSize.height / 18)));
    planet1->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto movePlanet1 = MoveTo::create(4, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto moveEasePlanet1 = EaseInOut::create(movePlanet1, 1);
    planet1->runAction(moveEasePlanet1);
    this->addChild(planet1, 2, 1);

    auto planet2 = Sprite::create("MainMenuBackgroundPlanet2.png");
    planet2->setPosition(Vec2((15 * visibleSize.width / 25),
        (13 * visibleSize.height / 18)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto movePlanet2 = MoveTo::create(4.8, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto moveEasePlanet2 = EaseBackOut::create(movePlanet2);
    planet2->runAction(moveEasePlanet2);
    this->addChild(planet2, 1, 2);

    auto spaceship = Sprite::create("MainMenuBackgroundSpaceship.png");
    /*spaceship->setPosition(Vec2(visibleSize.width, 0));
    auto moveSpaceship = MoveTo::create(3.0, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto moveEaseSpaceship = EaseInOut::create(moveSpaceship, 2);
    auto spaceshipDelay = DelayTime::create(2.0);
    auto spaceshipSeq = Sequence::create(spaceshipDelay, moveEaseSpaceship, NULL);*/
    spaceship->setPosition(Vec2((visibleSize.width / 3),
        (visibleSize.height / 2)));
    spaceship->setScale(0);
    auto scaleSpaceship = ScaleTo::create(3.0, GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto moveSpaceship = MoveTo::create(3.0, Vec2((visibleSize.width / 2),
                                                 (visibleSize.height / 2)));
    auto moveEaseSpaceship = EaseInOut::create(moveSpaceship, 5);
    auto spaceshipDelay = DelayTime::create(1);
    auto spaceshipSpawn = Spawn::create(moveEaseSpaceship, scaleSpaceship, NULL);
    auto spaceshipSeq = Sequence::create(spaceshipDelay, spaceshipSpawn, NULL);

    spaceship->runAction(spaceshipSeq);
    this->addChild(spaceship, 5, 3);

    auto particlesSpaceship = ParticleSun::create();
    particlesSpaceship->setGravity(Vec2(0, 0));
    //16,12
    particlesSpaceship->setPosition(Vec2(14 * (spaceship->getContentSize().width / 25), 13 * (spaceship->getContentSize().height / 25)));
    spaceship->addChild(particlesSpaceship, -1);

    Vector<cocos2d::MenuItem*> menuButtons;

    auto startButton = MenuItemImage::create(
        "MainMenuStartButton.png", "MainMenuStartButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuStartCallback, this));
    startButton->setAnchorPoint(Vec2(0, 0.5));
    startButton->setPosition(Vec2((2 * visibleSize.width / 25),
        (10 * visibleSize.height / 18)));
    auto startLabel = Label::createWithTTF(LocalizedString::create("START")->getCString(), "fonts/BebasNeue.otf", 50);
    startLabel->setColor(Color3B(219, 234, 241));
    startLabel->setPosition(startButton->getContentSize().width / 2, startButton->getContentSize().height / 2);
    startButton->addChild(startLabel);
    startButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(startButton);

    auto achievementsButton = MenuItemImage::create(
        "MainMenuAchButton.png", "MainMenuAchButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuAchievementsCallback, this));
    achievementsButton->setAnchorPoint(Vec2(1, 0.5));
    achievementsButton->setPosition(Vec2(startButton->getPositionX() + startButton->getBoundingBox().size.width,
        (8.5 * visibleSize.height / 18)));
    auto achLabel = Label::createWithTTF(LocalizedString::create("ACHIEVEMENTS")->getCString(), "fonts/BebasNeue.otf", 40);
    achLabel->setColor(Color3B(219, 234, 241));
    achLabel->setPosition(startButton->getContentSize().width / 2, achievementsButton->getContentSize().height / 2);
    achievementsButton->addChild(achLabel);
    achievementsButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(achievementsButton);

    auto configurationButton = MenuItemImage::create(
        "MainMenuSmallButton.png", "MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuConfigurationCallback, this));
    configurationButton->setAnchorPoint(Vec2(0, 0.5));
    //configurationButton->setPosition(Vec2(2 * (visibleSize.width / 25), 3 * (visibleSize.height / 18)));
    configurationButton->setPosition(Vec2((2 * visibleSize.width / 25),
        (2 * visibleSize.height / 18)));
    auto confLabel = Label::createWithTTF(LocalizedString::create("CONFIGURATION")->getCString(), "fonts/BebasNeue.otf", 30);
    confLabel->setColor(Color3B(73, 109, 118));
    confLabel->setPosition(configurationButton->getContentSize().width / 2, configurationButton->getContentSize().height / 2);
    configurationButton->addChild(confLabel);
    configurationButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(configurationButton);

    auto creditsButton = MenuItemImage::create(
        "MainMenuSmallButton.png", "MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuCreditsCallback, this));
    creditsButton->setAnchorPoint(Vec2(0, 0.5));
    creditsButton->setPosition(Vec2((2 * visibleSize.width / 25),
        (2 * visibleSize.height / 18)));
    auto credLabel = Label::createWithTTF(LocalizedString::create("CREDITS")->getCString(), "fonts/BebasNeue.otf", 30);
    credLabel->setColor(Color3B(73, 109, 118));
    credLabel->setPosition(creditsButton->getContentSize().width / 2, creditsButton->getContentSize().height / 2);
    creditsButton->addChild(credLabel);
    //menuButtons.pushBack(creditsButton);

    /*MenuItem* exitButton = MenuItemImage::create(
        "ExitButton.png", "ExitButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuExitCallback, this));
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 8)));
    menuButtons.pushBack(exitButton);*/

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(0, 0));
    menu->setOpacity(0);
    auto fadeMenu = FadeIn::create(2.0);
    auto menuDelay = DelayTime::create(4.4);
    auto menuSeq = Sequence::create(menuDelay, fadeMenu, NULL);
    menu->runAction(menuSeq);
    this->addChild(menu, 5, 4);

    if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
    }

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIMainMenu::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void UIMainMenu::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (stoppedAnimation == false) {
        endActions();
    }
}

void UIMainMenu::menuStartCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        auto scene = UIProgressMap::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
        }
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuAchievementsCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        auto scene = UIAchievements::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
        }
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuConfigurationCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        auto scene = UIConfiguration::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
        }
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuCreditsCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        auto scene = UICredits::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuExitCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void UIMainMenu::endActions(void)
{
    stoppedAnimation = true;

    Size visibleSize = Director::getInstance()->getVisibleSize();

    this->getChildByTag(0)->stopAllActions();
    this->getChildByTag(0)->setOpacity(255);
    this->getChildByTag(1)->stopAllActions();
    this->getChildByTag(1)->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->getChildByTag(2)->stopAllActions();
    this->getChildByTag(2)->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->getChildByTag(3)->stopAllActions();
    this->getChildByTag(3)->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->getChildByTag(3)->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->getChildByTag(4)->stopAllActions();
    this->getChildByTag(4)->setOpacity(255);
}

bool UIMainMenu::allActionsFinished(void)
{
    if (this->getChildByTag(0)->numberOfRunningActions() == 0 and this->getChildByTag(1)->numberOfRunningActions() == 0 and this->getChildByTag(2)->numberOfRunningActions() == 0 and this->getChildByTag(3)->numberOfRunningActions() == 0 and this->getChildByTag(4)->numberOfRunningActions() == 0) {
        return true;
    }
    return false;
}
