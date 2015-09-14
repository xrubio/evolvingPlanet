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
//  UIPower.h
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#ifndef __simulplay__UIPower__
#define __simulplay__UIPower__

#include "GameLevel.h"
#include "GameData.h"
#include "cocos2d.h"

using namespace cocos2d;

class UIGameplayMap;

class UIPower : public Node
{
public:
    UIPower(Power* p = 0);

    void setPosition(float x, float y);
    Power* getPower(void);
    bool getClicked(void);
    void setClicked(bool b);
    Sprite* getIcon(void);

    virtual void onTouchesBegan(Point touchLocation) = 0;
    virtual void onTouchesMoved(Touch* touchLocation) = 0;
    virtual void onTouchesEnded(Point touchLocation) = 0;
    virtual void update(float delta) = 0;

protected:
    Power* power;

    Sprite* icon;
    //Label* cooldown;

    bool clicked = false;
    bool disabled = false;
};

#endif /* defined(__simulplay__UIPower__) */
