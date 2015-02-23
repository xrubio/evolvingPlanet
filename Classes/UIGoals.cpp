//
//  UIGoals.cpp
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#include "UIGoals.h"
#include "GameData.h"
#include "LocalizedString.h"

Scene* UIGoals::createScene()
{
    auto scene = Scene::create();
    auto layer = UIGoals::create();
    scene->addChild(layer);
    return scene;
}

bool UIGoals::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
                                 visibleSize.height / 2));
    this->addChild(background, 0);

    auto title = Label::createWithSystemFont(LocalizedString::create("GOALS")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(visibleSize.width / 2,
                            visibleSize.height - ((visibleSize.height / 8))));
    //this->addChild(title, 1);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIGoals::menuBackCallback, this));
    backButton->setPosition(Vec2(4 * (visibleSize.width / 42), 34 * (visibleSize.height / 36)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(207, 203, 208));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    arrowBack = MenuItemImage::create("ArrowBack.png", "ArrowBackPressed.png", CC_CALLBACK_1(UIGoals::menuArrowBackCallback, this));
    arrowBack->setPosition(Vec2(2 * (visibleSize.width / 42), visibleSize.height / 2));
    menuButtons.pushBack(arrowBack);

    arrowNext = MenuItemImage::create("ArrowNext.png", "ArrowNextPressed.png", CC_CALLBACK_1(UIGoals::menuArrowNextCallback, this));
    arrowNext->setPosition(Vec2(40 * (visibleSize.width / 42), visibleSize.height / 2));
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
    pages->setCustomScrollThreshold(0.3);
    pages->setBackGroundImage("MainMenuBackground.png");
    pages->setTouchEnabled(true);
    pages->setSize(Size(visibleSize.width, visibleSize.height));
    //pages->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));
    pages->setPosition(Point(0, 0));

    auto layoutContext = Layout::create();
    //TRIAR ESTIL SEGONS EL LVL
    createContextLayout(layoutContext);
    layoutContext->setBackGroundImage("PageBackground.png");
    layoutContext->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));
    pages->addPage(layoutContext);

    auto layout = Layout::create();
    layout->setBackGroundImage("PageBackground.png");
    layout->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));

    //layout->addChild(title);
    //layout->addChild(menu);
    setLevelGoals(layout);
    pages->addPage(layout);

    //for (int i = 0; i < GameLevel::getInstance()->getNumInitialAgents().size(); i++) {
    for (int i = 0; i < 1; i++) {
        GameLevel::getInstance()->setCurrentAgentType(i);
        //auto scene = UIAgents::createScene();
        auto layout2 = Layout::create();
        layout2->setBackGroundImage("PageBackground.png");
        layout2->setSize(Size(34 * visibleSize.width / 42, 25 * visibleSize.height * 31));
        //layout2->addChild(scene);
        pages->addPage(layout2);
        createUIAgent(layout2);
    }

    pages->setTag(0);

    //SI HI HA UNA CONFIGURACIO INICIAL NO BUIDA, SET CURRENT PAGE ALS ATRIBUTS
    //pages->scrollToPage(2);

    this->addChild(pages);

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
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto filledAttribute = layout->getChildByTag(GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + (i * 5));
        auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
        blankAttribute->setPosition(filledAttribute->getPosition());
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

    if (GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) < 5) {
        GameLevel::getInstance()->setAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        GameLevel::getInstance()->setAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i], GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) + 1);
        Label* l = (Label*)layout->getChildByTag((i + 1) * 1100);
        l->setString(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])));

        auto blankAttribute = layout->getChildByTag((GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i]) - 1) + (i * 5));
        auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
        filledAttribute->setPosition(blankAttribute->getPosition());
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
    context->setString(LocalizedString::create("CONTEXT_LEVEL_1_INTRO")->getCString());
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
    context->setString(LocalizedString::create("CONTEXT_LEVEL_1_DEPLOYMENT")->getCString());
    context->setPosition(Vec2(visibleSize.width / 2, 3.5 * visibleSize.height / 12));
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
    context = Text::create(LocalizedString::create("CONTEXT_LEVEL_1_INTRO")->getCString(), "Corbel", 40);
    context->setTextColor(Color4B(216, 229, 235, 255));
    context->ignoreContentAdaptWithSize(false);
    context->setContentSize(Size(visibleSize.width / 1.5, visibleSize.height));
    context->setTextHorizontalAlignment(TextHAlignment::LEFT);
    context->setTextVerticalAlignment(TextVAlignment::CENTER);
    context->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //context->addChild(goalMap);
    layoutContext->addChild(context);

    contextLabel = Label::createWithTTF(LocalizedString::create("CONTEXT_TITLE_INTRO")->getCString(),
                                        "fonts/BebasNeue.otf", 100);
    contextLabel->setPosition(Vec2(7 * visibleSize.width / 42, 25 * visibleSize.height / 31));
    contextLabel->setColor(Color3B(211, 230, 236));
    contextLabel->setAnchorPoint(Vec2(0, 0.5));
    layoutContext->addChild(contextLabel);

    Vector<MenuItem*> contextVec;
    auto contextPageIntro = MenuItemImage::create("ContextPageButton.png", "ContextPageButtonPressed.png", "ContextPageButtonPressed.png",
                                                  CC_CALLBACK_1(UIGoals::contextPageIntroCallback, this));
    contextPageIntro->setPosition(Vec2(20 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    auto labelContextPageIntro = Label::createWithTTF("1", "fonts/BebasNeue.otf", 50);
    labelContextPageIntro->setPosition(contextPageIntro->getContentSize().width / 2, contextPageIntro->getContentSize().height / 2.2);
    contextPageIntro->addChild(labelContextPageIntro);
    contextPageIntro->setEnabled(false);
    contextVec.pushBack(contextPageIntro);

    auto contextPageDeployment = MenuItemImage::create("ContextPageButton.png", "ContextPageButtonPressed.png", "ContextPageButtonPressed.png",
                                                       CC_CALLBACK_1(UIGoals::contextPageDeploymentCallback, this));
    contextPageDeployment->setPosition(Vec2(23 * visibleSize.width / 42, 5 * visibleSize.height / 31));
    auto labelContextPageDeployment = Label::createWithTTF("2", "fonts/BebasNeue.otf", 50);
    labelContextPageDeployment->setPosition(contextPageDeployment->getContentSize().width / 2,
                                            contextPageDeployment->getContentSize().height / 2.2);
    contextPageDeployment->addChild(labelContextPageDeployment);
    contextVec.pushBack(contextPageDeployment);

    auto contextMenu = Menu::createWithArray(contextVec);
    contextMenu->setPosition(0, 0);
    layoutContext->addChild(contextMenu);

    contextImage = Sprite::create("Level1Background.png");
    contextImage->setScale(0.5, 0.4);
    contextImage->setPosition(visibleSize.width / 2, 6.8 * visibleSize.height / 12);
    contextImage->setVisible(false);

    layoutContext->addChild(contextImage);
}

void UIGoals::setLevelGoals(Layout* layout)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto goalMap = Sprite::create(GameLevel::getInstance()->getMapFilename() + "Background" + ".png");
    goalMap->setScale(0.4);
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
        pages->scrollToPage(2);
    }

    for (int i = 0; i < keys.size(); i++) {
        auto attLabel = Label::createWithTTF(string(LocalizedString::create(keys[i].c_str())->getCString()) + " - ",
                                             "fonts/BebasNeue.otf", 60);
        attLabel->setColor(Color3B(207, 203, 208));
        attLabel->setAnchorPoint(Vec2(1, 0.5));
        attLabel->setPosition(18 * visibleSize.width / 42, (25 - ((i + 1) * 5)) * visibleSize.height / 31);
        //attLabel->setPosition(16 * visibleSize.width / 42, origin.y + (keys.size() - i) * (visibleSize.height / (keys.size() + 1)));
        layout->addChild(attLabel, 1, (i + 1) * 1000);
        auto attNumLabel = Label::createWithTTF(to_string(GameLevel::getInstance()->getAttributeCost(GameLevel::getInstance()->getCurrentAgentType(), keys[i])), "fonts/BebasNeue.otf", 80);
        attNumLabel->setColor(Color3B(207, 203, 208));
        attNumLabel->setAnchorPoint(Vec2(1, 0.5));
        attNumLabel->setPosition(attLabel->getPositionX() + attNumLabel->getContentSize().width, attLabel->getPositionY());
        layout->addChild(attNumLabel, 1, (i + 1) * 1100);

        auto minusAttButton = MenuItemImage::create(
            "MinusButton.png", "MinusButtonPressed.png", CC_CALLBACK_1(UIGoals::minusAttCallback, this));
        minusAttButton->setPosition(Vec2(24 * visibleSize.width / 42, attLabel->getPosition().y));
        minusAttButton->setTag(i + 10);
        attributesButtons.pushBack(minusAttButton);

        auto plusAttButton = MenuItemImage::create(
            "PlusButton.png", "PlusButtonPressed.png", CC_CALLBACK_1(UIGoals::plusAttCallback, this));
        plusAttButton->setPosition(Vec2(33 * visibleSize.width / 42, attLabel->getPosition().y));
        plusAttButton->setTag(i + 50);
        attributesButtons.pushBack(plusAttButton);

        float posX = minusAttButton->getPosition().x + (minusAttButton->getContentSize().width / 2);
        float incX = ((plusAttButton->getPosition().x - (plusAttButton->getContentSize().width / 2)) - posX) / 6;
        int posY = (25 - ((i + 1) * 5)) * visibleSize.height / 31;

        for (int j = 0; j < 5; j++) {
            posX = posX + incX;
            auto blankAttribute = Sprite::create("BlankAttributePointButton.png");
            auto filledAttribute = Sprite::create("FilledAttributePointButton.png");
            if (j >= GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                blankAttribute->setPosition(Vec2(posX, posY));
                layout->addChild(blankAttribute, 1, tag);
            } else if (j < GameLevel::getInstance()->getAgentAttribute(GameLevel::getInstance()->getCurrentAgentType(), keys[i])) {
                filledAttribute->setPosition(Vec2(posX, posY));
                layout->addChild(filledAttribute, 1, tag);
            }
            tag++;
        }
    }

    Menu* attributesMenu = cocos2d::Menu::createWithArray(attributesButtons);
    attributesMenu->setPosition(Vec2(0, 0));
    layout->addChild(attributesMenu, 1, 100000);

    auto evolutionPointsIcon = Sprite::create("EvolutionPoints.png");
    auto evolutionPointsNumberIcon = Label::createWithTTF(to_string(GameLevel::getInstance()->getEvolutionPoints()), "fonts/BebasNeue.otf", 80);
    evolutionPointsNumberIcon->setColor(Color3B(211, 230, 236));
    evolutionPointsNumberIcon->setPosition(Vec2(evolutionPointsIcon->getContentSize().width / 2,
                                                evolutionPointsIcon->getContentSize().height / 2));
    evolutionPointsIcon->addChild(evolutionPointsNumberIcon);
    evolutionPointsIcon->setPosition(Vec2(7 * visibleSize.width / 42, 24 * visibleSize.height / 31));
    layout->addChild(evolutionPointsIcon, 10);
    auto evolutionPointsLabel = Label::createWithTTF(string(LocalizedString::create("EVOLUTION_POINTS")->getCString()),
                                                     "fonts/BebasNeue.otf", 100);
    evolutionPointsLabel->setPosition(Vec2(15 * visibleSize.width / 42, 24 * visibleSize.height / 31));
    evolutionPointsLabel->setColor(Color3B(211, 230, 236));
    layout->addChild(evolutionPointsLabel, 10);
}

void UIGoals::update(float delta)
{
    //GameLevel::getInstance()->setCurrentAgentType(pages->getCurPageIndex() - 2);
    if (pages->getCurPageIndex() > 0 and pages->getCurPageIndex() < pages->getPages().size() - 1) {
        arrowBack->setVisible(true);
        arrowNext->setVisible(true);
    } else {
        if (pages->getCurPageIndex() == 0) {
            arrowBack->setVisible(false);
            arrowNext->setVisible(true);
        } else if (pages->getCurPageIndex() == pages->getPages().size() - 1) {
            arrowBack->setVisible(true);
            arrowNext->setVisible(false);
        }
    }
}
