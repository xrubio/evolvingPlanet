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
//  UIGlobalPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIGlobalPower.h"
#include "UIGameplayMap.h"

UIGlobalPower::UIGlobalPower(Power* p) : UIPower(p)
{
    icon = Sprite::create("gui/PowerBackgroundButton.png");
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto button = Sprite::create("gui/" + p->getName() + "Button" + ".png");
    button->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(button, 2, 0);
    auto actionTimer = ProgressTimer::create(Sprite::create("gui/" + p->getName() + "ActionButton" + ".png"));
    actionTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    actionTimer->setType(ProgressTimer::Type::RADIAL);
    actionTimer->setPercentage(100.0);
    icon->addChild(actionTimer, 1, 1);
    auto cooldownTimer = ProgressTimer::create(Sprite::create("gui/PowerCooldownButton.png"));
    cooldownTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    cooldownTimer->setType(ProgressTimer::Type::BAR);
    cooldownTimer->setBarChangeRate(Vec2(0, 1));
    cooldownTimer->setMidpoint(Vec2(0, 1));
    cooldownTimer->setVisible(false);
    icon->addChild(cooldownTimer, 3, 2);
    //cooldown = Label::createWithSystemFont(to_string(power->getCooldownLeft()), "Arial Rounded MT Bold", 60);
    //cooldown->setColor(Color3B::MAGENTA);
    //cooldown->setVisible(false);
    //cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    //icon->addChild(cooldown, 3);
}

void UIGlobalPower::onTouchesBegan(Point touchLocation)
{
    if (((ProgressTimer*)icon->getChildByTag(2))->getPercentage() == 100.0 or disabled == true)
    {
        //DO NOTHING
    }
    else if (power->getCooldownLeft() <= 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
        clicked = true;
        icon->setScale(0.8 * GameData::getInstance()->getRaWConversion(), 0.8 * GameData::getInstance()->getRaHConversion());
        //auto button = (Sprite*)icon->getChildByTag(0);
        //button->setColor(Color3B::GRAY);
    }
}

void UIGlobalPower::onTouchesMoved(Touch* touchLocation)
{
}

void UIGlobalPower::onTouchesEnded(Point touchLocation)
{
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) and clicked) {
        //Activar boost reproduction un cop s'ha tocat i soltat a sobre la imatge que toca
        //icon->setColor(Color3B::GRAY);
        power->setDurationLeft(power->getDuration());
        GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() - power->getCost());
        ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);
        //cooldownTimer->setVisible(true);
        ((Sprite *)icon->getChildByTag(0))->setColor(Color3B::GREEN);
        cooldownTimer->setPercentage(100.0);
        //cooldown->setVisible(true);
    }
    actionTime = 0.0;
}

void UIGlobalPower::update(float delta)
{
    ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
    ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);

    /*if (clicked == false and power->getDurationLeft() > 0) {
        actionTime += delta;
        if (actionTime > power->getDuration()) {
            actionTime = 0.0;
        }
    }*/
    
    actionTimer->setPercentage((power->getDurationLeft() / power->getDuration()) * 100.0);

    //actionTimer->setPercentage((float(power->getDurationLeft())) / float(power->getDuration()) * 100.0);

    if (power->getCooldownLeft() > 0) {
        if (actionTimer->getPercentage() == 0) {
            //cooldown->setVisible(true);
            //cooldown->setString(to_string(power->getCooldownLeft()));
            ((Sprite *)icon->getChildByTag(0))->setColor(Color3B::WHITE);
            cooldownTimer->setVisible(true);
            cooldownTimer->setPercentage(float(power->getCooldownLeft()) / float(power->getCooldown()) * 100);
            clicked = false;
        }
    }
    else {
        actionTimer->setPercentage(100.0);
        actionTime = 0.0;
        //cooldown->setVisible(false);
        if (GameLevel::getInstance()->getTimeSpeed() > 0){
            cooldownTimer->setVisible(false);
        }
        if (((Sprite *)icon->getChildByTag(0))->getColor() != Color3B::GREEN) {
            if (GameLevel::getInstance()->getEvolutionPoints() >= power->getCost())
            {
                disabled = false;
                /*actionTimer->setColor(Color3B::WHITE);
                ((Sprite *)icon->getChildByTag(0))->setColor(Color3B::WHITE);*/
                cooldownTimer->setVisible(false);
                cooldownTimer->setPercentage(0);
            }
            else {
                disabled = true;
                /*actionTimer->setColor(Color3B::GRAY);
                ((Sprite *)icon->getChildByTag(0))->setColor(Color3B::GRAY);*/
                cooldownTimer->setVisible(true);
                cooldownTimer->setPercentage(100);
            }
        }
    }
}
