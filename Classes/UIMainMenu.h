//
//  UIMainMenu.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIMainMenu__
#define __simulplay__UIMainMenu__

#include "cocos2d.h"

using namespace cocos2d;

class UIMainMenu : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuStartCallback(Ref* pSender);
    void menuCreditsCallback(Ref* pSender);
    void menuExitCallback(Ref* pSender);

    CREATE_FUNC(UIMainMenu);
};

#endif /* defined(__simulplay__UIMainMenu__) */
