//
//  Die.h
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#ifndef __simulplay__Die__
#define __simulplay__Die__

#include "Act.h"

class Die : public Act {

public:
    Die(){};
    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__Die__) */
