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
    //this->addChild(background, 0);
    
    auto title = Sprite::create("gui/MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
                            (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    //this->addChild(title, 5, 0);
    
    auto planet2 = Sprite::create("gui/MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
                              (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    //this->addChild(planet2, 1, 2);
    
    hexagonButtonLevel0 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel0->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel0->setPosition(Vec2((visibleSize.width / 2) - (hexagonButtonLevel0->getBoundingBox().size.width * 6),
                                          visibleSize.height / 20));
    hexagonButtonLevel0->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel0->setEnabled(false);
    this->addChild(hexagonButtonLevel0, 1);
    
    hexagonButtonLevel1 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel1->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel1->setPosition(Vec2((visibleSize.width / 2) - (hexagonButtonLevel0->getBoundingBox().size.width * 2), visibleSize.height / 20));
    hexagonButtonLevel1->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel1->setEnabled(false);
    this->addChild(hexagonButtonLevel1, 1);
    
    hexagonButtonLevel2 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel2->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel2->setPosition(Vec2((visibleSize.width / 2) + (hexagonButtonLevel0->getBoundingBox().size.width * 2), visibleSize.height / 20));
    hexagonButtonLevel2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel2->setEnabled(false);
    this->addChild(hexagonButtonLevel2, 1);
    
    hexagonButtonLevel3 = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                "gui/ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel3->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel3->setPosition(Vec2((visibleSize.width / 2) + (hexagonButtonLevel0->getBoundingBox().size.width * 6), visibleSize.height / 20));
    hexagonButtonLevel3->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    hexagonButtonLevel3->setEnabled(false);
    this->addChild(hexagonButtonLevel3, 1);
    
    Vector<MenuItem*> menuButtons;
    arrowBack = MenuItemImage::create("gui/ArrowBack.png", "gui/ArrowBackPressed.png", CC_CALLBACK_1(UICredits::menuArrowBackCallback, this));
    arrowBack->setPosition(Vec2((2 * visibleSize.width / 42), (7.5 * visibleSize.height / 18)));
    arrowBack->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowBack);
    
    arrowNext = MenuItemImage::create("gui/ArrowNext.png", "gui/ArrowNextPressed.png", CC_CALLBACK_1(UICredits::menuArrowNextCallback, this));
    arrowNext->setPosition(Vec2((40 * visibleSize.width / 42), (7.5 * visibleSize.height / 18)));
    arrowNext->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowNext);
    
    auto backButton = MenuItemImage::create("gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UICredits::menuBackCallback, this));
    backButton->setPosition(Vec2((4 * visibleSize.width / 34), (1.5 * visibleSize.height / 25)));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);
    
    pages = PageView::create();
    pages->setCustomScrollThreshold(visibleSize.width / 6);
    pages->addChild(background);
    pages->addChild(title);
    pages->addChild(planet2);
    
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    
    auto layoutSimulpast = Layout::create();
    simulpast(layoutSimulpast);
    pages->addPage(layoutSimulpast);
    
    auto layoutMurphys = Layout::create();
    murphysToastStudios(layoutMurphys);
    pages->addPage(layoutMurphys);
    
    auto layoutThanks1 = Layout::create();
    specialThanks1(layoutThanks1);
    pages->addPage(layoutThanks1);
    
    auto layoutThanks2 = Layout::create();
    specialThanks2(layoutThanks2);
    pages->addPage(layoutThanks2);
    
    this->addChild(pages);
    
    this->scheduleUpdate();
    
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


void UICredits::menuArrowBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurPageIndex() <= 0)
    {
        return;
    }
    pages->scrollToPage(pages->getCurPageIndex() - 1);
}

void UICredits::menuArrowNextCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurPageIndex() >= pages->getPages().size() - 1)
    {
        return;
    }
    pages->scrollToPage(pages->getCurPageIndex() + 1);
}


