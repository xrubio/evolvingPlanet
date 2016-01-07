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
//  UITransitionScene.cpp
//  simulplay
//
//  Created by Guillem Laborda on 21/10/15.
//
//

#include "UITransitionScene.h"
#include "GameData.h"
#include "LocalizedString.h"
#include "UIProgressMap.h"
#include "UIStoryGallery.h"

Scene* UITransitionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = UITransitionScene::create();
    scene->addChild(layer);
    return scene;
}

bool UITransitionScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (GameData::getInstance()->getFirstTimeLevelCompleted() <= 0)
    {
        updateTimeToLoadScene = clock();
        this->scheduleUpdate();
    }
    else
    {
    
        Sprite * image;
        if (GameData::getInstance()->getFirstTimeLevelCompleted() == 21)
        {
            image = Sprite::create("art/locked/Escenari20_2.jpg");
        }
        
    image = Sprite::create("art/locked/Escenari"+to_string(GameData::getInstance()->getFirstTimeLevelCompleted())+".jpg");
    image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    image->setScale(visibleSize.width / image->getContentSize().width);
    this->addChild(image);
    
    auto unlockLabel = Label::createWithTTF(string(LocalizedString::create("TAP_TO_UNLOCK")), "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    unlockLabel->setTextColor(Color4B(50, 50, 100, 180));
    unlockLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    unlockLabel->setName("unlockLabel");
    this->addChild(unlockLabel);

    auto imageUnlocked = Sprite::create("art/Escenari"+to_string(GameData::getInstance()->getFirstTimeLevelCompleted())+".jpg");
    imageUnlocked->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    imageUnlocked->setScale(visibleSize.width / imageUnlocked->getContentSize().width);
    imageUnlocked->setOpacity(0.0f);
    imageUnlocked->setName("imageUnlocked");
    int diff = (visibleSize.height - imageUnlocked->getContentSize().height)/2;
    this->addChild(imageUnlocked);

    auto storyLine = Label::createWithTTF(LocalizedString::create(("LEVEL_" + to_string(GameData::getInstance()->getFirstTimeLevelCompleted()) + "_STORY").c_str(), "text"), "fonts/arial.ttf", 40 * GameData::getInstance()->getRaConversion());
    storyLine->setColor(Color3B(216, 229, 235));
    storyLine->setMaxLineWidth(0.6f*visibleSize.width);
    storyLine->setAnchorPoint(Vec2(0.0, 0.0));
    storyLine->enableShadow();
    storyLine->setPosition(Vec2(0.05f*visibleSize.width, 2*storyLine->getContentSize().height/3+diff));
    storyLine->setOpacity(0);
    storyLine->setName("text");
 
    this->addChild(storyLine);
    
    auto tapToContinue = Label::createWithTTF(string(LocalizedString::create("TAP_TO_CONTINUE")), "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    tapToContinue->setAnchorPoint(Vec2(0, 0));
    tapToContinue->setColor(Color3B::WHITE);
    tapToContinue->setPosition(visibleSize.width - tapToContinue->getContentSize().width, tapToContinue->getContentSize().height);
    tapToContinue->setOpacity(0);
    tapToContinue->setName("tapToContinue");
    this->addChild(tapToContinue);
    
    //tapToContinue->runAction(RepeatForever::create(Blink::create(2, 1)));
        
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UITransitionScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    return true;
}

void UITransitionScene::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (unlockedImage == false)
    {
        this->getChildByName("imageUnlocked")->runAction(FadeIn::create(3.0f));
        this->getChildByName("text")->runAction(FadeIn::create(2));
        this->getChildByName("unlockLabel")->runAction(FadeOut::create(0.5f));
        this->getChildByName("tapToContinue")->runAction(Sequence::create(DelayTime::create(2.9f), FadeIn::create(0.1), RepeatForever::create(Blink::create(2, 1)), nullptr));

        unlockedImage = true;
    }
    else
    {
        if (stoppedAnimation or allActionsFinished()) {
            auto scene = UIProgressMap::createScene();
            auto transition = TransitionFade::create(1.0f, scene);
            Director::getInstance()->popScene();
        }
        else {
            endActions();
        }
    }
}

void UITransitionScene::endActions(void)
{
    stoppedAnimation = true;
    
    this->getChildByName("imageUnlocked")->stopAllActions();
    this->getChildByName("imageUnlocked")->setOpacity(255);
    this->getChildByName("text")->stopAllActions();
    this->getChildByName("text")->setOpacity(255);
    this->getChildByName("tapToContinue")->setOpacity(255);
    this->getChildByName("tapToContinue")->runAction(RepeatForever::create(Blink::create(2, 1)));
}

bool UITransitionScene::allActionsFinished(void)
{
    if (this->getChildByName("imageUnlocked")->getNumberOfRunningActions() == 0 and this->getChildByName("text")->getNumberOfRunningActions() == 0) {
        return true;
    }
    return false;
}

void UITransitionScene::setLoadingAnimation(bool b)
{
    if (this->getChildByName("loading") == nullptr)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto loading = Sprite::create("gui/Loading.png");
        loading->setPosition(7 * visibleSize.width / 8, visibleSize.height / 8);
        loading->setScale(GameData::getInstance()->getRaHConversion());
        loading->setName("loading");
        loading->setVisible(true);
        this->addChild(loading, 500);
        loading->runAction(RepeatForever::create(RotateBy::create(1, 180)));
    }
}

void UITransitionScene::update(float delta)
{

    if (GameData::getInstance()->getNextScene() < 0 or ((clock() - float(updateTimeToLoadScene)) / CLOCKS_PER_SEC) <= 0.2)
    {
        return;
    }

    Scene *scene;
    TransitionFade *transition;
    switch (GameData::getInstance()->getNextScene())
    {
        case Story:
        {
            scene = UIStoryGallery::createScene();
            transition = TransitionFade::create(0.5f, scene);
            break;
        }
            
        default:
        {
            scene = UIMainMenu::createScene();
            transition = TransitionFade::create(0.5f, scene);
            break;
        }
    }
    GameData::getInstance()->setNextScene(-1);
    Director::getInstance()->replaceScene(transition);
}

