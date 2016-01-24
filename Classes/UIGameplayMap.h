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

class Message;
class Tutorial;

class UIGameplayMap : public Layer
{
    // TODO refactor into a class UIGameplayMapWithTutorial
    Tutorial * _tutorial;
public:
    pthread_mutex_t gameLevelMutex = PTHREAD_MUTEX_INITIALIZER;

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
    void changeGraphicCallback(Ref* pSender);
    void skipTutorial(Ref* pSender);
    void skipTutorialConfirm(Ref* pSender);
    void menuHintCallback(Ref* pSender);

    int getValueAtGameplayMap(int rgb, int posx, int posy);

    inline void updateAgents(void);
    virtual void update(float delta);
    bool selectSpriteForTouch(Node* sprite, Point p);

    void moveGoalPopup(int index);

    CREATE_FUNC(UIGameplayMap);

    bool drawInGameAchievementWindow = false;

private:
    Sprite* gameplayMap;
    // information diagrams for the map
    Sprite * _infoMap;
    // create text and color for legend of _infoMap
    void createLegendEntries();
    void updateLegend(bool visible);
    
    Image gameplayMapHotSpot;
    //Image* gameplayMapResources;
    unsigned char* dataGameplayMapHotSpot;
    //unsigned char* dataGameplayMapResources;

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
    
    std::vector<Vec2*> *numAgentsEvolutionPoints;
    PointArray *numAgentsEvolution;
    int indexAgentsEvolution = 0;
    // 0 = population, 1 = wood, 2 = mineral
    vector<WaveNode*> waveNodes;

    Label* timeSteps;
    Label* evolutionPointsLabel;
    Label* restaEvolutionPointsLabel;
    Sprite* evolutionPointsIcon;

    vector<UIPower*> powerButtons;

    int agentColor = 0;

    std::vector<Color4B> _agentsTextureData;
    Texture2D* agentsTexture;
    Sprite* agentsSprite;

    std::vector<Color4B> _exploitedMapTextureData;
    Texture2D* exploitedMapTexture = nullptr;
    Sprite* exploitedMapSprite;

    vector<int> keys;

    bool resourcesMap = false;

    bool firstPlayFF = true;
    
    bool _isWood = false;
    bool _isMineral = false;
    
    bool _playingBeforeHintPressed = false;
    
    float sqrOfDistanceBetweenPoints(Point p1, Point p2);
    void checkBackgroundLimitsInTheScreen(Point destPoint);
    int getValueAtGameplayMap(int rgb, Point pt);

    void createTimingThread(void);
    static void* createTiming(void* arg);
    void startTiming(void);
    void createNewLevelThread(void);
    static void* createLevel(void* arg);
    void playLevel(void);
    void initializeAgents(void);
    void setAttributesToInitialAgents(void);

    void createEndGameWindow(const LevelState & mode);
    void createEndGameWindowLevel20(const LevelState & mode);
    void createAchievementWindow(void);
    void createInGameAchievementWindow(Achievement *ach);

    bool checkPowersClicked(void);
    bool checkButtonPressed(Touch* touch);

    // 0 = square, 1 = triangle
    inline void drawAgent(const Point & pos, const Color4B & colour, int geometry = 0, const Color4B & colorBorder = Color4B::WHITE);
    inline void drawExploitedMap(const Point & pos, const Color4B & colour, int geometry = 0);

    // tutorial related stuff
    /** reference to current message. 0 if no message to show **/
    const Message * _message;
    // change spot location and check if there is a message about it
    void changeSpotPosition();

    // listener tutorial is removed/added based on need when a mission starts
    EventListenerTouchOneByOne * _listenerTutorial;

    void createTutorialGUI();
    void enableTutorialGUI();
    void disableTutorialGUI();
    
    void restoreGameplayMap(void);
    void updateAttributesButtons(void);


    // return the path to the goal icon depending on type and resourceType
    std::string getGoalIcon( const Goal * goal ) const;
public:
    // tutorial related methods
    void setMessage(const Message * message);
    void updateWave(int index, int variable, int maxVariable, Color4B color);
};

#endif /* defined(__simulplay__UIGameplayMap__) */

