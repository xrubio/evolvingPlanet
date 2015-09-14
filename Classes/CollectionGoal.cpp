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

CollectionGoal::CollectionGoal(int agType, int min, int max, int average, int des2, int des3, int goalAm) : Goal(agType, min, max, average, des2, des3), goalAmount(goalAm), currentAmount(0)
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

int CollectionGoal::getCurrentAmount(void)
{
    return currentAmount;
}

void CollectionGoal::setCurrentAmount(int currAm)
{
    currentAmount = currAm;
}

void CollectionGoal::checkGoal(int type, Agent* agent)
{
    int timeSteps = GameLevel::getInstance()->getTimeSteps();
    if (timeSteps > maxTime) {
        GameLevel::getInstance()->setFinishedGame(GoalFail);
    }
    else {
        if (currentAmount >= goalAmount) {
            if (minTime > timeSteps) {
                GameLevel::getInstance()->setFinishedGame(GoalFail);
            }
            else {
                completed = true;
                if (timeSteps >= averageTime - desviation2Star and timeSteps <= averageTime + desviation2Star) {
                    if (timeSteps >= averageTime - desviation3Star and timeSteps <= averageTime + desviation3Star) {
                        // 3 STARS
                        score = 3;
                    }
                    else {
                        //2 STARS
                        score = 2;
                    }
                }
                else {
                    //1 STAR
                    score = 1;
                }
            }
        }
    }
}
