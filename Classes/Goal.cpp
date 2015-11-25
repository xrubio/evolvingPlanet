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
//  Goal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#include "Goal.h"

Goal::Goal(int agType, int min, int max, int average, int des2, int des3) : agentType(agType), minTime(min), maxTime(max), averageTime(average), desviation2Star(des2), desviation3Star(des3), completed(false), score(0)
{
}

int Goal::getAgentType(void)
{
    return agentType;
}

int Goal::getMinTime(void)
{
    return minTime;
}

int Goal::getMaxTime(void)
{
    return maxTime;
}

int Goal::getAverageTime(void)
{
    return averageTime;
}

int Goal::getDesviation2Star(void)
{
    return desviation2Star;
}

int Goal::getDesviation3Star(void)
{
    return desviation3Star;
}

bool Goal::getCompleted(void)
{
    return completed;
}

int Goal::getScore(void)
{
    return score;
}

