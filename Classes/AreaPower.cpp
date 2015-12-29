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
//  AreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "AreaPower.h"
#include "GameData.h"

AreaPower::AreaPower( const std::string & name, const PowerType & type, float c, float dur, float cost) : Power(name, type, c, dur, cost), _radius(0.0f), _center(0.0f, 0.0f)
{
}

void AreaPower::setArea( const Vec2 & center, float radius )
{
    _center = center;
    _radius = _center.y - radius; 
    CCLOG("center: %f/%f radius: %f", _center.x, _center.y, _radius);
}

bool AreaPower::isInRadius( const Position & position ) const
{
    float x = float(position.getX()) * float(GameData::getInstance()->getResourcesWidth() / 480.0);
    float y = (GameData::getInstance()->getResourcesHeight()-GameData::getInstance()->getResourcesMargin())/2.0f + (position.getY()*float(GameData::getInstance()->getResourcesMargin()/320.0));
    return (abs(_center.distance(Vec2(x, y))) <= _radius);
}

