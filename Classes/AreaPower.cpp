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

AreaPower::AreaPower( const std::string & name, const PowerId & id, float c, float dur, string t, float cost, float rad) : Power(name, id, c, dur, t, cost)
{

    radius = rad;
}

float AreaPower::getRadius(void)
{
    return radius;
}

void AreaPower::setRadius(float r)
{
    radius = r;
}
