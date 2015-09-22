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

list<Agent*>::reverse_iterator Reproduce::execute(int typeAgent, Agent* agent)
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
        int maxIterations = 9;

        int minRandomX = agent->getPosition().getX() - radi;
        int maxRandomX = agent->getPosition().getX() + radi;
        int minRandomY = agent->getPosition().getY() - radi;
        int maxRandomY = agent->getPosition().getY() + radi;
        
        int posx = RandomHelper::random_int(minRandomX, maxRandomX);
        int posy = RandomHelper::random_int(minRandomY, maxRandomY);
        bool validTypeAgent = false;
        if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and
            GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
        {
            validTypeAgent = true;
        }
        while (maxIterations > 0 and validTypeAgent == false) {
            posx = RandomHelper::random_int(minRandomX, maxRandomX);
            posy = RandomHelper::random_int(minRandomY, maxRandomY);
            if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and
                GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
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
                type = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType();
            }
        }
 
    }

    if(Agent::_numOffspring>0)
    {
        if (type == agent->getType())
        {
            Agent::_numOffspring--;

            int maxIterations = 30;
            /*if (agent->getType() != 0) {
                fingerSpot.x = -1;
                fingerSpot.y = -1;
            }*/
            int minRandomX = agent->getPosition().getX() - mobility;
            int maxRandomX = agent->getPosition().getX() + mobility;
            int minRandomY = agent->getPosition().getY() - mobility;
            int maxRandomY = agent->getPosition().getY() + mobility;

            /*int posx = rand() % (2 * mobility) + (agent->getPosition().getX() - mobility);
            int posy = rand() % (2 * mobility) + (agent->getPosition().getY() - mobility);*/
            int posx = RandomHelper::random_int(minRandomX, maxRandomX);
            int posy = RandomHelper::random_int(minRandomY, maxRandomY);
            while (maxIterations > 0 and GameLevel::getInstance()->validatePosition(posx, posy) == false)
            {
                /*posx = rand() % (2 * mobility) + (agent->getPosition().getX() - mobility);
                posy = rand() % (2 * mobility) + (agent->getPosition().getY() - mobility);*/
                posx = RandomHelper::random_int(minRandomX, maxRandomX);
                posy = RandomHelper::random_int(minRandomY, maxRandomY);
                maxIterations--;
            }
            if (maxIterations > 0)
            {
                //auto ag = new Agent(GameLevel::getInstance()->getIdCounter(), 100, type, posx, posy);
                Agent* ag = GameLevel::getInstance()->getAgentsPool()[type].front();
                GameLevel::getInstance()->popFrontAgentsPool(type);
                ag->setId(GameLevel::getInstance()->getIdCounter());
                ag->setLife(200);
                ag->setType(type);
                ag->setPosition(posx, posy);
                ag->copyValues(type);
                GameLevel::getInstance()->addAgent(ag);
                GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
                GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
            }
        }
        //SI TIPUS DIFERENT -> INFLUENCIA CULTURAL PROVOCA QUE LAGENT ES CONVERTEIXI I NO ES REPRODUEIXI
        else
        {
            Agent::_numOffspring--;
            auto ag = new Agent(agent->getId(), agent->getLife(), type, agent->getPosition().getX(), agent->getPosition().getY());
            ag->copyValues(type);
            GameLevel::getInstance()->addAgent(ag);
            agent->setLife(0);
            //GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
            //GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
        }
    }
}

