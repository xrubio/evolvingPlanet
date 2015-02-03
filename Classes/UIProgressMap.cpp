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
    this->addChild(menu, 1);

    Vector<MenuItem*> level0Buttons;
    auto levelButton = MenuItemImage::create(
        "LevelPointerButton.png", "LevelPointerButton.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    levelButton->setPosition(71 * visibleSize.width / 204, 49 * visibleSize.height / 155);
    levelButton->setTag(1);
    level0Buttons.pushBack(levelButton);
    auto levelLabel = Label::createWithTTF("1", "fonts/BebasNeue.otf", 40);
    levelLabel->setColor(Color3B(32, 47, 55));
    levelLabel->setPosition(levelButton->getContentSize().width / 2, levelButton->getContentSize().height / 2);
    levelButton->addChild(levelLabel);

    auto shadow = Sprite::create("LevelPointerButtonShadow.png");
    shadow->setPosition(Vec2((71 * visibleSize.width / 204),
                             (49 * visibleSize.height / 155) - (levelButton->getContentSize().height / 1.5)));
    progressMap0->addChild(shadow);

    auto dropActionLevelButton = MoveBy::create(1.5, Vec2(0, 5));
    auto easeDropActionLevelButton = EaseOut::create(dropActionLevelButton, 1);
    auto dropActionLevelButton2 = MoveBy::create(1.5, Vec2(0, -5));
    auto easeDropActionLevelButton2 = EaseOut::create(dropActionLevelButton2, 1);
    auto seqDrop = Sequence::create(easeDropActionLevelButton, easeDropActionLevelButton2, NULL);
    levelButton->runAction(RepeatForever::create(seqDrop));

    auto level = Menu::createWithArray(level0Buttons);
    level->setPosition(0, 0);
    progressMap0->addChild(level, 3);

    Vector<MenuItem*> level1Buttons;
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
    progressMap2->addChild(level2);

    auto scollFrameSize = Size(visibleSize.width, visibleSize.height);
    scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    scrollView->setBackGroundColor(Color3B(200, 200, 200));
    scrollView->setSize(scollFrameSize);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

    scrollView->addChild(progressMap0, 0);
    scrollView->addChild(progressMap1, 1);
    scrollView->addChild(progressMap2, 2);
    this->addChild(scrollView);

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
    return true;
}

void UIProgressMap::menuBackCallback(Ref* pSender)
{
    auto scene = UIMainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
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

    auto iconLevel = Sprite::create("ExpansionGoalIcon.png");
    iconLevel->setAnchorPoint(Vec2(1, 0.75));
    iconLevel->setPosition(Vec2(mapPopup->getPosition().x + mapPopup->getBoundingBox().size.width,
                                mapPopup->getPosition().y - (mapPopup->getBoundingBox().size.height / 2)));
    popupBackground->addChild(iconLevel);

    auto briefText = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("BRIEF_LEVEL_1")->getCString(),
                                                            Size(13 * (popupBackground->getContentSize().width / 25),
                                                                 7 * (popupBackground->getContentSize().height / 14)),
                                                            TextHAlignment::LEFT, "Corbel", 26);
    briefText->setColor(Color3B(205, 202, 207));
    briefText->setPosition(17 * (popupBackground->getContentSize().width / 25), 7 * (popupBackground->getContentSize().height / 14));
    popupBackground->addChild(briefText);

    auto playButton = MenuItemImage::create("ProgressMapPlayButton.png", "ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIProgressMap::proceedLevelCallback, this));
    playButton->setPosition(Vec2(21 * (popupBackground->getContentSize().width / 25), 2 * (popupBackground->getContentSize().height / 14)));
    playButton->setTag(tag);
    auto labelPlay = Label::createWithTTF(LocalizedString::create("PLAY")->getCString(), "fonts/BebasNeue.otf", 40);
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
        filename = "level21";
        break;
    case 3:
        filename = "level211";
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
