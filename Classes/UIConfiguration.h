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
    void catFlagCallback(Ref* pSender);
    void esFlagCallback(Ref* pSender);
    void enFlagCallback(Ref* pSender);
    void colorCallback(Ref* pSender);
    void musicOnCallback(Ref* pSender);
    void musicOffCallback(Ref* pSender);
    void sfxOnCallback(Ref* pSender);
    void sfxOffCallback(Ref* pSender);

    CREATE_FUNC(UIConfiguration);

};

#endif /* defined(__simulplay__UIConfiguration__) */
