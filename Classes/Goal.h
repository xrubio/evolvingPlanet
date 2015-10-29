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
//  Goal.h
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#ifndef __simulplay__Goal__
#define __simulplay__Goal__

#include "Agent.h"

enum GoalType
{
    Collection = 0,
    Expansion = 1
};

class Goal {

public:
    Goal(int agentType, int min, int max, int average, int des2, int des3);

    int getAgentType(void);
    void setAgentType(int agType);
    int getMinTime(void);
    void setMinTime(int min);
    int getMaxTime(void);
    void setMaxTime(int max);
    int getAverageTime(void);
    void setAverageTime(int average);
    int getDesviation2Star(void);
    void setDesviation2Star(int des);
    int getDesviation3Star(void);
    void setDesviation3Star(int des);
    bool getCompleted(void);
    void setCompleted(bool c);
    int getScore(void);
    void setScore(int s);
    virtual GoalType getGoalType() const = 0;

    virtual bool checkGoal(int type = 0, Agent* agent = nullptr) = 0;

protected:
    int agentType;
    int minTime;
    int maxTime;
    int averageTime;
    int desviation2Star;
    int desviation3Star;
    bool completed;
    //1, 2, 3 stars
    int score;
};

#endif /* defined(__simulplay__Goal__) */

