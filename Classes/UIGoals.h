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
#include <ui/CocosGUI.h>

using namespace ui;

class UIGoals : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuNextCallback(Ref* pSender);
    void menuBackCallback(Ref* pSender);
    void menuMapCallback(Ref* pSender);

    void menuStartCallback(Ref* pSender);
    void minusAttCallback(Ref* pSender);
    void plusAttCallback(Ref* pSender);

    virtual void update(float delta);

    CREATE_FUNC(UIGoals);

private:
    vector<string> keys;
    bool gameStarted = true;
    PageView* pages;

    void createContextLayout(Layout* layoutContext);
    void setLevelGoals(Layout* layout);
    void createUIAgent(Layout* layout);
};

#endif /* defined(__simulplay__UIGoals__) */
