//
//  Collect.h
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#ifndef __simulplay__Collect__
#define __simulplay__Collect__

#include "Act.h"

class Collect : public Act {

public:
    Collect() {};
    Collect(Collect const&) {};
    Collect& operator=(Collect const&);

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__Collect__) */
