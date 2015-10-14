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
//  UIProgressMap.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIProgressMap__
#define __simulplay__UIProgressMap__

#include "UIMainMenu.h"
#include <ui/CocosGUI.h>

class UIProgressMap : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuBackCallback(Ref* pSender);
    void menuLevelZoneCallback(Ref* pSender);
    void menuLevelCallback(Ref* pSender);
    void proceedLevelCallback(Ref* pSender);
    void restoreProgressMap(Ref* pSender);
    void movePopupLevelCallback(Ref* pSender);
    void menuEraCallback(Ref* pSender);

    CREATE_FUNC(UIProgressMap);

private:
    cocos2d::ui::ScrollView* scrollView;
    Sprite* progressMap;
    Sprite* selectedBackground;
    int tagLevelSelected = 0;
    
    void setEpisode1(void);
    void setEpisode2(void);

};

#endif /* defined(__simulplay__UIProgressMap__) */
