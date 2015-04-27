//
//  Achievement.cpp
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#include "Achievement.h"

bool Achievement::getCompleted(void)
{
    return completed;
}

void Achievement::setCompleted(bool b)
{
    completed = b;
}

bool Achievement::getOccult(void)
{
    return occult;
}

void Achievement::setOccult(bool b)
{
    occult = b;
}