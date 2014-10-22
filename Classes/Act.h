//
//  Act.h
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#ifndef __simulplay__Act__
#define __simulplay__Act__

#include "Agent.h"

class UIGameplayMap;

class Act {

public:
    Act() {};
    Act(Act const&) {};
    Act& operator=(Act const&);

    virtual bool execute(int type = 0, int indexAgent = 0) = 0;
};

#endif /* defined(__simulplay__Act__) */
