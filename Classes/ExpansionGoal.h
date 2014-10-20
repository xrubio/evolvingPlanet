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
    ExpansionGoal(int min, int max, int average, int des2, int des3, int color = 0);
    ExpansionGoal(ExpansionGoal const&) {};
    ExpansionGoal& operator=(ExpansionGoal const&);

    int getColorZone(void);
    void setColorZone(int color);

    void checkGoal(int indexAgent = 0);

private:
    int colorZone;
};
#endif /* defined(__simulplay__ExpansionGoal__) */
