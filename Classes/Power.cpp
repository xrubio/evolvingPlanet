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

Power::Power( const std::string & name, const PowerType & type, float c, float dur, float _cost) : name(name), _type(type), _cooldown(c), _duration(dur), _cooldownLeft(_cooldown), _durationLeft(_duration), cost(_cost), _activated(false)
{
}


const string & Power::getName() const
{
    return name;
}

const PowerType & Power::getType() const
{
    return _type;
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

void Power::decreaseCooldownLeft(float c)
{    
    _lastCooldownLeft = _cooldownLeft;
    _cooldownLeft -= c;
    if(_cooldownLeft<=0.0f)
    {
        _activated = false;
    }
}

float Power::getDurationLeft(void) const
{
    return _durationLeft;
}

void Power::decreaseDurationLeft(float d)
{
    _lastDurationLeft = _durationLeft;
    _durationLeft -= d;
}
    
bool Power::isInEffect() const
{
    if(!_activated)
    {
        return false;
    }
    if(_durationLeft>0)
    {
        return true;
    }
    return false;
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

