//
//  Power.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "Power.h"

Power::Power( const std::string & name, const PowerId & id ) : name(name), id(id)
{
}


const string & Power::getName() const
{
    return name;
}

const PowerId & Power::getId() const
{
    return id;
}

float Power::getCooldown(void)
{
    return cooldown;
}

void Power::setCooldown(float c)
{
    cooldown = c;
}

float Power::getDuration(void)
{
    return duration;
}

void Power::setDuration(float d)
{
    duration = d;
}

float Power::getCooldownLeft(void)
{
    return cooldownLeft;
}

void Power::setCooldownLeft(float c)
{
    cooldownLeft = c;
}

float Power::getDurationLeft(void)
{
    return durationLeft;
}

void Power::setDurationLeft(float d)
{
    durationLeft = d;
}

string Power::getAttribute(void)
{
    return attribute;
}

void Power::setAttribute(string attr)
{
    attribute = attr;
}

string Power::getType(void)
{
    return type;
}

void Power::setType(string t)
{
    type = t;
}
