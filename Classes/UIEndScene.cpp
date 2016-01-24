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
//  UIEndScene.cpp
//  simulplay
//
//  Created by Guillem Laborda on 21/10/15.
//
//

#include "UIEndScene.h"
#include "GameData.h"
#include "LocalizedString.h"
#include "UIMainMenu.h"
#include "../libs/pugixml/pugixml.hpp"

Scene* UIEndScene::createScene()
{
    auto scene = Scene::create();
    auto layer = UIEndScene::create();
    scene->addChild(layer);
    return scene;
}

void UIEndScene::addJob(const std::string & jobName, float offset)
{
    auto label = Label::createWithTTF(LocalizedString::create(jobName.c_str()), "fonts/BebasNeue.otf", 65 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(255, 255, 255));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    label->setPosition(Vec2(visibleSize.width/2, offset));
    label->setAlignment(TextHAlignment::CENTER);
    this->addChild(label);
}

void UIEndScene::addName(const std::string & name, float offset)
{
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 45 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(255, 255, 255));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    label->setPosition(Vec2(visibleSize.width/2, offset));
    label->setMaxLineWidth(0.8f*visibleSize.width);
    label->setAlignment(TextHAlignment::CENTER);
    this->addChild(label);
}

void UIEndScene::addAcknowledgement(const std::string & name, float offset)
{
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 35 * GameData::getInstance()->getRaConversion());
    label->setColor(Color3B(255, 255, 255));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    label->setPosition(Vec2(visibleSize.width/2, offset));
    label->setAlignment(TextHAlignment::CENTER);
    this->addChild(label);
}


void UIEndScene::loadAcknowledgements() 
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

bool UIEndScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    
    auto tapToContinue = Label::createWithTTF(string(LocalizedString::create("TAP_TO_CONTINUE")), "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
    tapToContinue->setAnchorPoint(Vec2(0, 0));
    tapToContinue->setColor(Color3B::WHITE);
    tapToContinue->setPosition(visibleSize.width - tapToContinue->getContentSize().width, tapToContinue->getContentSize().height);
    tapToContinue->setOpacity(0);
    tapToContinue->setName("tap");
    tapToContinue->runAction(Sequence::create(DelayTime::create(63.0f), FadeIn::create(2.0f), nullptr));
    this->addChild(tapToContinue);
    
        
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(UIEndScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
   
    auto theEndLabel = Label::createWithTTF(string(LocalizedString::create("THE_END")), "fonts/BebasNeue.otf", 150 * GameData::getInstance()->getRaConversion());
    theEndLabel->setTextColor(Color4B(255, 255, 255, 230));
    theEndLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    theEndLabel->setName("theEndLabel");
    theEndLabel->setOpacity(0);
    theEndLabel->runAction(FadeIn::create(3.0f));
    this->addChild(theEndLabel);

    float offsetJob = 100.0f*GameData::getInstance()->getRaConversion();
    float offsetName = 50.0f*GameData::getInstance()->getRaConversion();
    float offsetAck = 40.0f*GameData::getInstance()->getRaConversion();

    float initPos = -offsetJob;
    // team
    addJob("DIRECTOR", initPos);
    initPos -= offsetName;
    addName("Xavier Rubio-Campillo", initPos);
    initPos -= offsetJob;

    addJob("LEAD_PROGRAMMER", initPos);
    initPos -= offsetName;
    addName("Guillem Laborda", initPos);
    initPos -= offsetJob;

    addJob("ADDITIONAL_PROGRAMMING", initPos);
    initPos -= offsetName;
    addName("Xavier Rubio-Campillo", initPos);
    initPos -= offsetJob;

    addJob("ART", initPos);
    initPos -= offsetName;
    addName("Guillem H. Pongiluppi", initPos);
    initPos -= offsetName;
    addName("David Ramos", initPos);
    initPos -= offsetJob;

    addJob("UI", initPos);
    initPos -= offsetName;
    addName("David Ramos", initPos);
    initPos -= offsetJob;
   
    addJob("STORY_CONTENT", initPos);
    initPos -= offsetName;
    addName("Xavier Rubio-Campillo", initPos);
    initPos -= offsetName;
    addName("Jorge Caro", initPos);
    initPos -= offsetName;
    addName("Guillem H. Pongiluppi", initPos);
    initPos -= offsetJob;
  
    addJob("MUSIC_CREDITS", initPos);
    initPos -= offsetName;
    addName("Guillem Laborda", initPos);
    initPos -= offsetJob;

    // other
    loadAcknowledgements();
    addJob("SPECIAL_THANKS", initPos);
    initPos -= offsetName;

    for(std::list<std::string>::const_iterator it=_names.begin(); it!=_names.end(); it++)
    {
        addAcknowledgement(*it, initPos);
        initPos -= offsetAck;
    }
    initPos -= offsetJob;

    addName("Programa RecerCaixa", initPos);
    initPos -= offsetName;
    addName("Obra Social La Caixa", initPos);
    initPos -= offsetName;
    addName("the SimulPast team", initPos);
    initPos -= offsetName;
    addName("Agència Catalana d'Universitats Públiques", initPos);
    initPos -= offsetName;
    addName("Barcelona Supercomputing Centre", initPos);
    initPos -= offsetName;
    addName("Ministerio de Ciencia e Innovación", initPos);
    initPos -= offsetName;
    addName("Cocos2d-x Development Team", initPos);
    initPos -= offsetName;
    addName("Wikimedia Commons", initPos);
    initPos -= offsetName;
    addName("Trajecte Final members", initPos);
    initPos -= offsetName;
    addName(LocalizedString::create("THANKS_PEOPLE"), initPos);    
    initPos -= offsetJob;

    initPos -= visibleSize.height/3;
    
    auto murphy = Label::createWithTTF(string(LocalizedString::create("MURPHY")), "fonts/BebasNeue.otf", 150 * GameData::getInstance()->getRaConversion());
    murphy->setTextColor(Color4B(255, 255, 255, 230));
    murphy->setPosition(Vec2(visibleSize.width/2, initPos));
    murphy->setAlignment(TextHAlignment::CENTER);
    this->addChild(murphy);

    initPos -= visibleSize.height/3;

    auto thanks = Label::createWithTTF(string(LocalizedString::create("THANKS_PLAYER")), "fonts/arial.ttf", 40 * GameData::getInstance()->getRaConversion());
    thanks->setTextColor(Color4B(255, 255, 255, 230));
    thanks->setPosition(Vec2(visibleSize.width/2, initPos));
    thanks->setMaxLineWidth(0.7f*visibleSize.width);
    thanks->setAlignment(TextHAlignment::CENTER);
    this->addChild(thanks);

    initPos -= visibleSize.height/3;

    for(auto child : getChildren())
    {
        // tap to continue is always fixed
        if(child->getName()=="tap")
        {
            continue;
        }
        child->runAction(Sequence::create(DelayTime::create(5.0f), MoveBy::create(60.0f, Vec2(0, -initPos)), nullptr));
    }

    return true;
}

void UIEndScene::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    auto node = getChildByName("tap");
    if(node->getOpacity()==0)
    {
        return;
    }
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(3.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

