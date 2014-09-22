//
//  Goal.cpp
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#include "Goal.h"

Goal::Goal(int min, int max, int average, int color)
{
    minTime = min;
    maxTime = max;
    averageTime = average;
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

int Goal::getColorZone(void)
{
    return colorZone;
}

void Goal::setColorZone(int color)
{
    colorZone = color;
}
