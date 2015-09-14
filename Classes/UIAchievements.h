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
//  UIAchievements.h
//  simulplay
//
//  Created by Guillem Laborda on 30/1/15.
//
//

#ifndef __simulplay__UIAchievements__
#define __simulplay__UIAchievements__

#include "UIMainMenu.h"
#include <ui/CocosGUI.h>

class UIAchievements : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void showAchievement(Ref* pSender, ui::Widget::TouchEventType aType);
    void restoreAchievementsWindow(Ref* pSender);

    CREATE_FUNC(UIAchievements);

private:
    ui::Button* selectedAchievement;
};

#endif /* defined(__simulplay__UIAchievements__) */
