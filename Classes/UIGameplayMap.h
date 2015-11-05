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
//  UIGameplayMap.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIGameplayMap__
#define __simulplay__UIGameplayMap__

#include <pthread.h>
#include "UIProgressMap.h"
#include "GameLevel.h"
#include "UIPower.h"
#include "WaveNode.h"

#define SECONS_PER_STEP 1

class Message;
class Tutorial;

class UIGameplayMap : public Layer
{
    // TODO refactor into a class UIGameplayMapWithTutorial
    Tutorial * _tutorial;
public:
    pthread_mutex_t gameLevelMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t timingMutex = PTHREAD_MUTEX_INITIALIZER;

    static Scene* createScene();

    virtual bool init();
    ~UIGameplayMap();

    void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const vector<Touch*>& touches, Event* event);
    bool onTouchBeganTutorial(Touch * touch, Event* event);
    void onTouchEndedTutorial(Touch * touch, Event* event);
    
    void onMouseScroll(Event* event);
    void menuBackCallback(Ref* pSender);

    // toggle between pause and play
    void togglePlay(Ref* pSender);
    // pause completely the game
    void pauseGame();

    void attributeSelectionCallback(Ref* pSender);
    void quitCallback(Ref* pSender);
    void retryCallback(Ref* pSender);
    void retryOkCallback(Ref* pSender);
    void NoCallback(Ref* pSender);
    void minusAttCallback(Ref* pSender);
    void plusAttCallback(Ref* pSender);
    void hideAchievementWindowCallback(Ref* pSender);
    void removeFingerSpot(Ref* pSender);

    int getValueAtGameplayMap(int rgb, int posx, int posy, int map);

    bool isInBoostResistanceArea(int posx, int posy);
    void restoreLand(void);
    inline void updateAgents(void);
    virtual void update(float delta);
    bool selectSpriteForTouch(Sprite* sprite, Point p);

    void moveGoalPopup(int index);

    float getTimeProgressBar(void);
    void setTimeProgressBar(float t);

    CREATE_FUNC(UIGameplayMap);

    bool play = false;

private:
    Sprite* gameplayMap;
    Image* gameplayMapHotSpot;
    Image* gameplayMapResources;
    unsigned char* dataGameplayMapHotSpot;
    unsigned char* dataGameplayMapResources;

    MenuItem* fingerSpot;

    bool moveBackground = false;
    bool moveBackgroundLeft = false;
    bool moveBackgroundRight = false;
    bool moveBackgroundUp = false;
    bool moveBackgroundDown = false;

    pthread_t timingThread;
    pthread_t gameLevelThread;

    bool endGameWindowPainted = false;

    Vector<Touch*> _touches;
    Point firstTouchLocation;
    clock_t timeFingerSpot = 0;

    MenuItem* backButton;
    ProgressTimer* timeBar;
    Sprite* timeBorderBar;
    Sprite* pauseDarkBackground;
    float timeProgressBar = 0.0;
    
    Vector<Sprite*> lifeBars;
    std::vector<Vec2*> *numAgentsEvolutionPoints;
    PointArray *numAgentsEvolution;
    int indexAgentsEvolution = 0;
    DrawNode *agentsEvolution;
    vector<WaveNode*> waveNodes;

    Label* timeSteps;
    Label* evolutionPointsLabel;
    Label* restaEvolutionPointsLabel;
    Sprite* evolutionPointsIcon;

    vector<UIPower*> powerButtons;

    //0 = life, 1 = reproduction, 2 = mobility, 3 = resistance
    int agentColor = 0;

    Color4B* agentsTextureData = new Color4B[2048 * 1536];
    Texture2D* agentsTexture;
    Sprite* agentsSprite;

    Color4B* exploitedMapTextureData = new Color4B[2048 * 1536];
    Texture2D* exploitedMapTexture;
    Sprite* exploitedMapSprite;

    vector<int> keys;

    bool resourcesMap = false;

    bool firstPlayFF = true;
    
    float sqrOfDistanceBetweenPoints(Point p1, Point p2);
    void checkBackgroundLimitsInTheScreen(Point destPoint);
    int getValueAtGameplayMap(int rgb, Point pt, int map);

    void createTimingThread(void);
    static void* createTiming(void* arg);
    void startTiming(void);
    void createNewLevelThread(void);
    static void* createLevel(void* arg);
    void playLevel(void);
    void initializeAgents(void);
    void setAttributesToInitialAgents(void);

    void createEndGameWindow(const LevelState & mode);
    void createAchievementWindow(void);

    bool checkPowersClicked(void);

    // 0 = square, 1 = triangle
    inline void drawAgent(Point pos, Color4B colour, int geometry = 0);
    inline void drawExploitedMap(Point pos, Color4B colour, int geometry = 0);

    // tutorial related stuff
    /** reference to current message. 0 if no message to show **/
    const Message * _message;
    // change spot location and check if there is a message about it
    void changeSpotPosition();

    // listener tutorial is removed/added based on need when a mission starts
    EventListenerTouchOneByOne * _listenerTutorial;
    
    void restoreGameplayMap(void);
    void updateAttributesButtons(void);

public:
    // tutorial related methods
    void setMessage(const Message * message);
    void updateWave(int indexAgent);
};

#endif /* defined(__simulplay__UIGameplayMap__) */

