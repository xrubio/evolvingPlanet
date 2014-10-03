//
//  Goal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#include "Goal.h"

Goal::Goal(int min, int max, int average, int des2, int des3, int color)
{
    minTime = min;
    maxTime = max;
    averageTime = average;
    desviation2Star = des2;
    desviation3Star = des3;
    colorZone = color;
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

int Goal::getColorZone(void)
{
    return colorZone;
}

void Goal::setColorZone(int color)
{
    colorZone = color;
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
