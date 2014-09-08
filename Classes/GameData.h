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

using namespace std;

class GameData {
public:
    static GameData* getInstance();

    vector<int> getLevelsCompleted(void);
    void setLevelsCompleted(vector<int> lvlsCompleted);
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

private:
    GameData() {}; // Private so that it can  not be called
    GameData(GameData const&) {}; // copy constructor is private
    GameData& operator=(GameData const&); // assignment operator is private

    static GameData* gamedataInstance;
    vector<int> levelsCompleted;
    bool gameStarted = false;
    string language;
    bool music = true;
    bool sfx = true;
};

#endif /* defined(__simulplay__GameData__) */
