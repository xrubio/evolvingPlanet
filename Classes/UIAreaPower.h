//
//  UIAreaPower.h
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#ifndef __simulplay__UIAreaPower__
#define __simulplay__UIAreaPower__

#include "UIPower.h"

class UIAreaPower : public UIPower {
public:
    UIAreaPower(Power* p = 0);

    Sprite* getArea(void);

    void onTouchesBegan(Point touchLocation);
    void onTouchesMoved(Touch* touchLocation);
    void onTouchesEnded(Point touchLocation);
    void update(float delta);

private:
    float actionTime = 0.0;
    Sprite* area;
};

#endif /* defined(__simulplay__UIAreaPower__) */
