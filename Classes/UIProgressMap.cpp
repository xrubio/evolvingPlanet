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

    //Alliberar memòria
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    //Set background progress map and all its functionalities
    progressMap0 = Sprite::create("ProgressMap0Background.jpg");
    progressMap0->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    /*auto progressMap1 = Sprite::create("ProgressMap1Background.png");
    progressMap1->setPosition(Vec2(visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    auto progressMap2 = Sprite::create("ProgressMap2Background.png");
    progressMap2->setPosition(Vec2(2 * visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    this->addChild(progressMap, 0);*/

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    backButton->setPosition(Vec2((3 * progressMap0->getBoundingBox().size.width / 34),
        (2 * progressMap0->getBoundingBox().size.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    progressMap0->addChild(menu, 5);

    auto popupLevelBackground = ProgressTimer::create(Sprite::create("ProgressMapLevelPopupBackground.png"));
    auto popupLevelBorderTop = Sprite::create("ProgressMapLevelPopupBorderTop.png");
    popupLevelBorderTop->setAnchorPoint(Vec2(0, 0));
    //popupLevelBorderTop->setPosition(Vec2(0, popupLevelBackground->getBoundingBox().size.height));
    auto labelBorderTop = Label::createWithTTF(LocalizedString::create("FIRST_ERA")->getCString(), "fonts/BebasNeue.otf", 60);
    labelBorderTop->setColor(Color3B(Color3B::WHITE));
    labelBorderTop->setPosition(Vec2(popupLevelBorderTop->getBoundingBox().size.width / 2,
        popupLevelBorderTop->getBoundingBox().size.height / 2));
    popupLevelBorderTop->addChild(labelBorderTop);
    auto popupLevelBorderBottom = Sprite::create("ProgressMapLevelPopupBorderBottom.png");
    popupLevelBorderBottom->setAnchorPoint(Vec2(0, 1));
    //popupLevelBorderBottom->setPosition(Vec2(0, 0));
    //ADD ARROWS TO MENU ITEM
    auto arrowRight = MenuItemImage::create("ProgressMapArrowOn.png", "ProgressMapArrowOff.png", "ProgressMapArrowOff.png");
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
    popupLevelBackground->addChild(popupLevelBorderBottom);
    popupLevelBackground->setAnchorPoint(Vec2(0, 0.5));
    popupLevelBackground->setPosition(Vec2(0, progressMap0->getBoundingBox().size.height / 2));
    
    popupLevelBorderTop->setPosition(Vec2(0, popupLevelBackground->getBoundingBox().size.height / 2));
    popupLevelBorderBottom->setPosition(Vec2(0, popupLevelBackground->getBoundingBox().size.height / 2));

    Vector<MenuItem*> levelButtons;

    for (int i = 0; i < 10; i++) {
        auto buttonLevel = MenuItem::create(CC_CALLBACK_1(UIProgressMap::menuLevelZoneCallback, this));
        buttonLevel->setTag(i + 1);
        buttonLevel->setContentSize(Size(popupLevelBackground->getBoundingBox().size.width,
            popupLevelBackground->getBoundingBox().size.height / 22));
        string space = " ";
        auto labelButtonLevel = Label::createWithTTF(LocalizedString::create("LEVEL")->getCString() + space + to_string(i + 1),
            "fonts/BebasNeue.otf", 45);
        labelButtonLevel->setColor(Color3B::WHITE);
        labelButtonLevel->setPosition(Vec2(2 * buttonLevel->getBoundingBox().size.width / 4, buttonLevel->getBoundingBox().size.height / 2));
        labelButtonLevel->setOpacity(0);
        labelButtonLevel->runAction(Sequence::create(DelayTime::create(1.1), FadeIn::create(0.4), NULL));
        buttonLevel->addChild(labelButtonLevel);
        auto hexagonButtonLevel = Sprite::create("ProgressMapHexagonLevelOff.png");
        hexagonButtonLevel->setPosition(Vec2(buttonLevel->getBoundingBox().size.width / 4, buttonLevel->getBoundingBox().size.height / 2));
        hexagonButtonLevel->setOpacity(0);
        hexagonButtonLevel->runAction(Sequence::create(DelayTime::create(1.1), FadeIn::create(0.4), NULL));
        buttonLevel->addChild(hexagonButtonLevel);
        if (i > 4) {
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
                starFile = "StarFullMini.png";
            }
            //Estrella buida
            else {
                starFile = "StarEmptyMini.png";
            }
            auto star = Sprite::create(starFile);
            star->setPosition(Vec2((4.65 * buttonLevel->getBoundingBox().size.width / 7) + (i * (star->getContentSize().width + star->getContentSize().width / 4)),
                buttonLevel->getBoundingBox().size.height / 2));
            star->setOpacity(0);
            star->runAction(Sequence::create(DelayTime::create(1.1), FadeIn::create(0.4), NULL));
            buttonLevel->addChild(star);
        }
        buttonLevel->setPosition(Vec2(2 * popupLevelBackground->getBoundingBox().size.width / 5,
            (20 - (i * 2)) * popupLevelBackground->getBoundingBox().size.height / 22));
        levelButtons.pushBack(buttonLevel);
    }
    auto menuLevelButtons = Menu::createWithArray(levelButtons);
    menuLevelButtons->setPosition(Vec2(0, 0));
    popupLevelBackground->addChild(menuLevelButtons);
    
    ProgressFromTo* horitzontalTimer = ProgressFromTo::create(0.6, 0, 100);
    popupLevelBackground->setType(ProgressTimer::Type::BAR);
    popupLevelBackground->setBarChangeRate(Vec2(0, 1));
    popupLevelBackground->setMidpoint(Vec2(0.5, 0.5));
    popupLevelBackground->runAction(Sequence::create(DelayTime::create(0.6), horitzontalTimer, NULL));
    
    auto moveUp = MoveTo::create(0.6, Vec2(0, popupLevelBackground->getBoundingBox().size.height));
    popupLevelBorderTop->runAction(Sequence::create(DelayTime::create(0.6), moveUp, NULL));
    
    auto moveDown = MoveTo::create(0.6, Vec2(0, 0));
    popupLevelBorderBottom->runAction(Sequence::create(DelayTime::create(0.6), moveDown, NULL));
    
    progressMap0->addChild(popupLevelBackground, 8);

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
    auto cloud2 = Sprite::create("Clouds2.png");
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

    auto cloud3 = Sprite::create("Clouds3.png");
    cloud3->setPosition(Vec2(progressMap0->getBoundingBox().size.width / 2, progressMap0->getBoundingBox().size.height / 2));
    cloud3->setOpacity(0);
    progressMap0->addChild(cloud3);
    auto fadeInC3 = FadeIn::create(15);
    auto fadeOutC3 = FadeOut::create(10);
    auto delayC32 = DelayTime::create(15);
    auto delayC3 = DelayTime::create(1.5);
    auto seqC3 = Sequence::create(delayC32, fadeInC3, delayC3, fadeOutC3, NULL);
    cloud3->runAction(RepeatForever::create(seqC3));

    auto cloud1 = Sprite::create("Clouds1.png");
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
                "LevelPointerButton.png", "LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition((80 * progressMap0->getContentSize().width / 204), (32 * progressMap0->getContentSize().height / 155));
            levelButton->setTag(1);
            auto levelLabel = Label::createWithTTF("1", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(80 * progressMap0->getContentSize().width / 204,
                (32 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            break;
        }
        case 2: {
            levelButton = MenuItemImage::create(
                "LevelPointerButton.png", "LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(76 * progressMap0->getContentSize().width / 204, 54 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(2);
            auto levelLabel = Label::createWithTTF("2", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(76 * progressMap0->getContentSize().width / 204,
                (54 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            break;
        }
        case 3: {
            levelButton = MenuItemImage::create(
                "LevelPointerButton.png", "LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(55 * progressMap0->getContentSize().width / 204, 71 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(3);
            auto levelLabel = Label::createWithTTF("3", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(55 * progressMap0->getContentSize().width / 204,
                (71 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            break;
        }
        case 4: {
            levelButton = MenuItemImage::create(
                "LevelPointerButton.png", "LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(55 * progressMap0->getContentSize().width / 204, 71 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(4);
            auto levelLabel = Label::createWithTTF("4", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(55 * progressMap0->getContentSize().width / 204,
                (71 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
            break;
        }
        case 5: {
            levelButton = MenuItemImage::create(
                "LevelPointerButton.png", "LevelPointerButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
            levelButton->setPosition(52 * progressMap0->getContentSize().width / 204, 103 * progressMap0->getContentSize().height / 155);
            levelButton->setTag(5);
            auto levelLabel = Label::createWithTTF("5", "fonts/BebasNeue.otf", 40);
            levelLabel->setColor(Color3B(32, 47, 55));
            levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
            levelButton->addChild(levelLabel);

            shadow = Sprite::create("LevelPointerButtonShadow.png");
            shadow->setPosition(Vec2(52 * progressMap0->getContentSize().width / 204,
                (103 * progressMap0->getContentSize().height / 155) - (levelButton->getContentSize().height / 1.5)));
            progressMap0->addChild(shadow, 1, tag);
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
        auto zoneTimer = ProgressTimer::create(Sprite::create("ZoneAreaLevel.png"));
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

    auto darkBackground = MenuItemImage::create("ProgressMapDarkBackground.png", "ProgressMapDarkBackground.png",
        CC_CALLBACK_1(UIProgressMap::restoreProgressMap, this));
    darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    menuButtons.pushBack(darkBackground);

    auto popupBackground = Sprite::create("ProgressMapPopupBackground.png");
    popupBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + (visibleSize.height / 2)));
    this->addChild(popupBackground, 20, 101);

    string space = " ";
    string lvl = LocalizedString::create("LEVEL")->getCString() + space + to_string(tag);
    auto levelLabel = Label::createWithTTF(lvl, "fonts/BebasNeue.otf", 100);
    levelLabel->setColor(Color3B(85, 108, 117));
    levelLabel->setAnchorPoint(Vec2(0, 0.5));
    levelLabel->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 12 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(levelLabel);

    LevelLoader loader;
    auto mapPopup = Sprite::create(loader.getLevelFileMap("level" + to_string(tag)) + "Background.jpg");
    mapPopup->setScale(0.21);
    mapPopup->setAnchorPoint(Vec2(0, 0.5));
    mapPopup->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(mapPopup);

    vector<string> goalTypes = loader.getGoalTypes("level" + to_string(tag));
    for (int i = 0; i < goalTypes.size(); i++) {
        Sprite* iconLevel;
        if (goalTypes[i] == "Expansion") {
            iconLevel = Sprite::create("ExpansionGoalIcon.png");
        }
        else if (goalTypes[i] == "Collection") {
            iconLevel = Sprite::create("CollectionGoalIcon.png");
        }
        iconLevel->setAnchorPoint(Vec2(1, 0.75));
        iconLevel->setPosition(Vec2(mapPopup->getPosition().x + mapPopup->getBoundingBox().size.width - ((iconLevel->getContentSize().width * i) + (iconLevel->getContentSize().width / 10) * i),
            mapPopup->getPosition().y - (mapPopup->getBoundingBox().size.height / 2)));
        popupBackground->addChild(iconLevel);
    }

    auto briefText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("BRIEF_LEVEL_" + to_string(tag)).c_str())->getCString(),
        Size(13 * (popupBackground->getContentSize().width / 25), 6 * (popupBackground->getContentSize().height / 14)),
        TextHAlignment::LEFT, "Corbel", 28);
    briefText->setColorSpaceHolder(Color3B(216, 229, 235));
    briefText->setPosition(Vec2(17 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(briefText);

    auto playButton = MenuItemImage::create("ProgressMapPlayButton.png", "ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIProgressMap::proceedLevelCallback, this));
    playButton->setPosition(Vec2(21 * (popupBackground->getContentSize().width / 25), 2 * (popupBackground->getContentSize().height / 14)));
    playButton->setTag(tag);
    auto labelPlay = Label::createWithTTF(LocalizedString::create("PLAY")->getCString(), "fonts/BebasNeue.otf", 50);
    labelPlay->setPosition(playButton->getContentSize().width / 2, playButton->getContentSize().height / 2);
    labelPlay->setColor(Color3B(205, 202, 207));
    playButton->addChild(labelPlay);
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(0, 0);
    popupBackground->addChild(playMenu);

    int score = 0;
    if (GameData::getInstance()->getLevelsCompleted().size() > tag) {
        score = GameData::getInstance()->getLevelScore(tag);
    }
    cout << "SCORE : " << score << endl;
    for (int i = 1; i < 4; i++) {
        string starFile;
        //Estrella plena
        if (score >= i) {
            starFile = "StarFull.png";
        }
        //Estrella buida
        else {
            starFile = "StarEmpty.png";
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

    auto delayPopup = DelayTime::create(0.4);
    auto popupMoveTo = MoveTo::create(1, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto popupEase = EaseBackOut::create(popupMoveTo);
    auto seqShowPopup = Sequence::create(delayPopup, popupEase, NULL);
    popupBackground->runAction(seqShowPopup);

    //PAINT TROPHY
    if (score > 0) {
        auto trophy = Sprite::create("TrophyIcon.png");
        trophy->setPosition(Vec2(22 * (popupBackground->getContentSize().width / 25), 12 * (popupBackground->getContentSize().height / 14)));
        popupBackground->addChild(trophy);
        trophy->setOpacity(0);
        trophy->setScale(3);
        auto trophyScale = ScaleTo::create(0.5, 1);
        auto trophyFade = FadeIn::create(0.1);
        auto trophyRotate = RotateBy::create(0.3, 360);
        auto trophySpawn = Spawn::create(trophyScale, trophyFade, Repeat::create(trophyRotate, 2), NULL);
        trophy->runAction(Sequence::create(DelayTime::create(1.6), trophySpawn, NULL));
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
        GameLevel::getInstance()->agentPixelSize = 2;
        break;
    case 2:
        filename = "level2";
        GameLevel::getInstance()->agentPixelSize = 2;
        break;
    case 3:
        filename = "level3";
        GameLevel::getInstance()->agentPixelSize = 1;
        break;
    case 4:
        filename = "level4";
        GameLevel::getInstance()->agentPixelSize = 2;
        break;
    case 5:
        filename = "level5";
        GameLevel::getInstance()->agentPixelSize = 2;
        break;
    case 6:
        filename = "level6";
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
