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
//  MultiplierPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "MultiplierPower.h"

MultiplierPower::MultiplierPower( const std::string & name, const PowerId & id, float c, float dur, float cl, float dl, string attr, string t, float cost, float mult) : Power(name, id, c, dur, cl, dl, attr, t, cost)
{
    multiplier = mult;
}

float MultiplierPower::getMultiplier(void)
{
    return multiplier;
}

void MultiplierPower::setMultiplier(float m)
{
    multiplier = m;
}
