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
//  UICredits.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UICredits.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UICredits::createScene()
{
    auto scene = Scene::create();
    auto layer = UICredits::create();
    scene->addChild(layer);
    return scene;
}

bool UICredits::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
                                 visibleSize.height / 2));
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
    auto backButton = MenuItemImage::create( "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UICredits::menuBackCallback, this));
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
    
    auto configLabel = Label::createWithTTF(LocalizedString::create("CREDITS"), "fonts/BebasNeue.otf", 100);
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2((popupBackground->getContentSize().width / 28) + backButton->getContentSize().width,
                                  13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);
    
    auto programmerLabel = Label::createWithTTF(LocalizedString::create("PROGRAMMER"), "fonts/BebasNeue.otf", 80);
    programmerLabel->setColor(Color3B(72, 108, 118));
    programmerLabel->setAnchorPoint(Vec2(0, 0.5));
    programmerLabel->setPosition(Vec2(3 * popupBackground->getContentSize().width / 28, 11.7 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(programmerLabel);
    auto xpLabel = Label::createWithTTF("Xavier Rubio", "fonts/BebasNeue.otf", 50);
    xpLabel->setColor(Color3B(72, 108, 118));
    xpLabel->setAnchorPoint(Vec2(0, 0.5));
    xpLabel->setPosition(Vec2(4 * popupBackground->getContentSize().width / 28, 10.4 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(xpLabel);
    auto gpLabel = Label::createWithTTF("Guillem Laborda", "fonts/BebasNeue.otf", 50);
    gpLabel->setColor(Color3B(72, 108, 118));
    gpLabel->setAnchorPoint(Vec2(0, 0.5));
    gpLabel->setPosition(Vec2(4 * popupBackground->getContentSize().width / 28, 9.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(gpLabel);
    
    auto designerLabel = Label::createWithTTF(LocalizedString::create("GRAPHIC_DESIGNER"), "fonts/BebasNeue.otf", 80);
    designerLabel->setColor(Color3B(72, 108, 118));
    designerLabel->setAnchorPoint(Vec2(0, 0.5));
    designerLabel->setPosition(Vec2(3 * popupBackground->getContentSize().width / 28, 7.3 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(designerLabel);
    auto gdLabel = Label::createWithTTF("Guillem H. Pongiluppi", "fonts/BebasNeue.otf", 50);
    gdLabel->setColor(Color3B(72, 108, 118));
    gdLabel->setAnchorPoint(Vec2(0, 0.5));
    gdLabel->setPosition(Vec2(4 * popupBackground->getContentSize().width / 28, 6.2 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(gdLabel);
    auto ddLabel = Label::createWithTTF("David Ramos", "fonts/BebasNeue.otf", 50);
    ddLabel->setColor(Color3B(72, 108, 118));
    ddLabel->setAnchorPoint(Vec2(0, 0.5));
    ddLabel->setPosition(Vec2(4 * popupBackground->getContentSize().width / 28, 5.3 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(ddLabel);
    
    auto musicLabel = Label::createWithTTF(LocalizedString::create("MUSIC_AND_SFX"), "fonts/BebasNeue.otf", 80);
    musicLabel->setColor(Color3B(72, 108, 118));
    musicLabel->setAnchorPoint(Vec2(0, 0.5));
    musicLabel->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 12 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(musicLabel);
    auto gsLabel = Label::createWithTTF("Guillem Laborda", "fonts/BebasNeue.otf", 50);
    gsLabel->setColor(Color3B(72, 108, 118));
    gsLabel->setAnchorPoint(Vec2(0, 0.5));
    gsLabel->setPosition(Vec2(16 * popupBackground->getContentSize().width / 28, 10.7 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(gsLabel);
    
    auto contentLabel = Label::createWithTTF(LocalizedString::create("CONTENT_AND_STORY"), "fonts/BebasNeue.otf", 80);
    contentLabel->setColor(Color3B(72, 108, 118));
    contentLabel->setAnchorPoint(Vec2(0, 0.5));
    contentLabel->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 8.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(contentLabel);
    auto jcLabel = Label::createWithTTF("Jorge Caro", "fonts/BebasNeue.otf", 50);
    jcLabel->setColor(Color3B(72, 108, 118));
    jcLabel->setAnchorPoint(Vec2(0, 0.5));
    jcLabel->setPosition(Vec2(16 * popupBackground->getContentSize().width / 28, 7.2 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(jcLabel);
    auto xcLabel = Label::createWithTTF("Xavier Rubio", "fonts/BebasNeue.otf", 50);
    xcLabel->setColor(Color3B(72, 108, 118));
    xcLabel->setAnchorPoint(Vec2(0, 0.5));
    xcLabel->setPosition(Vec2(16 * popupBackground->getContentSize().width / 28, 6.3 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(xcLabel);
    
    auto directorLabel = Label::createWithTTF(LocalizedString::create("DIRECTOR_AND_SLAVER"), "fonts/BebasNeue.otf", 80);
    directorLabel->setColor(Color3B(72, 108, 118));
    directorLabel->setAnchorPoint(Vec2(0, 0.5));
    directorLabel->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 4.1 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(directorLabel);
    auto xdLabel = Label::createWithTTF("Xavier Rubio", "fonts/BebasNeue.otf", 50);
    xdLabel->setColor(Color3B(72, 108, 118));
    xdLabel->setAnchorPoint(Vec2(0, 0.5));
    xdLabel->setPosition(Vec2(16 * popupBackground->getContentSize().width / 28, 2.8 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(xdLabel);
    
    return true;
}

void UICredits::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}
