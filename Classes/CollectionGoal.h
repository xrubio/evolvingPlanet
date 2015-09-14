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
//  CollectionGoal.h
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#ifndef __simulplay__CollectionGoal__
#define __simulplay__CollectionGoal__

#include "Goal.h"

class CollectionGoal : public Goal {

public:
    CollectionGoal(int agType, int min, int max, int average, int des2, int des3, int goalAm);

    int getGoalAmount(void);
    void setGoalAmount(int goalAm);
    int getCurrentAmount(void);
    void setCurrentAmount(int currAm);

    void checkGoal(int type, Agent* agent);
    GoalType getGoalType() const
    {
        return Collection;
    }
private:
    int goalAmount;
    int currentAmount;
};

#endif /* defined(__simulplay__CollectionGoal__) */

