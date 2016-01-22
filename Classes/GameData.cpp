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

vector<int> GameData::getLevelsFailedForHint(void)
{
    return _levelsFailedForHint;
}

void GameData::setLevelsFailedForHint(vector<int> lvlsFailedForHint)
{
    _levelsFailedForHint.swap(lvlsFailedForHint);
}

void GameData::setLevelFailedForHint(int lvl)
{
    _levelsFailedForHint[lvl]++;
    cocos2d::UserDefault::getInstance()->setIntegerForKey(("failed"+to_string(lvl)).c_str(), _levelsFailedForHint[lvl]);
    cocos2d::UserDefault::getInstance()->flush();
}

vector< vector<Achievement*> > GameData::getAchievements(void)
{
    return achievements;
}

vector<Achievement*> GameData::getAchievements(int i)
{
    return achievements.at(i);
}

void GameData::setAchievements(int i, vector<Achievement*> ach)
{
    achievements.at(i).swap(ach);
}

int GameData::getLevelScore(int level)
{
    return levelsCompleted.at(level);
}

void GameData::setLevelScore(int level, int score)
{
    //FIRST TIME LEVEL COMPLETED
    if (levelsCompleted.at(level) == 0) {
        levelsCompleted[level] = score;
        GameData::getInstance()->setFirstTimeLevelCompleted(level);
    }
    else
    {
        if (score > levelsCompleted.at(level)) {
            levelsCompleted[level] = score;
        }
    }
    cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(level).c_str(), levelsCompleted[level]);
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

const std::string & GameData::getLanguage(void)
{
    return language;
}

void GameData::setLanguage(const string & lang)
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

int GameData::getCurrentEra(void)
{
    return _currentEra;
}

void GameData::setCurrentEra(int i)
{
    _currentEra = i;
}

int GameData::getCurrentLevel(void)
{
    return _currentLevel;
}

void GameData::setCurrentLevel(int i)
{
    _currentLevel = i;
}

float GameData::getResourcesHeight(void)
{
    return resourcesHeight;
}

void GameData::setResourcesHeight(int r)
{
    resourcesHeight = r;
}

float GameData::getResourcesWidth(void)
{
    return resourcesWidth;
}

void GameData::setResourcesWidth(int r)
{
    resourcesWidth = r;
}

float GameData::getResourcesMargin(void)
{
    return resourcesMargin;
}

void GameData::setResourcesMargin(int r)
{
    resourcesMargin = r;
}

cocos2d::Color3B GameData::getPlayerColor(void)
{
    return _playerColor;
}

string GameData::getPlayerColorString(void)
{
    if (_playerColor.r == 63)
    {
        return "lightblue";
    }
    else if (_playerColor.r == 84)
    {
        return "darkblue";
    }
    else if (_playerColor.r == 255)
    {
        return "white";
    }
    else if (_playerColor.r == 165)
    {
        return "purple";
    }
    
    return "lightblue";
}

void GameData::setPlayerColor(string c)
{
    if (c == "lightblue")
    {
        _playerColor = Color3B(63, 239, 250);
    }
    else if (c == "darkblue")
    {
        _playerColor = Color3B(84, 169, 247);
    }
    else if (c == "white")
    {
        _playerColor = Color3B(255, 255, 255);
    }
    else if (c == "purple")
    {
        _playerColor = Color3B(165, 59, 175);
    }
}

bool GameData::getAnimationSpace(void)
{
    return _animationSpace;
}

void GameData::setAnimationSpace(bool b)
{
    _animationSpace = b;
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ssize_t tmpSize;
    const char* xmlData = (const char *)FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &tmpSize);
    result = doc.load(xmlData);
#else
    result = doc.load_file(fullPath.c_str());
#endif

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
            string textImage = "";
            if (string(signifier.child("RESOURCE").attribute("TXT").value()) != "")
            {
                textImage = string(signifier.child("RESOURCE").attribute("TXT").value())+"-";
            }
            
            //example: IMG-ACH_LVL1_3-misc/firstLogo.jpg
            string resource = string(signifier.child("RESOURCE").attribute("TYPE").value()) + "-" + textImage + string(signifier.child_value("RESOURCE"));
         
            key = to_string(level) + "_" + goalType;
            
            auto ach = new LevelAchievement(icon, resource, goalType, level, UserDefault::getInstance()->getBoolForKey(key.c_str()), false);
            if (goalType == "EPsLeft" or goalType == "Discovery")
            {
                ach->setVariable(stoi(logic.child_value("GOAL")));
            }
            achievements.at(level).push_back(ach);
        }
        //PROGRESS ACHIEVEMENT
        else if (strncmp(achs.attribute("TYPE").value(), "PROGRESS", 8) == 0) {
            int level = 0;
            string goalType = logic.child("GOAL").attribute("TYPE").value();
            //SIGNIFIER
            string icon = signifier.child_value("ICON");
            string textImage = "";
            if (string(signifier.child("RESOURCE").attribute("TXT").value()) != "")
            {
                textImage = string(signifier.child("RESOURCE").attribute("TXT").value())+"-";
            }
            string resource = string(signifier.child("RESOURCE").attribute("TYPE").value()) + "-" + textImage + string(signifier.child_value("RESOURCE"));
            key = to_string(level) + "_" + goalType;
                        
            auto ach = new ProgressAchievement(icon, resource, goalType, level, UserDefault::getInstance()->getBoolForKey(key.c_str()), false);
            achievements.at(0).push_back(ach);
        }
        
        achs = achs.next_sibling("ACHIEVEMENT");
    }
}

void GameData::resetGameProgress(void)
{
    UserDefault::getInstance()->setBoolForKey("firsttimeplaying", false);
    //RESET LEVELS
    for (int i = 0; i < levelsCompleted.size(); i++) {
        cocos2d::UserDefault::getInstance()->setIntegerForKey(to_string(i).c_str(), 0);
        levelsCompleted[i] = 0;
        cocos2d::UserDefault::getInstance()->setIntegerForKey(("failed"+to_string(i)).c_str(), 0);
        _levelsFailedForHint[i] = 0;
        setTutorial(i+1, true);
    }
    
    for (int i = 0; i < achievements.size(); i++)
    {
        for (int j = 0; j < achievements.at(i).size(); j++)
        {
            cocos2d::UserDefault::getInstance()->setBoolForKey((to_string(achievements.at(i).at(j)->getLevel())+"_"+achievements.at(i).at(j)->getGoalType()).c_str(), false);
        }
    }
    achievements.clear();
    if (achievements.size() <= 0)
    {
        loadAchievements();
    }
    _firstTimeLevelCompleted = 0;
    _currentEra = 0;
    _currentLevel = 1;
    
    cocos2d::UserDefault::getInstance()->flush();
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

