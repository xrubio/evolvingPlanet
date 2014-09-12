//
//  UIPower.h
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#ifndef __simulplay__UIPower__
#define __simulplay__UIPower__

#include "GameLevel.h"
#include "cocos2d.h"

using namespace cocos2d;

class UIGameplayMap;

class UIPower : public Node {
public:
    UIPower() {};
    UIPower(UIPower const&) {};
    UIPower& operator=(UIPower const&);

    void setPosition(float x, float y);
    bool getClicked(void);
    void setClicked(bool b);
    Sprite* getIcon(void);

    virtual void onTouchesBegan(Point touchLocation) = 0;
    virtual void onTouchesMoved(Touch* touchLocation) = 0;
    virtual void onTouchesEnded(Point touchLocation) = 0;
    virtual void update() = 0;

protected:
    Sprite* icon;
    Label* cooldown;

    bool clicked = false;
};

#endif /* defined(__simulplay__UIPower__) */
