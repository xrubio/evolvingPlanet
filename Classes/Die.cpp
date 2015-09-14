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
//  Die.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Die.h"
#include "UIGameplayMap.h"

list<Agent*>::reverse_iterator Die::execute(int type, Agent* agent)
{
    UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    float harm = gameplayMap->getValueAtGameplayMap(0, agent->getPosition()->getX(), agent->getPosition()->getY(), 0);
//    harm /= 4.0f;

    //int resistance = GameLevel::getInstance()->getAttributesValues(type, "RESISTANCE", agent->getValOfAttribute("RESISTANCE"));
    int resistanceVal = agent->getValOfAttribute("RESISTANCE");
    float resistance = 1.0f;
    switch (resistanceVal) {
    case 1:
        resistance = 1.1f;
        break;
    case 2:
        resistance = 1.3f;
        break;
    case 3:
        resistance = 1.5f;
        break;
    case 4:
        resistance = 1.7f;
        break;
    case 5:
        resistance = 2.0f;
        break;
    default:
        resistance = 1.0f;
        break;
    }
    //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
    Power* p = nullptr;
    for (size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
    {
        if (GameLevel::getInstance()->getPowers()[i]->getId() != ResistanceBoost)
        {
            continue;
        }
        p = GameLevel::getInstance()->getPowers()[i];
    }
    if (p != nullptr and p->getDurationLeft() > 0) {
        if (gameplayMap->isInBoostResistanceArea(agent->getPosition()->getX() * float(2048.0 / 480.0),
                ((1536 - 1365) / 2) + (agent->getPosition()->getY() * float(1365.0 / 320.0)))) {
            harm /= 2.0f;
        }
    }
    harm /= resistance;
    if (harm < 0) {
        harm = 0;
    }
    agent->setLife(agent->getLife() - int(harm));

    if (agent->getLife() <= 0) {
        GameLevel::getInstance()->addDeletedAgent(Point(agent->getPosition()->getX(), agent->getPosition()->getY()));
        return GameLevel::getInstance()->deleteAgent(agent->getType(), agent);
    }
}
