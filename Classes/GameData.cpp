//
//  GameData.cpp
//  simulplay
//
//  Created by Guillem Laborda on 14/08/14.
//
//

#include "GameData.h"
#include <cocos2d.h>
#include "../libs/pugixml/pugixml.hpp"

using namespace pugi;

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

vector<Achievement*> GameData::getAchievements(void)
{
    return achievements;
}

void GameData::setAchievements(vector<Achievement*> ach)
{
    achievements.swap(ach);
}

int GameData::getLevelScore(int level)
{
    return levelsCompleted[level];
}

void GameData::setLevelScore(int level, int score)
{
    if (levelsCompleted.size() == 0) {
        levelsCompleted.push_back(0);
    }
    if (level == levelsCompleted.size()) {
        levelsCompleted.push_back(score);
    }
    else {
        if (score > levelsCompleted[level]) {
            levelsCompleted.insert(levelsCompleted.begin() + level, score);
            levelsCompleted.erase(levelsCompleted.begin() + level + 1);
        }
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(level).c_str(), score);
    cocos2d::UserDefault::getInstance()->setIntegerForKey("maxLevel", (int)levelsCompleted.size());
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

float GameData::getRaConversion(void)
{
    return raConversion;
}

void GameData::setRaConversion(float r)
{
    raConversion = r;
}

float GameData::getRaWConversion(void)
{
    return raWConversion;
}

void GameData::setRaWConversion(float r)
{
    raWConversion = r;
}

float GameData::getRaHConversion(void)
{
    return raHConversion;
}

void GameData::setRaHConversion(float r)
{
    raHConversion = r;
}

float GameData::getHeightProportionalResolution(void)
{
    return heightProportionalResolution;
}

void GameData::setHeightProportionalResolution(float h)
{
    heightProportionalResolution = h;
}

float GameData::getRowDrawAgentPrecalc(void)
{
    return rowDrawAgentPrecalc;
}

void GameData::setRowDrawAgentPrecalc(float r)
{
    rowDrawAgentPrecalc = r;
}

float GameData::getColumnOffsetDrawAgentPrecalc(void)
{
    return columnOffsetDrawAgentPrecalc;
}

void GameData::setColumnOffsetDrawAgentPrecalc(float c)
{
    columnOffsetDrawAgentPrecalc = c;
}

float GameData::getColumnDrawAgentPrecalc(void)
{
    return columnDrawAgentPrecalc;
}

void GameData::setColumnDrawAgentPrecalc(float c)
{
    columnDrawAgentPrecalc = c;
}

bool GameData::getResetGameClicked(void)
{
    return resetGameClicked;
}

void GameData::setResetGameClicked(bool r)
{
    resetGameClicked = r;
}

void GameData::loadAchievements(void)
{
    xml_document doc;
    xml_parse_result result;
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("achievements.xml");
    result = doc.load_file((fullPath).c_str());

    //ACHIEVEMENTS
    xml_node achs = doc.child("ACHIEVEMENT");
    while (achs != nullptr) {
        if (strncmp(achs.attribute("TYPE").value(), "LEVEL", 5) == 0) {
        }
        else if (strncmp(achs.attribute("TYPE").value(), "PROGRESS", 8) == 0) {
        }
        achs = achs.next_sibling("ACHIEVEMENT");
    }
}

void GameData::resetGameProgress(void)
{
    for (int i = 0; i < levelsCompleted.size(); i++) {
        cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(i).c_str(), 0);
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey("maxLevel", 0);
    cocos2d::UserDefault::getInstance()->flush();
    levelsCompleted.clear();
}
