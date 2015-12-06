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
//  CollectionGoal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#include "CollectionGoal.h"
#include "UIGameplayMap.h"

CollectionGoal::CollectionGoal(int agType, int min, int max, int average, int des2, int des3, int goalAm, int resourceType) : Goal(agType, min, max, average, des2, des3), goalAmount(goalAm), _resourceType(resourceType)
{
}

int CollectionGoal::getGoalAmount(void)
{
    return goalAmount;
}

void CollectionGoal::setGoalAmount(int goalAm)
{
    goalAmount = goalAm;
}

bool CollectionGoal::checkGoal(int type, Agent* agent)
{
    if(type!=agentType)
    {
        return false;
    }
    
    int timeSteps = int(Timing::getInstance()->getTimeStep());
    // goal failed due to time
    if (timeSteps > maxTime) {
        GameLevel::getInstance()->setFinishedGame(GoalFailAfter);
        return false;
    }
    
    if (Agent::_resourcesPool.at(type).at(_resourceType)<goalAmount)
    {
        return false;
    }
    
    // resources collected they had to be
    if (minTime > timeSteps)
    {
        GameLevel::getInstance()->setFinishedGame(GoalFailBefore);
        return false;
    }
    
    completed = true;
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
