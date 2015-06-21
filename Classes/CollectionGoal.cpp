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
