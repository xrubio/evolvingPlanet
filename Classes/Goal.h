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

    virtual void checkGoal(int type = 0, Agent* agent = nullptr) = 0;

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

