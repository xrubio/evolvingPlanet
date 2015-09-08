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
//  Power.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "Power.h"

Power::Power( const std::string & name, const PowerId & id, float c, float dur, float cl, float dl, string attr, string t ) : name(name), id(id), cooldown(c), duration(dur), cooldownLeft(cl), durationLeft(dl), attribute(attr), type(t)
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

float Power::getCooldown(void)
{
    return cooldown;
}

void Power::setCooldown(float c)
{
    cooldown = c;
}

float Power::getDuration(void)
{
    return duration;
}

void Power::setDuration(float d)
{
    duration = d;
}

float Power::getCooldownLeft(void)
{
    return cooldownLeft;
}

void Power::setCooldownLeft(float c)
{
    cooldownLeft = c;
}

float Power::getDurationLeft(void)
{
    return durationLeft;
}

void Power::setDurationLeft(float d)
{
    durationLeft = d;
}

string Power::getAttribute(void)
{
    return attribute;
}

void Power::setAttribute(string attr)
{
    attribute = attr;
}

string Power::getType(void)
{
    return type;
}

void Power::setType(string t)
{
    type = t;
}

