#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("simulplay");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // multi-device resolution assets
    auto screenSize = glview->getFrameSize();
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> resDirOrders;
    
    if (2048 == screenSize.width or 2048 == screenSize.height)
    {
        resDirOrders.push_back("ipadhd");
        resDirOrders.push_back("ipad");
        resDirOrders.push_back("iphonehd5");
        resDirOrders.push_back("iphonehd");
        resDirOrders.push_back("iphone");
        
        glview->setDesignResolutionSize(2048, 1536, ResolutionPolicy::NO_BORDER);
    }
    else if (1024 == screenSize.width or 1024 == screenSize.height)
    {
        resDirOrders.push_back("ipad");
        resDirOrders.push_back("iphonehd5");
        resDirOrders.push_back("iphonehd");
        resDirOrders.push_back("iphone");
        
        glview->setDesignResolutionSize(1024, 768, ResolutionPolicy::NO_BORDER);
    }
    else if (1136 == screenSize.width or 1136 == screenSize.height)
    {
        resDirOrders.push_back("iphonehd5");
        resDirOrders.push_back("iphonehd");
        resDirOrders.push_back("iphone");
        
        glview->setDesignResolutionSize(1136, 640, ResolutionPolicy::NO_BORDER);
    }
    else if (960 == screenSize.width or 960 == screenSize.height)
    {
        resDirOrders.push_back("iphonehd");
        resDirOrders.push_back("iphone");
        
        glview->setDesignResolutionSize(960, 640, ResolutionPolicy::NO_BORDER);
    }
    else
    {
        if (screenSize.width > 1080)
        {
            resDirOrders.push_back("iphonehd");
            resDirOrders.push_back("iphone");
            
            glview->setDesignResolutionSize(960, 640, ResolutionPolicy::NO_BORDER);
        }
        else
        {
            resDirOrders.push_back("iphone");
            
            glview->setDesignResolutionSize(480, 320, ResolutionPolicy::NO_BORDER);
        }
    }
    
    fileUtils->setSearchPaths(resDirOrders);
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
