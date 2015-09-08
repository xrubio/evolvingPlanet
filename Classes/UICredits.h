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

class UICredits : public Layer {
public:
    static Scene* createScene();

    virtual bool init();
    
    void menuBackCallback(Ref* pSender);

    CREATE_FUNC(UICredits);
    
};

#endif /* defined(__simulplay__UICredits__) */
