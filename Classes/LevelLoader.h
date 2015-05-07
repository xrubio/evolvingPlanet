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

using namespace pugi;
using namespace std;

class LevelLoader {

public:
    LevelLoader(){};
    LevelLoader(LevelLoader const&){};
    LevelLoader& operator=(LevelLoader const&);

    void loadXmlFile(string filename);
    string getLevelFileMap(string filename);
    vector<string> getGoalTypes(string filename);

private:
    xml_document doc;
    xml_parse_result result;
};

#endif /* defined(__simulplay__LevelLoader__) */
