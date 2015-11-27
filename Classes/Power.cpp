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
//  Power.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "Power.h"

// TODO remove these two includes with CCLOG
#include <cocos2d.h>
#include "GameLevel.h"

Power::Power( const std::string & name, const PowerId & id, float c, float dur, const std::string & t, float _cost) : name(name), id(id), _cooldown(c), _duration(dur), _cooldownLeft(_cooldown), _durationLeft(_duration), type(t), cost(_cost), _activated(false)
{
}


const string & Power::getName() const
{
    return name;
}

const PowerId & Power::getId() const
{
    return id;
}

float Power::getCooldown(void) const
{
    return _cooldown;
}

void Power::setCooldown(float c)
{
    _cooldown = c;
}

float Power::getDuration(void) const
{
    return _duration;
}

float Power::getCooldownLeft(void) const
{
    return _cooldownLeft;
}

void Power::setCooldownLeft(float c)
{
    _lastCooldownLeft = _cooldownLeft;
    _cooldownLeft = c;
    if(_cooldownLeft<=0.0f)
    {
        CCLOG("cooldown finished at step: %d", GameLevel::getInstance()->getTimeSteps());
        _activated = false;
    }
}

float Power::getDurationLeft(void) const
{
    return _durationLeft;
}

void Power::setDurationLeft(float d)
{
    _lastDurationLeft = _durationLeft;
    _durationLeft = d;
}

string Power::getType(void)
{
    return type;
}

void Power::setType(string t)
{
    type = t;
}

float Power::getCost(void)
{
    return cost;
}

void Power::setCost(float c)
{
    cost = c;
}

void Power::activate()
{
    _durationLeft = _duration;
    _lastDurationLeft = _durationLeft;
    _cooldownLeft = 0.0f;
    _lastDurationLeft = _cooldownLeft;
    _activated = true;
}

