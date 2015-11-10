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
//  Reproduce.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Reproduce.h"
#include "GameLevel.h"
#include <base/ccRandom.h>

void Reproduce::execute(Agent* agent)
{   
    int type = agent->getType();
    // no available slot
    if(Agent::_numOffspring.at(type)<1)
    {
        return;
    }
  
    int mobility = agent->getValue(eMobility);
    cocos2d::Rect area = GameLevel::getInstance()->getArea(agent->getPosition(), mobility);

    int maxIterations = 30;
    while(maxIterations>0)
    {      
        int posx = int(cocos2d::RandomHelper::random_real(area.getMinX(), area.getMaxX()));
        int posy = int(cocos2d::RandomHelper::random_real(area.getMinY(), area.getMaxY()));

        // not a valid position
        if(GameLevel::getInstance()->validatePosition(posx, posy) == false)
        {
            maxIterations--;
            continue;
        }
        Agent::_numOffspring.at(type)--;
        Agent * ag = GameLevel::getInstance()->getAgentsPool().at(type).front();
        GameLevel::getInstance()->popFrontAgentsPool(type);
        ag->setId(GameLevel::getInstance()->getIdCounter());
        // mean of 200, but each agent is slightly different
        ag->setLife(cocos2d::RandomHelper::random_int(150,250));
        ag->setType(type);
        ag->setPosition(posx, posy);
        ag->copyValues(type);
        GameLevel::getInstance()->addAgent(ag);
        GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
        return;
    }
}

