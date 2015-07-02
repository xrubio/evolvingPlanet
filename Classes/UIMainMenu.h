//
//  UIMainMenu.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIMainMenu__
#define __simulplay__UIMainMenu__

#include "cocos2d.h"

using namespace cocos2d;

class UIMainMenu : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);

    void menuContinueCallback(Ref* pSender);
    void menuStartCallback(Ref* pSender);
    void menuAchievementsCallback(Ref* pSender);
    void menuConfigurationCallback(Ref* pSender);
    void menuCreditsCallback(Ref* pSender);
    void menuExitCallback(Ref* pSender);
    void menuResetNoCallback(Ref* pSender);
    void menuResetYesCallback(Ref* pSender);

    CREATE_FUNC(UIMainMenu);

private:
    bool stoppedAnimation = false;

    void endActions(void);
    bool allActionsFinished(void);
    void createWarningWindow(void);
};

#endif /* defined(__simulplay__UIMainMenu__) */
