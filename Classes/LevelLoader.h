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
//  LevelLoader.h
//  simulplay
//
//  Created by Guillem Laborda on 15/09/14.
//
//

#ifndef __simulplay__LevelLoader__
#define __simulplay__LevelLoader__

#include "../libs/pugixml/pugixml.hpp"
#include <vector>

using namespace pugi;
using namespace std;


class LevelLoader {

public:
    LevelLoader(){};

    void loadXmlFile(const std::string & filename);
    string getLevelFileMap(const std::string & filename);
    vector<string> getGoalTypes(const std::string & filename);
    void loadMaxResources(const std::string & filename);

private:
    xml_document doc;
    xml_parse_result result;
};

#endif /* defined(__simulplay__LevelLoader__) */
