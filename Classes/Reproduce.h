//
//  Reproduce.h
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#ifndef __simulplay__Reproduce__
#define __simulplay__Reproduce__

#include "Act.h"

class Reproduce : public Act {

public:
    Reproduce() {};
    Reproduce(Reproduce const&) {};
    Reproduce& operator=(Reproduce const&);

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__Reproduce__) */
