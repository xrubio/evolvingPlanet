/*
 * Copyright (c) 2015
 * MURPHY'S TOAST STUDIOS
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
//  Position.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

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
