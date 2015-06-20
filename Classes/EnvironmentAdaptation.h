//
//  EnvironmentAdaptation.h
//  simulplay
//
//  Created by Guillem Laborda on 24/10/14.
//
//

#ifndef __simulplay__EnvironmentAdaptation__
#define __simulplay__EnvironmentAdaptation__

#include "Act.h"

class EnvironmentAdaptation : public Act {

public:
    EnvironmentAdaptation(){};

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__EnvironmentAdaptation__) */
