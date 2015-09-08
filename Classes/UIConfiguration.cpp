/*
 * Copyright (c) 2015
 * MURPHY'S TOAST STUDIOS
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

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);

    auto title = Sprite::create("MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
        (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(title, 5, 0);

    auto planet2 = Sprite::create("MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
        (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(planet2, 1, 2);

    auto popupBackground = Sprite::create("ConfigurationBackground.png");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    this->addChild(popupBackground, 6);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIConfiguration::menuBackCallback, this));
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setPosition(Vec2(1 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    popupBackground->addChild(menu, 1, 20);

    auto configLabel = Label::createWithTTF(LocalizedString::create("CONFIGURATION"), "fonts/BebasNeue.otf", 100);
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);

    Vector<MenuItem*> languageItems;
    auto languageLabel = Label::createWithTTF(LocalizedString::create("LANGUAGE"), "fonts/BebasNeue.otf", 80);
    languageLabel->setColor(Color3B(72, 108, 118));
    languageLabel->setAnchorPoint(Vec2(1, 0.5));
    languageLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(languageLabel);

    auto catButton = MenuItemImage::create(
        "catFlag.png", "catFlagPressed.png", "catFlagPressed.png", CC_CALLBACK_1(UIConfiguration::catFlagCallback, this));
    catButton->setPosition(Vec2(11 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(catButton);

    auto esButton = MenuItemImage::create(
        "esFlag.png", "esFlagPressed.png", "esFlagPressed.png", CC_CALLBACK_1(UIConfiguration::esFlagCallback, this));
    esButton->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(esButton);

    auto enButton = MenuItemImage::create(
        "enFlag.png", "enFlagPressed.png", "enFlagPressed.png", CC_CALLBACK_1(UIConfiguration::enFlagCallback, this));
    enButton->setPosition(Vec2(19 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(enButton);

    string lang = GameData::getInstance()->getLanguage();
    if (lang == "cat") {
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

    Vector<MenuItem*> soundItems;

    auto musicLabel = Label::createWithTTF(LocalizedString::create("MUSIC"), "fonts/BebasNeue.otf", 80);
    musicLabel->setColor(Color3B(72, 108, 118));
    musicLabel->setAnchorPoint(Vec2(1, 0.5));
    musicLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(musicLabel);

    auto musicOnLabel = MenuItemImage::create("ActiveOff.png", "ActiveOff.png", "ActiveOn.png",
        CC_CALLBACK_1(UIConfiguration::musicOnCallback, this));
    musicOnLabel->setPosition(Vec2(Vec2(11 * popupBackground->getContentSize().width / 28,
        8 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOnLabel);

    auto musicSeparator = Sprite::create("ConfigurationSeparator.png");
    musicSeparator->setPosition(Vec2(Vec2(15 * popupBackground->getContentSize().width / 28,
        8 * popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(musicSeparator, 10);

    auto musicOffLabel = MenuItemImage::create("InactiveOff.png", "InactiveOff.png", "InactiveOn.png",
        CC_CALLBACK_1(UIConfiguration::musicOffCallback, this));
    musicOffLabel->setPosition(Vec2(Vec2(19 * popupBackground->getContentSize().width / 28,
        8 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOffLabel);

    auto sfxLabel = Label::createWithTTF(LocalizedString::create("SPECIAL_EFFECTS"), "fonts/BebasNeue.otf", 80);
    sfxLabel->setColor(Color3B(72, 108, 118));
    sfxLabel->setAnchorPoint(Vec2(1, 0.5));
    sfxLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(sfxLabel);

    auto sfxOnLabel = MenuItemImage::create("ActiveOff.png", "ActiveOff.png", "ActiveOn.png",
        CC_CALLBACK_1(UIConfiguration::sfxOnCallback, this));
    sfxOnLabel->setPosition(Vec2(Vec2(11 * popupBackground->getContentSize().width / 28,
        5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(sfxOnLabel);

    auto sfxSeparator = Sprite::create("ConfigurationSeparator.png");
    sfxSeparator->setPosition(Vec2(Vec2(15 * popupBackground->getContentSize().width / 28,
        5 * popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(sfxSeparator, 10);

    auto sfxOffLabel = MenuItemImage::create("InactiveOff.png", "InactiveOff.png", "InactiveOn.png",
        CC_CALLBACK_1(UIConfiguration::sfxOffCallback, this));
    sfxOffLabel->setPosition(Vec2(Vec2(19 * popupBackground->getContentSize().width / 28,
        5 * popupBackground->getContentSize().height / 16)));
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

    return true;
}

void UIConfiguration::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    UserDefault::getInstance()->setStringForKey("language", GameData::getInstance()->getLanguage());
    UserDefault::getInstance()->setBoolForKey("music", GameData::getInstance()->getMusic());
    UserDefault::getInstance()->setBoolForKey("sfx", GameData::getInstance()->getSFX());
    UserDefault::getInstance()->flush();

    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIConfiguration::catFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("cat");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::esFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("es");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::enFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("en");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::musicOnCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto musicOnLabel = (MenuItem*)pSender;
    musicOnLabel->setEnabled(false);
    auto menuSound = (Menu*)musicOnLabel->getParent();
    auto musicOffLabel = (MenuItem*)menuSound->getChildren().at(1);
    musicOffLabel->setEnabled(true);

    GameData::getInstance()->setMusic(true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
}

void UIConfiguration::musicOffCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
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
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
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
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto sfxOffLabel = (MenuItem*)pSender;
    sfxOffLabel->setEnabled(false);
    auto menuSound = (Menu*)sfxOffLabel->getParent();
    auto sfxOnLabel = (MenuItem*)menuSound->getChildren().at(2);
    sfxOnLabel->setEnabled(true);

    GameData::getInstance()->setSFX(false);
}
