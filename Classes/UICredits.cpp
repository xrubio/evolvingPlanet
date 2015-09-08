/*
 * Copyright (c) 2015
 * MURPHY'S TOAST STUDIOS
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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithSystemFont(LocalizedString::create("CREDITS"), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UICredits::menuBackCallback, this));

    auto menu = cocos2d::Menu::createWithItem(backButton);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
        origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);
    
    waveNode = new WaveNode();
    waveNode->init();
    
    float waveVertCount = 50;
    // Space the verticies out evenly across the screen for the wave.
    float vertexHorizontalSpacing = visibleSize.width / (float)waveVertCount;
    
    // Used to increment to the next vertexX position.
    float currentWaveVertX = 0;
    
    for (int i=0; i< waveVertCount; i++) {
        
        float time = (float)i;
        
        float waveX, waveY;
        
        waveX = currentWaveVertX;
        
        // Create the default sinusoid wave that will be displayed if the wave is not animated.
        waveY = visibleSize.height / 3;
        
        //NSLog(@"time:%.2f waveAmplitude:%.2f Wave:%.2f,%2.f wavefrequency:%.2f",time, waveAmplitude, waveX, waveY, wavefrequency);
        //[glWaveNode addToDynamicVerts2D:ccp(waveX,waveY) withColor:waveColor];
        waveNode->addToDynamicVerts2D(Point(waveX, waveY), Color4B(255, 255, 255, 255));
        
        currentWaveVertX += vertexHorizontalSpacing;
    }
    
    
    // Try experimenting with different draw modes to see the effect.
    //    glWaveNode.glDrawMode = kDrawPoints;
    waveNode->glDrawMode = kDrawLines;
    
    waveNode->setReadyToDrawDynamicVerts(true);
    this->addChild(waveNode, 1);
    
    waveNode->draw(Director::getInstance()->getRenderer(), Mat4::IDENTITY, true);

    this->scheduleUpdate();

    return true;
}

void UICredits::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UICredits::update(float delta)
{
    waveNode->draw(Director::getInstance()->getRenderer(), Mat4::IDENTITY, true);
}