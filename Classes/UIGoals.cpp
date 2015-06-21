//
//  UIGoals.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#define COCOS2D_DEBUG 1

#include "UIGoals.h"
#include "GameData.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIGoals::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGoals::create();
    scene->addChild(layer);
    return scene;
}

UIGoals::~UIGoals()
{
}

bool UIGoals::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (GameData::getInstance()->getMusic() == true and CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
    }

    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    //this->addChild(background, 0);

    hexagonButtonLevel0 = MenuItemImage::create("ProgressMapHexagonLevelOn.png", "ProgressMapHexagonLevelOff.png",
                                                        "ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel0->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel0->setPosition(Vec2((visibleSize.width / 2) - (hexagonButtonLevel0->getBoundingBox().size.width * 3),
                                          visibleSize.height / 20));
    hexagonButtonLevel0->setEnabled(false);
    this->addChild(hexagonButtonLevel0, 1);
    
    hexagonButtonLevel1 = MenuItemImage::create("ProgressMapHexagonLevelOn.png", "ProgressMapHexagonLevelOff.png",
                                                     "ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel1->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 20));
    hexagonButtonLevel1->setEnabled(false);
    this->addChild(hexagonButtonLevel1, 1);
    
    hexagonButtonLevel2 = MenuItemImage::create("ProgressMapHexagonLevelOn.png", "ProgressMapHexagonLevelOff.png",
                                                     "ProgressMapHexagonLevelOff.png");
    hexagonButtonLevel2->setColor(Color3B(120, 120, 120));
    hexagonButtonLevel2->setPosition(Vec2((visibleSize.width / 2) + (hexagonButtonLevel2->getBoundingBox().size.width * 3),
                                          visibleSize.height / 20));
    hexagonButtonLevel2->setEnabled(false);
    this->addChild(hexagonButtonLevel2, 1);

    auto title = Label::createWithSystemFont(LocalizedString::create("GOALS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height - ((visibleSize.height / 8))));
    //this->addChild(title, 1);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIGoals::menuBackCallback, this));
    backButton->setPosition(Vec2((4 * visibleSize.width / 42),
        (34 * visibleSize.height / 36)));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(207, 203, 208));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    arrowBack = MenuItemImage::create("ArrowBack.png", "ArrowBackPressed.png", CC_CALLBACK_1(UIGoals::menuArrowBackCallback, this));
    arrowBack->setPosition(Vec2((2 * visibleSize.width / 42), visibleSize.height / 2));
    arrowBack->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowBack);

    arrowNext = MenuItemImage::create("ArrowNext.png", "ArrowNextPressed.png", CC_CALLBACK_1(UIGoals::menuArrowNextCallback, this));
    arrowNext->setPosition(Vec2((40 * visibleSize.width / 42), visibleSize.height / 2));
    arrowNext->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    menuButtons.pushBack(arrowNext);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);

    if (GameData::getInstance()->getGameStarted() == false) {
        GameLevel::getInstance()->initializeAttributesCost();
    }
    GameLevel::getInstance()->setCurrentAgentType(0);
    map<string, int> atts = GameLevel::getInstance()->getAgentAttributes(GameLevel::getInstance()->getCurrentAgentType());
    int i = 0;
    for (map<string, int>::const_iterator it = atts.begin(); it != atts.end(); it++) {
        //si el cost de l'atribut es diferent de 0, es modificable
        if (GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), it->first) != 0) {
            keys.push_back(it->first);
        }
        i++;
    }

    pages = PageView::create();
    pages->setCustomScrollThreshold(visibleSize.width / 6);
    pages->addChild(background);
    //pages->setBackGroundImage("MainMenuBackground.png");
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    pages->setPosition(Point(0, 0));
    //pages->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));

    auto layoutContext = Layout::create();
    //TRIAR ESTIL SEGONS EL LVL
    createContextLayout(layoutContext);
    //layoutContext->setBackGroundImage("PageBackground.png");
    auto pageBackground = Sprite::create("PageBackground.png");
    pageBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pageBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContext->addChild(pageBackground, -1);
    layoutContext->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));
    //layoutContext->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pages->addPage(layoutContext);

    auto layout = Layout::create();
    auto pageBackground2 = Sprite::create("PageBackground.png");
    pageBackground2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pageBackground2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(pageBackground2, -1);
    layout->setSize(Size((34 * visibleSize.width / 42), (25 * visibleSize.height / 31)));

    //layout->addChild(title);
    //layout->addChild(menu);
    setLevelGoals(layout);
    pages->addPage(layout);

    //for (int i = 0; i < GameLevel::getInstance()->getNumInitialAgents().size(); i++) {
    for (int i = 0; i < 1; i++) {
        GameLevel::getInstance()->setCurrentAgentType(i);
        auto layout2 = Layout::create();
        auto pageBackground3 = Sprite::create("PageBackground.png");
        pageBackground3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        pageBackground3->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout2->addChild(pageBackground3, -1);
        //layout2->setSize(Size((34 * visibleSize.width / 42)  , (25 * visibleSize.height / 31)  ));
        pages->addPage(layout2);
        createUIAgent(layout2);
    }

    //temporal
    /*for (int index = 0; index < GameLevel::getInstance()->getNumInitialAgents().size(); index++) {
        auto layoutConfigValues = Layout::create();
        //layoutConfigValues->setBackGroundImage("PageBackground.png");
        auto pageBackground4 = Sprite::create("PageBackground.png");
        pageBackground4->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        pageBackground4->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layoutConfigValues->addChild(pageBackground4, -1);
        layoutConfigValues->setSize(Size((34 * visibleSize.width / 42),
            (25 * visibleSize.height / 31)));
        for (int i = 0; i < 6; i++) {
            auto label = Label::createWithSystemFont(to_string(i), "", 40);
            label->setPosition(Vec2((20 + (i * 5)) * layoutConfigValues->getContentSize().width / 42, 30 * layoutConfigValues->getContentSize().height / 31));
            layoutConfigValues->addChild(label);
        }
        map<string, vector<int> > temp = GameLevel::getInstance()->getAttributesValues()[index];
        int j = 0;
        for (map<string, vector<int> >::const_iterator it = temp.begin(); it != temp.end(); it++) {
            auto label = Label::createWithSystemFont(it->first, "", 40);
            label->setPosition(Vec2(12 * layoutConfigValues->getContentSize().width / 42, (28 - (j * 4)) * layoutConfigValues->getContentSize().height / 31));
            layoutConfigValues->addChild(label);

            for (int k = 0; k < 6; k++) {
                ui::TextField* textField = ui::TextField::create();
                textField->setPosition(Vec2((20 + (k * 5)) * layoutConfigValues->getContentSize().width / 42,
                    (28 - (j * 4)) * layoutConfigValues->getContentSize().height / 31));
                textField->setContentSize(Size(100, 44));
                textField->setPlaceHolder("0");
                textField->setFontSize(40);
                textField->setTag((j * 6) + k);
                layoutConfigValues->addChild(textField);
            }

            j++;
        }
        pages->addPage(layoutConfigValues);
    }*/

    pages->setTag(0);

    //SI HI HA UNA CONFIGURACIO INICIAL NO BUIDA, SET CURRENT PAGE ALS ATRIBUTS
    //pages->scrollToPage(2);

    //pages->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(pages);

    //this->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    this->scheduleUpdate();

    return true;
}

