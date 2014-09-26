//
//  UIGoals.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIGoals__
#define __simulplay__UIGoals__

#include "UIGameplayMap.h"
#include "UIProgressMap.h"
#include "CocosGUI.h"

using namespace ui;

class UIGoals : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuNextCallback(Ref* pSender);
    void menuMapCallback(Ref* pSender);

    CREATE_FUNC(UIGoals);

private:
    bool gameStarted = true;

    void setLevelGoals(Layout* layout);
};

#endif /* defined(__simulplay__UIGoals__) */