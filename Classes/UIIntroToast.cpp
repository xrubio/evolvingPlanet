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
//  UIIntroToast.cpp
//  simulplay
//
//  Created by Guillem Laborda on 3/12/15.
//
//

#include "UIIntroToast.h"
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

Scene* UIIntroToast::createScene()
{
    auto scene = Scene::create();
    auto layer = UIIntroToast::create();
    scene->addChild(layer);
    return scene;
}

bool UIIntroToast::init()
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/toast.mp3");

    VideoPlayer *p = VideoPlayer::create();
    p->setFileName("misc/logo.mp4");
    p->setName("video");
    p->setContentSize(Size(visibleSize.width, visibleSize.height + (3 * visibleSize.height / 10)));
    p->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //p->setKeepAspectRatioEnabled(true);
    this->addChild(p, 1);
    p->play();

// at least show the logo    
#else
    auto logo = Sprite::create("misc/murphy.png");
    logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));   
    logo->setName("logo");
    logo->setOpacity(0);
    logo->setScale(GameData::getInstance()->getRaWConversion());
    logo->runAction(Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7), FadeOut::create(0.3f), nullptr));
    this->addChild(logo);
#endif
    
    //load gallery images to cache
    int i;
    for (i = 1; i < GameData::getInstance()->getCurrentLevel(); i++)
    {
        Director::getInstance()->getTextureCache()->addImageAsync("art/Escenari"+to_string(i)+".jpg", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    }
    for (; i < 21; i++)
    {
        Director::getInstance()->getTextureCache()->addImageAsync("art/locked/Escenari"+to_string(i)+".jpg", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    }
    
    Director::getInstance()->getTextureCache()->addImageAsync("gui/Clouds3.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/Clouds2.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/Clouds1.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/StarFullMini.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/StarEmptyMini.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ZoneAreaLevel.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ProgressMapDarkBackground.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ProgressMapPopupBackground.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/StarFull.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/StarEmpty.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/LevelPointerButton.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/LevelPointerButtonShadow.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ZoneAreaLevel.png", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ProgressMap1Background.jpg", CC_CALLBACK_1(UIIntroToast::doNothing, this));
    Director::getInstance()->getTextureCache()->addImageAsync("gui/ProgressMap2Background.jpg", CC_CALLBACK_1(UIIntroToast::doNothing, this));

    this->runAction(Sequence::create(DelayTime::create(5.8), CallFunc::create(this, callfunc_selector(UIIntroToast::toMainMenu)), NULL));
    return true;
}

void UIIntroToast::doNothing(bool b)
{
    
}

void UIIntroToast::toMainMenu(void)
{
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto * video = (VideoPlayer*)(this->getChildByName("video"));
    if(video and !video->isPlaying())
    {
        this->removeChildByName("video");
        _eventDispatcher->removeEventListener(_listener);
        auto scene = UIMainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        return;
    }
# else
    Node * logo = this->getChildByName("logo");
    if(logo and logo->getNumberOfRunningActions()==0)
    {
        this->removeChild(logo);
        _eventDispatcher->removeEventListener(_listener);
        auto scene = UIMainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.1, scene));
        return;
    }
#endif*/
    if(this->getChildByName("video") != nullptr)
    {
        this->removeChildByName("video");
    }
    _eventDispatcher->removeEventListener(_listener);
    auto scene = UIMainMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
    return;
}

