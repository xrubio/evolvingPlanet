//
//  Achievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__Achievement__
#define __simulplay__Achievement__

class Achievement {

public:
    Achievement(){};
    Achievement(Achievement const&){};
    Achievement& operator=(Achievement const&);

    bool getCompleted(void);
    void setCompleted(bool b);
    bool getOccult(void);
    void setOccult(bool b);

    virtual void checkAchievements(int n, int level = -1) = 0;

protected:
    bool completed = false;
    bool occult = false;
};

#endif /* defined(__simulplay__Achievement__) */
