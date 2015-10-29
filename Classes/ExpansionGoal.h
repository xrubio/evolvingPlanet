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
//  ExpansionGoal.h
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#ifndef __simulplay__ExpansionGoal__
#define __simulplay__ExpansionGoal__

#include "Goal.h"

class ExpansionGoal : public Goal {

public:
    ExpansionGoal(int agType, int min, int max, int average, int des2, int des3, int color = 0);

    int getColorZone(void);
    void setColorZone(int color);
    int getMinDistanceToGoal(void);
    void setMinDistanceToGoal(int dis);
    Position getCenterArea(void);
    void setCenterArea(int x, int y);

    // returns true if agent of type completes the goal this step
    bool checkGoal(int type, Agent* agent);
    GoalType getGoalType() const
    {
        return Expansion;
    }

private:
    int colorZone;
    int minDistanceToGoal;
    Position centerArea;
};

#endif /* defined(__simulplay__ExpansionGoal__) */
