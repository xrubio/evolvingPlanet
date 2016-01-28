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
#include "UIStoryGallery.h"
#include "LocalizedString.h"
#include "GameData.h"
#include "UIIntroStory.h"
#include "UITransitionScene.h"

#include <audio/include/SimpleAudioEngine.h>

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
    auto loading = Sprite::create("gui/Loading.png");
    loading->setPosition(Vec2(21 * visibleSize.width / 25, 3 * visibleSize.height / 18));
    loading->setName("loading");
    loading->setVisible(false);
    this->addChild(loading, 40);
    Vector<cocos2d::MenuItem*> menuButtons;

    if (UserDefault::getInstance()->getBoolForKey("firsttimeplaying") == false)
    {
        auto continueButton = MenuItemImage::create(
                                                "gui/MainMenuStartButton.png", "gui/MainMenuStartButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuContinueCallback, this));
        continueButton->setAnchorPoint(Vec2(0, 0.5));
        continueButton->setPosition(Vec2((2 * visibleSize.width / 25),
                                     (10 * visibleSize.height / 18)));
        auto continueLabel = Label::createWithTTF(LocalizedString::create("CONTINUE"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
        continueLabel->setColor(Color3B(219, 234, 241));
        continueLabel->setPosition(1.1 * continueButton->getContentSize().width / 2, continueButton->getContentSize().height / 2);
        continueButton->addChild(continueLabel);
        continueButton->setScale(GameData::getInstance()->getRaHConversion());
        menuButtons.pushBack(continueButton);
    }
    else {
        //LOAD INTRO IMAGES TO CACHE
        loadIntroImages();
    }
    
    auto startButton = MenuItemImage::create(
        "gui/MainMenuStartButton.png", "gui/MainMenuStartButtonPressed.png", "gui/MainMenuStartButtonPressed.png",
                                             CC_CALLBACK_1(UIMainMenu::menuStartCallback, this));
    startButton->setAnchorPoint(Vec2(0, 0.5));
    startButton->setName("newCampaign");
    if (menuButtons.size() > 0)
    {
        startButton->setPosition(Vec2((2 * visibleSize.width / 25), (8.5 * visibleSize.height / 18)));
    }
    else
    {
        startButton->setPosition(Vec2((2 * visibleSize.width / 25), (10 * visibleSize.height / 18)));
    }
    
    auto startLabel = Label::createWithTTF(LocalizedString::create("NEW_GAME"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    startLabel->setColor(Color3B(219, 234, 241));
    startLabel->setPosition(1.1 * startButton->getContentSize().width / 2, startButton->getContentSize().height / 2);
    startButton->addChild(startLabel);
    startButton->setScale(GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(startButton);
    
    auto extrasButton = MenuItemToggle::createWithCallback(CC_CALLBACK_1(UIMainMenu::menuExtrasCallback, this), MenuItemImage::create("gui/MainMenuStartButton.png", "gui/MainMenuStartButton.png"), MenuItemImage::create("gui/MainMenuStartButtonPressed.png", "gui/MainMenuStartButtonPressed.png"), NULL);
    extrasButton->setAnchorPoint(Vec2(0, 0.5));
    extrasButton->setName("extras");
    extrasButton->setPosition(Vec2((2 * visibleSize.width / 25), startButton->getPositionY() - (1.5 * visibleSize.height / 18)));
    auto extrasLabel = Label::createWithTTF(LocalizedString::create("EXTRAS"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    extrasLabel->setColor(Color3B(219, 234, 241));
    extrasLabel->setPosition(1.1*extrasButton->getContentSize().width / 2, extrasButton->getContentSize().height / 2);
    extrasButton->addChild(extrasLabel, 3);
    extrasButton->setScale(GameData::getInstance()->getRaHConversion());
    extrasButton->setCascadeOpacityEnabled(true);
    menuButtons.pushBack(extrasButton);
    
    auto storyButton = MenuItemImage::create("gui/MainMenuAchButton.png", "gui/MainMenuAchButtonPressed.png",
                                             CC_CALLBACK_1(UIMainMenu::menuStoryCallback, this));
    storyButton->setPosition(Vec2(extrasButton->getPositionX() + extrasButton->getBoundingBox().size.width,
                                  extrasButton->getPositionY() - (1.2 * visibleSize.height / 18)));
    storyButton->setName("story");
    auto storyLabel = Label::createWithTTF(LocalizedString::create("STORY"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    storyLabel->setColor(Color3B(219, 234, 241));
    storyLabel->setPosition(1.1*storyButton->getContentSize().width / 2, storyButton->getContentSize().height / 2);
    storyButton->addChild(storyLabel);
    storyButton->setScale(GameData::getInstance()->getRaHConversion());
    storyButton->setVisible(false);
    menuButtons.pushBack(storyButton);

    auto achievementsButton = MenuItemImage::create(
        "gui/MainMenuAchButton.png", "gui/MainMenuAchButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuAchievementsCallback, this));
    achievementsButton->setPosition(Vec2(extrasButton->getPositionX() + extrasButton->getBoundingBox().size.width,
        storyButton->getPositionY() - (1.2 * visibleSize.height / 18)));
    achievementsButton->setName("achievements");
    auto achLabel = Label::createWithTTF(LocalizedString::create("ACHIEVEMENTS"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    achLabel->setColor(Color3B(219, 234, 241));
    achLabel->setPosition(1.1 * achievementsButton->getContentSize().width / 2, achievementsButton->getContentSize().height / 2);
    achievementsButton->addChild(achLabel);
    achievementsButton->setScale(GameData::getInstance()->getRaHConversion());
    achievementsButton->setVisible(false);
    menuButtons.pushBack(achievementsButton);
    
    int pos = 1;
    if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    {
        auto exitButton = MenuItemImage::create(
                                                "gui/MainMenuSmallButton.png", "gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuExitCallback, this));
        exitButton->setAnchorPoint(Vec2(0, 0.5));
        exitButton->setPosition(Vec2((21 * visibleSize.width / 25), (1 * visibleSize.height / 18)));
        auto exitLabel = Label::createWithTTF(LocalizedString::create("EXIT"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
        exitLabel->setColor(Color3B(73, 109, 118));
        exitLabel->setPosition(exitButton->getContentSize().width / 2, exitButton->getContentSize().height / 2);
        exitButton->addChild(exitLabel);
        exitButton->setScale(GameData::getInstance()->getRaHConversion());
        menuButtons.pushBack(exitButton);
        pos = 2;
    }
    
    auto creditsButton = MenuItemImage::create(
                                               "gui/MainMenuSmallButton.png", "gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuCreditsCallback, this));
    creditsButton->setAnchorPoint(Vec2(0, 0.5));
    creditsButton->setPosition(Vec2((21 * visibleSize.width / 25), (pos * visibleSize.height / 18)));
    auto credLabel = Label::createWithTTF(LocalizedString::create("CREDITS"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    credLabel->setColor(Color3B(73, 109, 118));
    credLabel->setPosition(creditsButton->getContentSize().width / 2, creditsButton->getContentSize().height / 2);
    creditsButton->addChild(credLabel);
    creditsButton->setScale(GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(creditsButton);
    
    auto configurationButton = MenuItemImage::create(
        "gui/MainMenuSmallButton.png", "gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIMainMenu::menuConfigurationCallback, this));
    configurationButton->setAnchorPoint(Vec2(0, 0.5));
    configurationButton->setPosition(Vec2((21 * visibleSize.width / 25), ((pos + 1) * visibleSize.height / 18)));
    auto confLabel = Label::createWithTTF(LocalizedString::create("OPTIONS"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    confLabel->setColor(Color3B(73, 109, 118));
    confLabel->setPosition(configurationButton->getContentSize().width / 2, configurationButton->getContentSize().height / 2);
    configurationButton->addChild(confLabel);
    configurationButton->setScale(GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(configurationButton);

    // links
    auto facebook = MenuItemImage::create("misc/facebook.png", "misc/facebookPressed.png", CC_CALLBACK_1(UIMainMenu::openFacebook, this));
    facebook->setAnchorPoint(Vec2(0, 0.5));
    facebook->setScale(GameData::getInstance()->getRaHConversion());
    facebook->setPosition(Vec2(0.86f*visibleSize.width, 0.93f*visibleSize.height));
    menuButtons.pushBack(facebook);
    
    auto twitter = MenuItemImage::create("misc/twitter.png", "misc/twitterPressed.png", CC_CALLBACK_1(UIMainMenu::openTwitter, this));
    twitter->setAnchorPoint(Vec2(0, 0.5));
    twitter->setScale(GameData::getInstance()->getRaHConversion());
    twitter->setPosition(Vec2(0.92f*visibleSize.width, 0.93f*visibleSize.height));
    menuButtons.pushBack(twitter);
    
    auto web = MenuItemImage::create("misc/browser.png", "misc/browserPressed.png", CC_CALLBACK_1(UIMainMenu::openWeb, this));
    web->setAnchorPoint(Vec2(0, 0.5));
    web->setScale(GameData::getInstance()->getRaHConversion());
    web->setPosition(Vec2(0.8f*visibleSize.width, 0.93f*visibleSize.height));
    menuButtons.pushBack(web);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setName("menu");
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 5, 4);

    if (GameData::getInstance()->getAnimationSpace() == true)
    {
        createSpaceAnimation();
    
        menu->setOpacity(0);
        auto fadeMenu = FadeIn::create(2.0);
        auto menuDelay = DelayTime::create(4.4);
        auto menuSeq = Sequence::create(menuDelay, fadeMenu, NULL);
        menu->runAction(menuSeq);
        GameData::getInstance()->setAnimationSpace(false);
    }
    else
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        auto background = Sprite::create("gui/MainMenuBackground.png");
        background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        this->addChild(background, 0);
        
        auto title = Sprite::create("gui/MainMenuTitle.png");
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setPosition(Vec2((2 * visibleSize.width / 25), (12 * visibleSize.height / 18)));
        title->setScale(GameData::getInstance()->getRaHConversion());
        this->addChild(title, 5, 0);
        
        auto planet1 = Sprite::create("gui/MainMenuBackgroundPlanet1.png");
        planet1->setPosition(Vec2((visibleSize.width / 2),(visibleSize.height / 2)));
        planet1->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        this->addChild(planet1, 2, 1);
        
        auto planet2 = Sprite::create("gui/MainMenuBackgroundPlanet2.png");
        planet2->setPosition(Vec2((visibleSize.width / 2),(visibleSize.height / 2)));
        planet2->setScale(GameData::getInstance()->getRaHConversion());
        this->addChild(planet2, 1, 2);
        
        auto spaceship = Sprite::create("gui/MainMenuBackgroundSpaceship.png");
        spaceship->setPosition(Vec2((visibleSize.width / 2), (visibleSize.height / 2)));
        spaceship->setScale(GameData::getInstance()->getRaHConversion());
        this->addChild(spaceship, 5, 3);
        
        auto particlesSpaceship = ParticleSun::create();
        particlesSpaceship->setGravity(Vec2(0, 0));
        //16,12
        particlesSpaceship->setPosition(Vec2(14 * (spaceship->getContentSize().width / 25), 13 * (spaceship->getContentSize().height / 25)));
        spaceship->addChild(particlesSpaceship, -1, 1);
        stoppedAnimation = true;
    }

    if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/main.mp3", true);
    }
    
    //LOAD ACHIEVEMENTS FROM XML
    if ( GameData::getInstance()->getAchievements().size() <= 0)
    {
        GameData::getInstance()->loadAchievements();
    }

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIMainMenu::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX or CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    {
        this->setKeyboardEnabled(true);
    }
    
    this->scheduleUpdate();
    
    return true;
}

void UIMainMenu::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if (stoppedAnimation == false) {
        endActions();
    }
}

void UIMainMenu::menuContinueCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {        
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        
        auto scene = UIProgressMap::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuStartCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        if (UserDefault::getInstance()->getBoolForKey("firsttimeplaying") == false)
        {
            //WARNING ERASING GAME DATA
            if (this->getChildByName("warningWindow") == nullptr)
            {
                if (GameData::getInstance()->getSFX() == true) {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
                }
                //LOAD INTRO IMAGES TO CACHE
                loadIntroImages();
                createWarningWindow(false);
                ((MenuItemImage*) pSender)->setEnabled(false);
            }
        }
        else
        {
            GameData::getInstance()->resetGameProgress();
            if (GameData::getInstance()->getSFX() == true) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
            }
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            auto scene = UIIntroStory::createScene();
            auto transition = TransitionFade::create(1.0f, scene);
            Director::getInstance()->replaceScene(transition);
        }
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuExtrasCallback(Ref* pSender)
{
    auto extrasButton = (MenuItemToggle*) pSender;
    if (stoppedAnimation or allActionsFinished()) {
        if (this->getChildByName("menu")->getChildByName("story")->isVisible() == false)
        {
            this->getChildByName("menu")->getChildByName("story")->setVisible(true);
            this->getChildByName("menu")->getChildByName("achievements")->setVisible(true);
            extrasButton->setSelectedIndex(1);
        }
        else
        {
            this->getChildByName("menu")->getChildByName("story")->setVisible(false);
            this->getChildByName("menu")->getChildByName("achievements")->setVisible(false);
            extrasButton->setSelectedIndex(0);
        }
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
    }
    else {
        extrasButton->setSelectedIndex(0);
        endActions();
    }
}

void UIMainMenu::menuStoryCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        _loading = true;
        
        update(0);
        auto scene = UIStoryGallery::createScene();
        auto transition = TransitionFade::create(0.5f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuAchievementsCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        
        auto scene = UIAchievements::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuConfigurationCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        auto scene = UIConfiguration::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuCreditsCallback(Ref* pSender)
{
    if (stoppedAnimation or allActionsFinished()) {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
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
    if (stoppedAnimation or allActionsFinished()) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    }
    else {
        endActions();
    }
}

void UIMainMenu::menuResetNoCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    this->removeChildByTag(30);
    if (_exit == false)
    {
        ((MenuItemImage*)(this->getChildByTag(4)->getChildByName("newCampaign")))->setEnabled(true);
    }
}

void UIMainMenu::menuResetYesCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    if (_exit == false)
    {
        GameData::getInstance()->resetGameProgress();
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        auto scene = UIIntroStory::createScene();
        auto transition = TransitionFade::create(1.0f, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else
    {
        menuExitCallback(this);
    }
}

void UIMainMenu::endActions(void)
{
    ParticleSun *p = (ParticleSun*)(this->getChildByTag(3)->getChildByTag(1));
    p->stopSystem();
    
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
    this->getChildByTag(3)->setScale(GameData::getInstance()->getRaHConversion());
    p->resetSystem();
    this->getChildByTag(4)->stopAllActions();
    this->getChildByTag(4)->setOpacity(255);
    this->getChildByName("menu")->getChildByName("extras")->stopAllActions();
    this->getChildByName("menu")->getChildByName("extras")->setOpacity(255);
}

bool UIMainMenu::allActionsFinished(void)
{
    if (this->getChildByTag(0)->getNumberOfRunningActions() == 0 and this->getChildByTag(1)->getNumberOfRunningActions() == 0 and this->getChildByTag(2)->getNumberOfRunningActions() == 0 and this->getChildByTag(3)->getNumberOfRunningActions() == 0 and
        this->getChildByTag(4)->getNumberOfRunningActions() == 0) {
        return true;
    }
    return false;
}

void UIMainMenu::createWarningWindow(bool exit)
{
    _exit = exit;
    auto alertBackground = Sprite::create("gui/ConfigurationAlert.png");
    alertBackground->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    alertBackground->setName("warningPopup");
    
    auto alertLabel = Label::createWithTTF(LocalizedString::create("WARNING"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    alertLabel->setColor(Color3B(255, 255, 255));
    alertLabel->setPosition(Vec2(alertBackground->getBoundingBox().size.width / 2, 5 * alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertLabel);
    
    Label* alertTextLabel;
    Label* alertConfirmationLabel;
    if (exit == false)
    {
        alertTextLabel = Label::createWithTTF(LocalizedString::create("WARNING_TEXT"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
        alertConfirmationLabel = Label::createWithTTF(LocalizedString::create("START") + string(" ?"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    }
    else
    {
        alertTextLabel = Label::createWithTTF(LocalizedString::create("WARNING_TEXT_EXIT"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
        alertConfirmationLabel = Label::createWithTTF(LocalizedString::create("SURE") + string(" ?"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    }
    
    alertTextLabel->setColor(Color3B(255, 255, 255));
    alertTextLabel->setMaxLineWidth(0.9f*alertBackground->getContentSize().width);
    alertTextLabel->setAlignment(TextHAlignment::CENTER);
    alertTextLabel->setPosition(Vec2(alertBackground->getBoundingBox().size.width / 2, 3 * alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertTextLabel);
    

    alertConfirmationLabel->setColor(Color3B(255, 255, 255));
    alertConfirmationLabel->setPosition(Vec2(1.2 * alertBackground->getBoundingBox().size.width / 4,
                                             alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertConfirmationLabel);
    
    Vector<MenuItem*> confirmReset;
    auto confirmResetYes = MenuItemImage::create("gui/ConfigurationResetYes.png", "gui/ConfigurationResetYesPressed.png",
                                                 CC_CALLBACK_1(UIMainMenu::menuResetYesCallback, this));
    confirmResetYes->setPosition(Vec2(Vec2(4 * alertBackground->getBoundingBox().size.width / 6,
                                           alertBackground->getBoundingBox().size.height / 6)));
    confirmReset.pushBack(confirmResetYes);
    auto confirmSeparator = Sprite::create("gui/ConfigurationResetSeparator.png");
    confirmSeparator->setPosition(Vec2(Vec2(4.65 * alertBackground->getBoundingBox().size.width / 6,
                                            alertBackground->getBoundingBox().size.height / 6)));
    alertBackground->addChild(confirmSeparator);
    auto confirmResetNo = MenuItemImage::create("gui/ConfigurationResetNo.png", "gui/ConfigurationResetNoPressed.png",
                                                CC_CALLBACK_1(UIMainMenu::menuResetNoCallback, this));
    confirmResetNo->setPosition(Vec2(Vec2(5.2 * alertBackground->getBoundingBox().size.width / 6,
                                          alertBackground->getBoundingBox().size.height / 6)));
    confirmReset.pushBack(confirmResetNo);
    
    auto menuConfirmReset = Menu::createWithArray(confirmReset);
    menuConfirmReset->setPosition(0, 0);
    alertBackground->addChild(menuConfirmReset, 10);
    alertBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    this->addChild(alertBackground, 30, 30);
}

void UIMainMenu::openFacebook( Ref * pSender )
{
    Application::getInstance()->openURL("https://www.facebook.com/evolvingPlanetGame");    
}

void UIMainMenu::openTwitter( Ref * pSender )
{
    Application::getInstance()->openURL("https://twitter.com/evoPlanetGame");    
}

void UIMainMenu::openWeb( Ref * pSender )
{
    Application::getInstance()->openURL("http://evolvingplanetgame.com");    
}

void UIMainMenu::createSpaceAnimation(void)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);
    
    auto title = Sprite::create("gui/MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25), (12 * visibleSize.height / 18)));
    title->setOpacity(0);
    title->setScale(GameData::getInstance()->getRaHConversion());
    auto fadeTitle = FadeIn::create(2.0);
    auto titleDelay = DelayTime::create(4.0);
    auto titleSeq = Sequence::create(titleDelay, fadeTitle, NULL);
    title->runAction(titleSeq);
    this->addChild(title, 5, 0);
    
    auto planet1 = Sprite::create("gui/MainMenuBackgroundPlanet1.png");
    planet1->setPosition(Vec2((3 * visibleSize.width / 25),
                              (7 * visibleSize.height / 18)));
    planet1->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto movePlanet1 = MoveTo::create(4, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto moveEasePlanet1 = EaseInOut::create(movePlanet1, 1);
    planet1->runAction(moveEasePlanet1);
    this->addChild(planet1, 2, 1);
    
    auto planet2 = Sprite::create("gui/MainMenuBackgroundPlanet2.png");
    planet2->setPosition(Vec2((15 * visibleSize.width / 25),
                              (13 * visibleSize.height / 18)));
    planet2->setScale(GameData::getInstance()->getRaHConversion());
    auto movePlanet2 = MoveTo::create(4.8, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto moveEasePlanet2 = EaseBackOut::create(movePlanet2);
    planet2->runAction(moveEasePlanet2);
    this->addChild(planet2, 1, 2);
    
    auto spaceship = Sprite::create("gui/MainMenuBackgroundSpaceship.png");
    spaceship->setPosition(Vec2((visibleSize.width / 3), (visibleSize.height / 2)));
    spaceship->setScale(0);
    auto scaleSpaceship = ScaleTo::create(3.0, GameData::getInstance()->getRaHConversion());
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
    spaceship->addChild(particlesSpaceship, -1, 1);
}

void UIMainMenu::loading(bool b)
{
    /*auto loading = this->getChildByName("loading");
    loading->setVisible(true);*/
}

void UIMainMenu::update(float delta)
{
    auto loading = this->getChildByName("loading");
    loading->setVisible(_loading);
}

void UIMainMenu::loadIntroImages(void)
{
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/planet.png", CC_CALLBACK_1(UIMainMenu::loading, this));
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/chamber_1.png", CC_CALLBACK_1(UIMainMenu::loading, this));
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/CamaraFront.png", CC_CALLBACK_1(UIMainMenu::loading, this));
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/transport_cut.jpg", CC_CALLBACK_1(UIMainMenu::loading, this));
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/transport.jpg", CC_CALLBACK_1(UIMainMenu::loading, this));
    Director::getInstance()->getTextureCache()->addImageAsync("misc/intro/logo.png", CC_CALLBACK_1(UIMainMenu::loading, this));
}

void UIMainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        if (this->getChildByName("warningWindow") == nullptr)
        {
            createWarningWindow(true);
        }
    }
}

