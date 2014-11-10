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
#include "CocosGUI.h"

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

    //Alliberar memòria
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    GameLevel::getInstance()->resetLevel();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //Set background progress map and all its functionalities
    auto progressMap = Sprite::create("ProgressMapBackground.png");
    progressMap->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                  visibleSize.height / 2 + origin.y));
    auto progressMap2 = Sprite::create("ProgressMap2Background.png");
    progressMap2->setPosition(Vec2(visibleSize.width + (visibleSize.width / 2) + origin.x,
                                   visibleSize.height / 2 + origin.y));
    //this->addChild(progressMap, 0);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuBackCallback, this));
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                           origin.y + (backButton->getContentSize().height / 2)));
    this->addChild(menu, 1);

    Vector<MenuItem*> level1Buttons;

    auto level0Button = MenuItemImage::create(
        "LevelButtonBackground.png", "LevelButtonBackground.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level0Button->setPosition(688, 980);
    level0Button->setTag(0);
    auto level0Label = Label::createWithSystemFont("0", "Arial", 40);
    level0Label->setPosition(level0Button->getContentSize().width / 2, level0Button->getContentSize().height / 2);
    level0Button->addChild(level0Label);
    level1Buttons.pushBack(level0Button);

    auto level1Button = MenuItemImage::create(
        "Level1Button.png", "Level1ButtonPressed.png", CC_CALLBACK_1(UIProgressMap::menuLevelCallback, this));
    level1Button->setPosition(288, 180);
    level1Button->setTag(1);
    level1Buttons.pushBack(level1Button);
    auto level1 = Menu::createWithArray(level1Buttons);
    level1->setPosition(0, 0);
    progressMap->addChild(level1, 1);

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
    auto scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 2, visibleSize.height));
    scrollView->setBackGroundColor(Color3B(200, 200, 200));
    scrollView->setSize(scollFrameSize);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

    scrollView->addChild(progressMap);
    scrollView->addChild(progressMap2);
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
    auto transition = TransitionShrinkGrow::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}
