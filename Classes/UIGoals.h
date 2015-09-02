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
    virtual ~UIGoals();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void menuStartCallback(Ref* pSender);
    void minusAttCallback(Ref* pSender);
    void plusAttCallback(Ref* pSender);
    void menuArrowBackCallback(Ref* pSender);
    void menuArrowNextCallback(Ref* pSender);
    void zoomImageInCallback(Ref* pSender);
    void zoomImageOutCallback(Ref* pSender);

    virtual void update(float delta);

    CREATE_FUNC(UIGoals);

private:
    vector<string> keys;
    bool gameStarted = true;
    PageView* pages;
    MenuItemImage* arrowNext;
    MenuItemImage* arrowBack;
    Label* evolutionPointsNumberIcon;
    
    MenuItemImage* hexagonButtonLevel0;
    MenuItemImage* hexagonButtonLevel1;
    MenuItemImage* hexagonButtonLevel2;

    void setLevelGoals(Layout* layout);
    void createUIAgent(Layout* layout);
};

#endif /* defined(__simulplay__UIGoals__) */
