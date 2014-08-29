//
//  LocalizedString.cpp
//  simulplay
//
//  Created by Guillem Laborda on 23/07/14.
//
//

#include "LocalizedString.h"
#include <string>
#include <iostream>

using namespace std;

CCString* LocalizedString::create(const char* key)
{
    const char* fileName = getSystemLang();
    CCString* str;

    string fname = fileName;
    fname = fname + ".plist";

    CCDictionary* language = CCDictionary::createWithContentsOfFile(fname.c_str());

    str = (CCString*)language->objectForKey(key);

    if (str == nullptr) {
        str = CCString::create(key);
    }

    return str;
}

const char* LocalizedString::getSystemLang()
{
    ccLanguageType curLanguage = CCApplication::sharedApplication()->getCurrentLanguage();

    switch (curLanguage) {
    case kLanguageEnglish:
        return "en";
        break;
    case kLanguageGerman:
        return "de";
        break;
    case kLanguageSpanish:
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