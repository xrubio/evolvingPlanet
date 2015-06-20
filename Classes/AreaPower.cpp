//
//  AreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#include "AreaPower.h"

AreaPower::AreaPower( const std::string & name, const PowerId & id, float c, float dur, float cl, float dl, string attr, string t, float rad) : Power(name, id, c, dur, cl, dl, attr, t)
{

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