void UIGoals::menuBackCallback(Ref* pSender)
{
    GameData::getInstance()->setGameStarted(false);
    GameLevel::getInstance()->resetLevel();
    auto scene = UIProgressMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

//Faig dues funcions per si el primer cop que es comença la partida s'ha d'intercalar una pantalla de càrrega
void UIGoals::menuStartCallback(Ref* pSender)
{
    /*if (GameData::getInstance()->getGameStarted() == false) {
     GameLevel::getInstance()->createLevel(0);
     }
     GameData::getInstance()->setGameStarted(true);*/
    /*for (int index = 0; index < GameLevel::getInstance()->getNumInitialAgents().size(); index++) {
        map<string, vector<int> > temp = GameLevel::getInstance()->getAttributesValues()[index];
        int j = 0;
        for (map<string, vector<int> >::const_iterator it = temp.begin(); it != temp.end(); it++) {
            for (int k = 0; k < 6; k++) {
                auto v = (ui::TextField*)pages->getPage(3 + index)->getChildByTag((j * 6) + k);
                if (v->getString().empty() == false) {
                    GameLevel::getInstance()->setAttributesValues(index, it->first, k, stoi(v->getString()));
                }
            }
            j++;
        }
    }*/
    GameLevel::getInstance()->setAgentAttributesInitialConfig(GameLevel::getInstance()->getAgentAllAttributes());
    auto scene = UIGameplayMap::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIGoals::minusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 10;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) > 0) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1);
        GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() + GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto filledAttribute = layout->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
        blankAttribute->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->removeChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
        layout->addChild(blankAttribute, 1, GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
    }
}

