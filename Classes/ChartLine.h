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
//  ChartLine.h
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 06/01/23.
//
//

#pragma once

#include "cocos2d.h"

class ChartLine : public cocos2d::DrawNode {

public:

    CREATE_FUNC(ChartLine);

    bool init() override;

    void drawAsPoly (const cocos2d::Color4F& color);
    
    void addPoint (const cocos2d::Vec2& point);
    
    std::vector<cocos2d::Vec2> getPoints ();
    
    void clearPoints ();
    
private:
    
    std::vector<cocos2d::Vec2> mNodePoints;
};
