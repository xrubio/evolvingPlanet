//
//  MainMenu.h
//  simulplay
//
//  Created by Guillem Laborda on 09/07/14.
//
//

#ifndef __simulplay__MainMenu__
#define __simulplay__MainMenu__

#include <iostream>
#include "cocos2d.h"
#include "DrawPoint.h"

class MainMenu : public cocos2d::Layer {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // a selector callback
    void menuMoveCallback(cocos2d::Ref* pSender);

    // a selector callback
    void menuPauseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};

#endif /* defined(__simulplay__MainMenu__) */
