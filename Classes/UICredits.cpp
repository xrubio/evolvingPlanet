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
#include "../libs/pugixml/pugixml.hpp"

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
    loadAcknowledgements();
    
    if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX or CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    {
        this->setKeyboardEnabled(true);
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
    auto popupBackground = Sprite::create("gui/EmptyBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size(0.9f*visibleSize.width, 0.9f*visibleSize.height));
    
    //SIMULPAST
    
    auto configLabel = Label::createWithTTF(LocalizedString::create("SIMULPAST_TITLE"), "fonts/BebasNeue.otf", 80 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0.5, 0.5));
    configLabel->setPosition(Vec2(0.5f * popupBackground->getContentSize().width, 0.9f*popupBackground->getContentSize().height));
    popupBackground->addChild(configLabel);
        
    auto simulText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("SIMULPAST_TEXT"), Size(0.9f*popupBackground->getContentSize().width, 0.9f*popupBackground->getContentSize().height), TextHAlignment::LEFT, "fonts/BebasNeue.otf", 45 * GameData::getInstance()->getRaConversion());
    simulText->setColorSpaceHolder(Color3B(216, 229, 235));
    simulText->setAnchorPoint(Vec2(0, 0.5));
    simulText->setPosition(Vec2(0.05f * popupBackground->getContentSize().width, 0.5f * popupBackground->getContentSize().height));
    popupBackground->addChild(simulText);

    auto simulLink = MenuItemImage::create("misc/simulpast.png", "misc/simulpast.png", CC_CALLBACK_1(UICredits::openSimulLink, this));
    simulLink->setAnchorPoint(Vec2(0, 0.5));
    simulLink->setPosition(Vec2(0.8f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));

    auto simulMenu = Menu::create(simulLink, NULL);
    simulMenu->setPosition(0, 0);
    popupBackground->addChild(simulMenu);
}

void UICredits::createGroup( const std::string & groupName, float x, float y, Sprite * background)
{
    auto label = Label::createWithTTF(LocalizedString::create(groupName.c_str()), "fonts/BebasNeue.otf", 65 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(216, 229, 235));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(x * background->getContentSize().width, y * background->getContentSize().height));
    background->addChild(label);
}


void UICredits::createName( const std::string & name, float x, float y, Sprite * background)
{   
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 45 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(216, 229, 235));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(x * background->getContentSize().width, y * background->getContentSize().height));
    background->addChild(label);
}

void UICredits::murphysToastStudios(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/EmptyBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //MURPHYS TOAST
    auto configLabel = Label::createWithTTF(LocalizedString::create("MURPHY"), "fonts/BebasNeue.otf", 80 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0.5, 0.5));
    configLabel->setPosition(Vec2(0.5f * popupBackground->getContentSize().width, 0.9f*popupBackground->getContentSize().height));
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
    // between names: 0.06
    // from last name to next group: 0.12f

    createGroup("UI", 0.6f, 0.75f, popupBackground);
    createName("David Ramos", 0.63f, 0.69f, popupBackground);

    createGroup("STORY_CONTENT", 0.6f, 0.57f, popupBackground);
    createName("Xavier Rubio-Campillo", 0.63f, 0.51f, popupBackground);
    createName("Jorge Caro", 0.63f, 0.45f, popupBackground);
    createName("Guillem H. Pongiluppi", 0.63f, 0.39f, popupBackground);

    createGroup("MUSIC_CREDITS", 0.6f, 0.24f, popupBackground);
    createName("Guillem Laborda", 0.63f, 0.18f, popupBackground);
}

void UICredits::createAcknowledgment( const std::string & name, float x, float y, Sprite * background)
{   
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 40 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(216, 229, 235));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(x * background->getContentSize().width, y * background->getContentSize().height));
    background->addChild(label);
}

void UICredits::loadAcknowledgements() 
{
    _names.clear();
    string dir = "data/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + "credits.xml");
    pugi::xml_document doc;
    pugi::xml_parse_result result;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ssize_t tmpSize;
    const char* xmlData = (const char *)FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &tmpSize);
    result = doc.load(xmlData);
#else
    result = doc.load_file(fullPath.c_str());
#endif
    pugi::xml_node thanks = doc.child("thanks1");

    pugi::xml_node nameNode = thanks.child("name");
    while(nameNode!=nullptr)
    {
        _names.push_back(nameNode.attribute("value").value());
        nameNode = nameNode.next_sibling("name");

    }
}

void UICredits::specialThanks1(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/EmptyBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //THANKS 1
    
    auto configLabel = Label::createWithTTF(LocalizedString::create("SPECIAL_THANKS"), "fonts/BebasNeue.otf", 80 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(0, 0.5));
    configLabel->setPosition(Vec2(0.36f * popupBackground->getContentSize().width, 0.9f*popupBackground->getContentSize().height));
    popupBackground->addChild(configLabel);

   
    float x = 0.05f;
    float y = 0.8f;
    float xInterval = 0.25f;
    float yInterval = 0.05f;

    for(std::list<std::string>::const_iterator it=_names.begin(); it!=_names.end(); it++)
    {
        if(y<0.1f)
        {
            y = 0.8f;
            x += xInterval;
        }
        createAcknowledgment(*it, x, y, popupBackground);
        y -= yInterval;
    }
}

void UICredits::specialThanks2(Layout* layout)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto popupBackground = Sprite::create("gui/EmptyBackground.png");
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(popupBackground, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    
    //THANKS 2
    auto credits = Sprite::create("misc/logos.png");
    credits->setPosition(0.5f*popupBackground->getContentSize().width, 0.5f*popupBackground->getContentSize().height);
    popupBackground->addChild(credits);
    
    auto supportLabel = Label::createWithTTF(LocalizedString::create("SUPPORT"), "fonts/BebasNeue.otf", 60* GameData::getInstance()->getRaConversion()); 
    supportLabel->setColor(Color3B(255, 255, 255));
    supportLabel->setAnchorPoint(Vec2(0, 0.5));
    supportLabel->setPosition(Vec2(0.05f*popupBackground->getContentSize().width, 0.93f*popupBackground->getContentSize().height));
    popupBackground->addChild(supportLabel);

    auto poweredLabel = Label::createWithTTF(LocalizedString::create("POWERED"), "fonts/BebasNeue.otf", 60* GameData::getInstance()->getRaConversion()); 
    poweredLabel->setColor(Color3B(255, 255, 255));
    poweredLabel->setAnchorPoint(Vec2(0, 0.5));
    poweredLabel->setPosition(Vec2(0.05f*popupBackground->getContentSize().width, 0.35f*popupBackground->getContentSize().height));
    popupBackground->addChild(poweredLabel);
    
    auto wikimediaLabel = Label::createWithTTF(LocalizedString::create("CONTENT_WIKIMEDIA"), "fonts/BebasNeue.otf", 60* GameData::getInstance()->getRaConversion()); 
    wikimediaLabel->setColor(Color3B(255, 255, 255));
    wikimediaLabel->setAnchorPoint(Vec2(0, 0.5));
    wikimediaLabel->setMaxLineWidth(0.15f*popupBackground->getContentSize().width);
    wikimediaLabel->setPosition(Vec2(0.7f * popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
    popupBackground->addChild(wikimediaLabel);

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


void UICredits::openSimulLink( Ref * pSender )
{
    Application::getInstance()->openURL("http://simulpast.es");    
}


void UICredits::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        menuBackCallback(this);
    }
}
