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
//  UIConfiguration.cpp
//  simulplay
//
//  Created by Guillem Laborda on 03/09/14.
//
//

#include "UIConfiguration.h"
#include "GameData.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIConfiguration::createScene()
{
    auto scene = Scene::create();
    auto layer = UIConfiguration::create();
    scene->addChild(layer);
    return scene;
}

bool UIConfiguration::init()
{
    if (!Layer::init()) {
        return false;
    }

    if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX or CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    {
        this->setKeyboardEnabled(true);
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);

    auto title = Sprite::create("gui/MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
        (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(title, 5, 0);

    auto planet2 = Sprite::create("gui/MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
        (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(planet2, 1, 2);

    auto popupBackground = Sprite::create("gui/ConfigurationBackground.png");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    this->addChild(popupBackground, 6);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIConfiguration::menuBackCallback, this));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    backButton->setPosition(Vec2((4 * visibleSize.width / 34), (1.5 * visibleSize.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);

    auto configLabel = Label::createWithTTF(LocalizedString::create("OPTIONS"), "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);

    Vector<MenuItem*> languageItems;
    auto languageLabel = Label::createWithTTF(LocalizedString::create("LANGUAGE"), "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    languageLabel->setColor(Color3B(72, 108, 118));
    languageLabel->setAnchorPoint(Vec2(1, 0.5));
    languageLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(languageLabel);

    auto catButton = MenuItemImage::create(
        "gui/catFlag.png", "gui/catFlagPressed.png", "gui/catFlagPressed.png", CC_CALLBACK_1(UIConfiguration::catFlagCallback, this));
    catButton->setPosition(Vec2(11 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(catButton);

    auto esButton = MenuItemImage::create(
        "gui/esFlag.png", "gui/esFlagPressed.png", "gui/esFlagPressed.png", CC_CALLBACK_1(UIConfiguration::esFlagCallback, this));
    esButton->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(esButton);

    auto enButton = MenuItemImage::create(
        "gui/enFlag.png", "gui/enFlagPressed.png", "gui/enFlagPressed.png", CC_CALLBACK_1(UIConfiguration::enFlagCallback, this));
    enButton->setPosition(Vec2(19 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(enButton);

    string lang = GameData::getInstance()->getLanguage();
    if (lang == "ca") {
        catButton->setEnabled(false);
    }
    else if (lang == "es") {
        esButton->setEnabled(false);
    }
    else if (lang == "en") {
        enButton->setEnabled(false);
    }

    auto menuLanguage = Menu::createWithArray(languageItems);
    menuLanguage->setPosition(0, 0);
    popupBackground->addChild(menuLanguage, 10);
    
    Vector<MenuItem*> colorItems;
    auto colorLabel = Label::createWithTTF(LocalizedString::create("BOT_COLOR"), "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    colorLabel->setColor(Color3B(72, 108, 118));
    colorLabel->setAnchorPoint(Vec2(1, 0.5));
    colorLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(colorLabel);
    
    auto lightblueButton = MenuItemImage::create("gui/LightblueAgentColor.png", "gui/LightblueAgentColorPressed.png", "gui/LightblueAgentColorPressed.png", CC_CALLBACK_1(UIConfiguration::colorCallback, this));
    lightblueButton->setName("lightblue");
    lightblueButton->setPosition(Vec2(11 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    colorItems.pushBack(lightblueButton);
    
    auto darkblueButton = MenuItemImage::create("gui/DarkblueAgentColor.png", "gui/DarkblueAgentColorPressed.png", "gui/DarkblueAgentColorPressed.png", CC_CALLBACK_1(UIConfiguration::colorCallback, this));
    darkblueButton->setName("darkblue");
    darkblueButton->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    colorItems.pushBack(darkblueButton);
    
    auto whiteButton = MenuItemImage::create("gui/WhiteAgentColor.png", "gui/WhiteAgentColorPressed.png", "gui/WhiteAgentColorPressed.png", CC_CALLBACK_1(UIConfiguration::colorCallback, this));
    whiteButton->setName("white");
    whiteButton->setPosition(Vec2(19 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    colorItems.pushBack(whiteButton);
    
    auto purpleButton = MenuItemImage::create("gui/PurpleAgentColor.png", "gui/PurpleAgentColorPressed.png", "gui/PurpleAgentColorPressed.png", CC_CALLBACK_1(UIConfiguration::colorCallback, this));
    purpleButton->setName("purple");
    purpleButton->setPosition(Vec2(23 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    colorItems.pushBack(purpleButton);
    
    string color = GameData::getInstance()->getPlayerColorString();
    if (color == "lightblue") {
        lightblueButton->setEnabled(false);
    }
    else if (color == "darkblue") {
        darkblueButton->setEnabled(false);
    }
    else if (color == "white") {
        whiteButton->setEnabled(false);
    }
    else if (color == "purple") {
        purpleButton->setEnabled(false);
    }
    
    auto menuColor = Menu::createWithArray(colorItems);
    menuColor->setPosition(0, 0);
    popupBackground->addChild(menuColor, 10);

    Vector<MenuItem*> soundItems;
    auto musicLabel = Label::createWithTTF(LocalizedString::create("MUSIC"), "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    musicLabel->setColor(Color3B(72, 108, 118));
    musicLabel->setAnchorPoint(Vec2(1, 0.5));
    musicLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(musicLabel);

    auto musicOnLabel = MenuItemImage::create("gui/ActiveOff.png", "gui/ActiveOff.png", "gui/ActiveOn.png",
        CC_CALLBACK_1(UIConfiguration::musicOnCallback, this));
    musicOnLabel->setPosition(Vec2(Vec2(11 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOnLabel);

    auto musicSeparator = Sprite::create("gui/ConfigurationSeparator.png");
    musicSeparator->setPosition(Vec2(Vec2(12 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(musicSeparator, 10);

    auto musicOffLabel = MenuItemImage::create("gui/InactiveOff.png", "gui/InactiveOff.png", "gui/InactiveOn.png",
        CC_CALLBACK_1(UIConfiguration::musicOffCallback, this));
    musicOffLabel->setPosition(Vec2(Vec2(13 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOffLabel);

    auto sfxLabel = Label::createWithTTF(LocalizedString::create("SPECIAL_EFFECTS"), "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    sfxLabel->setColor(Color3B(72, 108, 118));
    sfxLabel->setAnchorPoint(Vec2(1, 0.5));
    sfxLabel->setPosition(Vec2(20 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(sfxLabel);

    auto sfxOnLabel = MenuItemImage::create("gui/ActiveOff.png", "gui/ActiveOff.png", "gui/ActiveOn.png",
        CC_CALLBACK_1(UIConfiguration::sfxOnCallback, this));
    sfxOnLabel->setPosition(Vec2(Vec2(22 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(sfxOnLabel);

    auto sfxSeparator = Sprite::create("gui/ConfigurationSeparator.png");
    sfxSeparator->setPosition(Vec2(Vec2(23 * popupBackground->getContentSize().width / 28, 5* popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(sfxSeparator, 10);

    auto sfxOffLabel = MenuItemImage::create("gui/InactiveOff.png", "gui/InactiveOff.png", "gui/InactiveOn.png",
        CC_CALLBACK_1(UIConfiguration::sfxOffCallback, this));
    sfxOffLabel->setPosition(Vec2(Vec2(24 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(sfxOffLabel);

    if (GameData::getInstance()->getMusic() == true) {
        musicOnLabel->setEnabled(false);
    }
    else {
        musicOffLabel->setEnabled(false);
    }

    if (GameData::getInstance()->getSFX() == true) {
        sfxOnLabel->setEnabled(false);
    }
    else {
        sfxOffLabel->setEnabled(false);
    }

    auto menuSound = Menu::createWithArray(soundItems);
    menuSound->setPosition(0, 0);
    popupBackground->addChild(menuSound, 10);

    auto speedLabel = Label::createWithTTF(LocalizedString::create("GAME_SPEED"), "fonts/BebasNeue.otf", 90 * GameData::getInstance()->getRaConversion());
    speedLabel->setColor(Color3B(72, 108, 118));
    speedLabel->setAnchorPoint(Vec2(1, 0.5));
    speedLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(speedLabel);
 
    Vector<MenuItem*> speedItems;
    
    auto slowButton = MenuItemImage::create("gui/MainMenuSmallButton.png", "gui/MainMenuSmallButton.png","gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIConfiguration::speedSlowCallback, this));
    slowButton->setAnchorPoint(Vec2(0, 0.5));
    //slowButton->setScale(1.5f);
    slowButton->setPosition(Vec2(10 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    slowButton->setName("slowButton");
    auto slowLabel = Label::createWithTTF(LocalizedString::create("SLOW"), "fonts/BebasNeue.otf", 45 * GameData::getInstance()->getRaConversion());
    slowLabel->setColor(Color3B(205, 202, 207));
    slowLabel->setPosition(slowButton->getContentSize().width / 2, slowButton->getContentSize().height / 2);
    slowButton->addChild(slowLabel);
    speedItems.pushBack(slowButton);

    auto mediumButton = MenuItemImage::create("gui/MainMenuSmallButton.png", "gui/MainMenuSmallButton.png","gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIConfiguration::speedMediumCallback, this));
    mediumButton->setAnchorPoint(Vec2(0, 0.5));
    //mediumButton->setScale(1.5f);
    mediumButton->setPosition(Vec2(16 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    mediumButton->setName("mediumButton");
    auto mediumLabel = Label::createWithTTF(LocalizedString::create("MEDIUM"), "fonts/BebasNeue.otf", 45 * GameData::getInstance()->getRaConversion());
    mediumLabel->setColor(Color3B(205, 202, 207));
    mediumLabel->setPosition(mediumButton->getContentSize().width / 2, mediumButton->getContentSize().height / 2);
    mediumButton->addChild(mediumLabel);
    speedItems.pushBack(mediumButton);   
    
    auto fastButton = MenuItemImage::create("gui/MainMenuSmallButton.png", "gui/MainMenuSmallButton.png","gui/MainMenuSmallButtonPressed.png", CC_CALLBACK_1(UIConfiguration::speedFastCallback, this));
    fastButton->setAnchorPoint(Vec2(0, 0.5));
    //fastButton->setScale(1.5f);
    fastButton->setPosition(Vec2(22 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    fastButton->setName("fastButton");
    auto fastLabel = Label::createWithTTF(LocalizedString::create("FAST"), "fonts/BebasNeue.otf", 45 * GameData::getInstance()->getRaConversion());
    fastLabel->setColor(Color3B(205, 202, 207));
    fastLabel->setPosition(fastButton->getContentSize().width / 2, fastButton->getContentSize().height / 2);
    fastButton->addChild(fastLabel);
    speedItems.pushBack(fastButton);

    if(GameData::getInstance()->getGameSpeed()==eSlow)
    {
        slowButton->setEnabled(false);
    }
    else if(GameData::getInstance()->getGameSpeed()==eMed)
    {
        mediumButton->setEnabled(false);
    }
    else
    {
        fastButton->setEnabled(false);
    }

    auto menuSpeed = Menu::createWithArray(speedItems);
    menuSpeed->setPosition(0, 0);
    popupBackground->addChild(menuSpeed, 10);

    return true;
}

void UIConfiguration::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    UserDefault::getInstance()->setStringForKey("language", GameData::getInstance()->getLanguage());
    UserDefault::getInstance()->setBoolForKey("music", GameData::getInstance()->getMusic());
    UserDefault::getInstance()->setBoolForKey("sfx", GameData::getInstance()->getSFX());
    UserDefault::getInstance()->setStringForKey("playerColor", GameData::getInstance()->getPlayerColorString());
    UserDefault::getInstance()->setIntegerForKey("gameSpeed", int(GameData::getInstance()->getGameSpeed()));
    UserDefault::getInstance()->flush();

    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIConfiguration::catFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }

    GameData::getInstance()->setLanguage("ca");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::esFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }

    GameData::getInstance()->setLanguage("es");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::enFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }

    GameData::getInstance()->setLanguage("en");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::colorCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }

    auto color = (MenuItem*)pSender;
    GameData::getInstance()->setPlayerColor(color->getName());
    for (int i = 0; i < color->getParent()->getChildren().size(); i++)
    {
        ((MenuItem*)(color->getParent()->getChildren().at(i)))->setEnabled(true);
    }
    color->setEnabled(false);
}

void UIConfiguration::musicOnCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    auto musicOnLabel = (MenuItem*)pSender;
    musicOnLabel->setEnabled(false);
    auto menuSound = (Menu*)musicOnLabel->getParent();
    auto musicOffLabel = (MenuItem*)menuSound->getChildren().at(1);
    musicOffLabel->setEnabled(true);

    GameData::getInstance()->setMusic(true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/main.mp3", true);
}

void UIConfiguration::musicOffCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    auto musicOffLabel = (MenuItem*)pSender;
    musicOffLabel->setEnabled(false);
    auto menuSound = (Menu*)musicOffLabel->getParent();
    auto musicOnLabel = (MenuItem*)menuSound->getChildren().at(0);
    musicOnLabel->setEnabled(true);

    GameData::getInstance()->setMusic(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void UIConfiguration::sfxOnCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    auto sfxOnLabel = (MenuItem*)pSender;
    sfxOnLabel->setEnabled(false);
    auto menuSound = (Menu*)sfxOnLabel->getParent();
    auto sfxOffLabel = (MenuItem*)menuSound->getChildren().at(3);
    sfxOffLabel->setEnabled(true);

    GameData::getInstance()->setSFX(true);
}

void UIConfiguration::sfxOffCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    auto sfxOffLabel = (MenuItem*)pSender;
    sfxOffLabel->setEnabled(false);
    auto menuSound = (Menu*)sfxOffLabel->getParent();
    auto sfxOnLabel = (MenuItem*)menuSound->getChildren().at(2);
    sfxOnLabel->setEnabled(true);

    GameData::getInstance()->setSFX(false);
}

void UIConfiguration::speedSlowCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    // slow -> med -> fast
    auto pressed = (MenuItem*)pSender;
    pressed->setEnabled(false);

    auto menu = pressed->getParent();
    auto med = (MenuItem*)menu->getChildren().at(1);
    med->setEnabled(true);
    auto fast = (MenuItem*)menu->getChildren().at(2);
    fast->setEnabled(true);

    GameData::getInstance()->setGameSpeed(eSlow);
}

void UIConfiguration::speedMediumCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }    
    auto pressed = (MenuItem*)pSender;
    pressed->setEnabled(false);

    auto menu = pressed->getParent();
    auto slow = (MenuItem*)menu->getChildren().at(0);
    slow->setEnabled(true);
    auto fast = (MenuItem*)menu->getChildren().at(2);
    fast->setEnabled(true);
    
    GameData::getInstance()->setGameSpeed(eMed);
}

void UIConfiguration::speedFastCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }  
    auto pressed = (MenuItem*)pSender;
    pressed->setEnabled(false);

    auto menu = pressed->getParent();
    auto slow = (MenuItem*)menu->getChildren().at(0);
    slow->setEnabled(true);
    auto med = (MenuItem*)menu->getChildren().at(1);
    med->setEnabled(true);

    GameData::getInstance()->setGameSpeed(eFast);
}

void UIConfiguration::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        menuBackCallback(this);
    }
}

