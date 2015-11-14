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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create("gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png",
                                            CC_CALLBACK_1(UIStoryGallery::menuBackCallback, this));
    backButton->setPosition(Vec2((4 * visibleSize.width / 42),
                                 (34 * visibleSize.height / 36)));
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
    pages->setCustomScrollThreshold(visibleSize.width / 6);
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    //pages->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));
    
    // TODO make sure that number of levels is extracted from GameData
    int numLevels = 10;
    for (int i=1; i<=numLevels; i++)
    {
        auto layout = Layout::create();

        // unlocked
        // TODO, it should be: if(GameData::getInstance()->getLevelsCompleted().at(i)!=0)
        if(i<GameData::getInstance()->getLevelsCompleted().size() && GameData::getInstance()->getLevelsCompleted().at(i)!=0)
        {
            auto image = Sprite::create("art/Escenari"+to_string(i)+".jpg");
            image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            image->setScale(visibleSize.width / image->getContentSize().width);
            layout->addChild(image);
        }
        else
        {
            auto image = Sprite::create("art/locked/Escenari"+to_string(i)+".png");
            image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            image->setScale(visibleSize.width / image->getContentSize().width);
            layout->addChild(image);
        }

        auto contextDeployment = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameData::getInstance()->getFirstTimeLevelCompleted()) + "_DEPLOYMENT").c_str(), "text"), Size(visibleSize.width / (1.5 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "Arial Rounded MT Bold", 50 * GameData::getInstance()->getRaConversion());
        contextDeployment->setColorSpaceHolder(Color4B(216, 229, 235, 255));
        contextDeployment->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
        contextDeployment->setScaleX(GameData::getInstance()->getRaWConversion());
        contextDeployment->setScaleY(GameData::getInstance()->getRaHConversion());
        contextDeployment->setName("text");
        layout->addChild(contextDeployment);
        
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
    if (pages->getCurPageIndex() <= 0)
    {
        return;
    }

    pages->scrollToPage(pages->getCurPageIndex() - 1);
}

void UIStoryGallery::menuArrowNextCallback(Ref* pSender)
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

void UIStoryGallery::update(float delta)
{
    if (pages->getCurPageIndex() > 0 and pages->getCurPageIndex() < pages->getPages().size() - 1) {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    }
    else {
        if (pages->getCurPageIndex() == 0) {
            arrowBack->setVisible(false);
            arrowNext->setVisible(true);
        }
        else if (pages->getCurPageIndex() == pages->getPages().size() - 1) {
            arrowBack->setVisible(true);
            arrowNext->setVisible(false);
        }
    }
}