void UIGoals::plusAttCallback(Ref* pSender)
{
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    int tag = pMenuItem->getTag();
    int i = tag - 50;
    Layout* layout = (Layout*)(pMenuItem->getParent()->getParent());

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) < 5 and GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) <= GameLevel::getInstance()->getEvolutionPoints()) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        GameLevel::getInstance()->setEvolutionPoints(GameLevel::getInstance()->getEvolutionPoints() - GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]));
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto blankAttribute = layout->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
        filledAttribute->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->removeChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
        layout->addChild(filledAttribute, 1, (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
    }
}

void UIGoals::menuArrowBackCallback(Ref* pSender)
{
    if (pages->getCurPageIndex() > 0) {
        pages->scrollToPage(pages->getCurPageIndex() - 1);
    }
}

void UIGoals::menuArrowNextCallback(Ref* pSender)
{
    if (pages->getCurPageIndex() < pages->getPages().size() - 1) {
        pages->scrollToPage(pages->getCurPageIndex() + 1);
    }
}

void UIGoals::contextPageIntroCallback(Ref* pSender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    pMenuItem->setEnabled(false);
    Menu* menu = (Menu*)(pMenuItem->getParent());
    MenuItem* pMenuItemInvers = (MenuItem*)menu->getChildren().at(1);
    pMenuItemInvers->setEnabled(true);
    contextLabel->setString(LocalizedString::create("CONTEXT_TITLE_INTRO")->getCString());
    context->setString(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameLevel::getInstance()->getNumLevel()) + "_INTRO").c_str())->getCString());
    context->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    contextImage->setVisible(false);
}

void UIGoals::contextPageDeploymentCallback(Ref* pSender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    MenuItem* pMenuItem = (MenuItem*)(pSender);
    pMenuItem->setEnabled(false);
    Menu* menu = (Menu*)(pMenuItem->getParent());
    MenuItem* pMenuItemInvers = (MenuItem*)menu->getChildren().at(0);
    pMenuItemInvers->setEnabled(true);
    contextLabel->setString(LocalizedString::create("CONTEXT_TITLE_DEPLOYMENT")->getCString());
    context->setString(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameLevel::getInstance()->getNumLevel()) + "_DEPLOYMENT").c_str())->getCString());
    context->setPosition(Vec2(visibleSize.width / 2, (3.5 * visibleSize.height / 12)));
    contextImage->setVisible(true);
}

