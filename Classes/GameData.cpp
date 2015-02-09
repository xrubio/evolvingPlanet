//
//  GameData.cpp
//  simulplay
//
//  Created by Guillem Laborda on 14/08/14.
//
//

#include "GameData.h"

// Global static pointer used to ensure a single instance of the class.
GameData* GameData::gamedataInstance = NULL;

/** This function is called to create an instance of the class.
 Calling the constructor publicly is not allowed. The constructor
 is private and is only called by this Instance function.
 */
GameData* GameData::getInstance()
{
    if (!gamedataInstance) // Only allow one instance of class to be generated.
    {
        gamedataInstance = new GameData;
    }
    return gamedataInstance;
}

vector<int> GameData::getLevelsCompleted(void)
{
    return levelsCompleted;
}

void GameData::setLevelsCompleted(vector<int> lvlsCompleted)
{
    levelsCompleted.swap(lvlsCompleted);
}

int GameData::getLevelScore(int level)
{
    return levelsCompleted[level];
}

void GameData::setLevelScore(int level, int score)
{
    //COMPROVAR QUE LA PUNTUACIO SIGUI MILLOR ABANS DE SOBREESCRIURE
    if (levelsCompleted.size() == 0) {
        levelsCompleted.push_back(0);
    }
    if (level == levelsCompleted.size()) {
        levelsCompleted.push_back(score);
    } else {
        levelsCompleted.insert(levelsCompleted.begin() + level, score);
        levelsCompleted.erase(levelsCompleted.begin() + level + 1);
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(level).c_str(), score);
    cocos2d::UserDefault::getInstance()->setIntegerForKey("maxLevel", levelsCompleted.size());
    cocos2d::UserDefault::getInstance()->flush();
    //levelsCompleted.push_back(score);
}

bool GameData::getGameStarted(void)
{
    return gameStarted;
}

void GameData::setGameStarted(bool gameStrtd)
{
    gameStarted = gameStrtd;
}

string GameData::getLanguage(void)
{
    return language;
}

void GameData::setLanguage(string lang)
{
    language = lang;
}

bool GameData::getMusic(void)
{
    return music;
}

void GameData::setMusic(bool m)
{
    music = m;
}

bool GameData::getSFX(void)
{
    return sfx;
}

void GameData::setSFX(bool s)
{
    sfx = s;
}
