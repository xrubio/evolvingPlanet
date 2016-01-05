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
#include <extensions/cocos-ext.h>
#include <ui/CocosGUI.h>
#include <audio/include/SimpleAudioEngine.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include <ui/UIVideoPlayer.h>
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace cocos2d::experimental::ui;

#endif

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

// video player only for ios/android    
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    VideoPlayer *p = VideoPlayer::create();
    p->setFileName("audio/logo_02.mp4");
    p->setName("video");
    p->setContentSize(Size(visibleSize.width, visibleSize.height + (3 * visibleSize.height / 10)));
    p->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //p->setKeepAspectRatioEnabled(true);
    p->play();
    this->addChild(p, 1);

// at least show the logo    
#else*/
    auto logo = Sprite::create("misc/murphy.png");
    logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));   
    logo->setName("logo");
    logo->setOpacity(0);
    logo->runAction(Sequence::create(FadeIn::create(0.3f), DelayTime::create(0.7), FadeOut::create(0.5f), nullptr));
    this->addChild(logo);
//#endif
    
    //load gallery images to cache
    for (int i = 1; i < 21; i++)
    {
        Director::getInstance()->getTextureCache()->addImageAsync("art/Escenari"+to_string(i)+".jpg", CC_CALLBACK_1(UIIntro::doNothing, this));
    }

    
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(UIIntro::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);

    this->schedule(SEL_SCHEDULE(&UIIntro::update), 1.3);
    
    return true;
}

bool UIIntro::onTouchesBegan(Touch* touch, Event* event)
{
    
// video player only for ios/android    
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    if (((VideoPlayer*)this->getChildByName("video")) != nullptr)
    {
        return true;
    }
# endif*/
    
    //SKIP ANIMATION
    if (this->getChildByName("camaraFront") != nullptr)
    {
        this->getChildByName("camaraFront")->stopAllActions();
        this->getChildByName("camaraDoor")->stopAllActions();
        this->getChildByName("camaraBack")->stopAllActions();
        this->getChildByName("camaraHexagon")->stopAllActions();
        
        this->getChildByName("camaraFront")->runAction(FadeOut::create(0.5));
        this->getChildByName("camaraDoor")->runAction(FadeOut::create(0.5));
        this->getChildByName("camaraBack")->runAction(FadeOut::create(0.5));
        this->getChildByName("camaraHexagon")->runAction(FadeOut::create(0.5));

        _eventDispatcher->removeEventListener(_listener);
        auto scene = UIMainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
    }

    return true;
}

void UIIntro::runIntro()
{
    auto camaraBack = Sprite::create("misc/CamaraBack.jpg");
    camaraBack->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    camaraBack->setScale(GameData::getInstance()->getRaHConversion());
    camaraBack->setName("camaraBack");
    this->addChild(camaraBack, 0);
    
    auto camaraHexagon = Sprite::create("misc/CamaraHexagon.png");
    camaraHexagon->setPosition(Vec2(9 * Director::getInstance()->getVisibleSize().width / 19,  6 * Director::getInstance()->getVisibleSize().height / 11));
    camaraHexagon->setName("camaraHexagon");
    camaraHexagon->setScale(GameData::getInstance()->getRaHConversion());
    this->addChild(camaraHexagon, 1);
    camaraHexagon->runAction(Sequence::create(Spawn::create(Repeat::create(RotateBy::create(0.3, -270), 3), ScaleTo::create(0.9, 1.5), NULL),Spawn::create(Repeat::create(RotateBy::create(0.25, 360), 4), ScaleTo::create(1.0, 0.5), NULL), FadeOut::create(0.5), NULL));
    
    auto camaraFront = ProgressTimer::create(Sprite::create("misc/CamaraFront.png"));
    camaraFront->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    camaraFront->setPercentage(0.0);
    camaraFront->setName("camaraFront");
    camaraFront->setScale(GameData::getInstance()->getRaHConversion());
    this->addChild(camaraFront, 1);
    
    auto radialTimer = ProgressFromTo::create(3.0, 0, 66);
    auto radialTimer2 = ProgressFromTo::create(0.2, 66, 90);
    camaraFront->setMidpoint(Vec2(0, 0));
    camaraFront->setBarChangeRate(Vec2(0, 1));
    camaraFront->setType(ProgressTimer::Type::BAR);
    camaraFront->runAction(Sequence::create(DelayTime::create(1.3), radialTimer, radialTimer2, NULL));

    auto particleSmoke = ParticleSystemQuad::create("audio/CamaraParticle.plist");
    particleSmoke->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 0.0));
    auto moveParticle = MoveTo::create(5.0, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 4));
    this->addChild(particleSmoke, 5);
    particleSmoke->runAction(moveParticle);
    
    auto camaraDoor = Sprite::create("misc/CamaraDoor.png");
    camaraDoor->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));//5*Director::getInstance()->getVisibleSize().height/24));
    camaraDoor->setScale(GameData::getInstance()->getRaHConversion());
    camaraDoor->setOpacity(0);
    camaraDoor->setAnchorPoint(Vec2(0.5, 0.66));
    camaraDoor->setName("camaraDoor");
    this->addChild(camaraDoor, 3);
    auto moveDoor = Spawn::create(FadeIn::create(1.5), MoveTo::create(3.0, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 0.66)), NULL);
    camaraDoor->runAction(Sequence::create(DelayTime::create(1.3), moveDoor, NULL));
}

void UIIntro::update(float delta)
{
    
// video player only for ios/android    
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto * video = (VideoPlayer*)(this->getChildByName("video"));
    if(video and !video->isPlaying())
    {
        this->removeChildByName("video");
        runIntro();
        return;
    }
# else*/
    Node * logo = this->getChildByName("logo");
    if(logo and logo->getNumberOfRunningActions()==0)
    {
        this->removeChild(logo);
        runIntro();
        return;
    }
//#endif

    auto * camaraFront = this->getChildByName("camaraFront");
    auto * camaraDoor = this->getChildByName("camaraDoor");
    if(camaraFront and camaraFront->getNumberOfRunningActions() == 0 and camaraDoor and camaraDoor->getNumberOfRunningActions()==0)
    {
        _eventDispatcher->removeEventListener(_listener);
        auto scene = UIMainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
    }
}

void UIIntro::doNothing(bool b)
{
    
}

