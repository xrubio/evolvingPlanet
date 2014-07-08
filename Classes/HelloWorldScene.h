#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // a selector callback
    void menuMoveCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    cocos2d::Sprite *worldMap;
    cocos2d::Sprite *hotSpot;
    cocos2d::LabelTTF *label;

    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    
    
private:
    bool moveBackground = false;
    bool moveBackgroundLeft = false;
    bool moveBackgroundRight = false;
    bool moveBackgroundUp = false;
    bool moveBackgroundDown = false;
    bool zoomBackground = true;
    float zoomScale = 0.5;
    std::vector<cocos2d::Touch *> _touches;
        
    bool selectSpriteForTouch(cocos2d::Sprite *sprite, cocos2d::Point p);
    void checkBackgroundLimitsInTheScreen(cocos2d::Point destPoint);
    int getValueAtPoint(cocos2d::Point pt);
    void continentSelection(cocos2d::Touch *touch);
    float sqrOfDistanceBetweenPoints(cocos2d::Point p1, cocos2d::Point p2);
    void pinchZoomWithMovedTouch(cocos2d::Touch *movedTouch);


};

#endif // __HELLOWORLD_SCENE_H__
