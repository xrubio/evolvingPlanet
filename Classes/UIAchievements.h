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
    void showAchievement(Ref* pSender, ui::Widget::TouchEventType aType);
    void restoreAchievementsWindow(Ref* pSender);

    CREATE_FUNC(UIAchievements);

private:
    ui::Button* selectedAchievement;
};

#endif /* defined(__simulplay__UIAchievements__) */
