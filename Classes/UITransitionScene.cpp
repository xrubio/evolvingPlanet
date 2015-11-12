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

    auto image = Sprite::create("art/locked/Escenari"+to_string(GameData::getInstance()->getFirstTimeLevelCompleted())+".png");
    image->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    image->setScale(visibleSize.width / 2500);
    this->addChild(image);
    
    auto unlockLabel = Label::createWithTTF(string(LocalizedString::create("TAP_TO_UNLOCK")), "fonts/BebasNeue.otf", 100);
    unlockLabel->setTextColor(Color4B(216, 229, 235, 60));
    unlockLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    unlockLabel->setName("unlockLabel");
    this->addChild(unlockLabel);

    auto imageUnlocked = Sprite::create("art/Escenari"+to_string(GameData::getInstance()->getFirstTimeLevelCompleted())+".jpg");
    imageUnlocked->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    imageUnlocked->setScale(visibleSize.width / 2500);
    imageUnlocked->setOpacity(0.0f);
    imageUnlocked->setName("imageUnlocked");
    this->addChild(imageUnlocked);
    
    auto contextDeployment = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameData::getInstance()->getFirstTimeLevelCompleted()) + "_DEPLOYMENT").c_str(), "text"), Size(visibleSize.width / (1.5 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "Arial Rounded MT Bold", 40);
    contextDeployment->setColorSpaceHolder(Color4B(216, 229, 235, 255));
    contextDeployment->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
    contextDeployment->setScaleX(GameData::getInstance()->getRaWConversion());
    contextDeployment->setScaleY(GameData::getInstance()->getRaHConversion());
    contextDeployment->setOpacity(0);
    contextDeployment->setName("text");
    this->addChild(contextDeployment);
    
    auto tapToContinue = Label::createWithTTF(string(LocalizedString::create("TAP_TO_CONTINUE")), "fonts/BebasNeue.otf", 50);
    tapToContinue->setColor(Color3B::WHITE);
    tapToContinue->setAnchorPoint(Vec2(0, 0));
    tapToContinue->setPosition(visibleSize.width - tapToContinue->getContentSize().width, tapToContinue->getContentSize().height);
    tapToContinue->setOpacity(0);
    tapToContinue->setName("tapToContinue");
    this->addChild(tapToContinue);
    
    //tapToContinue->runAction(RepeatForever::create(Blink::create(2, 1)));
    
    this->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UITransitionScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
            Director::getInstance()->replaceScene(transition);
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
