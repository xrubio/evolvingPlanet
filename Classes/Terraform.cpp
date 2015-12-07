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
//  Terraform.cpp
//  simulplay
//
//  Created by Guillem Laborda on 07/12/15.
//
//

#include "Terraform.h"
#include "UIGameplayMap.h"

void Terraform::execute(Agent* agent)
{
    //if its not terraformed and is terraformable
    if (GameLevel::getInstance()->getTerraformed(agent->getPosition().getX(), agent->getPosition().getY()) == false and  GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(2, agent->getPosition().getX(), agent->getPosition().getY()) == 4)
    {
        //check if agent exploits
        float probTerraform = agent->getValue(eTerraform);
        if(cocos2d::RandomHelper::random_real(0.0f, 1.0f) >= probTerraform)
        {
            return;
        }
        GameLevel::getInstance()->setTerraformed(agent->getPosition().getX(), agent->getPosition().getY(), true);
    }
}

