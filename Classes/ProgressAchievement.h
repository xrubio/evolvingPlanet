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

class ProgressAchievement : public Achievement
{
public:
    ProgressAchievement(bool c = false, bool o = false);
    
    // TODO implement this method
    void checkAchievements(int n, int level)
    {
    }

private:
    int numGames;
};
#endif /* defined(__simulplay__ProgressAchievement__) */
