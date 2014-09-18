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
    UIAreaPower() {};
    UIAreaPower(Power* p);
    UIAreaPower(UIAreaPower const&) {};
    UIAreaPower& operator=(UIAreaPower const&);

    Sprite* getArea(void);

    void onTouchesBegan(Point touchLocation);
    void onTouchesMoved(Touch* touchLocation);
    void onTouchesEnded(Point touchLocation);
    void update();

private:
    Sprite* area;
};

#endif /* defined(__simulplay__UIAreaPower__) */
