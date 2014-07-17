#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <pthread.h>
#include "LevelPlayDomain.h"

class HelloWorld : public cocos2d::Layer
{
public:
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    struct SimpleStructure
    {
        LevelPlayDomain *lvl;
        HelloWorld *hllwrld;
    };
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    cocos2d::Sprite *worldMap;
    cocos2d::Image *backgroundHotSpot;
    unsigned char *dataBackgroundHotSpot;
    cocos2d::LabelTTF *label;
    cocos2d::Sprite *mario;

    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    
    void setBackgroundWidth(int width);
    void setBackgroundHeight(int height);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuPauseCallback(cocos2d::Ref* pSender);
    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuFastForwardCallback(cocos2d::Ref* pSender);

    virtual void update(float delta);
    
    
private:
    bool moveBackground = false;
    bool moveBackgroundLeft = false;
    bool moveBackgroundRight = false;
    bool moveBackgroundUp = false;
    bool moveBackgroundDown = false;
    bool zoomBackground = true;
    bool panned = false;
    float zoomScale = 0.5;
    cocos2d::Vector<cocos2d::Touch *> _touches;
    int backgroundWidth = 1024;
    int backgroundHeight = 768;
        
    cocos2d::Point firstTouchLocation;

    time_t timeTaken;
    
    LevelPlayDomain *newLevel;
    
    
    bool selectSpriteForTouch(cocos2d::Sprite *sprite, cocos2d::Point p);
    void checkBackgroundLimitsInTheScreen(cocos2d::Point destPoint);
    void continentSelection(cocos2d::Touch *touch);
    int getValueAtPointFromPNG(cocos2d::Point pt);
    float sqrOfDistanceBetweenPoints(cocos2d::Point p1, cocos2d::Point p2);
    void pinchZoomWithMovedTouch(cocos2d::Touch *movedTouch);
    void setBackgroundSize(void);
    
    void createThreadForNewLevel(void);
    static void* createLevel(void* arg);
    void playLevel(void* arg);
    
};

#endif // __HELLOWORLD_SCENE_H__
