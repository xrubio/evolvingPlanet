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
//  LevelAchievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__LevelAchievement__
#define __simulplay__LevelAchievement__

#include "Achievement.h"
#include "GameData.h"

class LevelAchievement : public Achievement {

public:
    LevelAchievement(string icon, string resource, string goalType, int level, bool completed = false, bool occult = false);
    
    // TODO implement this method
    bool checkAchievement(string typeAch, int level)
    {
        if (GameData::getInstance()->getFirstTimeLevelCompleted() == level)
        {
            if (typeAch == "COMPLETED")
            {
                if (GameData::getInstance()->getLevelScore(level) > 0)
                {
                    _completed = true;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (typeAch == "PERFECT")
            {
                if (GameData::getInstance()->getLevelScore(level) == 3)
                {
                    _completed = true;
                    return true;
                }
            }
        }
        return false;
    }
};

#endif /* defined(__simulplay__LevelAchievement__) */
