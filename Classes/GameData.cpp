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
//  GameData.cpp
//  simulplay
//
//  Created by Guillem Laborda on 14/08/14.
//
//

#include "GameData.h"
#include "LevelAchievement.h"
#include "ProgressAchievement.h"
#include <cocos2d.h>
#include "../libs/pugixml/pugixml.hpp"

using namespace pugi;
using namespace cocos2d;

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

vector< vector<Achievement*> > GameData::getAchievements(void)
{
    return achievements;
}

vector<Achievement*> GameData::getAchievements(int i)
{
    return achievements[i];
}

void GameData::setAchievements(int i, vector<Achievement*> ach)
{
    achievements[i].swap(ach);
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
    //FIRST TIME LEVEL COMPLETED
    if (level == levelsCompleted.size()) {
        levelsCompleted.push_back(score);
        GameData::getInstance()->setFirstTimeLevelCompleted(level);
    }
    else if (level > levelsCompleted.size())
    {
        while (level > levelsCompleted.size())
        {
            levelsCompleted.push_back(0);
        }
        GameData::getInstance()->setFirstTimeLevelCompleted(level);
        levelsCompleted.push_back(score);
    }
    else
    {
        if (score > levelsCompleted[level]) {
            levelsCompleted[level] = score;
        }
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(level).c_str(), score);
    cocos2d::UserDefault::getInstance()->setIntegerForKey("maxLevel", (int)levelsCompleted.size());
    cocos2d::UserDefault::getInstance()->flush();
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

int GameData::getFirstTimeLevelCompleted(void)
{
    return _firstTimeLevelCompleted;
}

void GameData::setFirstTimeLevelCompleted(int l)
{
    _firstTimeLevelCompleted = l;
}

void GameData::loadAchievements(void)
{
    string dir = "data/";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif
    
    xml_document doc;
    xml_parse_result result;
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + "achievements.xml");
    result = doc.load_file((fullPath).c_str());
    
    //initialize achievements vec of vecs
    if (achievements.size() == 0)
    {
        for (int i = 0; i < NUM_LEVELS + 1; i++)
        {
            vector<Achievement*> achs;
            achievements.push_back(achs);
        }
    }

    //ACHIEVEMENTS
    xml_node achs = doc.child("ACHIEVEMENT");
    while (achs != nullptr) {
        xml_node logic = achs.child("COMPLETION_LOGIC");
        xml_node signifier = achs.child("SIGNIFIER");
        string key;
        
        //LEVEL ACHIEVEMENT
        if (strncmp(achs.attribute("TYPE").value(), "LEVEL", 5) == 0) {
            int level = atoi(logic.child_value("LEVEL"));
            string goalType = logic.child("GOAL").attribute("TYPE").value();
            //SIGNIFIER
            string icon = signifier.child_value("ICON");
            string resource = string(signifier.child("RESOURCE").attribute("TYPE").value()) + "/" + string(signifier.child_value("RESOURCE"));
         
            key = to_string(level) + "_" + goalType;
            
            auto ach = new LevelAchievement(icon, resource, goalType, level, UserDefault::getInstance()->getBoolForKey(key.c_str()), false);
            achievements[level].push_back(ach);
        }
        //PROGRESS ACHIEVEMENT
        else if (strncmp(achs.attribute("TYPE").value(), "PROGRESS", 8) == 0) {
            int level = 0;
            string goalType = logic.child("GOAL").attribute("TYPE").value();
            //SIGNIFIER
            string icon = signifier.child_value("ICON");
            string resource = string(signifier.child("RESOURCE").attribute("TYPE").value()) + "/" + string(signifier.child_value("RESOURCE"));
                        
            key = to_string(level) + "_" + goalType;
                        
            auto ach = new ProgressAchievement(icon, resource, goalType, level, UserDefault::getInstance()->getBoolForKey(key.c_str()), false);
            achievements[0].push_back(ach);
        }
        
        achs = achs.next_sibling("ACHIEVEMENT");
    }
}

void GameData::resetGameProgress(void)
{
    for (int i = 0; i < levelsCompleted.size(); i++) {
        cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(i).c_str(), 0);
        setTutorial(i+1, true);
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey("maxLevel", 1);
    cocos2d::UserDefault::getInstance()->flush();
    levelsCompleted.clear();
    levelsCompleted.push_back(0);
    if (GameData::getInstance()->getAchievements().size() <= 0)
    {
        GameData::getInstance()->loadAchievements();
    }
    for (int i = 0; i < achievements.size(); i++)
    {
        for (int j = 0; j < achievements[i].size(); j++)
        {
            cocos2d::UserDefault::getInstance()->setBoolForKey((to_string(achievements[i][j]->getLevel())+"_"+achievements[i][j]->getGoalType()).c_str(), false);
        }
    }
    achievements.clear();
    _firstTimeLevelCompleted = 0;
}

bool GameData::launchTutorial(int level) const
{
    std::stringstream key;
    key << "tutorial_"+std::to_string(level);
    CCLOG("launch tutorial: %s", key.str().c_str());
    return cocos2d::UserDefault::getInstance()->getBoolForKey(key.str().c_str(), true);
}

void GameData::setTutorial(int level, bool launch)
{
    std::stringstream key;
    key << "tutorial_"+std::to_string(level);
    CCLOG("set tutorial: %s", key.str().c_str());
    cocos2d::UserDefault::getInstance()->setBoolForKey(key.str().c_str(),launch);
}

