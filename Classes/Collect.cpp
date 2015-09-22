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
//  Collect.cpp
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#include "Collect.h"
#include "UIGameplayMap.h"
#include "CollectionGoal.h"

list<Agent*>::reverse_iterator Collect::execute(int type, Agent* agent)
{
    CCLOG("Collect is an Act with temporal implementation");
    //UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    GameLevel::getInstance()->setTimeExploited(agent->getPosition().getX(), agent->getPosition().getY(),
        GameLevel::getInstance()->getTimeExploited(agent->getPosition().getX(),
            agent->getPosition().getY()) + 1);

    float efficiency;// = GameLevel::getInstance()->getAttributesValues(type, "RECOLLECTION", agent->getValOfAttribute("RECOLLECTION")) / 100;
    /*float efficiency;
    switch (recollection) {
    case 1:
        efficiency = 0.2;
        break;
    case 2:
        efficiency = 0.3;
        break;
    case 3:
        efficiency = 0.4;
        break;
    case 4:
        efficiency = 0.5;
        break;
    case 5:
        efficiency = 0.7;
        break;
    case 6:
        efficiency = 0.8;
        break;
    case 7:
        efficiency = 1;
        break;
    case 8:
        efficiency = 1.3;
        break;
    case 9:
        efficiency = 1.5;
        break;
    case 10:
        efficiency = 2;
        break;
    default:
        efficiency = 0;
        break;
    }*/

    Power* p = nullptr;
    Power* pRest = nullptr;
    for (size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
    {
        PowerId powerId = GameLevel::getInstance()->getPowers()[i]->getId();
        if (powerId == RecollectionBoost)
        {
            p = GameLevel::getInstance()->getPowers()[i];
        }
        else if (powerId == RestoreLand)
        {
            pRest = GameLevel::getInstance()->getPowers()[i];
        }
    }
    if (p != nullptr and p->getDurationLeft() > 0) {
        efficiency += 0.5;
    }

    if (pRest != nullptr and pRest->getDurationLeft() > 0) {
        //RESTORE LAND
        GameLevel::getInstance()->getUIGameplayMap()->restoreLand();
        pRest->setDurationLeft(0);
        pRest->setCooldownLeft(pRest->getCooldown() + 1);
    }

    for (size_t i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        if (GameLevel::getInstance()->getGoals()[i]->getAgentType() == type and ((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->getGoalAmount() > 0) {
            int mapSelector = 0;
            if (GameLevel::getInstance()->getDepleted(agent->getPosition().getX(), agent->getPosition().getY())) {
                mapSelector = 1;
            }
            else if (GameLevel::getInstance()->getEnvironmentAdaptation(agent->getPosition().getX(), agent->getPosition().getY())) {
                mapSelector = 2;
            }

            ((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->setCurrentAmount(((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->getCurrentAmount() + (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(mapSelector, agent->getPosition().getX(), agent->getPosition().getY(), 1) * efficiency));
        }
    }
}
