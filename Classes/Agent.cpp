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
//  Agents.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include "Agent.h"
#include "GameLevel.h"

std::vector<int> Agent::_numOffspring = std::vector<int>();
std::vector<int> Agent::_numInfluenced = std::vector<int>();

Agent::Agent(int i, int lf, int t, int posx, int posy) : id(i), life(lf), type(t), position(0), _attributes(GameLevel::_numAttributes, 0.0f)
{
    position = new Position(posx, posy);
}

int Agent::getId(void)
{
    return id;
}

void Agent::setId(int i)
{
    id = i;
}

int Agent::getLife(void)
{
    return life;
}

void Agent::setLife(int lf)
{
    life = lf;
}

int Agent::getType(void)
{
    return type;
}

void Agent::setType(int t)
{
    type = t;
}

const Position & Agent::getPosition(void) const
{
    return *position;
}

void Agent::setPosition(int posx, int posy)
{
    position->setPosition(posx, posy);
}

float Agent::getValue(int att) const
{
    float value = _attributes.at(att);
    // if technology multiply result
    if(att==eTechnology or _attributes.at(eTechnology)==0.0f)
    {
        return value;
    }
    else
    {
        return value*_attributes.at(eTechnology);
    }
}

void Agent::setValue(int att, float val)
{
    _attributes.at(att) = val;
}

void Agent::copyValues( int type )
{
    const GameLevel::Levels & currentValues = GameLevel::getInstance()->getAgentAttributes(type);
    for(size_t i=0; i<currentValues.size(); i++)
    {
        int currentValue = currentValues.at(i);
        // if value not set skip the attribute (and leave value at 0)
        if(currentValue==-1)
        {
            continue;
        }
        float value = GameLevel::getInstance()->getValueAtLevel(int(i), currentValue);
        setValue(int(i), value);
    }
}

