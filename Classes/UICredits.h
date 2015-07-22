//
//  UICredits.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UICredits__
#define __simulplay__UICredits__

#include "UIMainMenu.h"
#include "WaveNode.h"

class UICredits : public Layer {
public:
    static Scene* createScene();

    virtual bool init();
    
    virtual void update(float delta);

    void menuBackCallback(Ref* pSender);

    CREATE_FUNC(UICredits);
    
    WaveNode *waveNode;
};

#endif /* defined(__simulplay__UICredits__) */
