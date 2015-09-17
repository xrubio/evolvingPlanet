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

Agent::Agent(int i, int lf, int t, int posx, int posy) : id(i), life(lf), type(t), position(0)
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

Position* Agent::getPosition(void)
{
    return position;
}

void Agent::setPosition(int posx, int posy)
{
    position->setPosition(posx, posy);
}

float Agent::getValue(const string & att) const
{
    AttributesMap::const_iterator it = _attributes.find(att);
    if(it==_attributes.end())
    {
        return 0.0f;
    }
    float value = it->second;

    // TODO XRC increase efficiency storing the value in a different attribute?
    // if technology multiply result
    it = _attributes.find("TECHNOLOGY");
    if(att=="TECHNOLOGY" or it==_attributes.end())
    {
        return value;
    }
    else
    {
        return value*it->second;
    }
}

void Agent::setValue(const string & att, float val)
{
    _attributes[att] = val;
}

void Agent::copyValues( int type )
{
    const GameLevel::LevelsMap & currentValues = GameLevel::getInstance()->getAgentAttributes(type);
    for(GameLevel::LevelsMap::const_iterator it=currentValues.begin(); it!=currentValues.end(); it++)
    {
        float value = GameLevel::getInstance()->getValueAtLevel(it->first, it->second);
        setValue(it->first, value);
    }
}

