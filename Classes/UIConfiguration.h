//
//  UIConfiguration.h
//  simulplay
//
//  Created by Guillem Laborda on 03/09/14.
//
//

#ifndef __simulplay__UIConfiguration__
#define __simulplay__UIConfiguration__

#include "UIMainMenu.h"

class UIConfiguration : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void menuResetCallback(Ref* pSender);
    void menuResetNoCallback(Ref* pSender);
    void menuResetYesCallback(Ref* pSender);
    void catFlagCallback(Ref* pSender);
    void esFlagCallback(Ref* pSender);
    void enFlagCallback(Ref* pSender);
    void musicOnCallback(Ref* pSender);
    void musicOffCallback(Ref* pSender);
    void sfxOnCallback(Ref* pSender);
    void sfxOffCallback(Ref* pSender);

    CREATE_FUNC(UIConfiguration);

private:
    void createWarningWindow(Sprite* popupBackground);
};

#endif /* defined(__simulplay__UIConfiguration__) */
