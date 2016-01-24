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

#include "Influence.h"
#include "GameLevel.h"
#include <base/ccRandom.h>

void Influence::execute(Agent* agent)
{
    int type = agent->getType();

    // no available slot
    if(Agent::_numInfluenced.at(type)<1)
    {
        return;
    }
    float probInfluence = agent->getValue(eInfluence);
    if(cocos2d::RandomHelper::random_real(0.0f, 1.0f)>= probInfluence)
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
    
        if(GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType()!=type)
        {
            Agent * target = GameLevel::getInstance()->getAgentAtMap(posx, posy);
            Agent * ag = new Agent(target->getId(), target->getLife(), type, posx, posy);
            ag->copyValues(type);
            GameLevel::getInstance()->addAgent(ag);
            target->setLife(0);
            Agent::_numInfluenced.at(type)--;
            return;
        }
        maxIterations--;
    }
}

