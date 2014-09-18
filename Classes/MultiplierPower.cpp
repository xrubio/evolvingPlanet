//
//  MultiplierPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "MultiplierPower.h"

MultiplierPower::MultiplierPower(string n, int c, int dur, int cl, int dl, string attr, string t, float mult)
{
    name = n;
    cooldown = c;
    duration = dur;
    cooldownLeft = cl;
    durationLeft = dl;
    attribute = attr;
    type = t;
    multiplier = mult;
}

float MultiplierPower::getMultiplier(void)
{
    return multiplier;
}

void MultiplierPower::setMultiplier(float m)
{
    multiplier = m;
}