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

void ExpansionGoal::checkGoal(int type, Agent* agent)
{
    /*int dist = Point(agent->getPosition().getX(), agent->getPosition().getY()).getDistance(Vec2(centerArea.getX(), centerArea.getY()));
    if (dist < minDistanceToGoal) {
        minDistanceToGoal = dist;
    }*/
    int timeSteps = GameLevel::getInstance()->getTimeSteps();
    if (type == agentType) {
        if (timeSteps > maxTime) {
            GameLevel::getInstance()->setFinishedGame(GoalFailAfter);
        }
        else {
            //Check agent at goal zone
            if (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(1, agent->getPosition().getX(),
                    agent->getPosition().getY(), 0) == colorZone) {
                if (minTime > timeSteps) {
                    GameLevel::getInstance()->setFinishedGame(GoalFailBefore);
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
}
