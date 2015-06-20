//
//  Timing.h
//  simulplay
//
//  Created by Guillem Laborda on 09/01/15.
//
//

#ifndef __simulplay__Timing__
#define __simulplay__Timing__

#include "GameLevel.h"

class Timing {
public:
    static Timing* getInstance();

    void start(void);

    bool act = false;

private:
    static Timing* timingInstance;

    Timing(){};
};

#endif /* defined(__simulplay__Timing__) */
