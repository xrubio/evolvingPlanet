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
//  UIAreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIAreaPower.h"
#include "UIGameplayMap.h"
#include "AreaPower.h"

#include <editor-support/cocostudio/SimpleAudioEngine.h>

UIAreaPower::UIAreaPower(Power* p) : UIPower(p)
{
    icon = Sprite::create("gui/PowerBackgroundButton.png");
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto button = Sprite::create("gui/boosts/" + p->getName() + "Button" + ".png");
    button->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(button, 2, 0);
    auto actionTimer = ProgressTimer::create(Sprite::create("gui/boosts/" + p->getName() + "ActionButton" + ".png"));
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
    active = Sprite::create("gui/PowerActive.png");
    active->setPosition(Vec2(icon->getContentSize().width / 2, icon->getContentSize().height / 2));
    active->setVisible(false);
    active->setOpacity(50);
    icon->addChild(active, 100);
    area = Sprite::create("gui/BoostResistanceArea.png");
    area->setScale(GameData::getInstance()->getRaWConversion());
    area->setOpacity(100);
    area->setVisible(false);
}

Sprite* UIAreaPower::getArea(void)
{
    return area;
}

void UIAreaPower::onTouchesBegan(const Point & touchLocation)
{
    // do nothing if it's working or there are not enough EPs
    if(power->isActivated() or disabled == true)
    {
       return;
    }
    if(GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation))
    {
        if (GameData::getInstance()->getSFX() == true)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
        }
        
        clicked = true;
        area->setPosition(area->getParent()->convertToNodeSpace(icon->getPosition()));
        area->setVisible(true);
        icon->setScale(1.25 * GameData::getInstance()->getRaWConversion(), 1.25 * GameData::getInstance()->getRaHConversion());
        icon->setColor(Color3B::GRAY);
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

bool UIAreaPower::onTouchesEnded(const Point & touchLocation)
{
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    if (clicked) {
        icon->setColor(Color3B::WHITE);
        auto button = (Sprite*)icon->getChildByTag(0);
        button->setColor(Color3B::WHITE);
        if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) == false) {

            power->activate();
            ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);
            GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() - power->getCost());
            active->setVisible(true);
            cooldownTimer->setPercentage(100.0);
            clicked = false;
            auto * p = (AreaPower*)power;
            p->setArea(area->getPosition(), area->getBoundingBox().getMinY());
            return true;
        }
        else
        {
            area->setVisible(false);
        }
    }
    clicked = false;
    return false;
}

void UIAreaPower::update(float delta)
{
    // area is not visible if not clicked and not activated
    if (clicked == false and !power->isInEffect())
    {
        area->setVisible(false);
    }

    // no activated, the only update is check if there are enough EP to activate it
    if(!power->isActivated())
    {   
        ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
        ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);
        // enough points
        if (GameLevel::getInstance()->getEvolutionPoints() >= power->getCost())
        {
            actionTimer->setPercentage(100);
            ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);
            disabled = false;
            cooldownTimer->setVisible(false);
        }
        else
        {
            disabled = true;
            actionTimer->setPercentage(100);
            cooldownTimer->setPercentage(100);
            cooldownTimer->setVisible(true);
        }
        return;
    }

    // in effect
    if(power->isInEffect())
    {
        ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
        actionTimer->setPercentage((power->getDurationLeft() / power->getDuration()) * 100.0);
    }
    // cooldown
    else
    {
        ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);
        // transition
        if(power->getLastDurationLeft()>0)
        {
            ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
            actionTimer->setPercentage(0);

            ((Sprite *)icon->getChildByTag(0))->setColor(Color3B::WHITE);
            active->setVisible(false);
            cooldownTimer->setVisible(true);
        }
        cooldownTimer->setPercentage(float(power->getCooldownLeft()) / float(power->getCooldown()) * 100);
    }
}

