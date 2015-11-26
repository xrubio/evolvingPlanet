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

#include "AppDelegate.h"
#include "UIMainMenu.h"
#include "GameData.h"
#include "LocalizedString.h"
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
        glview = GLViewImpl::createWithFullScreen("Evolving Planet");
        director->setOpenGLView(glview);
        //glview->setDesignResolutionSize(2048, 1536, ResolutionPolicy::NO_BORDER);
    }
    auto screenSize = glview->getFrameSize();

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // multi-device resolution assets
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> resDirOrders;
    
    CCLOG("graphic config for screen: %d/%d", int(screenSize.width), int(screenSize.height));
    if (screenSize.width > 1536) {
        CCLOG("\tusing artwork of size 2048");
        resDirOrders.push_back("tablethd");
        resDirOrders.push_back("tableth");
        resDirOrders.push_back("tablet");
        resDirOrders.push_back("phone");
        GameData::getInstance()->setResourcesWidth(2048);
        GameData::getInstance()->setResourcesHeight(1536);
        GameData::getInstance()->setResourcesMargin(1365);
    } else if (screenSize.width > 1024) {
        CCLOG("\tusing artwork of size 1536");
        resDirOrders.push_back("tableth");
        resDirOrders.push_back("tablet");
        resDirOrders.push_back("phone");
        GameData::getInstance()->setResourcesWidth(1536);
        GameData::getInstance()->setResourcesHeight(1152);
        GameData::getInstance()->setResourcesMargin(1024);
    } else if (screenSize.width > 512) {
        CCLOG("\tusing artwork of size 1024");
        resDirOrders.push_back("tablet");
        resDirOrders.push_back("phone");
        GameData::getInstance()->setResourcesWidth(1024);
        GameData::getInstance()->setResourcesHeight(768);
        GameData::getInstance()->setResourcesMargin(682);
    } else {
        CCLOG("\tusing artwork of size 512");
        resDirOrders.push_back("phone");
        GameData::getInstance()->setResourcesWidth(512);
        GameData::getInstance()->setResourcesHeight(384);
        GameData::getInstance()->setResourcesMargin(341);
    }
    
    fileUtils->setSearchPaths(resDirOrders);

    glview->setDesignResolutionSize(screenSize.width, screenSize.height, ResolutionPolicy::NO_BORDER);

    //fileUtils->setSearchPaths(resDirOrders);

    //Primer cop language = '', agafar del dispositiu per defecte
    string lang = UserDefault::getInstance()->getStringForKey("language");
    if (lang.size() == 0) {
        GameData::getInstance()->setLanguage(LocalizedString::getSystemLang());
        GameData::getInstance()->setMusic(true);
        GameData::getInstance()->setSFX(true);
        UserDefault::getInstance()->setBoolForKey("firsttimeplaying", true);
        UserDefault::getInstance()->setStringForKey("agentColor", "red");
    }
    else {
        GameData::getInstance()->setLanguage(lang);
        GameData::getInstance()->setMusic(UserDefault::getInstance()->getBoolForKey("music"));
        GameData::getInstance()->setSFX(UserDefault::getInstance()->getBoolForKey("sfx"));
        GameData::getInstance()->setAgentColor(UserDefault::getInstance()->getStringForKey("agentColor"));
        UserDefault::getInstance()->setBoolForKey("firsttimeplaying", false);
    }
  
    // conversion is adjusted to the proportion of the largest of the two axis compared to its value in the standard screen (2048x1536)
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float widthConversion = GameData::getInstance()->getResourcesWidth()/2048;
    float heightConversion = visibleSize.height/1536;
    CCLOG("\tvisible size: %d/%d scaling: %f/%f", int(visibleSize.width), int(visibleSize.height), widthConversion, heightConversion);
    CCLOG("\tusing conversion rate: %f", std::max(widthConversion, heightConversion));
    GameData::getInstance()->setRaConversion(widthConversion);

    GameData::getInstance()->setRaWConversion(visibleSize.width / GameData::getInstance()->getResourcesWidth());
    GameData::getInstance()->setRaHConversion(visibleSize.height / GameData::getInstance()->getResourcesHeight());
    /*GameData::getInstance()->setHeightProportionalResolution((Director::getInstance()->getVisibleSize().width / 480.0) * 320.0);
    GameData::getInstance()->setRowDrawAgentPrecalc(Director::getInstance()->getVisibleSize().width / 480.0);
    GameData::getInstance()->setColumnOffsetDrawAgentPrecalc((Director::getInstance()->getVisibleSize().height -
                                                              GameData::getInstance()->getHeightProportionalResolution()) / 2.0);
    GameData::getInstance()->setColumnDrawAgentPrecalc(GameData::getInstance()->getHeightProportionalResolution() / 320.0);*/
    GameData::getInstance()->setHeightProportionalResolution((GameData::getInstance()->getResourcesWidth() / 480.0) * 320.0);
    GameData::getInstance()->setRowDrawAgentPrecalc(GameData::getInstance()->getResourcesWidth() / 480.0);
    GameData::getInstance()->setColumnOffsetDrawAgentPrecalc((GameData::getInstance()->getResourcesHeight() - GameData::getInstance()->getResourcesMargin()) / 2.0);
    GameData::getInstance()->setColumnDrawAgentPrecalc(GameData::getInstance()->getResourcesMargin() / 320.0);

    //LOAD LEVELS
    vector<int> levelsCompleted;
    for (int i = 0; i < NUM_LEVELS + 1; i++) {
        levelsCompleted.push_back(UserDefault::getInstance()->getIntegerForKey(to_string(i).c_str()));
    }
    GameData::getInstance()->setLevelsCompleted(levelsCompleted);

   
    /*
    ////// UNLOCK ACHIEVEMENTS
    
    UserDefault::getInstance()->setBoolForKey("1_COMPLETED", true);
    UserDefault::getInstance()->setBoolForKey("1_PERFECT", true);
    UserDefault::getInstance()->setBoolForKey("1_EVPOINTSLEFT", true);
    UserDefault::getInstance()->setBoolForKey("2_COMPLETED", true);
    UserDefault::getInstance()->setBoolForKey("2_PERFECT", true);
    UserDefault::getInstance()->setBoolForKey("2_DISCOVER", true);
    UserDefault::getInstance()->setBoolForKey("3_COMPLETED", true);
    UserDefault::getInstance()->setBoolForKey("3_PERFECT", true);
    UserDefault::getInstance()->setBoolForKey("3_NOPOWERS", true);
    UserDefault::getInstance()->setBoolForKey("4_COMPLETED", true);
    UserDefault::getInstance()->setBoolForKey("4_PERFECT", true);
    */


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
