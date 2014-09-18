//
//  AreaPower.h
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#ifndef __simulplay__AreaPower__
#define __simulplay__AreaPower__

#include "Power.h"

class AreaPower : public Power {

public:
    AreaPower() {};
    AreaPower(string n, int c, int dur, int cl, int dl, string attr, string t, float rad);
    AreaPower(AreaPower const&) {};
    AreaPower& operator=(AreaPower const&);

    float getRadius(void);
    void setRadius(float r);

private:
    float radius;
};

#endif /* defined(__simulplay__AreaPower__) */
