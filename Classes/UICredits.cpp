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
    
    Vector<MenuItem*> languageItems;
    auto languageLabel = Label::createWithTTF(LocalizedString::create("LANGUAGE"), "fonts/BebasNeue.otf", 80);
    languageLabel->setColor(Color3B(72, 108, 118));
    languageLabel->setAnchorPoint(Vec2(1, 0.5));
    languageLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(languageLabel);
    
    
    auto menuLanguage = Menu::createWithArray(languageItems);
    menuLanguage->setPosition(0, 0);
    popupBackground->addChild(menuLanguage, 10);
    
    return true;
}

void UICredits::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}
