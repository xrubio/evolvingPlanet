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
//  UIStoryGallery.cpp
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 30/10/15.
//
//

#include "UIStoryGallery.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIStoryGallery::createScene()
{
    auto scene = Scene::create();
    auto layer = UIStoryGallery::create();
    scene->addChild(layer);
    return scene;
}

bool UIStoryGallery::init()
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

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create("gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png",
                                            CC_CALLBACK_1(UIStoryGallery::menuBackCallback, this));
    //backButton->setPosition(Vec2((4 * visibleSize.width / 42), (34 * visibleSize.height / 36)));
    backButton->setPosition(Vec2((4 * visibleSize.width / 34), (1.5 * visibleSize.height / 25)));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    backLabel->setColor(Color3B(207, 203, 208));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    arrowBack = MenuItemImage::create("gui/ArrowBack.png", "gui/ArrowBackPressed.png", CC_CALLBACK_1(UIStoryGallery::menuArrowBackCallback, this));
    arrowBack->setPosition(Vec2((2 * visibleSize.width / 42), visibleSize.height / 2));
    arrowBack->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowBack);
    
    arrowNext = MenuItemImage::create("gui/ArrowNext.png", "gui/ArrowNextPressed.png", CC_CALLBACK_1(UIStoryGallery::menuArrowNextCallback, this));
    arrowNext->setPosition(Vec2((40 * visibleSize.width / 42), visibleSize.height / 2));
    arrowNext->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowNext);
    
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 10);
    
    GameLevel::getInstance()->setCurrentAgentType(0);
    
    pages = PageView::create();
    pages->setTouchEnabled(true);
    pages->setContentSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    //pages->setContentSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));
    
    for (int i = 1; i < NUM_LEVELS + 1; i++)
    {
        auto layout = Layout::create();
        // unlocked
        if(GameData::getInstance()->getLevelsCompleted().at(i) != 0)
        {
            Sprite* image;
            if (i == 21)
            {
                image = Sprite::create("art/Escenari20_2.jpg");
            }
            else
            {
                image = Sprite::create("art/Escenari"+to_string(i)+".jpg");
            }
            image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            image->setScale(visibleSize.width / image->getContentSize().width);

            int diff = (visibleSize.height - image->getContentSize().height*visibleSize.width / image->getContentSize().width)/2;
            layout->addChild(image);
            
            auto storyLine = Label::createWithTTF(LocalizedString::create(("LEVEL_" + to_string(i) + "_STORY").c_str(), "text"), "fonts/arial.ttf", 55 * GameData::getInstance()->getRaConversion());
            storyLine->setColor(Color3B(216, 229, 235));
            storyLine->setMaxLineWidth(0.9f*visibleSize.width);
            storyLine->setAnchorPoint(Vec2(0.0, 0.0));
            storyLine->enableShadow();
            storyLine->setPosition(Vec2(0.05f*visibleSize.width, 1.05*diff));
            
            layout->addChild(storyLine);
        }
        else
        {
            Sprite* image;
            if (i == 21)
            {
                image = Sprite::create("art/locked/Escenari20_2.jpg");
            }
            else
            {
                image = Sprite::create("art/locked/Escenari"+to_string(i)+".jpg");
            }
            image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            image->setScale(visibleSize.width / image->getContentSize().width);
            layout->addChild(image);

            int numMission = i;
            if(i==21)
            {
                numMission = 20;
            }
            auto unlockLabel = Label::createWithTTF(string(LocalizedString::create("PASS_MISSION")+" "+to_string(numMission)+" "+LocalizedString::create("TO_UNLOCK_STORY")), "fonts/BebasNeue.otf", 75 * GameData::getInstance()->getRaConversion());
            unlockLabel->setTextColor(Color4B(50, 50, 100, 180));
            unlockLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            unlockLabel->setName("unlockLabel");
            layout->addChild(unlockLabel);
        }
        
        pages->addPage(layout);
    }

    this->addChild(pages);
    
    this->scheduleUpdate();
    
    return true;
}

void UIStoryGallery::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIStoryGallery::menuArrowBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurrentPageIndex() <= 0)
    {
        return;
    }

    pages->scrollToPage(pages->getCurrentPageIndex() - 1);
}

void UIStoryGallery::menuArrowNextCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    if (pages->getCurrentPageIndex() >= pages->getItems().size() - 1)
    {
        return;
    }
    
    pages->scrollToPage(pages->getCurrentPageIndex() + 1);
}

void UIStoryGallery::update(float delta)
{
    if (pages->getCurrentPageIndex() > 0 and pages->getCurrentPageIndex() < pages->getItems().size() - 1) {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    }
    else {
        if (pages->getCurrentPageIndex() == 0) {
            arrowBack->setVisible(false);
            arrowNext->setVisible(true);
        }
        else if (pages->getCurrentPageIndex() == pages->getItems().size() - 1) {
            arrowBack->setVisible(true);
            arrowNext->setVisible(false);
        }
    }
}

void UIStoryGallery::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        menuBackCallback(this);
    }
}
