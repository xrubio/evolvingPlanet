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
//  Kill.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/11/14.
//
//

#include "Kill.h"
#include "UIGameplayMap.h"

void Kill::execute(Agent* agent)
{
    float probKill = agent->getValue(Hostility);
    int mobility = agent->getValue(Mobility);
    if(RandomHelper::random_real(0.0f, 1.0f)>= probKill)
    {
        return;
    }
    int minRandomX = agent->getPosition().getX() - mobility;
    int maxRandomX = agent->getPosition().getX() + mobility;
    int minRandomY = agent->getPosition().getY() - mobility;
    int maxRandomY = agent->getPosition().getY() + mobility;

    int maxIterations = 10;
    while (maxIterations > 0)
    {
        int posx = RandomHelper::random_int(minRandomX, maxRandomX);
        int posy = RandomHelper::random_int(minRandomY, maxRandomY);

        if(GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
        {
            GameLevel::getInstance()->getAgentAtMap(posx, posy)->setLife(0);
            return;
        }
        maxIterations--;
    }
}

