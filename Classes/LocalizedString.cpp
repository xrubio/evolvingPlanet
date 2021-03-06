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

string LocalizedString::create(const char* key, const std::string & dir)
{
    const char* fileName = GameData::getInstance()->getLanguage().c_str();
    if (strlen(fileName) == 0) {
        fileName = getSystemLang();
    }
    string str = "";

    string fname = fileName;
    string directory = "lang/";
    if (dir != "")
    {
        fname = dir + "_" + fname;
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    directory = "";
#endif

    fname = directory + fname + ".plist";
    cocos2d::ValueMap language = FileUtils::getInstance()->getValueMapFromFile(fname.c_str());

    if (language.find(key) != language.end())
    {
        str = language.at(key).asString();
    }
    else
    {
        CCLOG("LANG warning: key: %s not found in file: %s", key, fname.c_str());
        str = key;
    }
    
    return str;
}

const char* LocalizedString::getSystemLang()
{
    std::string currentLanguageCode = Application::getInstance()->getCurrentLanguageCode();
    // by now the only two languages
    if(currentLanguageCode=="ca")
    {
        return "ca";
    }
    if(currentLanguageCode=="es")
    {
        return "es";
    }
    return "en";

    /*
    //getCurrentLanguage -> cat no existeix!!!!
    LanguageType curLanguage = Application::getInstance()->getCurrentLanguage();

    switch (curLanguage) {
//    case LanguageType::CATALAN:
//        return "cat";
        break;
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
    */
}

LocalizedString::LocalizedString() {}

LocalizedString::~LocalizedString() {}
