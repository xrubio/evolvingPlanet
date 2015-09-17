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
#include "MultiplierPower.h"
#include "UIGameplayMap.h"
#include "GameLevel.h"

list<Agent*>::reverse_iterator Reproduce::execute(int typeAgent, Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(typeAgent).at(indexAgent);

    int type = agent->getType();
    int mobility = agent->getValue("MOBILITY");
    // TODO XRC move this to method
    //INFLUENCIA CULTURAL - CALCULAR TIPUS
    float probCulture = agent->getValue("CULTURAL_INFLUENCE");
    if (probCulture > 0.0f)
    {
        //CERCAR AGENT EN RADI
        int radi = 3;
        int maxIterations = 9;

        int minRandomX = agent->getPosition()->getX() - radi;
        int maxRandomX = agent->getPosition()->getX() + radi;
        int minRandomY = agent->getPosition()->getY() - radi;
        int maxRandomY = agent->getPosition()->getY() + radi;
        
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
            float targetCulture = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getValue("CULTURAL_INFLUENCE");
            // if a random value between 0 and targetCulture is lower than 0 and ownCulture influence the agent
            if(RandomHelper::random_real(0.0f, targetCulture)<RandomHelper::random_real(0.0f, probCulture))
            {
                type = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType();
            }
        }
 
    }

    //REPRODUIR-SE NORMAL (SI HI HA INLFUENCIA CULTURAL I EL TIPUS ES EL MATEIX TAMBE)
    bool maxReached = false;
    if (GameLevel::getInstance()->getMaxAllAgents() > 0) {
        int numAgents = 0;
        for (size_t i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
            numAgents += GameLevel::getInstance()->getAgents()[i].size();
        }
        maxReached = numAgents >= GameLevel::getInstance()->getMaxAllAgents();
    }
    else {
        maxReached = GameLevel::getInstance()->getAgents()[typeAgent].size() >= GameLevel::getInstance()->getMaxAgent(agent->getType());
    }
    if (type == agent->getType() and maxReached == false)
    {
        float probReproduction = agent->getValue("REPRODUCTION");

        Power* p = nullptr;
        for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
        {
            if (GameLevel::getInstance()->getPowers()[i]->getId() != ReproductionBoost)
            {
                continue;
            }
            p = GameLevel::getInstance()->getPowers()[i];
        }
        if (p != nullptr and p->getDurationLeft() > 0) {
            probReproduction *= ((MultiplierPower*)p)->getMultiplier();
        }
        //srand(time(NULL));
        //if ((rand() % 100) < probReproduction) {
        if (RandomHelper::random_real(0.0f, 1.0f)<probReproduction)
        {
            int maxIterations = 30;
            Point fingerSpot = GameLevel::getInstance()->getAgentDirections()[typeAgent];
            /*if (agent->getType() != 0) {
                fingerSpot.x = -1;
                fingerSpot.y = -1;
            }*/
            int minRandomX = agent->getPosition()->getX() - mobility;
            int maxRandomX = agent->getPosition()->getX() + mobility;
            int minRandomY = agent->getPosition()->getY() - mobility;
            int maxRandomY = agent->getPosition()->getY() + mobility;
            //INDICACIO DE DIRECCIO AMB EL DIT
            if (fingerSpot.x > -1 and fingerSpot.y > -1) {
                if (fingerSpot.x < agent->getPosition()->getX()) {
                    //A L'ESQUERRA
                    maxRandomX = agent->getPosition()->getX();
                }
                else if (fingerSpot.x > agent->getPosition()->getX()) {
                    //A LA DRETA
                    minRandomX = agent->getPosition()->getX();
                }
                if (fingerSpot.y < agent->getPosition()->getY()) {
                    //A BAIX
                    maxRandomY = agent->getPosition()->getY();
                }
                else if (fingerSpot.y > agent->getPosition()->getY()) {
                    //A DALT
                    minRandomY = agent->getPosition()->getY();
                }
            }

            /*int posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
            int posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);*/
            int posx = RandomHelper::random_int(minRandomX, maxRandomX);
            int posy = RandomHelper::random_int(minRandomY, maxRandomY);
            while (maxIterations > 0 and GameLevel::getInstance()->validatePosition(posx, posy) == false) {
                /*posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
                posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);*/
                posx = RandomHelper::random_int(minRandomX, maxRandomX);
                posy = RandomHelper::random_int(minRandomY, maxRandomY);
                maxIterations--;
            }
            if (maxIterations > 0) {
                //auto ag = new Agent(GameLevel::getInstance()->getIdCounter(), 100, type, posx, posy);
                Agent* ag = GameLevel::getInstance()->getAgentsPool()[type].front();
                GameLevel::getInstance()->popFrontAgentsPool(type);
                ag->setId(GameLevel::getInstance()->getIdCounter());
                ag->setLife(100);
                ag->setType(type);
                ag->setPosition(posx, posy);
                ag->copyValues(type);
                GameLevel::getInstance()->addAgent(ag);
                GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
                GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
            }
        }
    }
    //SI TIPUS DIFERENT -> INFLUENCIA CULTURAL PROVOCA QUE LAGENT ES CONVERTEIXI I NO ES REPRODUEIXI
    else if (type != agent->getType()) {
        auto ag = new Agent(agent->getId(), agent->getLife(), type, agent->getPosition()->getX(), agent->getPosition()->getY());
        ag->copyValues(type);
        GameLevel::getInstance()->addAgent(ag);
        agent->setLife(0);
        //GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
        //GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
    }
}

