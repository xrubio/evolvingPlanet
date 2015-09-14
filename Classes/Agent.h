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
#include <map>
#include <string>

using namespace std;

class Agent {

public:
    Agent(int i = 0, int lf = 0, int t = 0, int posx = 0, int posy = 0);
    ~Agent(){};

    int getId(void);
    void setId(int i);
    int getLife(void);
    void setLife(int lf);
    int getType(void);
    void setType(int t);
    Position* getPosition(void);
    void setPosition(int posx, int posy);
    map<string, int> getAttributes(void);
    void setAttributes(map<string, int> map);
    int getValOfAttribute(string att);
    void setValOfAttribute(string att, int val);

private:
    int id;
    int life;
    int type;
    Position* position;
    map<string, int> attributes;
};

#endif /* defined(__simulplay__Agent__) */
