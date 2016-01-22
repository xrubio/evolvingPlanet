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
//  UIEndScene.h
//  simulplay
//
//  Created by Guillem Laborda on 21/10/15.
//
//

#ifndef __simulplay__UIEndScene__
#define __simulplay__UIEndScene__

#include <cocos2d.h>
#include <string>
#include <list>

using namespace cocos2d;

class UIEndScene : public Layer
{

    void addJob(const std::string & jobName, float offset);
    void addName(const std::string & name, float offset);
    void addAcknowledgement( const std::string & name, float offset);

    // load thanks names from xml
    void loadAcknowledgements();
    // acknowledgement names
    std::list<std::string> _names; 

public:
    static Scene* createScene();
    
    virtual bool init();
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    
    CREATE_FUNC(UIEndScene);
};

#endif /* defined(__simulplay__UIEndScene__) */
