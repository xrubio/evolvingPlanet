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
#include "Power.h"
#include "UIGameplayMap.h"
#include "GameLevel.h"

void Reproduce::execute(Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(typeAgent).at(indexAgent);

    int type = agent->getType();
    int mobility = agent->getValue(Mobility);
    // TODO XRC move this to method
    //INFLUENCIA CULTURAL - CALCULAR TIPUS
    float probCulture = agent->getValue(CulturalInfluence);
    if (probCulture > 0.0f)
    {
        //CERCAR AGENT EN RADI
        int radi = 3;

        int minRandomX = agent->getPosition().getX() - radi;
        int maxRandomX = agent->getPosition().getX() + radi;
        int minRandomY = agent->getPosition().getY() - radi;
        int maxRandomY = agent->getPosition().getY() + radi;
        
        bool validTypeAgent = false;
        int maxIterations = 9;
        int posx = 0;
        int posy = 0;
        while (maxIterations > 0 and validTypeAgent == false)
        {
            posx = RandomHelper::random_int(minRandomX, maxRandomX);
            posy = RandomHelper::random_int(minRandomY, maxRandomY);
            if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
            {
                validTypeAgent = true;
            }
            maxIterations--;
        }
        
        //SI S'HA TROBAT AGENT I INFLUEIX
        if (maxIterations > 0)
        {
            float targetCulture = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getValue(CulturalInfluence);
            // if a random value between 0 and targetCulture is lower than 0 and ownCulture influence the agent
            if(RandomHelper::random_real(0.0f, targetCulture)<RandomHelper::random_real(0.0f, probCulture))
            {
                CCLOG("agent influenced!");
                // XRC TODO això és correcte?
                type = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType();
            }
        }
 
    }

    if(Agent::_numOffspring.at(type)>0)
    {
        if (type == agent->getType())
        {
            Agent::_numOffspring.at(type)--;

            int maxIterations = 30;

            int minRandomX = agent->getPosition().getX() - mobility;
            int maxRandomX = agent->getPosition().getX() + mobility;
            int minRandomY = agent->getPosition().getY() - mobility;
            int maxRandomY = agent->getPosition().getY() + mobility;

            int posx = RandomHelper::random_int(minRandomX, maxRandomX);
            int posy = RandomHelper::random_int(minRandomY, maxRandomY);
            while (maxIterations > 0 and GameLevel::getInstance()->validatePosition(posx, posy) == false)
            {
                posx = RandomHelper::random_int(minRandomX, maxRandomX);
                posy = RandomHelper::random_int(minRandomY, maxRandomY);
                maxIterations--;
            }
            if (maxIterations > 0)
            {
                Agent* ag = GameLevel::getInstance()->getAgentsPool().at(type).front();
                GameLevel::getInstance()->popFrontAgentsPool(type);
                ag->setId(GameLevel::getInstance()->getIdCounter());
                ag->setLife(200);
                ag->setType(type);
                ag->setPosition(posx, posy);
                ag->copyValues(type);
                GameLevel::getInstance()->addAgent(ag);
                GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
            }
        }
        //SI TIPUS DIFERENT -> INFLUENCIA CULTURAL PROVOCA QUE LAGENT ES CONVERTEIXI I NO ES REPRODUEIXI
        // XRC TODO mirar si offspring és correcte
        else
        {
            Agent::_numOffspring.at(type)--;
            auto ag = new Agent(agent->getId(), agent->getLife(), type, agent->getPosition().getX(), agent->getPosition().getY());
            ag->copyValues(type);
            GameLevel::getInstance()->addAgent(ag);
            agent->setLife(0);
        }
    }
}

