/*
 * Copyright (c) 2015
 * MURPHY'S TOAST GAMES
 *
 * This file is part of Evolving Planet.
 * Evolving Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Evolving Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Evolving Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    void menuExtrasCallback(Ref* pSender);
    void menuStoryCallback(Ref* pSender);
    void menuAchievementsCallback(Ref* pSender);
    void menuConfigurationCallback(Ref* pSender);
    void menuCreditsCallback(Ref* pSender);
    void menuExitCallback(Ref* pSender);
    void menuResetNoCallback(Ref* pSender);
    void menuResetYesCallback(Ref* pSender);
    
    CREATE_FUNC(UIMainMenu);

private:
    bool stoppedAnimation = false;
    bool loading = false;

    void endActions(void);
    bool allActionsFinished(void);
    void createWarningWindow(void);
        
    void openFacebook( Ref * pSender );
    void openTwitter( Ref * pSender );
    void openWeb( Ref * pSender );
};

#endif /* defined(__simulplay__UIMainMenu__) */
