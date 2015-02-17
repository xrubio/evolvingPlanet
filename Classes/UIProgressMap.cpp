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
#include "SlidingMenu.h"
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

    if (GameData::getInstance()->getMusic() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("intro_body1.mp3", true);
    }

    //Alliberar memòria
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    //Set background progress map and all its functionalities
    progressMap0 = Sprite::create("ProgressMap0Background.png");
    progressMap0->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto progressMap1 = Sprite::create("ProgressMap1Background.png");
    progressMap1->setPosition(Vec2(visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    auto progressMap2 = Sprite::create("ProgressMap2Background.png");
    progressMap2->setPosition(Vec2(2 * visibleSize.width + (visibleSize.width / 2), visibleSize.height / 2));
    //this->addChild(progressMap, 0);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    backButton->setPosition(Vec2(3 * (visibleSize.width / 34), 2 * (visibleSize.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 5);

    Vector<MenuItem*> levelButtons;
    //LEVEL 1
    auto level1Button = MenuItemImage::create(
        "LevelPointerButton.png", "LevelPointerButton.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level1Button->setPosition(71 * visibleSize.width / 204, 50 * visibleSize.height / 155);
    level1Button->setTag(1);
    levelButtons.pushBack(level1Button);
    auto level1Label = Label::createWithTTF("1", "fonts/BebasNeue.otf", 40);
    level1Label->setColor(Color3B(32, 47, 55));
    level1Label->setPosition(level1Button->getContentSize().width / 2, level1Button->getContentSize().height / 2);
    level1Button->addChild(level1Label);

    auto shadow1 = Sprite::create("LevelPointerButtonShadow.png");
    shadow1->setPosition(Vec2((71 * visibleSize.width / 204),
                              (50 * visibleSize.height / 155) - (level1Button->getContentSize().height / 1.5)));
    progressMap0->addChild(shadow1);

    //LEVEL 2
    auto level2Button = MenuItemImage::create(
        "LevelPointerButton.png", "LevelPointerButton.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level2Button->setPosition(62 * visibleSize.width / 204, 59 * visibleSize.height / 155);
    level2Button->setTag(2);
    levelButtons.pushBack(level2Button);
    auto level2Label = Label::createWithTTF("2", "fonts/BebasNeue.otf", 40);
    level2Label->setColor(Color3B(32, 47, 55));
    level2Label->setPosition(level2Button->getContentSize().width / 2, level2Button->getContentSize().height / 2);
    level2Button->addChild(level2Label);

    auto shadow2 = Sprite::create("LevelPointerButtonShadow.png");
    shadow2->setPosition(Vec2((62 * visibleSize.width / 204),
                              (59 * visibleSize.height / 155) - (level2Button->getContentSize().height / 1.5)));
    progressMap0->addChild(shadow2);

    //LEVEL 3
    auto level3Button = MenuItemImage::create(
        "LevelPointerButton.png", "LevelPointerButton.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level3Button->setPosition(48 * visibleSize.width / 204, 64 * visibleSize.height / 155);
    level3Button->setTag(3);
    levelButtons.pushBack(level3Button);
    auto level3Label = Label::createWithTTF("3", "fonts/BebasNeue.otf", 40);
    level3Label->setColor(Color3B(32, 47, 55));
    level3Label->setPosition(level3Button->getContentSize().width / 2, level3Button->getContentSize().height / 2);
    level3Button->addChild(level3Label);

    auto shadow3 = Sprite::create("LevelPointerButtonShadow.png");
    shadow3->setPosition(Vec2((48 * visibleSize.width / 204),
                              (64 * visibleSize.height / 155) - (level3Button->getContentSize().height / 1.5)));
    progressMap0->addChild(shadow3);

    for (int i = 0; i < levelButtons.size(); i++) {
        auto dropActionLevelButton = MoveBy::create(1.5, Vec2(0, 5));
        auto easeDropActionLevelButton = EaseOut::create(dropActionLevelButton, 1);
        auto dropActionLevelButton2 = MoveBy::create(1.5, Vec2(0, -5));
        auto easeDropActionLevelButton2 = EaseOut::create(dropActionLevelButton2, 1);
        auto seqDrop = Sequence::create(easeDropActionLevelButton, easeDropActionLevelButton2, NULL);
        auto actionRep = RepeatForever::create(seqDrop);
        levelButtons.at(i)->runAction(actionRep);
    }

    auto level = Menu::createWithArray(levelButtons);
    level->setPosition(0, 0);
    progressMap0->addChild(level, 3);

    /*Vector<MenuItem*> level1Buttons;
    auto level1Button = MenuItemImage::create(
        "Level1Button.png", "Level1ButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level1Button->setPosition(288, 180);
    //level1Button->setTag(1);
    level1Buttons.pushBack(level1Button);
    auto level1 = Menu::createWithArray(level1Buttons);
    level1->setPosition(0, 0);
    progressMap1->addChild(level1, 1);

    auto level21Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level21Button->setPosition(688, 980);
    level21Button->setTag(2);
    auto level21Label = Label::createWithSystemFont("2.1", "Arial", 40);
    level21Label->setPosition(level21Button->getContentSize().width / 2, level21Button->getContentSize().height / 2);
    level21Button->addChild(level21Label);

    auto level211Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level211Button->setPosition(988, 980);
    level211Button->setTag(3);
    auto level211Label = Label::createWithSystemFont("2.1.1", "Arial", 40);
    level211Label->setPosition(level211Button->getContentSize().width / 2, level211Button->getContentSize().height / 2);
    level211Button->addChild(level211Label);

    auto level212Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level212Button->setPosition(1288, 980);
    level212Button->setTag(4);
    auto level212Label = Label::createWithSystemFont("2.1.2", "Arial", 40);
    level212Label->setPosition(level212Button->getContentSize().width / 2, level212Button->getContentSize().height / 2);
    level212Button->addChild(level212Label);

    auto level3Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level3Button->setPosition(688, 680);
    level3Button->setTag(5);
    auto level3Label = Label::createWithSystemFont("3", "Arial", 40);
    level3Label->setPosition(level3Button->getContentSize().width / 2, level3Button->getContentSize().height / 2);
    level3Button->addChild(level3Label);

    auto level4Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level4Button->setPosition(388, 680);
    level4Button->setTag(6);
    auto level4Label = Label::createWithSystemFont("4", "Arial", 40);
    level4Label->setPosition(level4Button->getContentSize().width / 2, level4Button->getContentSize().height / 2);
    level4Button->addChild(level4Label);

    Vector<MenuItem*> level2Buttons;
    level2Buttons.pushBack(level21Button);
    level2Buttons.pushBack(level211Button);
    level2Buttons.pushBack(level212Button);
    level2Buttons.pushBack(level3Button);
    level2Buttons.pushBack(level4Button);

    auto level2 = Menu::createWithArray(level2Buttons);
    level2->setPosition(0, 0);
    progressMap2->addChild(level2);*/

    auto scollFrameSize = Size(visibleSize.width, visibleSize.height);
    scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    scrollView->setBackGroundColor(Color3B(200, 200, 200));
    scrollView->setSize(scollFrameSize);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

    scrollView->addChild(progressMap0, 0);
    //scrollView->addChild(progressMap1, 1);
    //scrollView->addChild(progressMap2, 2);
    this->addChild(scrollView, 3);

    /*
    PageView* pageView = PageView::create();
    pageView->setTouchEnabled(true);
    pageView->setSize(Size(visibleSize.width,visibleSize.height));
    pageView->setPosition(Point(0,0));
    
    Layout* layout = Layout::create();
    layout->setSize(Size(visibleSize.width,visibleSize.height));
    ImageView* imageView = ImageView::create();
    imageView->setTouchEnabled(true);
    //imageView->setScale9Enabled(true);
    imageView->loadTexture("ProgressMapBackground.png");
    imageView->setSize(Size(visibleSize.width,visibleSize.height));
    imageView->setPosition(Point(layout->getSize().width / 2, layout->getSize().height / 2));
    layout->addChild(imageView);
    
    MenuItem* level1Button = MenuItemImage::create(
                                                   "Level1Button.png", "Level1ButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevel1Callback, this));
    level1Button->setPosition(288,180);
    Vector<MenuItem*> level1Buttons;
    level1Buttons.pushBack(level1Button);
    Menu* level1 = Menu::createWithArray(level1Buttons);
    level1->setPosition(0,0);
    layout->addChild(level1);
    
    pageView->addPage(layout);
    
    Layout* layout2 = Layout::create();
    layout2->setSize(Size(visibleSize.width,visibleSize.height));
    ImageView* imageView2 = ImageView::create();
    imageView2->setTouchEnabled(true);
    //imageView2->setScale9Enabled(true);
    imageView2->loadTexture("ProgressMap2Background.png");
    imageView->setSize(Size(visibleSize.width,visibleSize.height));
    imageView2->setPosition(Point(layout2->getSize().width / 2, layout2->getSize().height / 2));
    layout2->addChild(imageView2);
    pageView->addPage(layout2);

    this->addChild(pageView);
    
    Vector<MenuItem*> menuButtons;
    MenuItem* backButton = MenuItemImage::create(
                                                 "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    menuButtons.pushBack(backButton);
    
    Menu* menu = Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                           origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);
    */

    //CLOUDS
    auto cloud2 = Sprite::create("Clouds2.png");
    cloud2->setPosition(Vec2(3 * visibleSize.width / 4, visibleSize.height / 2));
    progressMap0->addChild(cloud2);
    auto movBy1 = MoveBy::create(10, Vec2(-15, -15));
    auto easeBy1 = EaseIn::create(movBy1, 1);
    auto movBy2 = MoveBy::create(15, Vec2(15, 15));
    auto easeBy2 = EaseIn::create(movBy2, 2);
    auto seqC2 = Sequence::create(easeBy1, easeBy2, NULL);
    cloud2->runAction(RepeatForever::create(seqC2));

    auto cloud3 = Sprite::create("Clouds3.png");
    cloud3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    cloud3->setOpacity(0);
    progressMap0->addChild(cloud3);
    auto fadeInC3 = FadeIn::create(15);
    auto fadeOutC3 = FadeOut::create(10);
    auto delayC32 = DelayTime::create(15);
    auto delayC3 = DelayTime::create(1.5);
    auto seqC3 = Sequence::create(delayC32, fadeInC3, delayC3, fadeOutC3, NULL);
    cloud3->runAction(RepeatForever::create(seqC3));

    auto cloud1 = Sprite::create("Clouds1.png");
    cloud1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    cloud1->setOpacity(0);
    progressMap0->addChild(cloud1);
    auto fadeIn = FadeIn::create(35);
    auto move = MoveTo::create(60, Vec2(visibleSize.width, 5 * visibleSize.height / 8));
    //auto ease = EaseIn::create(move, 1);
    auto spawn = Spawn::create(fadeIn, move, NULL);
    auto move2 = MoveTo::create(25, Vec2(visibleSize.width * 1.5, 5 * visibleSize.height / 8));
    auto fadeOut = FadeOut::create(15);
    auto spawn2 = Spawn::create(fadeOut, move2, NULL);
    auto relocate = MoveTo::create(0.1, Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto delayC1 = DelayTime::create(5);
    auto seq = Sequence::create(spawn, spawn2, relocate, delayC1, NULL);
    cloud1->runAction(RepeatForever::create(seq));

    //RESET INITIAL CONFIG
    GameLevel::getInstance()->resetAgentAttributesInitialConfig();

    return true;
}

void UIProgressMap::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIProgressMap::menuLevelCallback(Ref* pSender)
{
    GameData::getInstance()->setGameStarted(false);
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();

    /*auto zone = Sprite::create("ZoneAreaLevel.png");
    zone->setPosition(Vec2(pMenuItem->getPositionX(), pMenuItem->getPositionY() - (pMenuItem->getContentSize().height / 3)));
    pMenuItem->getParent()->addChild(zone, 2);*/

    ProgressFromTo* radialTimer = ProgressFromTo::create(1, 0, 100);
    auto zoneTimer = ProgressTimer::create(Sprite::create("ZoneAreaLevel.png"));
    zoneTimer->setPosition(Vec2(pMenuItem->getPositionX(), pMenuItem->getPositionY() - (pMenuItem->getContentSize().height / 4)));
    zoneTimer->setType(ProgressTimer::Type::RADIAL);
    zoneTimer->runAction(radialTimer);
    (pMenuItem->getParent()->getParent())->addChild(zoneTimer, 2, 102);
    //this->addChild(zoneTimer, 2, 102);

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
    auto mapPopup = Sprite::create(loader.getLevelFileMap("level" + to_string(tag)) + "Background.png");
    mapPopup->setScale(0.21);
    mapPopup->setAnchorPoint(Vec2(0, 0.5));
    mapPopup->setPosition(Vec2(2 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14)));
    popupBackground->addChild(mapPopup);

    vector<string> goalTypes = loader.getGoalTypes("level" + to_string(tag));
    for (int i = 0; i < goalTypes.size(); i++) {
        Sprite* iconLevel;
        if (goalTypes[i] == "Expansion") {
            iconLevel = Sprite::create("ExpansionGoalIcon.png");
        } else if (goalTypes[i] == "Collection") {
            iconLevel = Sprite::create("CollectionGoalIcon.png");
        }
        iconLevel->setAnchorPoint(Vec2(1, 0.75));
        iconLevel->setPosition(Vec2(mapPopup->getPosition().x + mapPopup->getBoundingBox().size.width - ((iconLevel->getContentSize().width * i) + (iconLevel->getContentSize().width / 10) * i),
                                    mapPopup->getPosition().y - (mapPopup->getBoundingBox().size.height / 2)));
        popupBackground->addChild(iconLevel);
    }

    auto briefText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("BRIEF_LEVEL_1")->getCString(),
                                                            Size(13 * (popupBackground->getContentSize().width / 25),
                                                                 7 * (popupBackground->getContentSize().height / 14)),
                                                            TextHAlignment::LEFT, "Corbel", 26);
    briefText->setTextColor(Color4B(205, 202, 207, 255));
    briefText->setPosition(17 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14));
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

    auto delayDark = DelayTime::create(0.5);
    auto darkFadeIn = FadeIn::create(1);
    menu->setOpacity(0);
    auto seqDarkBackground = Sequence::create(delayDark, darkFadeIn, NULL);
    menu->runAction(seqDarkBackground);

    auto delayPopup = DelayTime::create(1.2);
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
        auto trophyScale = ScaleTo::create(0.6, 1);
        auto trophyFade = FadeIn::create(0.1);
        auto trophyRotate = RotateBy::create(0.3, 360);
        auto trophySpawn = Spawn::create(trophyScale, trophyFade, Repeat::create(trophyRotate, 2), NULL);
        trophy->runAction(Sequence::create(DelayTime::create(1.6), trophySpawn, NULL));
    }
}

void UIProgressMap::proceedLevelCallback(Ref* pSender)
{
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
        filename = "level212";
        break;
    case 5:
        filename = "level3";
        break;
    case 6:
        filename = "level4";
        break;
    default:
        filename = "level0";
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
    this->removeChildByTag(100);
    //Popup
    this->removeChildByTag(101);
    //Hexagon
    progressMap0->removeChildByTag(102);
}
