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
    /*int dist = Point(agent->getPosition()->getX(), agent->getPosition()->getY()).getDistance(Vec2(centerArea.getX(), centerArea.getY()));
    if (dist < minDistanceToGoal) {
        minDistanceToGoal = dist;
    }*/
    int timeSteps = GameLevel::getInstance()->getTimeSteps();
    if (type == agentType) {
        if (timeSteps > maxTime) {
            GameLevel::getInstance()->setFinishedGame(2);
        }
        else {
            //Check agent at goal zone
            if (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(1, agent->getPosition()->getX(),
                    agent->getPosition()->getY(), 0) == colorZone) {
                if (minTime > timeSteps) {
                    GameLevel::getInstance()->setFinishedGame(2);
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
