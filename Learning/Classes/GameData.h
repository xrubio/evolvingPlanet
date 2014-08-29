//
//  GameData.h
//  simulplay
//
//  Created by Guillem Laborda on 21/07/14.
//
//

#ifndef __simulplay__GameData__
#define __simulplay__GameData__

#include <iostream>

class GameData {
public:
    static GameData* getInstance();
    int getAgePlayer();
    void setAgePlayer(int age);

private:
    GameData() {}; // Private so that it can  not be called
    GameData(GameData const&) {}; // copy constructor is private
    GameData& operator=(GameData const&) {}; // assignment operator is private
    static GameData* m_pInstance;
    int agePlayer;
};

#endif /* defined(__simulplay__GameData__) */
