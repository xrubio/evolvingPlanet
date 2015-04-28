//
//  ProgressAchievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__ProgressAchievement__
#define __simulplay__ProgressAchievement__

#include "Achievement.h"

class ProgressAchievement : public Achievement {

public:
    ProgressAchievement() {};
    ProgressAchievement(bool c, bool o);
    ProgressAchievement(ProgressAchievement const&) {};
    ProgressAchievement& operator=(ProgressAchievement const&);

    void checkAchievements(int n);

private:
    int numGames;
};
#endif /* defined(__simulplay__ProgressAchievement__) */