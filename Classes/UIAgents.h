//
//  UIAgents.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIAgents__
#define __simulplay__UIAgents__

#include "UIGameplayMap.h"
#include "UIProgressMap.h"
#include "GameLevel.h"

class UIAgents : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuStartCallback(Ref* pSender);
    void menuMapCallback(Ref* pSender);
    void minusAttCallback(Ref* pSender);
    void plusAttCallback(Ref* pSender);

    CREATE_FUNC(UIAgents);

private:
    vector<string> keys;
};

#endif /* defined(__simulplay__UIAgents__) */
