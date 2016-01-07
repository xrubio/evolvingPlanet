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

#include "Trade.h"
#include "GameLevel.h"
#include <base/ccRandom.h>

void Trade::execute(Agent* agent)
{
    int type = agent->getType();
    
    // no available slot
    if(Agent::_numTraded.at(type)<1)
    {
        return;
    }
    float probTrade = agent->getValue(eTrade);
    if(cocos2d::RandomHelper::random_real(0.0f, 1.0f)>= probTrade)
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
        
        int typeCpu;
        if(GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and (typeCpu =GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType()) != type)
        {
            //calculate which resource take
            float total = Agent::_resourcesPool.at(typeCpu).at(Wood) + Agent::_resourcesPool.at(typeCpu).at(Mineral);
            float pWood = float(Agent::_resourcesPool.at(typeCpu).at(Wood)) / total;
            float prob = cocos2d::RandomHelper::random_real(0.0f, 1.0f);
            Resource resourceTaken;

            if (prob < pWood)
            {
                resourceTaken = Wood;
            }
            // not wood, mineral
            else
            {
                resourceTaken = Mineral;
            }
           
            // check if the resource to take is already at maximum
            if(Agent::_resourcesPool.at(type).at(resourceTaken)>Agent::_resourcesPoolMax.at(resourceTaken))
            {
                return;
            }
            //check if the poblation has resources
            if (Agent::_resourcesPool.at(typeCpu).at(resourceTaken) > 0)
            {
                //you
                Agent::_resourcesPool.at(type).at(resourceTaken)++;
                //cpu
                Agent::_resourcesPool.at(typeCpu).at(resourceTaken)--;
                Agent::_numTraded.at(type)--;
                return;
            }
        }
        maxIterations--;
    }
}

