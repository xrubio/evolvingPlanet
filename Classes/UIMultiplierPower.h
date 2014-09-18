//
//  UIMultiplierPower.h
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#ifndef __simulplay__UIMultiplierPower__
#define __simulplay__UIMultiplierPower__

#include "UIPower.h"

class UIMultiplierPower : public UIPower {
public:
    UIMultiplierPower() {};
    UIMultiplierPower(Power* p);
    UIMultiplierPower(UIMultiplierPower const&) {};
    UIMultiplierPower& operator=(UIMultiplierPower const&);

    void onTouchesBegan(Point touchLocation);
    void onTouchesMoved(Touch* touchLocation);
    void onTouchesEnded(Point touchLocation);
    void update();

private:
};
#endif /* defined(__simulplay__UIMultiplierPower__) */
