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
//  UIIntro.cpp
//  simulplay
//
//  Created by Guillem Laborda on 3/12/15.
//
//

#include "UIIntro.h"
#include "UIMainMenu.h"
#include "GameData.h"

#include <ui/UIVideoPlayer.h>

#include <audio/include/SimpleAudioEngine.h>

USING_NS_CC;

Scene* UIIntro::createScene()
{
    auto scene = Scene::create();
    auto layer = UIIntro::create();
    scene->addChild(layer);
    return scene;
}

bool UIIntro::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    /*auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    background->setOpacity(0);
    this->addChild(background, 2);*/
    
    this->setColor(Color3B(211, 210, 210));
  
    /*if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/main.mp3", true);
    }*/
    
    cocos2d::experimental::ui::VideoPlayer *p = cocos2d::experimental::ui::VideoPlayer::create();
    p->setFileName("audio/logo_02.mov");
    p->setName("video");
    p->setContentSize(Size(visibleSize.width, visibleSize.height + (3 * visibleSize.height / 10)));
    p->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //p->setKeepAspectRatioEnabled(true);
    p->play();
    this->addChild(p, 1);
    
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(UIIntro::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);

    this->scheduleUpdate();
    
    return true;
}

bool UIIntro::onTouchesBegan(Touch* touch, Event* event)
{
    if (((cocos2d::experimental::ui::VideoPlayer*)this->getChildByName("video")) != nullptr /*and ((cocos2d::experimental::ui::VideoPlayer*)this->getChildByName("video"))->isPlaying()*/)
    {
        CCLOG("TAP");
        return true;
    }
    return true;
}

void UIIntro::update(float delta)
{
    if (((cocos2d::experimental::ui::VideoPlayer*)this->getChildByName("video")) != nullptr and((cocos2d::experimental::ui::VideoPlayer*)this->getChildByName("video"))->isPlaying() == false)
    {
        //this->removeChild(((cocos2d::experimental::ui::VideoPlayer*)this->getChildByName("video")));
        _eventDispatcher->removeEventListener(_listener);
        auto scene = UIMainMenu::createScene();
        Director::getInstance()->replaceScene(scene);
        
    }
}

