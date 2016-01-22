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
//  UIProgressMap.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIProgressMap.h"
#include "UIGoals.h"
#include "GameData.h"
#include "LevelLoader.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

using namespace ui;

Scene* UIProgressMap::createScene()
{
    auto scene = Scene::create();
    auto layer = UIProgressMap::create();
    scene->addChild(layer);
    return scene;
}

bool UIProgressMap::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/main.mp3", true);
    }
    
    Director::getInstance()->setAnimationInterval(1.0 / 60);
    //setLoadingAnimation(false);
    //Alliberar memòria
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    backButton->setPosition(Vec2((3 * visibleSize.width / 34), (2 * visibleSize.height / 25)));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 5);
    
    auto eraWindow = Sprite::create("gui/ProgressMapEraWindow.png");
    eraWindow->setPosition(Vec2(3.3 * visibleSize.width / 4, 6.3 * visibleSize.height / 7));
    eraWindow->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    eraWindow->setName("eraWindow");
    
    auto eraLabel = Label::createWithTTF(LocalizedString::create("FIRSTERA"), "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    eraLabel->setPosition(eraWindow->getContentSize().width / 2, eraWindow->getContentSize().height / 2);
    eraLabel->setColor(Color3B(137, 146, 155));
    eraLabel->setName("eraLabel");
    eraWindow->addChild(eraLabel);
    
    auto arrowPrev = MenuItemImage::create("gui/ArrowBackEra.png", "gui/ArrowBackEraPressed.png",
                                           CC_CALLBACK_1(UIProgressMap::menuEraCallback, this));
    arrowPrev->setName("arrowPrev");
    arrowPrev->setAnchorPoint(Vec2(1, 0.5));
    arrowPrev->setPosition(Vec2(0, eraWindow->getContentSize().height / 2));
    arrowPrev->setVisible(false);

    auto arrowNext = MenuItemImage::create("gui/ArrowNextEra.png", "gui/ArrowNextEraPressed.png",
                                           CC_CALLBACK_1(UIProgressMap::menuEraCallback, this));
    arrowNext->setName("arrowNext");
    arrowNext->setAnchorPoint(Vec2(0, 0.5));
    arrowNext->setPosition(Vec2(eraWindow->getContentSize().width, eraWindow->getContentSize().height / 2));
    arrowNext->setVisible(false);
    
    Vector<MenuItem*> eraButtonVec;
    eraButtonVec.pushBack(arrowPrev);
    eraButtonVec.pushBack(arrowNext);
    auto menuEra = Menu::createWithArray(eraButtonVec);
    menuEra->setPosition(0, 0);
    menuEra->setName("menuEra");
    eraWindow->addChild(menuEra, 5);
    
    this->addChild(eraWindow, 5);
    
    
    int ftlc = GameData::getInstance()->getFirstTimeLevelCompleted();
    
    if (ftlc > 0 and ftlc < 21)
    {
        GameData::getInstance()->setCurrentLevel(ftlc + 1);
    }
    else if (ftlc == 21)
    {
        GameData::getInstance()->setCurrentLevel(ftlc);
    }
    
    pages = PageView::create();
    pages->setCustomScrollThreshold(visibleSize.width / 6);
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    this->addChild(pages);
    
    pages->addPage(setEpisode1());
    if (GameData::getInstance()->getCurrentLevel() > 10)
    {
        pages->addPage(setEpisode2());
    }
    
    pages->setCurPageIndex(GameData::getInstance()->getCurrentEra());
    
    //RESET INITIAL CONFIG
    GameLevel::getInstance()->resetAgentAttributesInitialConfig();
    
    //ANIMATION OF COMPLETED LEVEL AND UNLOCKING NEW LEVEL
    if (ftlc > 0 and ftlc < 22)
    {
        //STAR ANIMATION
        auto pointerLevel = (MenuItem*)(pages->getPage(ftlc/11)->getChildByName("progressMap")->getChildByName("menuLevelButton")->getChildByTag(min(ftlc, 20)));

        auto star1 = (Sprite*)(pointerLevel->getChildByName("star1"));
        auto scalePlus1 = ScaleTo::create(0.4, 1.7);
        auto scaleMinus1 = ScaleTo::create(0.4, 1);
        star1->runAction(Sequence::create(DelayTime::create(1), scalePlus1, scaleMinus1, NULL));
            
        auto star2 = (Sprite*)(pointerLevel->getChildByName("star2"));
        auto scalePlus2 = ScaleTo::create(0.4, 1.7);
        auto scaleMinus2 = ScaleTo::create(0.4, 1);
        star2->runAction(Sequence::create(DelayTime::create(1.4), scalePlus2, scaleMinus2, NULL));
        
        auto star3 = (Sprite*)(pointerLevel->getChildByName("star3"));
        auto scalePlus3 = ScaleTo::create(0.4, 1.7);
        auto scaleMinus3 = ScaleTo::create(0.4, 1);
        star3->runAction(Sequence::create(DelayTime::create(1.8), scalePlus3, scaleMinus3, NULL));
        
        float delayTime = 2;
        
        if (ftlc == 10)
        {
            //ADD PAGE SECOND ERA AND THEN SCROLL
            pages->setCurPageIndex(1);
            delayTime = 2.2;
        }
        
        if (ftlc < 20)
        {
            //POINTER LEVEL UNLOCKED ANIMATION
            auto pointerNextLevel = (MenuItem*)(pages->getPage(ftlc/11)->getChildByName("progressMap")->getChildByName("menuLevelButton")->getChildByTag(ftlc + 1));
            
            Vec2 pos = pointerNextLevel->getPosition();
            pointerNextLevel->setPositionY(visibleSize.height * 1.5);
            auto fall = MoveTo::create(0.6, pos);
            auto fallEase = EaseBackOut::create(fall);
            pointerNextLevel->runAction(Sequence::create(DelayTime::create(delayTime), fallEase, RepeatForever::create(Sequence::create(MoveBy::create(1.5, Vec2(0, pointerNextLevel->getContentSize().width*0.5)), MoveBy::create(1.5, Vec2(0, -pointerNextLevel->getContentSize().width*0.5)), NULL)), NULL));
            
            
            /*auto dropActionLevelButton = MoveBy::create(1.5, Vec2(0, 5));
            auto easeDropActionLevelButton = EaseOut::create(dropActionLevelButton, 1);
            auto dropActionLevelButton2 = MoveBy::create(1.5, Vec2(0, -5));
            auto easeDropActionLevelButton2 = EaseOut::create(dropActionLevelButton2, 1);
            auto seqDrop = Sequence::create(easeDropActionLevelButton, easeDropActionLevelButton2, NULL);
            auto actionRep = RepeatForever::create(seqDrop);
            pointerNextLevel->runAction(actionRep);*/
        }
    
        //RESET FIRST TIME COMPLETED LEVEL FLAG
        GameData::getInstance()->setFirstTimeLevelCompleted(0);
    }
    
    if (GameData::getInstance()->getCurrentLevel() < 21)
    {
        auto pointerCurrentLevel = (MenuItem*)(pages->getPage(GameData::getInstance()->getCurrentLevel()/11)->getChildByName("progressMap")->getChildByName("menuLevelButton")->getChildByTag(GameData::getInstance()->getCurrentLevel()));
        pointerCurrentLevel->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.5, Vec2(0, pointerCurrentLevel->getContentSize().width*0.5)), MoveBy::create(1.5, Vec2(0, -pointerCurrentLevel->getContentSize().width*0.5)), NULL)));
    }
    
    
    auto pageIndicator = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                               "gui/ProgressMapHexagonLevelOff.png");
    pageIndicator->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    unsigned long int numPages = (GameData::getInstance()->getCurrentLevel() / 12) + 1;
    int initialPosX = (visibleSize.width / 2) - ((numPages / 2) * (pageIndicator->getContentSize().width * 3));
    int incrX = (pageIndicator->getContentSize().width * 3);
    for (int i = 0; i < numPages; i++)
    {
        auto pageIndicator = MenuItemImage::create("gui/ProgressMapHexagonLevelOn.png", "gui/ProgressMapHexagonLevelOff.png",
                                                "gui/ProgressMapHexagonLevelOff.png");
        //pageIndicator->setColor(Color3B(120, 120, 120));
        pageIndicator->setPosition(Vec2(initialPosX + incrX * i, visibleSize.height / 20));
        pageIndicator->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        pageIndicator->setEnabled(false);
        pageIndicator->setName("pageIndicator"+to_string(i));
        this->addChild(pageIndicator, 1);
        pagesIndicatorVec.pushBack(pageIndicator);
    }
    
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapBackButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapBackButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowBack.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowBackPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowNext.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowNextPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PageBackground.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapPlayButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapPlayButtonPressed.png");
    
    this->scheduleUpdate();

    return true;
}

