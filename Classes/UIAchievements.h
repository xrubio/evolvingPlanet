//
//  UIAchievements.h
//  simulplay
//
//  Created by Guillem Laborda on 30/1/15.
//
//

#ifndef __simulplay__UIAchievements__
#define __simulplay__UIAchievements__

#include "UIMainMenu.h"
#include <ui/CocosGUI.h>

class UIAchievements : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);

    CREATE_FUNC(UIAchievements);
};

#endif /* defined(__simulplay__UIAchievements__) */
