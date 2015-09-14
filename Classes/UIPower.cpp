/*
 * Copyright (c) 2015
 * MURPHY'S TOAST GAMES
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
//  UIPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIPower.h"

UIPower::UIPower(Power * p) : power(p)
{
}

void UIPower::setPosition(float x, float y)
{
    icon->setPosition(x, y);
    //cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
}

Power* UIPower::getPower(void)
{
    return power;
}

bool UIPower::getClicked(void)
{
    return clicked;
}

void UIPower::setClicked(bool b)
{
    clicked = b;
}

Sprite* UIPower::getIcon()
{
    return icon;
}
