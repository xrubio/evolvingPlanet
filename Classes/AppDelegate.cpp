#include "AppDelegate.h"
#include "UIMainMenu.h"
#include "GameData.h"
#include "LocalizedString.h"
#include <iostream>
#include <string>

#include <audio/include/SimpleAudioEngine.h>

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {  
        glview = GLViewImpl::createWithFullScreen("simulplay");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //Primer cop language = '', agafar del dispositiu per defecte
    string lang = UserDefault::getInstance()->getStringForKey("language");
    if (lang.size() == 0) {
        GameData::getInstance()->setLanguage(LocalizedString::getSystemLang());
    } else {
        GameData::getInstance()->setLanguage(lang);
    }
    GameData::getInstance()->setMusic(UserDefault::getInstance()->getBoolForKey("music"));
    GameData::getInstance()->setSFX(UserDefault::getInstance()->getBoolForKey("sfx"));

    // create a scene. it's an autorelease object
    auto scene = UIMainMenu::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone
// call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    UserDefault::getInstance()->setStringForKey("language", GameData::getInstance()->getLanguage());
    UserDefault::getInstance()->setBoolForKey("music", GameData::getInstance()->getMusic());
    UserDefault::getInstance()->setBoolForKey("sfx", GameData::getInstance()->getSFX());
    UserDefault::getInstance()->flush();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
