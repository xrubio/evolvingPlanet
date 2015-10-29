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
//  ExpansionGoal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#include "ExpansionGoal.h"
#include "UIGameplayMap.h"

ExpansionGoal::ExpansionGoal(int agType, int min, int max, int average, int des2, int des3, int color) : Goal(agType, min, max, average, des2, des3), colorZone(color), minDistanceToGoal(99999)
{
}

int ExpansionGoal::getColorZone(void)
{
    return colorZone;
}

void ExpansionGoal::setColorZone(int color)
{
    colorZone = color;
}

int ExpansionGoal::getMinDistanceToGoal(void)
{
    return minDistanceToGoal;
}

void ExpansionGoal::setMinDistanceToGoal(int dis)
{
    minDistanceToGoal = dis;
}

Position ExpansionGoal::getCenterArea(void)
{
    return centerArea;
}

void ExpansionGoal::setCenterArea(int x, int y)
{
    centerArea.setPosition(x, y);
}

bool ExpansionGoal::checkGoal(int type, Agent* agent)
{
    // not correct agent, exit
    if(type!=agentType)
    {
        return false;
    }

    int timeSteps = GameLevel::getInstance()->getTimeSteps();
    // goal failed due to time
    if (timeSteps > maxTime)
    {
        GameLevel::getInstance()->setFinishedGame(GoalFailAfter);
        return false;
    }

    // goal is completed if the agent is within the color coded zone for the goal
    int agentColorCode = GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(1, agent->getPosition().getX(), agent->getPosition().getY(), 0);
    if(agentColorCode != colorZone)
    {
        return false;
    }
    
    // the agent arrived before it had to be within the color coded goal zone
    if (minTime > timeSteps)
    {
        GameLevel::getInstance()->setFinishedGame(GoalFailBefore);
        return false;
    }
            
    completed = true;

    // compute deviation
    int diff = std::abs(averageTime-timeSteps);
    if(diff<=desviation3Star)
    {
        score = 3;
    }
    else if(diff<=desviation2Star)
    {
        score = 2;
    }
    else
    {
        score = 1;
    }
    CCLOG("goal completed in step: %d average: %d diff: %d 3star: %d 2star: %d final score: %d", timeSteps, averageTime, diff, desviation3Star, desviation2Star, score);
    return true;
}

