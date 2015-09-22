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
//  EnvironmentAdaptation.cpp
//  simulplay
//
//  Created by Guillem Laborda on 24/10/14.
//
//

#include "EnvironmentAdaptation.h"
#include "UIGameplayMap.h"

list<Agent*>::reverse_iterator EnvironmentAdaptation::execute(int type, Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);
    float adaptation = agent->getValue(Adaptation);
    //JUGAR AMB EL TEMPS QUE HA ESTAT OCUPAT L'ESPAI I LA PROBBILITAT
    if (GameLevel::getInstance()->getTimeExploited(agent->getPosition().getX(), agent->getPosition().getY()) > 2 and RandomHelper::random_real(0.0f,1.0f)< adaptation)
    {
        GameLevel::getInstance()->setEnvironmentAdaptation(agent->getPosition().getX(), agent->getPosition().getY(), true);
    }
}

