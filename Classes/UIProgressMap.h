//
//  UIProgressMap.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIProgressMap__
#define __simulplay__UIProgressMap__

#include "UIMainMenu.h"

class UIProgressMap : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void menuLevel1Callback(Ref* pSender);

    CREATE_FUNC(UIProgressMap);
};

#endif /* defined(__simulplay__UIProgressMap__) */
