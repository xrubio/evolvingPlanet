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
//  Achievement.cpp
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#include "Achievement.h"

Achievement::Achievement(const std::string & icon, const std::string & resource, const std::string & goalType, int level, bool completed, bool occult) : _icon(icon), _resource(resource), _goalType(goalType), _level(level), _completed(completed), _occult(occult)
{
}

string Achievement::getIcon() const
{
    return _icon;
}

string Achievement::getResource() const
{
    return _resource;
}

string Achievement::getGoalType() const
{
    return _goalType;
}

int Achievement::getLevel() const
{
    return _level;
}

bool Achievement::getCompleted() const
{
    //return true;
    return _completed;
}

bool Achievement::getOccult() const
{
    return _occult;
}
