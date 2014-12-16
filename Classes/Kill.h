//
//  Kill.h
//  simulplay
//
//  Created by Guillem Laborda on 10/11/14.
//
//

#ifndef __simulplay__Kill__
#define __simulplay__Kill__

#include "Act.h"

class Kill : public Act {

public:
    Kill() {};
    Kill(Kill const&) {};
    Kill& operator=(Kill const&);

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__Kill__) */
