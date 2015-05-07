//
//  CollectionGoal.h
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#ifndef __simulplay__CollectionGoal__
#define __simulplay__CollectionGoal__

#include "Goal.h"

class CollectionGoal : public Goal {

public:
    CollectionGoal(){};
    CollectionGoal(int agType, int min, int max, int average, int des2, int des3, string gType, int goalAm);
    CollectionGoal(CollectionGoal const&){};
    CollectionGoal& operator=(CollectionGoal const&);

    int getGoalAmount(void);
    void setGoalAmount(int goalAm);
    int getCurrentAmount(void);
    void setCurrentAmount(int currAm);

    inline void checkGoal(int type, Agent* agent);

private:
    int goalAmount;
    int currentAmount = 0;
};

#endif /* defined(__simulplay__CollectionGoal__) */
