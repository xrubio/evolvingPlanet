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
//  Agents.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__Agent__
#define __simulplay__Agent__

#include "Position.h"
#include <vector>
#include <string>

using namespace std;

class Agent {

public:
    typedef std::vector<float> Attributes;
    // number of children to create on a given time step
    static std::vector<int> _numOffspring;
    // number of agents that can be influenced to create on a given time step
    static std::vector<int> _numInfluenced;
    // number of agents that can trade resources on a given time step
    static std::vector<int> _numTraded;
    // pool of resources -> Wood, Mineral
    static std::vector< std::vector<int> > _resourcesPool;
    // pool of max resources -> Wood, Mineral
    static std::vector<int> _resourcesPoolMax;

    Agent(int i = 0, int lf = 0, int t = 0, int posx = 0, int posy = 0);
    ~Agent()
    {
        if (position)
        {
            delete position;
        }
    }

    int getId(void);
    void setId(int i);
    int getLife(void) const;
    // maybe we could set init life as default and then only implement decreaseLife(int amount)
    void setLife(int lf);
    int getType(void);
    void setType(int t);
    const Position & getPosition(void) const;
    void setPosition(int posx, int posy);
    float getValue(int att) const;
    void setValue(int att, float val);
    // copy attribute values at current levels to agent of type
    void copyValues(int type);
    
private:
    int id;
    int life;
    int type;
    Position* position;
    Attributes _attributes;
};

#endif /* defined(__simulplay__Agent__) */

