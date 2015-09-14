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
//  EnvironmentAdaptation.h
//  simulplay
//
//  Created by Guillem Laborda on 24/10/14.
//
//

#ifndef __simulplay__EnvironmentAdaptation__
#define __simulplay__EnvironmentAdaptation__

#include "Act.h"

class EnvironmentAdaptation : public Act {

public:
    EnvironmentAdaptation(){};

    list<Agent*>::reverse_iterator execute(int type, Agent* agent);
};

#endif /* defined(__simulplay__EnvironmentAdaptation__) */
