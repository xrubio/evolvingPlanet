//
//  Position.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include <iostream>
#include "Position.h"

Position::Position(int init_x, int init_y)
{
    x = init_x;
    y = init_y;
}

void Position::setPosition(int init_x, int init_y)
{
    x = init_x;
    y = init_y;
}

int Position::getX() const
{
    return x;
}

int Position::getY() const
{
    return y;
}