void UIGoals::createContextLayout(Layout* layoutContext)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    layoutContext->setSize(Size(visibleSize.width / 2, visibleSize.height / 2));
    //auto goalMap = Sprite::create(GameLevel::getInstance()->getMapFilename() + "Background" + ".png");
    //goalMap->setScale(0.4);
    //goalMap->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 2));
    /*auto context = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("CONTEXT_LEVEL_1")->getCString(),
                                                          Size(visibleSize.width / 2, visibleSize.height),
                                                          TextHAlignment::LEFT, "Arial", 50);
    context->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    context->setTextColor(Color4B(216, 229, 235, 255));*/
    context = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(("CONTEXT_LEVEL_" + to_string(GameLevel::getInstance()->getNumLevel()) + "_INTRO").c_str())->getCString(), Size(visibleSize.width / (1.5 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "Corbel", 40);
    context->setColorSpaceHolder(Color4B(216, 229, 235, 255));
    context->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //context->addChild(goalMap);
    context->setScaleX(GameData::getInstance()->getRaWConversion());
    context->setScaleY(GameData::getInstance()->getRaHConversion());
    layoutContext->addChild(context);

    contextLabel = Label::createWithTTF(LocalizedString::create("CONTEXT_TITLE_INTRO")->getCString(),
        "fonts/BebasNeue.otf", 100);
    contextLabel->setPosition(Vec2(7 * visibleSize.width / 42, 25 * visibleSize.height / 31));
    contextLabel->setColor(Color3B(211, 230, 236));
    contextLabel->setAnchorPoint(Vec2(0, 0.5));
    contextLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layoutContext->addChild(contextLabel);

    Vector<MenuItem*> contextVec;
    auto contextPageIntro = MenuItemImage::create("ContextPageButton.png", "ContextPageButtonPressed.png", "ContextPageButtonPressed.png",
        CC_CALLBACK_1(UIGoals::contextPageIntroCallback, this));
    contextPageIntro->setPosition(Vec2(20 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    contextPageIntro->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto labelContextPageIntro = Label::createWithTTF("1", "fonts/BebasNeue.otf", 50);
    labelContextPageIntro->setPosition(contextPageIntro->getContentSize().width / 2, contextPageIntro->getContentSize().height / 2.2);
    contextPageIntro->addChild(labelContextPageIntro);
    contextPageIntro->setEnabled(false);
    contextVec.pushBack(contextPageIntro);

    auto contextPageDeployment = MenuItemImage::create("ContextPageButton.png", "ContextPageButtonPressed.png", "ContextPageButtonPressed.png",
        CC_CALLBACK_1(UIGoals::contextPageDeploymentCallback, this));
    contextPageDeployment->setPosition(Vec2(23 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    contextPageDeployment->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto labelContextPageDeployment = Label::createWithTTF("2", "fonts/BebasNeue.otf", 50);
    labelContextPageDeployment->setPosition(contextPageDeployment->getContentSize().width / 2,
        contextPageDeployment->getContentSize().height / 2.2);
    contextPageDeployment->addChild(labelContextPageDeployment);
    contextVec.pushBack(contextPageDeployment);

    auto contextMenu = Menu::createWithArray(contextVec);
    contextMenu->setPosition(0, 0);
    layoutContext->addChild(contextMenu);

    contextImage = Sprite::create("Level1Background.jpg");
    contextImage->setScale(0.5 * GameData::getInstance()->getRaWConversion(), 0.4 * GameData::getInstance()->getRaHConversion());
    contextImage->setPosition(visibleSize.width / 2, 6.8 * visibleSize.height / 12);
    contextImage->setVisible(false);

    layoutContext->addChild(contextImage);
}

void UIGoals::setLevelGoals(Layout* layout)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto goalMap = Sprite::create(GameLevel::getInstance()->getMapFilename() + "Background" + ".jpg");
    goalMap->setScale(0.4 * GameData::getInstance()->getRaConversion());
    goalMap->setPosition(Vec2((visibleSize.width / 4) + 130, origin.y + visibleSize.height / 2));
    layout->addChild(goalMap, 1);

    auto goal1 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_1")->getCString(),
        Size(visibleSize.width / 3, visibleSize.height / 4),
        TextHAlignment::CENTER, "Arial", 40);
    auto goal2 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_2")->getCString(),
        Size(visibleSize.width / 3, visibleSize.height / 4),
        TextHAlignment::CENTER, "Arial", 40);
    auto goal3 = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create("GOAL_TEXT_LVL1_3")->getCString(),
        Size(visibleSize.width / 3, visibleSize.height / 4),
        TextHAlignment::CENTER, "Arial", 40);

    goal1->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (5 * visibleSize.height / 8)));
    goal2->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (4 * visibleSize.height / 8)));
    goal3->setPosition(Vec2(origin.x + 3 * (visibleSize.width / 4), origin.y + (3 * visibleSize.height / 8)));

    goal1->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    goal2->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    goal3->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    layout->addChild(goal1, 1);
    layout->addChild(goal2, 1);
    layout->addChild(goal3, 1);
}

