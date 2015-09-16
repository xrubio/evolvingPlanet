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
//  Deplete.cpp
//  simulplay
//
//  Created by Guillem Laborda on 23/10/14.
//
//

#include "Deplete.h"
#include "UIGameplayMap.h"

list<Agent*>::reverse_iterator Deplete::execute(int type, Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    int sustainability;// = GameLevel::getInstance()->getAttributesValues(type, "SUSTAINABILITY", agent->getValOfAttribute("SUSTAINABILITY"));
    /*switch (sustainability) {
    case 1:
        sustainability = 70;
        break;
    case 2:
        sustainability = 65;
        break;
    case 3:
        sustainability = 60;
        break;
    case 4:
        sustainability = 50;
        break;
    case 5:
        sustainability = 40;
        break;
    case 6:
        sustainability = 30;
        break;
    case 7:
        sustainability = 25;
        break;
    case 8:
        sustainability = 15;
        break;
    case 9:
        sustainability = 10;
        break;
    case 10:
        sustainability = 0;
        break;
    default:
        sustainability = 100;
        break;
    }*/

    //JUGAR AMB EL TEMPS QUE HA ESTAT OCUPAT L'ESPAI I LA PROBBILITAT
    if (GameLevel::getInstance()->getTimeExploited(agent->getPosition()->getX(), agent->getPosition()->getY()) > 2
        and (rand() % 100) < sustainability) {
        GameLevel::getInstance()->setDepleted(agent->getPosition()->getX(), agent->getPosition()->getY(), true);
    }
}