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
#include "GameLevel.h"
#include <base/ccRandom.h>

void Kill::execute(Agent* agent)
{
    float probKill = agent->getValue(eWarfare);
    if(agent->getType()==0 and GameLevel::getInstance()->powerIsInEffect(WarfareBoost))
    {
        probKill = GameLevel::getInstance()->getValueAtLevel(eWarfare, 5);
    }
    if(cocos2d::RandomHelper::random_real(0.0f, 1.0f)>= probKill)
    {
        return;
    }
    
    int mobility = agent->getValue(eMobility);
    cocos2d::Rect area = GameLevel::getInstance()->getArea(agent->getPosition(), mobility);

    int maxIterations = 10;
    while (maxIterations > 0)
    {
        int posx = int(cocos2d::RandomHelper::random_real(area.getMinX(), area.getMaxX()));
        int posy = int(cocos2d::RandomHelper::random_real(area.getMinY(), area.getMaxY()));

        if(GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
        {
            GameLevel::getInstance()->getAgentAtMap(posx, posy)->setLife(0);
            return;
        }
        maxIterations--;
    }
}

