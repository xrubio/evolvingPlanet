//
//  Goal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#include "Goal.h"

Goal::Goal(int agType, int min, int max, int average, int des2, int des3, string gType)
{
    agentType = agType;
    minTime = min;
    maxTime = max;
    averageTime = average;
    desviation2Star = des2;
    desviation3Star = des3;
    goalType = gType;
}

int Goal::getAgentType(void)
{
    return agentType;
}

void Goal::setAgentType(int agType)
{
    agentType = agType;
}

int Goal::getMinTime(void)
{
    return minTime;
}

void Goal::setMinTime(int min)
{
    minTime = min;
}

int Goal::getMaxTime(void)
{
    return maxTime;
}

void Goal::setMaxTime(int max)
{
    maxTime = max;
}

int Goal::getAverageTime(void)
{
    return averageTime;
}

void Goal::setAverageTime(int average)
{
    averageTime = average;
}

int Goal::getDesviation2Star(void)
{
    return desviation2Star;
}

void Goal::setDesviation2Star(int des)
{
    desviation2Star = des;
}

int Goal::getDesviation3Star(void)
{
    return desviation3Star;
}

void Goal::setDesviation3Star(int des)
{
    desviation3Star = des;
}

bool Goal::getCompleted(void)
{
    return completed;
}

void Goal::setCompleted(bool c)
{
    completed = c;
}

int Goal::getScore(void)
{
    return score;
}

void Goal::setScore(int s)
{
    score = s;
}

string Goal::getGoalType(void)
{
    return goalType;
}

void Goal::setGoalType(string gType)
{
    goalType = gType;
}