void UIGoals::createUIAgent(Layout* layout)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithSystemFont(LocalizedString::create("AGENTS")->getCString() + to_string(GameLevel::getInstance()->getCurrentAgentType()), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - ((visibleSize.height / 8))));
    //layout->addChild(title, 1);

    Vector<cocos2d::MenuItem*> menuButtons;

    auto playButton = MenuItemImage::create(
        "ProgressMapPlayButton.png", "ProgressMapPlayButtonPressed.png", CC_CALLBACK_1(UIGoals::menuStartCallback, this));
    playButton->setPosition(Vec2(34 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    playButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto startPlay = Label::createWithTTF(LocalizedString::create("START")->getCString(), "fonts/BebasNeue.otf", 50);
    startPlay->setPosition(playButton->getContentSize().width / 2, playButton->getContentSize().height / 2);
    startPlay->setColor(Color3B(207, 203, 208));
    playButton->addChild(startPlay);
    menuButtons.pushBack(playButton);

    Menu* menu = cocos2d::Menu::createWithArray(menuButtons);
    menu->setPosition(Vec2(0, 0));
    layout->addChild(menu, 10);

    Vector<cocos2d::MenuItem*> attributesButtons;
    int tag = 0;

    if (GameLevel::getInstance()->getAgentAttributesInitialConfig().empty() == false) {
        GameLevel::getInstance()->setAgentAttributes(GameLevel::getInstance()->getAgentAttributesInitialConfig());
        int initialEvolutionPoints = 10;
        for (int i = 0; i < 3; i++) {
            GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i],
                GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
            int n = GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]);
            while (n > 0) {
                initialEvolutionPoints -= n;
                n--;
            }
        }
        GameLevel::getInstance()->setEvolutionPoints(initialEvolutionPoints);
        pages->scrollToPage(2);
    }

    for (int i = 0; i < keys.size(); i++) {
        auto attLabel = Label::createWithTTF(string(LocalizedString::create(keys[i].c_str())->getCString()) + " - ",
            "fonts/BebasNeue.otf", 60);
        attLabel->setColor(Color3B(207, 203, 208));
        attLabel->setAnchorPoint(Vec2(1, 0.5));
        attLabel->setPosition(18 * visibleSize.width / 42, (25 - ((i + 1) * 5)) * visibleSize.height / 31);
        attLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        //attLabel->setPosition(16 * visibleSize.width / 42, origin.y + (keys.size() - i) * (visibleSize.height / (keys.size() + 1)));
        layout->addChild(attLabel, 1, (i + 1) * 1000);
        auto attNumLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])), "fonts/BebasNeue.otf", 80);
        attNumLabel->setColor(Color3B(207, 203, 208));
        attNumLabel->setAnchorPoint(Vec2(1, 0.5));
        attNumLabel->setPosition(attLabel->getPositionX() + attNumLabel->getContentSize().width, attLabel->getPositionY());
        attNumLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        layout->addChild(attNumLabel, 1, (i + 1) * 1100);

        auto minusAttButton = MenuItemImage::create(
            "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIGoals::minusAttCallback, this));
        minusAttButton->setPosition(Vec2(24 * visibleSize.width / 42, attLabel->getPosition().y));
        minusAttButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        minusAttButton->setTag(i + 10);
        attributesButtons.pushBack(minusAttButton);

        auto plusAttButton = MenuItemImage::create(
            "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIGoals::plusAttCallback, this));
        plusAttButton->setPosition(Vec2(33 * visibleSize.width / 42, attLabel->getPosition().y));
        plusAttButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        plusAttButton->setTag(i + 50);
        attributesButtons.pushBack(plusAttButton);

        float posX = minusAttButton->getPosition().x + (minusAttButton->getBoundingBox().size.width / 2);
        float incX = ((plusAttButton->getPosition().x - (plusAttButton->getBoundingBox().size.width / 2)) - posX) / 6;
        int posY = (25 - ((i + 1) * 5)) * visibleSize.height / 31;

        for (int j = 0; j < 5; j++) {
            posX = posX + incX;
            auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
            auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
            if (j >= GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                blankAttribute->setPosition(Vec2(posX, posY));
                blankAttribute->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
                layout->addChild(blankAttribute, 1, tag);
            }
            else if (j < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                filledAttribute->setPosition(Vec2(posX, posY));
                filledAttribute->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
                layout->addChild(filledAttribute, 1, tag);
            }
            tag++;
        }
    }

    Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    layout->addChild(attributesMenu, 1, 100000);

    auto evolutionPointsIcon = Sprite::create("EvolutionPoints.png");
    evolutionPointsNumberIcon = Label::createWithTTF(to_string(GameLevel::getInstance()->getEvolutionPoints()), "fonts/BebasNeue.otf", 80);
    evolutionPointsNumberIcon->setColor(Color3B(211, 230, 236));
    evolutionPointsNumberIcon->setPosition(Vec2(evolutionPointsIcon->getContentSize().width / 2,
        evolutionPointsIcon->getContentSize().height / 2));
    evolutionPointsIcon->addChild(evolutionPointsNumberIcon);
    evolutionPointsIcon->setPosition(Vec2(7 * visibleSize.width / 42, 24 * visibleSize.height / 31));
    evolutionPointsIcon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    layout->addChild(evolutionPointsIcon, 10);
    auto evolutionPointsLabel = Label::createWithTTF(string(LocalizedString::create("EVOLUTION_POINTS")->getCString()),
        "fonts/BebasNeue.otf", 100);
    evolutionPointsLabel->setPosition(Vec2(15 * visibleSize.width / 42, 24 * visibleSize.height / 31));
    evolutionPointsLabel->cocos2d::Node::setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    evolutionPointsLabel->setColor(Color3B(211, 230, 236));
    layout->addChild(evolutionPointsLabel, 10);
}

void UIGoals::update(float delta)
{
    //GameLevel::getInstance()->setCurrentAgentType(pages->getCurPageIndex() - 2);
    if (pages->getCurPageIndex() == 0)
    {
        hexagonButtonLevel0->setEnabled(true);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 1)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(true);
        hexagonButtonLevel2->setEnabled(false);
    }
    else if (pages->getCurPageIndex() == 2)
    {
        hexagonButtonLevel0->setEnabled(false);
        hexagonButtonLevel1->setEnabled(false);
        hexagonButtonLevel2->setEnabled(true);
    }
    
    if (pages->getCurPageIndex() > 0 and pages->getCurPageIndex() < pages->getPages().size() - 1) {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    }
    else {
        if (pages->getCurPageIndex() == 0) {
            arrowBack->setVisible(false);
            arrowNext->setVisible(true);
        }
        else if (pages->getCurPageIndex() == pages->getPages().size() - 1) {
            arrowBack->setVisible(true);
            arrowNext->setVisible(false);
        }
    }

    evolutionPointsNumberIcon->setString(to_string(GameLevel::getInstance()->getEvolutionPoints()));
}
