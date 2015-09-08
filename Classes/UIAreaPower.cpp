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
//  UIAreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIAreaPower.h"
#include "UIGameplayMap.h"

UIAreaPower::UIAreaPower(Power* p) : UIPower(p)
{
    icon = Sprite::create("PowerBackgroundButton.png");
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto button = Sprite::create(p->getName() + "Button" + ".png");
    button->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(button, 2, 0);
    auto actionTimer = ProgressTimer::create(Sprite::create(p->getName() + "ActionButton" + ".png"));
    actionTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    actionTimer->setType(ProgressTimer::Type::RADIAL);
    icon->addChild(actionTimer, 1, 1);
    auto cooldownTimer = ProgressTimer::create(Sprite::create("PowerCooldownButton.png"));
    cooldownTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    cooldownTimer->setType(ProgressTimer::Type::RADIAL);
    cooldownTimer->setVisible(false);
    icon->addChild(cooldownTimer, 3, 2);
    //cooldown = Label::createWithSystemFont(to_string(power->getCooldownLeft()), "Arial Rounded MT Bold", 60);
    //cooldown->setColor(Color3B::BLUE);
    //cooldown->setVisible(false);
    //cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    //icon->addChild(cooldown);
    area = Sprite::create("BoostResistanceArea.png");
    area->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    area->setOpacity(100);
    area->setVisible(false);
}

Sprite* UIAreaPower::getArea(void)
{
    return area;
}

void UIAreaPower::onTouchesBegan(Point touchLocation)
{
    if (power->getCooldownLeft() <= 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
        clicked = true;
        area->setPosition(area->getParent()->convertToNodeSpace(icon->getPosition()));
        area->setVisible(true);
        icon->setScale(1.25 * GameData::getInstance()->getRaWConversion(), 1.25 * GameData::getInstance()->getRaHConversion());
        icon->setColor(Color3B::GRAY);
        auto button = (Sprite*)icon->getChildByTag(0);
        button->setColor(Color3B::GRAY);
    }
}

void UIAreaPower::onTouchesMoved(Touch* touchLocation)
{
    if (clicked) {
        Point touchArea = area->getParent()->convertToNodeSpace(Director::getInstance()->convertToGL(touchLocation->getLocationInView()));
        Point touch = Director::getInstance()->convertToGL(touchLocation->getLocationInView());
        if (touchLocation) {
            area->setPosition(touchArea);
        }
    }
}

void UIAreaPower::onTouchesEnded(Point touchLocation)
{
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    if (clicked) {
        icon->setColor(Color3B::WHITE);
        auto button = (Sprite*)icon->getChildByTag(0);
        button->setColor(Color3B::WHITE);
        if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) == false) {
            power->setDurationLeft(power->getDuration());
            //cooldown->setVisible(true);
        }
    }
    clicked = false;
}

void UIAreaPower::update(float delta)
{
    ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
    ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);

    actionTimer->setPercentage((power->getDurationLeft() / power->getDuration()) * 100.0);

    if (clicked == false and power->getDurationLeft() <= 0) {
        area->setVisible(false);
    }
    if (power->getCooldownLeft() > 0) {
        //cooldown->setVisible(true);
        //cooldown->setString(to_string(power->getCooldownLeft()));
        cooldownTimer->setVisible(true);
        cooldownTimer->setPercentage(float(power->getCooldownLeft()) / float(power->getCooldown()) * 100);
    }
    else {
        //cooldown->setVisible(false);
        cooldownTimer->setVisible(false);
        actionTime = 0.0;
        actionTimer->setPercentage(100.0);
    }
}
