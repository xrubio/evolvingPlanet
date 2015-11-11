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
    void menuArrowBackCallback(Ref* pSender);
    void menuArrowNextCallback(Ref* pSender);
    void zoomImageInCallback(Ref* pSender);
    void zoomImageOutCallback(Ref* pSender);

    virtual void update(float delta);

    CREATE_FUNC(UIGoals);

private:
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

