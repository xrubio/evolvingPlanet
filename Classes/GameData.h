//
//  GameData.h
//  simulplay
//
//  Created by Guillem Laborda on 14/08/14.
//
//

#ifndef __simulplay__GameData__
#define __simulplay__GameData__

#include "Achievement.h"
#include <vector>
#include <string>

using namespace std;

class GameData {
public:
    static GameData* getInstance();

    vector<int> getLevelsCompleted(void);
    void setLevelsCompleted(vector<int> lvlsCompleted);
    vector<Achievement*> getAchievements(void);
    void setAchievements(vector<Achievement*> ach);
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
    bool getResetGameClicked(void);
    void setResetGameClicked(bool r);

    void loadAchievements(void);
    void resetGameProgress(void);

private:
    GameData() {}; // Private so that it can  not be called
    GameData(GameData const&) {}; // copy constructor is private
    GameData& operator=(GameData const&); // assignment operator is private

    static GameData* gamedataInstance;
    vector<int> levelsCompleted;
    vector<Achievement*> achievements;
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
    bool resetGameClicked = false;
    
};

#endif /* defined(__simulplay__GameData__) */
