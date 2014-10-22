//
//  ExpansionGoal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#include "ExpansionGoal.h"
#include "UIGameplayMap.h"

ExpansionGoal::ExpansionGoal(int agType, int min, int max, int average, int des2, int des3, int color)
{
    agentType = agType;
    minTime = min;
    maxTime = max;
    averageTime = average;
    desviation2Star = des2;
    desviation3Star = des3;
    colorZone = color;
}

int ExpansionGoal::getColorZone(void)
{
    return colorZone;
}

void ExpansionGoal::setColorZone(int color)
{
    colorZone = color;
}

void ExpansionGoal::checkGoal(int type, int indexAgent)
{
    int timeSteps = GameLevel::getInstance()->getTimeSteps();
    if (type == agentType) {
        if (timeSteps > maxTime) {
            GameLevel::getInstance()->setFinishedGame(2);
        } else {
            //Check agent at goal zone
            if (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMapHotSpot(1, GameLevel::getInstance()->getAgents().at(type).at(indexAgent)->getPosition()->getX(), GameLevel::getInstance()->getAgents().at(type).at(indexAgent)->getPosition()->getY()) == colorZone) {
                if (minTime > timeSteps) {
                    GameLevel::getInstance()->setFinishedGame(2);
                } else {
                    completed = true;
                    if (timeSteps >= averageTime - desviation2Star and timeSteps <= averageTime + desviation2Star) {
                        if (timeSteps >= averageTime - desviation3Star and timeSteps <= averageTime + desviation3Star) {
                            // 3 STARS
                            score = 3;
                        } else {
                            //2 STARS
                            score = 2;
                        }
                    } else {
                        //1 STAR
                        score = 1;
                    }
                }
            }
        }
    }
}