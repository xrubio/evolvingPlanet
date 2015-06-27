//
//  LocalizedString.h
//  simulplay
//
//  Created by Guillem Laborda on 23/07/14.
//
//

#ifndef __simulplay__LocalizedString__
#define __simulplay__LocalizedString__

#include "cocos2d.h"

using namespace cocos2d;

class LocalizedString {
public:
    static std::string create(const char* key);

    static const char* getSystemLang();

    LocalizedString();
    virtual ~LocalizedString();
};

#endif /* defined(__simulplay__LocalizedString__) */
