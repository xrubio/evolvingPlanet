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
#include <list>

class UIGameplayMap;

class Act
{
/** This class follows a Strategy pattern for implementing different actions with the same interface **/    

public:
    Act(){};

    virtual list<Agent*>::reverse_iterator execute(int type = 0, Agent* agent = nullptr) = 0;
};

#endif /* defined(__simulplay__Act__) */
