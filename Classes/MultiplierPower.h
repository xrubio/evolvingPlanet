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
    MultiplierPower() {};
    MultiplierPower(string n, int c, int dur, int cl, int dl, string attr, string t, float mult);
    MultiplierPower(MultiplierPower const&) {};
    MultiplierPower& operator=(MultiplierPower const&);

    float getMultiplier(void);
    void setMultiplier(float m);

private:
    float multiplier;
};

#endif /* defined(__simulplay__MultiplierPower__) */