void UIProgressMap::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIProgressMap::menuLevelCallback(Ref* pSender)
{
    //setLoadingAnimation(true);
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    GameData::getInstance()->setGameStarted(false);
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    
    ProgressFromTo* radialTimer = ProgressFromTo::create(0.3, 0, 100);
    auto zoneTimer = ProgressTimer::create(Sprite::create("gui/ZoneAreaLevel.png"));
    switch (tag) {
        case 1:
            zoneTimer->setScale(0.8);
            break;
        case 2:
            zoneTimer->setScale(0.6);
            break;
        case 3:
            zoneTimer->setScale(1.1);
            break;
        case 4:
            zoneTimer->setScale(1.1);
            break;
        case 5:
            zoneTimer->setScale(0.6);
            break;
        case 6:
            zoneTimer->setScale(0.6);
            break;
        case 7:
            zoneTimer->setScale(0.6);
            break;
        case 8:
            zoneTimer->setScale(0.6);
            break;
        case 9:
            zoneTimer->setScale(0.8);
            break;
        case 10:
            zoneTimer->setScale(1.0);
            break;
        case 11:
            zoneTimer->setScale(0.7);
            break;
        case 12:
            zoneTimer->setScale(0.75);
            break;
        case 13:
            zoneTimer->setScale(0.75);
            break;
        case 14:
            zoneTimer->setScale(0.8);
            break;
        case 15:
            zoneTimer->setScale(0.55);
            break;
        case 16:
            zoneTimer->setScale(0.6);
            break;
        case 17:
            zoneTimer->setScale(0.8);
            break;
        case 18:
            zoneTimer->setScale(0.6);
            break;
        case 19:
            zoneTimer->setScale(0.65);
            break;
        case 20:
            zoneTimer->setScale(0.7);
            break;
        default:
            zoneTimer->setScale(1.0);
            break;
    }
    zoneTimer->setPosition(Vec2(pMenuItem->getPositionX(), pMenuItem->getPositionY() - (pMenuItem->getContentSize().height / 4)));
    zoneTimer->setType(ProgressTimer::Type::RADIAL);
    zoneTimer->runAction(radialTimer);
    pages->getPage(pages->getCurPageIndex())->getChildByName("progressMap")->addChild(zoneTimer, 1, 102);

    Vector<MenuItem*> menuButtons;

    auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
        CC_CALLBACK_1(UIProgressMap::restoreProgressMap, this));
    darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    menuButtons.pushBack(darkBackground);

    auto popupBackground = MenuItemImage::create("gui/ProgressMapPopupBackground.png", "gui/ProgressMapPopupBackground.png",
                                                 CC_CALLBACK_1(UIProgressMap::restoreProgressMap, this));
    popupBackground->setName("popupBackground");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + (visibleSize.height / 2)));
    popupBackground->setTag(101);
    popupBackground->setLocalZOrder(20);
    menuButtons.pushBack(popupBackground);

    string space = " ";
    string lvl = (LocalizedString::create("LEVEL")) + space + to_string(tag);
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    levelLabel->setColor(Color3B(85, 108, 117));
    levelLabel->setAnchorPoint(Vec2(0, 0.5));
    levelLabel->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 12 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(levelLabel);

    LevelLoader loader;
    auto mapPopup = Sprite::create("maps/"+loader.getLevelFileMap("level" + to_string(tag)) + "Background.jpg");
    mapPopup->setScale(0.21);
    mapPopup->setAnchorPoint(Vec2(0, 0.5));
    mapPopup->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(mapPopup, 1);
    
    if (tag > 10 and tag < 20)
    {
        auto mapForestPopup = Sprite::create("maps/"+loader.getLevelFileMap("level" + to_string(tag)) + "Forest.png");
        mapForestPopup->setScale(0.21);
        mapForestPopup->setAnchorPoint(Vec2(0, 0.5));
        mapForestPopup->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
        popupBackground->addChild(mapForestPopup, 2);
    }

    vector<string> goalTypes = loader.getGoalTypes("level" + to_string(tag));
    
    // three types of goals
    std::vector<int> numGoals;
    numGoals.push_back(0);
    numGoals.push_back(0);
    numGoals.push_back(0);
    
    for (size_t i = 0; i < goalTypes.size(); i++)
    {
        if(goalTypes.at(i)=="Dispersal")
        {
            numGoals.at(Dispersal) += 1;
        }
        else if(goalTypes.at(i)=="Population")
        {
            numGoals.at(Population) += 1;
        }
        // Resources
        else
        {
            numGoals.at(Resources) += 1;
        }
    }

    // three types of goals
    std::string goalPath;
    if(numGoals.at(Dispersal)==0)
    {
        goalPath = "gui/goals/DispersalGoal0.png";
    }
    else
    {
        goalPath = "gui/goals/DispersalGoal.png";
    }
    auto iconLevel = Sprite::create(goalPath);
    iconLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
    iconLevel->setPosition(Vec2(0.08f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
    popupBackground->addChild(iconLevel);
    
    if(numGoals.at(Dispersal)>0)
    {
        auto numGoalsLabel = Label::createWithTTF(to_string(numGoals.at(Dispersal)), "fonts/arial.ttf", 40* GameData::getInstance()->getRaConversion());
        numGoalsLabel->setColor(Color3B(216, 229, 235));
        numGoalsLabel->enableShadow();
        numGoalsLabel->setAnchorPoint(Vec2(0, 0.5));
        numGoalsLabel->setPosition(Vec2(0.08f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
        popupBackground->addChild(numGoalsLabel);
    }
    
    if(numGoals.at(Population)==0)
    {
        goalPath = "gui/goals/PopulationGoal0.png";
    }
    else
    {
        goalPath = "gui/goals/PopulationGoal.png";
    }
    iconLevel = Sprite::create(goalPath);
    iconLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
    iconLevel->setPosition(Vec2(0.19f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
    popupBackground->addChild(iconLevel);
    if(numGoals.at(Population)>0)
    {
        auto numGoalsLabel = Label::createWithTTF(to_string(numGoals.at(Population)), "fonts/arial.ttf", 40* GameData::getInstance()->getRaConversion());
        numGoalsLabel->setColor(Color3B(216, 229, 235));
        numGoalsLabel->enableShadow();
        numGoalsLabel->setAnchorPoint(Vec2(0, 0.5));
        numGoalsLabel->setPosition(Vec2(0.19f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
        popupBackground->addChild(numGoalsLabel);
    }
    
    
    if(numGoals.at(Resources)==0)
    {
        goalPath = "gui/goals/ResourcesGoal0.png";
    }
    else
    {
        goalPath = "gui/goals/ResourcesGoal.png";
    }
    iconLevel = Sprite::create(goalPath);
    iconLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
    iconLevel->setPosition(Vec2(0.3f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
    popupBackground->addChild(iconLevel);
    if(numGoals.at(Resources)>0)
        {
            auto numGoalsLabel = Label::createWithTTF(to_string(numGoals.at(Resources)), "fonts/arial.ttf", 40* GameData::getInstance()->getRaConversion());
            numGoalsLabel->setColor(Color3B(216, 229, 235));
            numGoalsLabel->enableShadow();
            numGoalsLabel->setAnchorPoint(Vec2(0, 0.5));
            numGoalsLabel->setPosition(Vec2(0.3f*popupBackground->getContentSize().width, 0.2f*popupBackground->getContentSize().height));
            popupBackground->addChild(numGoalsLabel);
    }
    
    std::stringstream achStr;
    int numCompleted = 0;
    int numAchievements = 0;

    for (int j = 0; j < GameData::getInstance()->getAchievements().at(tag).size(); j++)
    {
        numAchievements++;
        if(GameData::getInstance()->getAchievements().at(tag).at(j)->getCompleted() == true)
        {
            numCompleted++;
        }
    }

    achStr << numCompleted << " " << LocalizedString::create("OF") << " " << numAchievements << " " << LocalizedString::create("ACH_EARNED");
    auto textAchievements = Label::createWithTTF(achStr.str(), "fonts/arial.ttf", 30 * GameData::getInstance()->getRaConversion());
    textAchievements->setColor(Color3B(216, 229, 235));
    textAchievements->setAnchorPoint(Vec2(0.5, 0.5));
    textAchievements->setPosition(Vec2((0.08f * popupBackground->getContentSize().width) + (mapPopup->getBoundingBox().size.width*0.5f), 0.08f*popupBackground->getContentSize().height));
    popupBackground->addChild(textAchievements);

    auto briefText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("BRIEF_LEVEL_" + to_string(tag)).c_str(), "text"),
        Size(13 * (popupBackground->getContentSize().width / 25), 6 * (popupBackground->getContentSize().height / 14)),
        TextHAlignment::LEFT, "fonts/BebasNeue.otf", 32 * GameData::getInstance()->getRaConversion());
    briefText->setColorSpaceHolder(Color3B(216, 229, 235));
    briefText->setPosition(Vec2(17 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(briefText);

    auto playButton = MenuItemImage::create("gui/ProgressMapPlayButton.png", "gui/ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIProgressMap::proceedLevelCallback, this));
    playButton->setPosition(Vec2(21 * (popupBackground->getContentSize().width / 25), 2 * (popupBackground->getContentSize().height / 14)));
    playButton->setTag(tag);
    auto labelPlay = Label::createWithTTF(LocalizedString::create("PLAY"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    labelPlay->setPosition(playButton->getContentSize().width / 2, playButton->getContentSize().height / 2);
    labelPlay->setColor(Color3B(205, 202, 207));
    playButton->addChild(labelPlay);
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(0, 0);
    popupBackground->addChild(playMenu);

    int score =  GameData::getInstance()->getLevelScore(tag);
    if (tag == 20)
    {
        score = max(GameData::getInstance()->getLevelScore(tag), GameData::getInstance()->getLevelScore(21));
    }
    CCLOG("SCORE: %i", score);
    for (int i = 1; i < 4; i++) {
        string starFile;
        //Estrella plena
        if (score >= i) {
            starFile = "gui/StarFull.png";
        }
        //Estrella buida
        else {
            starFile = "gui/StarEmpty.png";
        }
        auto star = Sprite::create(starFile);
        star->setPosition(Vec2((11 * (popupBackground->getContentSize().width / 25)) + (i * (star->getContentSize().width + star->getContentSize().width / 3)),
            11.5 * (popupBackground->getContentSize().height / 14)));
        popupBackground->addChild(star);
    }

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 10, 100);

    auto darkFadeIn = FadeIn::create(1);
    menu->setOpacity(0);
    menu->runAction(darkFadeIn);

    //auto delayPopup = DelayTime::create(0.4);
    auto popupMoveTo = MoveTo::create(0.7, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto popupEase = EaseBackOut::create(popupMoveTo);
    auto seqShowPopup = Sequence::create(DelayTime::create(0.3), popupEase, NULL);
    popupBackground->runAction(seqShowPopup);

    //PAINT TROPHY
    if (numCompleted == 3) {
        auto trophy = Sprite::create("gui/TrophyIcon.png");
        trophy->setPosition(Vec2(22 * (popupBackground->getContentSize().width / 25), 12 * (popupBackground->getContentSize().height / 14)));
        popupBackground->addChild(trophy);
        trophy->setOpacity(0);
        trophy->setScale(3);
        auto trophyScale = ScaleTo::create(0.5, 1);
        auto trophyFade = FadeIn::create(0.1);
        auto trophyRotate = RotateBy::create(0.3, 360);
        auto trophySpawn = Spawn::create(trophyScale, trophyFade, Repeat::create(trophyRotate, 2), NULL);
        trophy->runAction(Sequence::create(DelayTime::create(0.5), trophySpawn, NULL));
    }
    
    //setLoadingAnimation(false);
}

void UIProgressMap::proceedLevelCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }
    //setLoadingAnimation(true);
    auto pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();

    string filename;
    switch (tag) {
    case 0:
        filename = "level0";
        break;
    case 1:
        filename = "level1";
        break;
    case 2:
        filename = "level2";
        break;
    case 3:
        filename = "level3";
        break;
    case 4:
        filename = "level4";
        break;
    case 5:
        filename = "level5";
        break;
    case 6:
        filename = "level6";
        break;
    case 7:
        filename = "level7";
        break;
    case 8:
        filename = "level8";
        break;
    case 9:
        filename = "level9";
        break;
    case 10:
        filename = "level10";
        break;
    case 11:
        filename = "level11";
        break;
    case 12:
        filename = "level12";
        break;
    case 13:
        filename = "level13";
        break;
    case 14:
        filename = "level14";
        break;
    case 15:
        filename = "level15";
        break;
    case 16:
        filename = "level16";
        break;
    case 17:
        filename = "level17";
        break;
    case 18:
        filename = "level18";
        break;
    case 19:
        filename = "level19";
        break;
    case 20:
        filename = "level20";
        break;
    default:
        filename = "level1";
        break;
    }
    LevelLoader loader;
    loader.loadXmlFile(filename);

    GameData::getInstance()->setCurrentEra(int(pages->getCurPageIndex()));
    
    auto scene = UIGoals::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIProgressMap::restoreProgressMap(Ref* pSender)
{
    if (((MenuItemImage*)pSender)->getName() == "popupBackground")
    {
        return;
    }
    
    //Menu
    if (this->getChildByTag(100) != nullptr) {
        this->removeChildByTag(100);
    }
    //Popup
    /*if (this->getChildByTag(101) != nullptr) {
        this->removeChildByTag(101);
    }*/
    //Hexagon
    if (pages->getPage(pages->getCurPageIndex())->getChildByName("progressMap")->getChildByTag(102) != nullptr) {
        pages->getPage(pages->getCurPageIndex())->getChildByName("progressMap")->removeChildByTag(102);
    }
}

void UIProgressMap::menuEraCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
    }
    /*auto arrowPrev = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowPrev");
    auto arrowNext = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowNext");*/
    auto arrow = (MenuItem*)pSender;
    ssize_t currentPage = pages->getCurPageIndex();
    
    if (arrow->getName() == "arrowPrev")
    {
        if (currentPage > 0)
        {
            pages->scrollToPage(currentPage - 1);
        }
    }
    else if (arrow->getName() == "arrowNext")
    {
        if (currentPage < NUM_ERAS)
        {
            pages->scrollToPage(currentPage + 1);
        }
    }
}

Layout* UIProgressMap::setEpisode1(void)
{
    auto layout = Layout::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto label = (Label*)this->getChildByName("eraWindow")->getChildByName("eraLabel");
    label->setString(LocalizedString::create("FIRSTERA"));
    
    auto progressMap = Sprite::create("gui/ProgressMap1Background.jpg");
    progressMap->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    progressMap->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    progressMap->setName("progressMap");
    layout->addChild(progressMap);
    
    //CLOUDS
    auto cloud2 = Sprite::create("gui/Clouds2.png");
    cloud2->setPosition(Vec2((3.2 * progressMap->getContentSize().width / 4),
                             (0.90 * progressMap->getContentSize().height / 2)));
    progressMap->addChild(cloud2);
    auto movBy1 = MoveBy::create(10, Vec2(-15 * GameData::getInstance()->getRaConversion(), -15 * GameData::getInstance()->getRaConversion()));
    auto easeBy1 = EaseIn::create(movBy1, 1);
    auto movBy2 = MoveBy::create(15, Vec2(15 * GameData::getInstance()->getRaConversion(), 15 * GameData::getInstance()->getRaConversion()));
    auto easeBy2 = EaseIn::create(movBy2, 2);
    auto seqC2 = Sequence::create(easeBy1, easeBy2, NULL);
    cloud2->runAction(RepeatForever::create(seqC2));
    
    auto cloud3 = Sprite::create("gui/Clouds3.png");
    cloud3->setPosition(Vec2(progressMap->getContentSize().width / 2, progressMap->getContentSize().height / 2));
    cloud3->setOpacity(0);
    progressMap->addChild(cloud3);
    auto fadeInC3 = FadeIn::create(15);
    auto fadeOutC3 = FadeOut::create(10);
    auto delayC32 = DelayTime::create(15);
    auto delayC3 = DelayTime::create(1.5);
    auto seqC3 = Sequence::create(delayC32, fadeInC3, delayC3, fadeOutC3, NULL);
    cloud3->runAction(RepeatForever::create(seqC3));
    
    auto cloud1 = Sprite::create("gui/Clouds1.png");
    cloud1->setPosition(Vec2(progressMap->getContentSize().width / 2, progressMap->getContentSize().height / 2));
    cloud1->setOpacity(0);
    progressMap->addChild(cloud1);
    auto fadeIn = FadeIn::create(35);
    auto move = MoveTo::create(60, Vec2(progressMap->getContentSize().width, (5 * progressMap->getContentSize().height / 8)));
    //auto ease = EaseIn::create(move, 1);
    auto spawn = Spawn::create(fadeIn, move, NULL);
    auto move2 = MoveTo::create(25, Vec2((progressMap->getContentSize().width * 1.5), (5 * progressMap->getContentSize().height / 8)));
    auto fadeOut = FadeOut::create(15);
    auto spawn2 = Spawn::create(fadeOut, move2, NULL);
    auto relocate = MoveTo::create(0.1, Vec2(progressMap->getContentSize().width / 2, progressMap->getContentSize().height / 2));
    auto delayC1 = DelayTime::create(5);
    auto seq = Sequence::create(spawn, spawn2, relocate, delayC1, NULL);
    cloud1->runAction(RepeatForever::create(seq));
    
    Vector<MenuItem *> levelButtonVec;
    for (int i = 1; i <= min(10, GameData::getInstance()->getCurrentLevel()); i++)
    {
        //COMPROVAR NIVELLS ACTIVATS
        
        auto levelButton = MenuItemImage::create("gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png",
                                            CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
        levelButton->setTag(i);
        auto levelLabel = Label::createWithTTF(to_string(i), "fonts/monofonto.ttf", 38 * GameData::getInstance()->getRaConversion());
        levelLabel->setPosition(levelButton->getContentSize().width/2, levelButton->getContentSize().height/2);
        levelLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        levelButton->addChild(levelLabel);
        levelLabel->setColor(Color3B(32, 47, 55));
        
        auto shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
        progressMap->addChild(shadow, 1);
        
        switch (i) {
            case 1: {
                levelButton->setPosition((72 * progressMap->getContentSize().width / 204), (32 * progressMap->getContentSize().height / 155));
    
                shadow->setPosition(Vec2(72 * progressMap->getContentSize().width / 204,
                             (32 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 2: {
                levelButton->setPosition(68 * progressMap->getContentSize().width / 204, 54 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(68 * progressMap->getContentSize().width / 204,
                             (54 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 3: {
                levelButton->setPosition(55 * progressMap->getContentSize().width / 204, 71 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(55 * progressMap->getContentSize().width / 204,
                             (71 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 4: {
                levelButton->setPosition(41 * progressMap->getContentSize().width / 204, 71 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(41 * progressMap->getContentSize().width / 204,
                             (71 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 5: {
                levelButton->setPosition(44 * progressMap->getContentSize().width / 204, 103 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(44 * progressMap->getContentSize().width / 204,
                             (103 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 6: {
                levelButton->setPosition(66 * progressMap->getContentSize().width / 204, 91 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(66 * progressMap->getContentSize().width / 204,
                             (91 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 7: {
                levelButton->setPosition(85 * progressMap->getContentSize().width / 204, 86 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(85 * progressMap->getContentSize().width / 204,
                             (86 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 8: {
                levelButton->setPosition(95 * progressMap->getContentSize().width / 204, 67 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(95 * progressMap->getContentSize().width / 204,
                             (67 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 9: {

                levelButton->setPosition(106 * progressMap->getContentSize().width / 204, 101 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(106 * progressMap->getContentSize().width / 204,
                             (101 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 10: {
                levelButton->setPosition(130 * progressMap->getContentSize().width / 204, 88 * progressMap->getContentSize().height / 155);

                shadow->setPosition(Vec2(130 * progressMap->getContentSize().width / 204,
                             (88 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }

            default:
                break;
        }
        //temporary for all unlocked levels
        setStars(i, levelButton);

        /*if (nextLevelUnlockedStars == false)
        {
            setStars(i, levelButton);
            if (GameData::getInstance()->getLevelScore(i) == 0)
            {
                nextLevelUnlockedStars = true;
            }
        }*/
        levelButtonVec.pushBack(levelButton);
    }

    auto menuLevelButton = Menu::createWithArray(levelButtonVec);
    menuLevelButton->setPosition(Vec2(0, 0));
    menuLevelButton->setName("menuLevelButton");
    progressMap->addChild(menuLevelButton, 3, 0);
    
    return layout;
}

Layout* UIProgressMap::setEpisode2(void)
{
    auto layout = Layout::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto progressMap = Sprite::create("gui/ProgressMap2Background.jpg");
    progressMap->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    progressMap->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    progressMap->setName("progressMap");
    layout->addChild(progressMap);
    
    auto cloud2 = Sprite::create("gui/Clouds2.png");
    cloud2->setPosition(Vec2((3.2 * progressMap->getContentSize().width / 4), (0.75 * progressMap->getContentSize().height / 2)));
    progressMap->addChild(cloud2);
    auto movBy1 = MoveBy::create(8, Vec2(-20 * GameData::getInstance()->getRaConversion(), -20 * GameData::getInstance()->getRaConversion()));
    auto easeBy1 = EaseIn::create(movBy1, 1);
    auto movBy2 = MoveBy::create(15, Vec2(20 * GameData::getInstance()->getRaConversion(), 20 * GameData::getInstance()->getRaConversion()));
    auto easeBy2 = EaseIn::create(movBy2, 2);
    auto seqC2 = Sequence::create(easeBy1, easeBy2, NULL);
    cloud2->runAction(RepeatForever::create(seqC2));
    
    Vector<MenuItem *> levelButtonVec;
    for (int i = 11; i <= min(20, GameData::getInstance()->getCurrentLevel()); i++)
    {
        //COMPROVAR NIVELLS ACTIVATS
        
        auto levelButton = MenuItemImage::create("gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png",
                                                 CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
        levelButton->setTag(i);
        auto levelLabel = Label::createWithTTF(to_string(i), "fonts/monofonto.ttf", 38 * GameData::getInstance()->getRaConversion());
        levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
        levelButton->addChild(levelLabel);
        levelLabel->setColor(Color3B(32, 47, 55));
        
        auto shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
        progressMap->addChild(shadow, 1);
        
        switch (i) {
            case 11: {
                levelButton->setPosition(155 * progressMap->getContentSize().width / 204, 87 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(155 * progressMap->getContentSize().width / 204,
                                         (87 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 12: {
                levelButton->setPosition(130 * progressMap->getContentSize().width / 204, 78 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(130 * progressMap->getContentSize().width / 204,
                                         (78 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 13: {
                levelButton->setPosition(106 * progressMap->getContentSize().width / 204, 86 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(106 * progressMap->getContentSize().width / 204,
                                         (86 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 14: {
                levelButton->setPosition(78 * progressMap->getContentSize().width / 204, 84 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(78 * progressMap->getContentSize().width / 204,
                                         (84 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 15: {
                levelButton->setPosition(48 * progressMap->getContentSize().width / 204, 102 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(48 * progressMap->getContentSize().width / 204,
                                         (102 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 16: {
                levelButton->setPosition(83 * progressMap->getContentSize().width / 204, 105 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(83 * progressMap->getContentSize().width / 204,
                                         (105 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 17: {
                levelButton->setPosition(56 * progressMap->getContentSize().width / 204, 75 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(56 * progressMap->getContentSize().width / 204,
                                         (75 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 18: {
                levelButton->setPosition(88 * progressMap->getContentSize().width / 204, 67 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(88 * progressMap->getContentSize().width / 204,
                                         (67 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 19: {
                levelButton->setPosition(104 * progressMap->getContentSize().width / 204, 58 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(104 * progressMap->getContentSize().width / 204,
                                         (58 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 20: {
                levelButton->setPosition(165 * progressMap->getContentSize().width / 204, 48 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(165 * progressMap->getContentSize().width / 204,
                                         (48 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
                
            default:
                break;
        }
        //temporary for all unlocked levels
        setStars(i, levelButton);

        
        /*if (nextLevelUnlockedStars == false)
        {
            setStars(i, levelButton);
            if (GameData::getInstance()->getLevelScore(i) == 0)
            {
                nextLevelUnlockedStars = true;
            }
        }*/
        levelButtonVec.pushBack(levelButton);
    }
    
    auto menuLevelButton = Menu::createWithArray(levelButtonVec);
    menuLevelButton->setPosition(Vec2(0, 0));
    menuLevelButton->setName("menuLevelButton");
    progressMap->addChild(menuLevelButton, 3, 0);
    
    return layout;
}

void UIProgressMap::setStars(int level, MenuItemImage* levelButton)
{
        int score = GameData::getInstance()->getLevelScore(level);
        if (level == 20)
        {
            score = max(GameData::getInstance()->getLevelScore(level), GameData::getInstance()->getLevelScore(21));
        }
        for (int i = 1; i < 4; i++) {
            string starFile;
            //Estrella plena
            if (score >= i) {
                starFile = "gui/StarFullMini.png";
            }
            //Estrella buida
            else {
                starFile = "gui/StarEmptyMini.png";
            }
            auto star = Sprite::create(starFile);
            star->setPosition(Vec2((levelButton->getContentSize().width / 2) - star->getContentSize().width + (star->getContentSize().width * (i - 1)), levelButton->getContentSize().height + star->getContentSize().height / 2));
            star->setName("star"+to_string(i));
            levelButton->addChild(star);
        }
}

void UIProgressMap::update(float delta)
{
    auto arrowPrev = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowPrev");
    auto arrowNext = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowNext");
    auto label = (Label*)this->getChildByName("eraWindow")->getChildByName("eraLabel");

    if (GameData::getInstance()->getCurrentLevel() > 10)
    {
    switch (pages->getCurPageIndex()) {
        case 1:
        {
            arrowPrev->setVisible(true);
            arrowNext->setVisible(false);
            label->setString(LocalizedString::create("SECONDERA"));
            break;
        }
        default:
        {
            arrowPrev->setVisible(false);
            arrowNext->setVisible(true);
            label->setString(LocalizedString::create("FIRSTERA"));
            break;
        }
    }
    }
    
    for (int i = 0; i < pagesIndicatorVec.size(); i++)
    {
        if (i == pages->getCurPageIndex())
        {
            pagesIndicatorVec.at(i)->setEnabled(true);
        }
        else
        {
            pagesIndicatorVec.at(i)->setEnabled(false);
        }
    }
}

void UIProgressMap::setLoadingAnimation(bool visible)
{
    if (this->getChildByName("loading") != nullptr)
    {
        this->getChildByName("loading")->setVisible(visible);
    }
    else
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto loading = Sprite::create("gui/Loading.png");
        loading->setPosition(7 * visibleSize.width / 8, visibleSize.height / 8);
        loading->setScale(GameData::getInstance()->getRaHConversion());
        loading->setName("loading");
        loading->setVisible(visible);
        this->addChild(loading, 500);
        loading->runAction(RepeatForever::create(RotateBy::create(1, 180)));
    }
}
