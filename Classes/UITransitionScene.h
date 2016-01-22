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
//  UITransitionScene.h
//  simulplay
//
//  Created by Guillem Laborda on 21/10/15.
//
//

#ifndef __simulplay__UITransitionScene__
#define __simulplay__UITransitionScene__

#include <cocos2d.h> 

using namespace cocos2d;

class UITransitionScene : public Layer {
public:
    static Scene* createScene();
    
    virtual bool init();
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    
    virtual void update(float delta);
    
    CREATE_FUNC(UITransitionScene);
    
private:
    bool stoppedAnimation = false;
    bool unlockedImage = false;
    
    void endActions(void);
    bool allActionsFinished(void);
    
    clock_t updateTimeToLoadScene = 0;
    bool loadset = false;
    bool _theEnd = false;
    
};

#endif /* defined(__simulplay__UITransitionScene__) */

