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
    ExpansionGoal() {};
    ExpansionGoal(int agType, int min, int max, int average, int des2, int des3, string gType, int color = 0);
    ExpansionGoal(ExpansionGoal const&) {};
    ExpansionGoal& operator=(ExpansionGoal const&);

    int getColorZone(void);
    void setColorZone(int color);
    int getMinDistanceToGoal(void);
    void setMinDistanceToGoal(int dis);
    Position getCenterArea(void);
    void setCenterArea(int x, int y);

    inline void checkGoal(int type, Agent* agent);

private:
    int colorZone;
    int minDistanceToGoal = 99999;
    Position centerArea;
};

#endif /* defined(__simulplay__ExpansionGoal__) */