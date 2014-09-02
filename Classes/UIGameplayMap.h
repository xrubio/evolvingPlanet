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

    int getValueAtGameplayMapHotSpot(int posx, int posy);
    bool isInBoostResistanceArea(int posx, int posy);
    void addAgent(Agent* ag);
    void deleteAgent(int tag, int i);
    void updateAgents(vector<Agent*> agentsDomain);
    virtual void update(float delta);

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
    bool movePower1 = false;
    bool movePower2 = false;

    cocos2d::Vector<Touch*> _touches;
    cocos2d::Point firstTouchLocation;

    DrawPoint* agents;
    //vector<Sprite*> agentsSprite;

    Label* timeSteps;
    Label* evolutionPointsLabel;

    Sprite* power1Button; //boost reproduction
    Sprite* power2Button; //boost resistance
    Sprite* areaPower2;
    Label* cooldownPower1;
    Label* cooldownPower2;

    void pinchZoomWithMovedTouch(Touch* movedTouch);
    bool selectSpriteForTouch(Sprite* sprite, Point p);
    float sqrOfDistanceBetweenPoints(Point p1, Point p2);
    void checkBackgroundLimitsInTheScreen(Point destPoint);
    int getValueAtGameplayMapHotSpot(Point pt);

    void createNewLevelThread(void);
    static void* createLevel(void* arg);
    void playLevel(void);
    void initializeAgents(void);
};

#endif /* defined(__simulplay__UIGameplayMap__) */
