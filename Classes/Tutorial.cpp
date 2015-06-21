
#include "Tutorial.h"
#include "GameLevel.h"
#include "../libs/pugixml/pugixml.hpp"
#include <string>

Tutorial::Tutorial()
{
}

Tutorial::~Tutorial()
{
}

bool Tutorial::loadTutorial()
{ 
    string dir = "data/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + "tutorial.xml");
    pugi::xml_document doc;
    doc.load_file((fullPath).c_str());

    int numLevel = GameLevel::getInstance()->getNumLevel();
    pugi::xml_node level = doc.child("level");
    bool found = false;
    while(level!= nullptr)
    {
        int num = stoi(level.attribute("num").value());
        if(numLevel==num)
        {
            std::cout << "level: " << numLevel << " found!" << std::endl;
            return true;
        }
        level = level.next_sibling("level");
    }
    // not available tutorial for level
    if(!found)
    {
        std::cout << "level: " << numLevel << " not found!" << std::endl;
        return false;
    }
}

