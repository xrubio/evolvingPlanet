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
//  ChartLine.cpp
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 06/01/23.
//
//

#include "ChartLine.h"

USING_NS_CC;

bool ChartLine::init() {
    return DrawNode::init();
}

void ChartLine::drawAsPoly (const Color4F& color) {
    
    if (mNodePoints.size() > 1)
    {
        drawPoly(mNodePoints.data(), static_cast<int>(mNodePoints.size()), false, color);
    }
}

void ChartLine::addPoint (const cocos2d::Vec2& point) {
    mNodePoints.emplace_back(point);
}

std::vector<Vec2> ChartLine::getPoints () {
    return mNodePoints;
}

void ChartLine::clearPoints () {
    mNodePoints.clear();
}
