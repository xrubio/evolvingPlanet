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
//  GameData.h
//  simulplay
//
//  Created by Guillem Laborda on 14/08/14.
//
//

#ifndef __simulplay__GameData__
#define __simulplay__GameData__

#include <vector>
#include <string>
#include "Achievement.h"

using namespace std;

enum toScene
{
    MainMenu = 0,
    Story = 1
};

#define NUM_LEVELS 20

class GameData {
public:
    static GameData* getInstance();

    vector<int> getLevelsCompleted(void);
    void setLevelsCompleted(vector<int> lvlsCompleted);
    vector< vector<Achievement*> > getAchievements(void);
    vector<Achievement*> getAchievements(int i);
    void setAchievements(int i, vector<Achievement*> ach);
    int getLevelScore(int level);
    void setLevelScore(int level, int score);
    bool getGameStarted(void);
    void setGameStarted(bool gameStrtd);
    string getLanguage(void);
    void setLanguage(string lang);
    bool getMusic(void);
    void setMusic(bool m);
    bool getSFX(void);
    void setSFX(bool s);
    float getRaConversion(void);
    void setRaConversion(float r);
    float getRaWConversion(void);
    void setRaWConversion(float r);
    float getRaHConversion(void);
    void setRaHConversion(float r);
    float getHeightProportionalResolution(void);
    void setHeightProportionalResolution(float h);
    float getRowDrawAgentPrecalc(void);
    void setRowDrawAgentPrecalc(float r);
    float getColumnOffsetDrawAgentPrecalc(void);
    void setColumnOffsetDrawAgentPrecalc(float c);
    float getColumnDrawAgentPrecalc(void);
    void setColumnDrawAgentPrecalc(float c);
    int getFirstTimeLevelCompleted(void);
    void setFirstTimeLevelCompleted(int l);
    int getNextScene(void);
    void setNextScene(int s);
    
    float getResourcesHeight(void);
    void setResourcesHeight(int r);
    float getResourcesWidth(void);
    void setResourcesWidth(int r);
    float getResourcesMargin(void);
    void setResourcesMargin(int r);
    
    cocos2d::Color3B getAgentColor(void);
    string getAgentColorString(void);
    void setAgentColor(string c);

    void loadAchievements(void);
    void resetGameProgress(void);

    // tutorial management
    bool launchTutorial(int level) const;
    void setTutorial(int level, bool launch);

private:
    GameData(){}; // Private so that it can not be called

    static GameData* gamedataInstance;
    vector<int> levelsCompleted;     //pos = level
    //0 = progress achievements, 1-X = level achievements
    vector< vector<Achievement*> > achievements;
    bool gameStarted = false;
    string language;
    bool music = true;
    bool sfx = true;
    float raConversion = 1.0;
    float raWConversion = 1.0;
    float raHConversion = 1.0;
    float heightProportionalResolution;
    float rowDrawAgentPrecalc;
    float columnOffsetDrawAgentPrecalc;
    float columnDrawAgentPrecalc;
    int _firstTimeLevelCompleted = 0;
    int nextScene = 0;
    float resourcesHeight;
    float resourcesWidth;
    float resourcesMargin;
    cocos2d::Color3B _agentColor = cocos2d::Color3B(255, 4, 4);

};

#endif /* defined(__simulplay__GameData__) */

