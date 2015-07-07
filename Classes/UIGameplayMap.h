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
    void onMouseScroll(Event* event);
    void menuBackCallback(Ref* pSender);
    void menuGoalsCallback(Ref* pSender);
    void menuAgentsCallback(Ref* pSender);
    void pauseCallback(Ref* pSender);
    void playCallback(Ref* pSender);
    void fastForwardCallback(Ref* pSender);
    void attributeSelectionCallback(Ref* pSender);
    void quitCallback(Ref* pSender);
    void retryCallback(Ref* pSender);
    void retryOkCallback(Ref* pSender);
    void NoCallback(Ref* pSender);
    void agentTypeCallback(Ref* pSender);
    void moveAttCallback(Ref* pSender);
    void minusAttCallback(Ref* pSender);
    void plusAttCallback(Ref* pSender);
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
    //Sprite* fingerSpotArea;

    bool moveBackground = false;
    bool moveBackgroundLeft = false;
    bool moveBackgroundRight = false;
    bool moveBackgroundUp = false;
    bool moveBackgroundDown = false;
    float zoomScale = 1;
    Point centerZoom = Point(1024, 768);

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
    Sprite* goalPopup;
    float timeProgressBar = 0.0;
    
    Vector<Sprite*> lifeBars;

    Label* timeSteps;
    Label* evolutionPointsLabel;
    Label* collect1PointsLabel;
    Label* distanceLabel;

    vector<UIPower*> powerButtons;

    //0 = life, 1 = reproduction, 2 = mobility, 3 = resistance
    int agentColor = 0;

    Color4B* agentsTextureData = new Color4B[2048 * 1536];
    Texture2D* agentsTexture;
    Sprite* agentsSprite;

    Color4B* exploitedMapTextureData = new Color4B[2048 * 1536];
    Texture2D* exploitedMapTexture;
    Sprite* exploitedMapSprite;

    vector<string> keys;

    bool resourcesMap = false;

    void pinchZoomWithMovedTouch(Touch* movedTouch);
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

    void createEndGameWindow(const LevelState & mode);
    bool checkPowersClicked(void);

    // 0 = square, 1 = triangle
    inline void drawAgent(Point pos, Color4B colour, int geometry = 0);
    inline void drawExploitedMap(Point pos, Color4B colour, int geometry = 0);

    // tutorial related stuff
    /** reference to current message. 0 if no message to show **/
    const Message * _message;
public:
    // tutorial related methods
    void setMessage(const Message * message);
};

#endif /* defined(__simulplay__UIGameplayMap__) */

