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

    //Alliberar memòria
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    //Set background progress map and all its functionalities
    progressMap0 = Sprite::create("gui/ProgressMap0Background.jpg");
    progressMap0->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    /*auto progressMap1 = Sprite::create("ProgressMap1Background.png");
    progressMap1->setPosition(Vec2(visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    auto progressMap2 = Sprite::create("ProgressMap2Background.png");
    progressMap2->setPosition(Vec2(2 * visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    this->addChild(progressMap, 0);*/

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    backButton->setPosition(Vec2((3 * progressMap0->getBoundingBox().size.width / 34),
        (2 * progressMap0->getBoundingBox().size.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    progressMap0->addChild(menu, 5);

    auto popupLevelBackground = Sprite::create("gui/ProgressMapLevelPopupBackground.png");
    //ADD ARROWS TO MENU ITEM
    /*auto arrowRight = MenuItemImage::create("ProgressMapArrowOn.png", "ProgressMapArrowOff.png", "ProgressMapArrowOff.png");
    arrowRight->setPosition(Vec2(5 * popupLevelBorderBottom->getBoundingBox().size.width / 6,
        popupLevelBorderBottom->getBoundingBox().size.height / 2));
    popupLevelBorderBottom->addChild(arrowRight);
    auto arrowLeft = MenuItemImage::create("ProgressMapArrowOn.png", "ProgressMapArrowOff.png", "ProgressMapArrowOff.png");
    arrowLeft->setEnabled(false);
    arrowLeft->setRotation(180);
    arrowLeft->setPosition(Vec2(1 * popupLevelBorderBottom->getBoundingBox().size.width / 6,
        popupLevelBorderBottom->getBoundingBox().size.height / 2));
    popupLevelBorderBottom->addChild(arrowLeft);
    popupLevelBackground->addChild(popupLevelBorderTop);
    popupLevelBackground->addChild(popupLevelBorderBottom);*/
    
    auto labelBorderTop = Label::createWithTTF(LocalizedString::create("FIRST_ERA").c_str(), "fonts/BebasNeue.otf", 60);
    labelBorderTop->setColor(Color3B(Color3B::WHITE));
    labelBorderTop->setPosition(Vec2(popupLevelBackground->getBoundingBox().size.width / 2,
                                     (popupLevelBackground->getContentSize().height + popupLevelBackground->getContentSize().height / 24.3) -  (popupLevelBackground->getContentSize().height / 12.15)));
    popupLevelBackground->addChild(labelBorderTop);
    
    popupLevelBackground->setAnchorPoint(Vec2(0, 0.5));
    popupLevelBackground->setPosition(Vec2(progressMap0->getBoundingBox().size.width, progressMap0->getBoundingBox().size.height / 2));
    
    //popupLevelBorderTop->setPosition(Vec2(0, popupLevelBackground->getBoundingBox().size.height / 2));
    //popupLevelBorderBottom->setPosition(Vec2(0, popupLevelBackground->getBoundingBox().size.height / 2));
    
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
    
    popupLevelBackground->runAction(EaseBackInOut::create(MoveTo::create(1.0, Vec2(progressMap0->getBoundingBox().size.width - popupLevelBackground->getBoundingBox().size.width, popupLevelBackground->getPositionY()))));
    
    progressMap0->addChild(popupLevelBackground, 8, 103);

    this->addChild(progressMap0);

    /*auto scollFrameSize = Size(visibleSize.width, visibleSize.height);
    scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    scrollView->setBackGroundColor(Color3B(200, 200, 200));
    scrollView->setSize(scollFrameSize);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

    scrollView->addChild(progressMap0, 0);
    //scrollView->addChild(progressMap1, 1);
    //scrollView->addChild(progressMap2, 2);
    this->addChild(scrollView, 3);*/

    //CLOUDS
    auto cloud2 = Sprite::create("gui/Clouds2.png");
    cloud2->setPosition(Vec2((3 * progressMap0->getBoundingBox().size.width / 4),
        (progressMap0->getBoundingBox().size.height / 2)));
    progressMap0->addChild(cloud2);
    auto movBy1 = MoveBy::create(10, Vec2(-15 * GameData::getInstance()->getRaConversion(),
                                         -15 * GameData::getInstance()->getRaConversion()));
    auto easeBy1 = EaseIn::create(movBy1, 1);
    auto movBy2 = MoveBy::create(15, Vec2(15 * GameData::getInstance()->getRaConversion(),
                                         15 * GameData::getInstance()->getRaConversion()));
    auto easeBy2 = EaseIn::create(movBy2, 2);
    auto seqC2 = Sequence::create(easeBy1, easeBy2, NULL);
    cloud2->runAction(RepeatForever::create(seqC2));

    auto cloud3 = Sprite::create("gui/Clouds3.png");
    cloud3->setPosition(Vec2(progressMap0->getBoundingBox().size.width / 2, progressMap0->getBoundingBox().size.height / 2));
    cloud3->setOpacity(0);
    progressMap0->addChild(cloud3);
    auto fadeInC3 = FadeIn::create(15);
    auto fadeOutC3 = FadeOut::create(10);
    auto delayC32 = DelayTime::create(15);
    auto delayC3 = DelayTime::create(1.5);
    auto seqC3 = Sequence::create(delayC32, fadeInC3, delayC3, fadeOutC3, NULL);
    cloud3->runAction(RepeatForever::create(seqC3));

    auto cloud1 = Sprite::create("gui/Clouds1.png");
    cloud1->setPosition(Vec2(progressMap0->getBoundingBox().size.width / 2, progressMap0->getBoundingBox().size.height / 2));
    cloud1->setOpacity(0);
    progressMap0->addChild(cloud1);
    auto fadeIn = FadeIn::create(35);
    auto move = MoveTo::create(60, Vec2(progressMap0->getBoundingBox().size.width,
                                       (5 * progressMap0->getBoundingBox().size.height / 8)));
    //auto ease = EaseIn::create(move, 1);
    auto spawn = Spawn::create(fadeIn, move, NULL);
    auto move2 = MoveTo::create(25, Vec2((progressMap0->getBoundingBox().size.width * 1.5),
                                        (5 * progressMap0->getBoundingBox().size.height / 8)));
    auto fadeOut = FadeOut::create(15);
    auto spawn2 = Spawn::create(fadeOut, move2, NULL);
    auto relocate = MoveTo::create(0.1, Vec2(progressMap0->getBoundingBox().size.width / 2, progressMap0->getBoundingBox().size.height / 2));
    auto delayC1 = DelayTime::create(5);
    auto seq = Sequence::create(spawn, spawn2, relocate, delayC1, NULL);
    cloud1->runAction(RepeatForever::create(seq));

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

void UIProgressMap::menuLevelZoneCallback(Ref* pSender)
{
    auto pMenuItem = (MenuItem*)(pSender);
    bool doNothing = false;
    int tag = pMenuItem->getTag();
    MenuItemImage* levelButton;
    Sprite* shadow;
    if (progressMap0->getChildByTag(tag) == nullptr and tag != tagLevelSelected) {
        selectedBackground->setPosition(pMenuItem->getPosition());
        selectedBackground->setVisible(true);
        tagLevelSelected = tag;
        //Flags
        for (int i = 0; i < 11; i++) {
            if (progressMap0->getChildByTag(i) != nullptr) {
                progressMap0->removeChildByTag(i);
            }
        }
        //Hexagon
        if (progressMap0->getChildByTag(102) != nullptr) {
            progressMap0->removeChildByTag(102);
        }
        restoreProgressMap(nullptr);
        switch (tag) {
        case 1: {
            levelButton = MenuItemImage::create(
                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition((72 * progressMap0->getContentSize().width / 204), (32 * progressMap0->getContentSize().height / 155));
            levelButton->setTag(1);
            auto levelLabel = Label::createWithTTF("1", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(72 * progressMap0->getContentSize().width / 204,
                (32 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level1Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level1HotSpotsBase.png");
            break;
        }
        case 2: {
            levelButton = MenuItemImage::create(
                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(68 * progressMap0->getContentSize().width / 204, 54 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(2);
            auto levelLabel = Label::createWithTTF("2", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(68 * progressMap0->getContentSize().width / 204,
                (54 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level2Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level2HotSpotsBase.png");
            break;
        }
        case 3: {
            levelButton = MenuItemImage::create(
                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(47 * progressMap0->getContentSize().width / 204, 71 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(3);
            auto levelLabel = Label::createWithTTF("3", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(47 * progressMap0->getContentSize().width / 204,
                (71 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level3Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level3HotSpotsBase.png");
            break;
        }
        case 4: {
            levelButton = MenuItemImage::create(
                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(47 * progressMap0->getContentSize().width / 204, 71 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(4);
            auto levelLabel = Label::createWithTTF("4", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(47 * progressMap0->getContentSize().width / 204,
                (71 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level3Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level3HotSpotsBase.png");
            break;
        }
        case 5: {
            levelButton = MenuItemImage::create(
                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(44 * progressMap0->getContentSize().width / 204, 103 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(5);
            auto levelLabel = Label::createWithTTF("5", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(44 * progressMap0->getContentSize().width / 204,
                (103 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }
        case 6: {
            levelButton = MenuItemImage::create(
                                                "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(66 * progressMap0->getContentSize().width / 204, 91 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(6);
            auto levelLabel = Label::createWithTTF("6", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);
            
            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(66 * progressMap0->getContentSize().width / 204,
                                        (91 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }
        case 7: {
            levelButton = MenuItemImage::create(
                                                    "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(85 * progressMap0->getContentSize().width / 204, 86 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(7);
            auto levelLabel = Label::createWithTTF("7", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);
            
            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(85 * progressMap0->getContentSize().width / 204,
                                        (86 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }
        case 8: {
            levelButton = MenuItemImage::create(
                                                    "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(95 * progressMap0->getContentSize().width / 204, 67 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(8);
            auto levelLabel = Label::createWithTTF("8", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);
            
            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(95 * progressMap0->getContentSize().width / 204,
                                         (67 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }
        case 9: {
            levelButton = MenuItemImage::create(
                                                    "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(106 * progressMap0->getContentSize().width / 204, 101 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(9);
            auto levelLabel = Label::createWithTTF("9", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);
                
            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(106 * progressMap0->getContentSize().width / 204,
                                         (101 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }
        case 10: {
            levelButton = MenuItemImage::create(
                                                    "gui/LevelPointerButton.png", "gui/LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(130 * progressMap0->getContentSize().width / 204, 88 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(10);
            auto levelLabel = Label::createWithTTF("10", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);
                
            shadow = Sprite::create("gui/LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(130 * progressMap0->getContentSize().width / 204,
                                         (88 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            //Director::getInstance()->getTextureCache()->addImage("Level5Background.jpg");
            //Director::getInstance()->getTextureCache()->addImage("Level5HotSpotsBase.png");
            break;
        }

        default:
            doNothing = true;
            break;
        }
    }
    else {
        doNothing = true;
    }

    if (doNothing == false) {
        auto menuLevelButton = Menu::createWithItem(levelButton);
        menuLevelButton->setPosition(Vec2(0, 0));
        progressMap0->addChild(menuLevelButton, 3, 0);

        Vec2 pos = levelButton->getPosition();
        levelButton->setPositionY(progressMap0->getBoundingBox().size.height);
        auto fall = MoveTo::create(0.6, pos);
        auto fallEase = EaseBackOut::create(fall);
        levelButton->runAction(fallEase);

        auto dropActionLevelButton = MoveBy::create(1.5, Vec2(0, 5));
        auto easeDropActionLevelButton = EaseOut::create(dropActionLevelButton, 1);
        auto dropActionLevelButton2 = MoveBy::create(1.5, Vec2(0, -5));
        auto easeDropActionLevelButton2 = EaseOut::create(dropActionLevelButton2, 1);
        auto seqDrop = Sequence::create(easeDropActionLevelButton, easeDropActionLevelButton2, NULL);
        auto actionRep = RepeatForever::create(seqDrop);
        levelButton->runAction(actionRep);

        ProgressFromTo* radialTimer = ProgressFromTo::create(0.5, 0, 100);
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
        zoneTimer->setPosition(Vec2(pos.x, pos.y - (levelButton->getContentSize().height / 4)));
        zoneTimer->setType(ProgressTimer::Type::RADIAL);
        auto wait = DelayTime::create(0.4);
        zoneTimer->runAction(Sequence::create(wait, radialTimer, NULL));
        progressMap0->addChild(zoneTimer, 1, 102);
    }
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
    auto seqShowPopup = Sequence::create(popupEase, NULL);
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
}

void UIProgressMap::movePopupLevelCallback(Ref* pSender)
{
    MenuItem* p = (MenuItem*)pSender;
    Sprite* background = (Sprite*)p->getParent()->getParent();
    if (background->getNumberOfRunningActions() == 0)
    {
    if (p->getParent()->getParent()->getPositionX() < progressMap0->getBoundingBox().size.width) {
        //VISIBLE
        auto move = MoveTo::create(0.5, Vec2(progressMap0->getBoundingBox().size.width, background->getPositionY()));
        auto ease = EaseBackInOut::create(move);
        background->runAction(ease);
        background->removeChildByName("menuArrow");
        auto arrowExpand = MenuItemImage::create("gui/ArrowExpand.png", "gui/ArrowExpandPressed.png",
                                                 CC_CALLBACK_1(UIProgressMap::movePopupLevelCallback, this));
        arrowExpand->setAnchorPoint(Vec2(1, 0));
        arrowExpand->setPosition(0, 0);
        Menu* arrowMenu = Menu::createWithItem(arrowExpand);
        arrowMenu->setPosition(Vec2(0, (background->getContentSize().height + background->getContentSize().height / 24.3) -
                                    (11.5 * (background->getContentSize().height / 12.15))));
        arrowMenu->setName("menuArrow");
        background->addChild(arrowMenu);
    }
    else {
        //INVISIBLE
        auto move = MoveTo::create(0.5, Vec2(progressMap0->getBoundingBox().size.width - background->getBoundingBox().size.width, background->getPositionY()));
        auto ease = EaseBackInOut::create(move);
        background->runAction(ease);
        background->removeChildByName("menuArrow");
        auto arrowRetract = MenuItemImage::create("gui/ArrowRetract.png", "gui/ArrowRetractPressed.png",
                                                  CC_CALLBACK_1(UIProgressMap::movePopupLevelCallback, this));
        arrowRetract->setAnchorPoint(Vec2(1, 0));
        arrowRetract->setPosition(0, 0);
        Menu* arrowMenu = Menu::createWithItem(arrowRetract);
        arrowMenu->setPosition(Vec2(0, (background->getContentSize().height + background->getContentSize().height / 24.3) -
                                    (11.5 * (background->getContentSize().height / 12.15))));
        arrowMenu->setName("menuArrow");
        background->addChild(arrowMenu);
    }
    }
}


