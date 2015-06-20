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
    UIMultiplierPower(Power* p = 0);
    
    void onTouchesBegan(Point touchLocation);
    void onTouchesMoved(Touch* touchLocation);
    void onTouchesEnded(Point touchLocation);
    void update(float delta);

private:
    float actionTime = 0.0;
};
#endif /* defined(__simulplay__UIMultiplierPower__) */
