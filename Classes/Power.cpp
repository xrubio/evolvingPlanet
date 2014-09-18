//
//  Power.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "Power.h"

string Power::getName(void)
{
    return name;
}

void Power::setName(string n)
{
    name = n;
}

int Power::getCooldown(void)
{
    return cooldown;
}

void Power::setCooldown(int c)
{
    cooldown = c;
}

int Power::getDuration(void)
{
    return duration;
}

void Power::setDuration(int d)
{
    duration = d;
}

int Power::getCooldownLeft(void)
{
    return cooldownLeft;
}

void Power::setCooldownLeft(int c)
{
    cooldownLeft = c;
}

int Power::getDurationLeft(void)
{
    return durationLeft;
}

void Power::setDurationLeft(int d)
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
