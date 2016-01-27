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
//  UIStoryGallery.h
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 30/10/15.
//
//

#ifndef __simulplay__UIStoryGallery__
#define __simulplay__UIStoryGallery__

#include "UIGameplayMap.h"
#include "UIProgressMap.h"
#include <ui/CocosGUI.h>

using namespace ui;

class UIStoryGallery : public Layer {
public:
    static Scene* createScene();
    
    virtual bool init();
    
    void menuBackCallback(Ref* pSender);
    void menuArrowBackCallback(Ref* pSender);
    void menuArrowNextCallback(Ref* pSender);
    
    virtual void update(float delta);
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    
    CREATE_FUNC(UIStoryGallery);
    
private:
    PageView* pages;
    MenuItemImage* arrowNext;
    MenuItemImage* arrowBack;
    
};

#endif /* defined(__simulplay__UIStoryGallery__) */