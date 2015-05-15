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
#include <ui/CocosGUI.h>

class UIProgressMap : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void menuLevelZoneCallback(Ref* pSender);
    void menuLevelCallback(Ref* pSender);
    void proceedLevelCallback(Ref* pSender);
    void restoreProgressMap(Ref* pSender);

    CREATE_FUNC(UIProgressMap);

private:
    cocos2d::ui::ScrollView* scrollView;
    Sprite* progressMap0;
};

#endif /* defined(__simulplay__UIProgressMap__) */
