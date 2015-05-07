//
//  LevelAchievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__LevelAchievement__
#define __simulplay__LevelAchievement__

#include "Achievement.h"

class LevelAchievement : public Achievement {

public:
    LevelAchievement(){};
    LevelAchievement(bool c, bool o);
    LevelAchievement(LevelAchievement const&){};
    LevelAchievement& operator=(LevelAchievement const&);

    void checkAchievements(int n, int level);
};

#endif /* defined(__simulplay__LevelAchievement__) */
