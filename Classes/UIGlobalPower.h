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
//  UIGlobalPower.h
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#ifndef __simulplay__UIGlobalPower__
#define __simulplay__UIGlobalPower__

#include "UIPower.h"

class UIGlobalPower : public UIPower {
public:
    UIGlobalPower(Power* p = 0);
    
    void onTouchesBegan(const Point & touchLocation);
    void onTouchesMoved(Touch* touchLocation);
    bool onTouchesEnded(const Point & touchLocation);
    void update(float delta);
};

#endif /* defined(__simulplay__UIGlobalPower__) */
