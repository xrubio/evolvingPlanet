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

class Goal {

public:
    Goal() {};
    Goal(int agentType, int min, int max, int average, int des2, int des3, string gType);
    Goal(Goal const&) {};
    Goal& operator=(Goal const&);

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
    string getGoalType(void);
    void setGoalType(string gType);

    virtual void checkGoal(int type = 0, Agent* agent = nullptr) = 0;

protected:
    int agentType;
    int minTime;
    int maxTime;
    int averageTime;
    int desviation2Star;
    int desviation3Star;
    bool completed = false;
    //1, 2, 3 stars
    int score = 0;
    string goalType = "";
};

#endif /* defined(__simulplay__Goal__) */
