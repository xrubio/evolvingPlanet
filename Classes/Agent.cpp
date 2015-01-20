//
//  Agents.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include "Agent.h"

Agent::Agent(int i, int lf, int t, int posx, int posy)
{
    id = i;
    life = lf;
    type = t;
    position = new Position(posx, posy);
}

int Agent::getId(void)
{
    return id;
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

map<string, int> Agent::getAttributes(void)
{
    return attributes;
}

void Agent::setAttributes(map<string, int> map)
{
    attributes = map;
}

int Agent::getValOfAttribute(string att)
{
    if (attributes.count(att) != 1) {
        return -1;
    } else {
        return attributes[att];
    }
}

void Agent::setValOfAttribute(string att, int val)
{
    attributes[att] = val;
}
