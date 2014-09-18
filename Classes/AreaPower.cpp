//
//  AreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "AreaPower.h"

AreaPower::AreaPower(string n, int c, int dur, int cl, int dl, string attr, string t, float rad)
{
    name = n;
    cooldown = c;
    duration = dur;
    cooldownLeft = cl;
    durationLeft = dl;
    attribute = attr;
    type = t;
    radius = rad;
}

float AreaPower::getRadius(void)
{
    return radius;
}

void AreaPower::setRadius(float r)
{
    radius = r;
}