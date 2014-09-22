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
#include "DrawPoint.h"
#include "GameLevel.h"
#include "UIPower.h"

class UIGameplayMap : public Layer {
public:
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    static Scene* createScene();

    virtual bool init();

    void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const vector<Touch*>& touches, Event* event);
    void menuBackCallback(Ref* pSender);
    void menuGoalsCallback(Ref* pSender);
    void menuAgentsCallback(Ref* pSender);
    void pauseCallback(Ref* pSender);
    void playCallback(Ref* pSender);
    void fastForwardCallback(Ref* pSender);
    void lifeCallback(Ref* pSender);
    void reproductionCallback(Ref* pSender);
    void mobilityCallback(Ref* pSender);
    void resistanceCallback(Ref* pSender);

    int getValueAtGameplayMapHotSpot(int posx, int posy);
    bool isInBoostResistanceArea(int posx, int posy);
    void updateAgents(vector<Agent*> agentsDomain);
    virtual void update(float delta);
    bool selectSpriteForTouch(Sprite* sprite, Point p);

    CREATE_FUNC(UIGameplayMap);

    bool play = false;

private:
    Sprite* gameplayMap;
    Image* gameplayMapHotSpot;
    unsigned char* dataGameplayMapHotSpot;

    bool moveBackground = false;
    bool moveBackgroundLeft = false;
    bool moveBackgroundRight = false;
    bool moveBackgroundUp = false;
    bool moveBackgroundDown = false;
    float zoomScale = 1;

    bool endGameWindowPainted = false;

    cocos2d::Vector<Touch*> _touches;
    cocos2d::Point firstTouchLocation;

    MenuItem* backButton;

    Label* timeSteps;
    Label* evolutionPointsLabel;

    vector<UIPower*> powerButtons;

    //0 = life, 1 = reproduction, 2 = mobility, 3 = resistance
    int agentColor = 0;

    void pinchZoomWithMovedTouch(Touch* movedTouch);
    float sqrOfDistanceBetweenPoints(Point p1, Point p2);
    void checkBackgroundLimitsInTheScreen(Point destPoint);
    int getValueAtGameplayMapHotSpot(Point pt);

    void createNewLevelThread(void);
    static void* createLevel(void* arg);
    void playLevel(void);
    void initializeAgents(void);

    void createEndGameWindow(int mode);
    bool checkPowersClicked(void);

    void changeAgentColourAndOpacity(Sprite* s, Agent* agent, vector<string>* keys);
};

#endif /* defined(__simulplay__UIGameplayMap__) */
