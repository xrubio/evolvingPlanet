//
//  ExpansionGoal.h
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#ifndef __simulplay__ExpansionGoal__
#define __simulplay__ExpansionGoal__

#include "Goal.h"

class ExpansionGoal : public Goal {

public:
    ExpansionGoal(int agType, int min, int max, int average, int des2, int des3, int color = 0);

    int getColorZone(void);
    void setColorZone(int color);
    int getMinDistanceToGoal(void);
    void setMinDistanceToGoal(int dis);
    Position getCenterArea(void);
    void setCenterArea(int x, int y);

    void checkGoal(int type, Agent* agent);
    GoalType getGoalType() const
    {
        return Expansion;
    }

private:
    int colorZone;
    int minDistanceToGoal;
    Position centerArea;
};

#endif /* defined(__simulplay__ExpansionGoal__) */
