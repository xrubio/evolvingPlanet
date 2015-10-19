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
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
    }
    
    Director::getInstance()->setAnimationInterval(1.0 / 60);
    
    //LOAD ACHIEVEMENTS FROM XML
    if ( GameData::getInstance()->getAchievements().size() <= 0)
    {
        GameData::getInstance()->loadAchievements();
    }

    //Alliberar memòria
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    backButton->setPosition(Vec2((3 * visibleSize.width / 34), (2 * visibleSize.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 5);

    /*auto popupLevelBackground = Sprite::create("gui/ProgressMapLevelPopupBackground.png");
    
    auto labelBorderTop = Label::createWithTTF(LocalizedString::create("FIRST_ERA").c_str(), "fonts/BebasNeue.otf", 60);
    labelBorderTop->setColor(Color3B(Color3B::WHITE));
    labelBorderTop->setPosition(Vec2(popupLevelBackground->getBoundingBox().size.width / 2,
                                     (popupLevelBackground->getContentSize().height + popupLevelBackground->getContentSize().height / 24.3) -  (popupLevelBackground->getContentSize().height / 12.15)));
    popupLevelBackground->addChild(labelBorderTop);
    
    popupLevelBackground->setAnchorPoint(Vec2(0, 0.5));
    popupLevelBackground->setPosition(Vec2(progressMap->getBoundingBox().size.width, progressMap->getBoundingBox().size.height / 2));
    
    selectedBackground = Sprite::create("gui/ProgressMapLevelSelected.png");
    selectedBackground->setVisible(false);
    popupLevelBackground->addChild(selectedBackground);

    Vector<MenuItem*> levelButtons;

    for (int i = 0; i < 10; i++) {
        auto buttonLevel = MenuItem::create(CC_CALLBACK_1(UIProgressMap::menuLevelZoneCallback, this));
        buttonLevel->setContentSize(selectedBackground->getContentSize());
        buttonLevel->setTag(i + 1);
        buttonLevel->setContentSize(Size(popupLevelBackground->getBoundingBox().size.width,
            popupLevelBackground->getBoundingBox().size.height / 22));
        string space = " ";
        auto labelButtonLevel = Label::createWithTTF(LocalizedString::create("LEVEL") + space + to_string(i + 1),
            "fonts/BebasNeue.otf", 45);
        labelButtonLevel->setColor(Color3B::WHITE);
        labelButtonLevel->setPosition(Vec2(1.3 * buttonLevel->getBoundingBox().size.width / 4, buttonLevel->getBoundingBox().size.height / 2));
        labelButtonLevel->setOpacity(0);
        labelButtonLevel->runAction(Sequence::create(DelayTime::create(1.1), FadeIn::create(0.4), NULL));
        buttonLevel->addChild(labelButtonLevel);
        if (i > 9) {
            buttonLevel->setEnabled(false);
            labelButtonLevel->setColor(Color3B::GRAY);
        }
        int score = 0;
        if (GameData::getInstance()->getLevelsCompleted().size() > i + 1) {
            score = GameData::getInstance()->getLevelScore(i + 1);
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
            star->setPosition(Vec2((3.85 * buttonLevel->getBoundingBox().size.width / 7) + (i * (star->getContentSize().width + star->getContentSize().width / 4)),
                buttonLevel->getBoundingBox().size.height / 2));
            star->setOpacity(0);
            star->runAction(Sequence::create(DelayTime::create(1.1), FadeIn::create(0.4), NULL));
            buttonLevel->addChild(star);
        }
        buttonLevel->setPosition(Vec2(popupLevelBackground->getBoundingBox().size.width / 2,
            (popupLevelBackground->getContentSize().height + popupLevelBackground->getContentSize().height / 24.3) - ((i + 2) * (popupLevelBackground->getContentSize().height / 12.15))));
        levelButtons.pushBack(buttonLevel);
    }
    auto menuLevelButtons = Menu::createWithArray(levelButtons);
    menuLevelButtons->setPosition(Vec2(0, 0));
    popupLevelBackground->addChild(menuLevelButtons, 1);
    
    auto arrowRetract = MenuItemImage::create("gui/ArrowRetract.png", "gui/ArrowRetractPressed.png",
                                              CC_CALLBACK_1(UIProgressMap::movePopupLevelCallback, this));
    arrowRetract->setAnchorPoint(Vec2(1, 0));
    arrowRetract->setPosition(0, 0);
    Menu* arrowMenu = Menu::createWithItem(arrowRetract);
    arrowMenu->setPosition(Vec2(0, (popupLevelBackground->getContentSize().height + popupLevelBackground->getContentSize().height / 24.3) -
                                (11.5 * (popupLevelBackground->getContentSize().height / 12.15))));
    arrowMenu->setName("menuArrow");
    popupLevelBackground->addChild(arrowMenu);
    
    popupLevelBackground->runAction(EaseBackInOut::create(MoveTo::create(1.0, Vec2(progressMap->getBoundingBox().size.width - popupLevelBackground->getBoundingBox().size.width, popupLevelBackground->getPositionY()))));
    
    progressMap->addChild(popupLevelBackground, 8, 103);*/
    
    auto eraWindow = Sprite::create("gui/ProgressMapEraWindow.png");
    eraWindow->setPosition(Vec2(3.3 * visibleSize.width / 4, 6.3 * visibleSize.height / 7));
    eraWindow->setName("eraWindow");
    
    auto eraLabel = Label::createWithTTF(LocalizedString::create("FIRST")+" "+LocalizedString::create("ERA"), "fonts/BebasNeue.otf", 80);
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
    
    Vector<MenuItem*> eraButtonVec;
    eraButtonVec.pushBack(arrowPrev);
    eraButtonVec.pushBack(arrowNext);
    auto menuEra = Menu::createWithArray(eraButtonVec);
    menuEra->setPosition(0, 0);
    menuEra->setName("menuEra");
    eraWindow->addChild(menuEra, 5);
    
    this->addChild(eraWindow, 5);

    /*for (int i = 1; i < 3; i++)
    {
        auto eraButton = MenuItemImage::create("gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png",
                                               "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuEraCallback, this));
        eraButton->setPosition(Vec2(((28 + (i * 2)) * visibleSize.width / 34), (2 * visibleSize.height / 25)));
        eraButton->setTag(i);
        auto eraLabel = Label::createWithTTF(LocalizedString::create("FIRST")+LocalizedString::create("ERA"), "fonts/BebasNeue.otf", 40);
        eraLabel->setPosition(eraButton->getContentSize().width / 2, eraButton->getContentSize().height / 2);
        eraLabel->setColor(Color3B(32, 47, 55));
        eraButton->addChild(eraLabel);
        if (i == 1)
        {
            eraButton->setEnabled(false);
        }
        eraButtonVec.pushBack(eraButton);
    }
    
    auto menuEra = Menu::createWithArray(eraButtonVec);
    menuEra->setPosition(0, 0);
    menuEra->setName("menuEra");
    this->addChild(menuEra, 5);*/
    
    setEpisode1();

    //RESET INITIAL CONFIG
    GameLevel::getInstance()->resetAgentAttributesInitialConfig();

    this->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapHexagonLevelOn.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapBackButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapBackButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowBack.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowBackPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowNext.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ArrowNextPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PageBackground.png");
    Director::getInstance()->getTextureCache()->addImage("gui/BlankAttributePointButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/FilledAttributePointButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ContextPageButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ContextPageButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapPlayButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/ProgressMapPlayButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/MinusButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/MinusButtonPressed.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlusButton.png");
    Director::getInstance()->getTextureCache()->addImage("gui/PlusButtonPressed.png");

    return true;
}

void UIProgressMap::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIProgressMap::menuLevelCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
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
        default:
            zoneTimer->setScale(1.0);
            break;
    }
    zoneTimer->setPosition(Vec2(pMenuItem->getPositionX(), pMenuItem->getPositionY() - (pMenuItem->getContentSize().height / 4)));
    zoneTimer->setType(ProgressTimer::Type::RADIAL);
    zoneTimer->runAction(radialTimer);
    progressMap->addChild(zoneTimer, 1, 102);

    Vector<MenuItem*> menuButtons;

    auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
        CC_CALLBACK_1(UIProgressMap::restoreProgressMap, this));
    darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    menuButtons.pushBack(darkBackground);

    auto popupBackground = Sprite::create("gui/ProgressMapPopupBackground.png");
    popupBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + (visibleSize.height / 2)));
    this->addChild(popupBackground, 20, 101);

    string space = " ";
    string lvl = (LocalizedString::create("LEVEL")) + space + to_string(tag);
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 100);
    levelLabel->setColor(Color3B(85, 108, 117));
    levelLabel->setAnchorPoint(Vec2(0, 0.5));
    levelLabel->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 12 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(levelLabel);

    LevelLoader loader;
    auto mapPopup = Sprite::create("maps/"+loader.getLevelFileMap("level" + to_string(tag)) + "Background.jpg");
    mapPopup->setScale(0.21);
    mapPopup->setAnchorPoint(Vec2(0, 0.5));
    mapPopup->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(mapPopup);

    vector<string> goalTypes = loader.getGoalTypes("level" + to_string(tag));
    for (size_t i = 0; i < goalTypes.size(); i++) {
        Sprite* iconLevel;
        if (goalTypes[i] == "Expansion") {
            iconLevel = Sprite::create("gui/ExpansionGoalIcon.png");
        }
        else if (goalTypes[i] == "Collection") {
            iconLevel = Sprite::create("gui/CollectionGoalIcon.png");
        }
        iconLevel->setAnchorPoint(Vec2(1, 0.75));
        iconLevel->setPosition(Vec2(mapPopup->getPosition().x + mapPopup->getBoundingBox().size.width - ((iconLevel->getContentSize().width * i) + (iconLevel->getContentSize().width / 10) * i),
            mapPopup->getPosition().y - (mapPopup->getBoundingBox().size.height / 2)));
        popupBackground->addChild(iconLevel);
    }

    auto briefText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("BRIEF_LEVEL_" + to_string(tag)).c_str(), "text"),
        Size(13 * (popupBackground->getContentSize().width / 25), 6 * (popupBackground->getContentSize().height / 14)),
        TextHAlignment::LEFT, "Corbel", 28);
    briefText->setColorSpaceHolder(Color3B(216, 229, 235));
    briefText->setPosition(Vec2(17 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(briefText);

    auto playButton = MenuItemImage::create("gui/ProgressMapPlayButton.png", "gui/ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIProgressMap::proceedLevelCallback, this));
    playButton->setPosition(Vec2(21 * (popupBackground->getContentSize().width / 25), 2 * (popupBackground->getContentSize().height / 14)));
    playButton->setTag(tag);
    auto labelPlay = Label::createWithTTF(LocalizedString::create("PLAY"), "fonts/BebasNeue.otf", 50);
    labelPlay->setPosition(playButton->getContentSize().width / 2, playButton->getContentSize().height / 2);
    labelPlay->setColor(Color3B(205, 202, 207));
    playButton->addChild(labelPlay);
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(0, 0);
    popupBackground->addChild(playMenu);

    int score = 0;
    if (GameData::getInstance()->getLevelsCompleted().size() > size_t(tag)) {
        score = GameData::getInstance()->getLevelScore(tag);
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
    if (score == 3) {
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
}

void UIProgressMap::proceedLevelCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
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
    default:
        filename = "level1";
        break;
    }
    LevelLoader loader;
    loader.loadXmlFile(filename);

    auto scene = UIGoals::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIProgressMap::restoreProgressMap(Ref* pSender)
{
    //Menu
    if (this->getChildByTag(100) != nullptr) {
        this->removeChildByTag(100);
    }
    //Popup
    if (this->getChildByTag(101) != nullptr) {
        this->removeChildByTag(101);
    }
    //Hexagon
    if (progressMap->getChildByTag(102) != nullptr) {
        progressMap->removeChildByTag(102);
    }
}

void UIProgressMap::menuEraCallback(Ref* pSender)
{
    auto arrowPrev = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowPrev");
    auto arrowNext = (MenuItem*)this->getChildByName("eraWindow")->getChildByName("menuEra")->getChildByName("arrowNext");
    if (progressMap->getChildByName("menuLevelButton")->getChildByTag(1) != NULL)
    {
        setEpisode2();
        arrowNext->setVisible(false);
        arrowPrev->setVisible(true);
    }
    else if (progressMap->getChildByName("menuLevelButton")->getChildByTag(11) != NULL)
    {
        setEpisode1();
        arrowNext->setVisible(true);
        arrowPrev->setVisible(false);
    }
}

void UIProgressMap::setEpisode1(void)
{
    this->removeChild(progressMap);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto label = (Label*)this->getChildByName("eraWindow")->getChildByName("eraLabel");
    label->setString(LocalizedString::create("FIRST") + " " + LocalizedString::create("ERA"));
    
    progressMap = Sprite::create("gui/ProgressMap1Background.jpg");
    progressMap->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    
    //CLOUDS
    auto cloud2 = Sprite::create("gui/Clouds2.png");
    cloud2->setPosition(Vec2((3.2 * progressMap->getBoundingBox().size.width / 4),
                             (0.95 * progressMap->getBoundingBox().size.height / 2)));
    progressMap->addChild(cloud2);
    auto movBy1 = MoveBy::create(10, Vec2(-15 * GameData::getInstance()->getRaConversion(),
                                          -15 * GameData::getInstance()->getRaConversion()));
    auto easeBy1 = EaseIn::create(movBy1, 1);
    auto movBy2 = MoveBy::create(15, Vec2(15 * GameData::getInstance()->getRaConversion(),
                                          15 * GameData::getInstance()->getRaConversion()));
    auto easeBy2 = EaseIn::create(movBy2, 2);
    auto seqC2 = Sequence::create(easeBy1, easeBy2, NULL);
    cloud2->runAction(RepeatForever::create(seqC2));
    
    auto cloud3 = Sprite::create("gui/Clouds3.png");
    cloud3->setPosition(Vec2(progressMap->getBoundingBox().size.width / 2, progressMap->getBoundingBox().size.height / 2));
    cloud3->setOpacity(0);
    progressMap->addChild(cloud3);
    auto fadeInC3 = FadeIn::create(15);
    auto fadeOutC3 = FadeOut::create(10);
    auto delayC32 = DelayTime::create(15);
    auto delayC3 = DelayTime::create(1.5);
    auto seqC3 = Sequence::create(delayC32, fadeInC3, delayC3, fadeOutC3, NULL);
    cloud3->runAction(RepeatForever::create(seqC3));
    
    auto cloud1 = Sprite::create("gui/Clouds1.png");
    cloud1->setPosition(Vec2(progressMap->getBoundingBox().size.width / 2, progressMap->getBoundingBox().size.height / 2));
    cloud1->setOpacity(0);
    progressMap->addChild(cloud1);
    auto fadeIn = FadeIn::create(35);
    auto move = MoveTo::create(60, Vec2(progressMap->getBoundingBox().size.width,
                                        (5 * progressMap->getBoundingBox().size.height / 8)));
    //auto ease = EaseIn::create(move, 1);
    auto spawn = Spawn::create(fadeIn, move, NULL);
    auto move2 = MoveTo::create(25, Vec2((progressMap->getBoundingBox().size.width * 1.5),
                                         (5 * progressMap->getBoundingBox().size.height / 8)));
    auto fadeOut = FadeOut::create(15);
    auto spawn2 = Spawn::create(fadeOut, move2, NULL);
    auto relocate = MoveTo::create(0.1, Vec2(progressMap->getBoundingBox().size.width / 2, progressMap->getBoundingBox().size.height / 2));
    auto delayC1 = DelayTime::create(5);
    auto seq = Sequence::create(spawn, spawn2, relocate, delayC1, NULL);
    cloud1->runAction(RepeatForever::create(seq));
    
    Vector<MenuItem *> levelButtonVec;
    for (int i = 1; i < 11; i++)
    {
        //COMPROVAR NIVELLS ACTIVATS
        
        auto levelButton = MenuItemImage::create("gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png",
                                            CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
        levelButton->setTag(i);
        auto levelLabel = Label::createWithTTF(to_string(i), "fonts/BebasNeue.otf", 40);
        levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
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
        setStars(i, levelButton->getPosition());
        levelButtonVec.pushBack(levelButton);
    }

    auto menuLevelButton = Menu::createWithArray(levelButtonVec);
    menuLevelButton->setPosition(Vec2(0, 0));
    menuLevelButton->setName("menuLevelButton");
    progressMap->addChild(menuLevelButton, 3, 0);
    
    this->addChild(progressMap);
}

void UIProgressMap::setEpisode2(void)
{
    this->removeChild(progressMap);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto label = (Label*)this->getChildByName("eraWindow")->getChildByName("eraLabel");
    label->setString(LocalizedString::create("SECOND") + " " + LocalizedString::create("ERA"));
    
    progressMap = Sprite::create("gui/ProgressMap1Background.jpg");
    progressMap->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    
    Vector<MenuItem *> levelButtonVec;
    for (int i = 11; i < 21; i++)
    {
        //COMPROVAR NIVELLS ACTIVATS
        
        auto levelButton = MenuItemImage::create("gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png",
                                                 CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
        levelButton->setTag(i);
        auto levelLabel = Label::createWithTTF(to_string(i), "fonts/BebasNeue.otf", 40);
        levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
        levelButton->addChild(levelLabel);
        levelLabel->setColor(Color3B(32, 47, 55));
        
        auto shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
        progressMap->addChild(shadow, 1);
        
        switch (i) {
            case 11: {
                levelButton->setPosition((72 * progressMap->getContentSize().width / 204), (32 * progressMap->getContentSize().height / 155));
                
                shadow->setPosition(Vec2(72 * progressMap->getContentSize().width / 204,
                                         (32 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 12: {
                levelButton->setPosition(68 * progressMap->getContentSize().width / 204, 54 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(68 * progressMap->getContentSize().width / 204,
                                         (54 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 13: {
                levelButton->setPosition(53 * progressMap->getContentSize().width / 204, 71 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(53 * progressMap->getContentSize().width / 204,
                                         (71 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 14: {
                levelButton->setPosition(43 * progressMap->getContentSize().width / 204, 71 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(43 * progressMap->getContentSize().width / 204,
                                         (71 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 15: {
                levelButton->setPosition(44 * progressMap->getContentSize().width / 204, 103 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(44 * progressMap->getContentSize().width / 204,
                                         (103 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 16: {
                levelButton->setPosition(66 * progressMap->getContentSize().width / 204, 91 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(66 * progressMap->getContentSize().width / 204,
                                         (91 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 17: {
                levelButton->setPosition(85 * progressMap->getContentSize().width / 204, 86 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(85 * progressMap->getContentSize().width / 204,
                                         (86 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 18: {
                levelButton->setPosition(95 * progressMap->getContentSize().width / 204, 67 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(95 * progressMap->getContentSize().width / 204,
                                         (67 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 19: {
                
                levelButton->setPosition(106 * progressMap->getContentSize().width / 204, 101 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(106 * progressMap->getContentSize().width / 204,
                                         (101 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
            case 20: {
                levelButton->setPosition(130 * progressMap->getContentSize().width / 204, 88 * progressMap->getContentSize().height / 155);
                
                shadow->setPosition(Vec2(130 * progressMap->getContentSize().width / 204,
                                         (88 * progressMap->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
                break;
            }
                
            default:
                break;
        }
        levelButtonVec.pushBack(levelButton);
    }
    
    auto menuLevelButton = Menu::createWithArray(levelButtonVec);
    menuLevelButton->setPosition(Vec2(0, 0));
    menuLevelButton->setName("menuLevelButton");
    progressMap->addChild(menuLevelButton, 3, 0);
    
    this->addChild(progressMap);

}

void UIProgressMap::setStars(int i, Vec2 pos)
{
    if (GameData::getInstance()->getLevelsCompleted().size() > i )
    {
        int score = GameData::getInstance()->getLevelScore(i);
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
            star->setPosition(Vec2(pos.x - star->getContentSize().width + (star->getContentSize().width * (i - 1)), pos.y + star->getContentSize().height * 1.8));
            progressMap->addChild(star);
        }
    }
}

