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
        "gui/BackButton.png", "gui/BackButtonPressed.png", CC_CALLBACK_1(UICredits::menuBackCallback, this));

    auto menu = cocos2d::Menu::createWithItem(backButton);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
        origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);
    
    return true;
}

void UICredits::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}
