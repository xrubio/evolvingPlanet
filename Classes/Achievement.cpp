//
//  Achievement.cpp
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#include "Achievement.h"

Achievement::Achievement( bool c, bool o) : completed(c), occult(o)
{
}

bool Achievement::getCompleted() const
{
    return completed;
}

bool Achievement::getOccult() const
{
    return occult;
}
