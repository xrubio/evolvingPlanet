//
//  LocalizedString.cpp
//  simulplay
//
//  Created by Guillem Laborda on 23/07/14.
//
//

#include "LocalizedString.h"
#include "GameData.h"
#include <string>

using namespace std;

string LocalizedString::create(const char* key)
{
    const char* fileName = GameData::getInstance()->getLanguage().c_str();
    if (strlen(fileName) == 0) {
        fileName = getSystemLang();
    }
    string str = "";

    string fname = fileName;
    string dir = "lang/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    fname = dir + fname + ".plist";

    //CCDictionary* language = CCDictionary::createWithContentsOfFile(fname.c_str());
    cocos2d::ValueMap language = FileUtils::getInstance()->getValueMapFromFile(fname.c_str());

    if (language.find(key) != language.end())
    {
        str = language.at(key).asString();
    }
    else
    {
        str = key;
    }
    
    return str;
}

const char* LocalizedString::getSystemLang()
{
    //getCurrentLanguage -> cat no existeix!!!!
    LanguageType curLanguage = Application::getInstance()->getCurrentLanguage();

    switch (curLanguage) {
    case LanguageType::ENGLISH:
        return "en";
        break;
    case LanguageType::SPANISH:
        return "es";
        break;
    default:
        return "en";
        break;
    }

    return "en";
}
LocalizedString::LocalizedString() {}

LocalizedString::~LocalizedString() {}
