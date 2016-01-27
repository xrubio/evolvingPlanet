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
//  UICredits.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UICredits__
#define __simulplay__UICredits__

#include "UIMainMenu.h"
#include "GameData.h"
#include <ui/CocosGUI.h>

using namespace ui;

class UICredits : public Layer {
public:
    static Scene* createScene();

    virtual bool init();
    
    void menuBackCallback(Ref* pSender);
    void menuArrowBackCallback(Ref* pSender);
    void menuArrowNextCallback(Ref* pSender);
    void openSimulLink( Ref * pSender );
    
    virtual void update(float delta);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);

    CREATE_FUNC(UICredits);
    
private:
    
    PageView *pages;
    
    MenuItemImage* arrowNext;
    MenuItemImage* arrowBack;
    
    MenuItemImage* hexagonButtonLevel0;
    MenuItemImage* hexagonButtonLevel1;
    MenuItemImage* hexagonButtonLevel2;
    MenuItemImage* hexagonButtonLevel3;
    
    void simulpast(Layout* layout);
    void murphysToastStudios(Layout* layout);
    void specialThanks1(Layout* layout);
    void specialThanks2(Layout* layout);

    // add a groupName label at position x/y of background
    void createGroup( const std::string & groupName, float x, float y, Sprite * background);
    // add a name label at position x/y of background
    void createName( const std::string & name, float x, float y, Sprite * background);
    // add an acknowledgement name at position x/y of background
    void createAcknowledgment( const std::string & name, float x, float y, Sprite * background);
    // load thanks names from xml
    void loadAcknowledgements();
    // acknowledgement names
    std::list<std::string> _names; 
};

#endif /* defined(__simulplay__UICredits__) */
