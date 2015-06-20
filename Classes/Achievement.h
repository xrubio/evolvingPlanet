//
//  Achievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__Achievement__
#define __simulplay__Achievement__

class Achievement
{
    bool completed;
    bool occult;
public:
    Achievement( bool c=false, bool o=false);
    virtual ~Achievement(){}
    bool getCompleted() const;
    bool getOccult() const;

    virtual void checkAchievements(int n, int level = -1) = 0;
};

#endif /* defined(__simulplay__Achievement__) */
