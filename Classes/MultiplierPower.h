//
//  MultiplierPower.h
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#ifndef __simulplay__MultiplierPower__
#define __simulplay__MultiplierPower__

#include "Power.h"

class MultiplierPower : public Power {

public:
    MultiplierPower(const std::string & name, const PowerId & id, float c, float dur, float cl, float dl, string attr, string t, float mult);
    
    float getMultiplier(void);
    void setMultiplier(float m);

private:
    float multiplier;
};

#endif /* defined(__simulplay__MultiplierPower__) */
