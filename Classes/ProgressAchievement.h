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
//  ProgressAchievement.h
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#ifndef __simulplay__ProgressAchievement__
#define __simulplay__ProgressAchievement__

#include "Achievement.h"

class ProgressAchievement : public Achievement
{
public:
    ProgressAchievement(string icon, string resource, string goalType, int level, bool completed = false, bool occult = false);
    
    // TODO implement this method
    bool checkAchievement(string typeAch, int level)
    {
        return false;
    }

private:
    int numGames;
};
#endif /* defined(__simulplay__ProgressAchievement__) */
