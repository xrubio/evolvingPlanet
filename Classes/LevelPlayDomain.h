//
//  LevelPlayDomain.h
//  simulplay
//
//  Created by Guillem Laborda on 16/07/14.
//
//

#ifndef __simulplay__LevelPlayDomain__
#define __simulplay__LevelPlayDomain__

#include <iostream>
#include <cocos2d.h>

class LevelPlayDomain
{
public:
    
    int agePlayer = 0;
    cocos2d::Point positionPlayer;
    
    bool finishedLevel = false;
    clock_t timeTaken;
    
    int velocityMultiplier = 1;
    int direction = 1;
    
    LevelPlayDomain();
    
    void playLevel(void);
    
};
#endif /* defined(__simulplay__LevelPlayDomain__) */
