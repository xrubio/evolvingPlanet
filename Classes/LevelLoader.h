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

    void loadXmlFile(string filename);
    string getLevelFileMap(string filename);
    vector<string> getGoalTypes(string filename);

private:
    xml_document doc;
    xml_parse_result result;
};

#endif /* defined(__simulplay__LevelLoader__) */
