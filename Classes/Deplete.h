//
//  Deplete.h
//  simulplay
//
//  Created by Guillem Laborda on 23/10/14.
//
//

#ifndef __simulplay__Deplete__
#define __simulplay__Deplete__

#include "Act.h"

class Deplete : public Act {

public:
    Deplete() {};
    Deplete(Deplete const&) {};
    Deplete& operator=(Deplete const&);

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__Deplete__) */