void UICredits::simulpast(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/ConfigurationBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //SIMULPAST
    
    auto configLabel = Label::createWithTTF("SIMULPAST", "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0, 0.5));
    configLabel->setPosition(Vec2((3 * popupBackground->getContentSize().width / 28),
                                  14 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);
    
}


void UICredits::createGroup( const std::string & groupName, float x, float y, Sprite * background)
{
    auto label = Label::createWithTTF(LocalizedString::create(groupName.c_str()), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(72, 108, 118));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(x * background->getContentSize().width, y * background->getContentSize().height));
    background->addChild(label);
}


void UICredits::createName( const std::string & name, float x, float y, Sprite * background)
{   
    auto label = Label::createWithTTF(name, "fonts/arial_rounded_mt_bold.ttf", 50 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(72, 108, 118));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(x * background->getContentSize().width, y * background->getContentSize().height));
    background->addChild(label);
}

void UICredits::murphysToastStudios(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/ConfigurationBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //MURPHYS TOAST
    auto configLabel = Label::createWithTTF(LocalizedString::create("MURPHY"), "fonts/BebasNeue.otf", 70 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0, 0.5));
    configLabel->setPosition(Vec2(0.36f * popupBackground->getContentSize().width, 0.9f*popupBackground->getContentSize().height));
    popupBackground->addChild(configLabel);

    // first column
    // between names: 0.06
    // from last name to next group: 0.12f
    createGroup("DIRECTOR", 0.1f, 0.75f, popupBackground);
    createName("Xavier Rubio-Campillo", 0.13f, 0.69f, popupBackground);
        
    createGroup("LEAD_PROGRAMMER", 0.1f, 0.57f, popupBackground);
    createName("Guillem Laborda", 0.13f, 0.51f, popupBackground);

    createGroup("ADDITIONAL_PROGRAMMING", 0.1f, 0.39f, popupBackground);
    createName("Xavier Rubio-Campillo", 0.13f, 0.33f, popupBackground);
   
    createGroup("ART", 0.1f, 0.21f, popupBackground);
    createName("Guillem H. Pongiluppi", 0.13f, 0.15f, popupBackground);
    createName("David Ramos", 0.13f, 0.09f, popupBackground);
    
    // second column  
    // between names: 0.07
    // from last name to next group: 0.14f

    createGroup("UI", 0.6f, 0.75f, popupBackground);
    createName("David Ramos", 0.63f, 0.68f, popupBackground);

    createGroup("STORY", 0.6f, 0.54f, popupBackground);
    createName("Xavier Rubio-Campillo", 0.63f, 0.47f, popupBackground);
    createName("Jorge Caro", 0.63f, 0.40f, popupBackground);
    createName("Guillem H. Pongiluppi", 0.63f, 0.33f, popupBackground);

    createGroup("MUSIC", 0.6f, 0.19f, popupBackground);
    createName("Guillem Laborda", 0.63f, 0.12f, popupBackground);
}

void UICredits::specialThanks1(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/ConfigurationBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //THANKS 1
    
    auto configLabel = Label::createWithTTF("SPECIAL THANKS", "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0, 0.5));
    configLabel->setPosition(Vec2((3 * popupBackground->getContentSize().width / 28),
                                  14 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);
}

void UICredits::specialThanks2(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/ConfigurationBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //THANKS 2
    
    auto configLabel = Label::createWithTTF("SPECIAL THANKS", "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0, 0.5));
    configLabel->setPosition(Vec2((3 * popupBackground->getContentSize().width / 28),
                                  14 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);
}

void UICredits::update(float delta)
{
    if (pages->getCurPageIndex() == 0)
    {
        hexagonButtonLevel0->setEnabled(true);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(false);
        hexagonButtonLevel3->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 1)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(true);
        hexagonButtonLevel2->setEnabled(false);
        hexagonButtonLevel3->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 2)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(true);
        hexagonButtonLevel3->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 3)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(false);
        hexagonButtonLevel3->setEnabled(true);
    }
    
    if (pages->getCurPageIndex() == 0)
    {
        arrowBack->setVisible(false);
        arrowNext->setVisible(true);
    }
    else if (pages->getCurPageIndex() == 3)
    {
        arrowBack->setVisible(true);
        arrowNext->setVisible(false);
    }
    else
    {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    }
}
