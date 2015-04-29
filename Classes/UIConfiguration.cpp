//
//  UIConfiguration.cpp
//  simulplay
//
//  Created by Guillem Laborda on 03/09/14.
//
//

#include "UIConfiguration.h"
#include "GameData.h"
#include "LocalizedString.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIConfiguration::createScene()
{
    auto scene = Scene::create();
    auto layer = UIConfiguration::create();
    scene->addChild(layer);
    return scene;
}

bool UIConfiguration::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
                                 visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);

    auto title = Sprite::create("MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
                            (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(title, 5, 0);

    auto planet2 = Sprite::create("MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
                              (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(planet2, 1, 2);

    auto popupBackground = Sprite::create("ConfigurationBackground.png");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    this->addChild(popupBackground, 6);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "ProgressMapBackButton.png", "ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIConfiguration::menuBackCallback, this));
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setPosition(Vec2(1 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK")->getCString(), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);
    
    auto resetButton = MenuItemImage::create(
        "ConfigurationResetGame.png", "ConfigurationResetGamePressed.png", "ConfigurationResetGamePressed.png",
                                             CC_CALLBACK_1(UIConfiguration::menuResetCallback, this));
    resetButton->setPosition(Vec2(24 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    auto resetLabel = Label::createWithTTF(LocalizedString::create("RESET")->getCString(), "fonts/BebasNeue.otf", 50);
    resetLabel->setColor(Color3B(205, 202, 207));
    resetLabel->setPosition(resetButton->getContentSize().width / 2, resetButton->getContentSize().height / 2);
    resetButton->addChild(resetLabel);
    menuButtons.pushBack(resetButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    popupBackground->addChild(menu, 1, 20);
    
    if (GameData::getInstance()->getResetGameClicked())
    {
        createWarningWindow(popupBackground);
        resetButton->setEnabled(false);
    }

    auto configLabel = Label::createWithTTF(LocalizedString::create("CONFIGURATION")->getCString(), "fonts/BebasNeue.otf", 100);
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);

    Vector<MenuItem*> languageItems;
    auto languageLabel = Label::createWithTTF(LocalizedString::create("LANGUAGE")->getCString(), "fonts/BebasNeue.otf", 80);
    languageLabel->setColor(Color3B(72, 108, 118));
    languageLabel->setAnchorPoint(Vec2(1, 0.5));
    languageLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(languageLabel);

    auto catButton = MenuItemImage::create(
        "catFlag.png", "catFlagPressed.png", "catFlagPressed.png", CC_CALLBACK_1(UIConfiguration::catFlagCallback, this));
    catButton->setPosition(Vec2(11 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(catButton);

    auto esButton = MenuItemImage::create(
        "esFlag.png", "esFlagPressed.png", "esFlagPressed.png", CC_CALLBACK_1(UIConfiguration::esFlagCallback, this));
    esButton->setPosition(Vec2(15 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(esButton);

    auto enButton = MenuItemImage::create(
        "enFlag.png", "enFlagPressed.png", "enFlagPressed.png", CC_CALLBACK_1(UIConfiguration::enFlagCallback, this));
    enButton->setPosition(Vec2(19 * popupBackground->getContentSize().width / 28, 11 * popupBackground->getContentSize().height / 16));
    languageItems.pushBack(enButton);

    string lang = GameData::getInstance()->getLanguage();
    if (lang == "cat") {
        catButton->setEnabled(false);

    } else if (lang == "es") {
        esButton->setEnabled(false);

    } else if (lang == "en") {
        enButton->setEnabled(false);
    }

    auto menuLanguage = Menu::createWithArray(languageItems);
    menuLanguage->setPosition(0, 0);
    popupBackground->addChild(menuLanguage, 10);

    Vector<MenuItem*> soundItems;

    auto musicLabel = Label::createWithTTF(LocalizedString::create("MUSIC")->getCString(), "fonts/BebasNeue.otf", 80);
    musicLabel->setColor(Color3B(72, 108, 118));
    musicLabel->setAnchorPoint(Vec2(1, 0.5));
    musicLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(musicLabel);

    auto musicOnLabel = MenuItemImage::create("ActiveOff.png", "ActiveOff.png", "ActiveOn.png",
                                              CC_CALLBACK_1(UIConfiguration::musicOnCallback, this));
    musicOnLabel->setPosition(Vec2(Vec2(11 * popupBackground->getContentSize().width / 28,
                                        8 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOnLabel);

    auto musicSeparator = Sprite::create("ConfigurationSeparator.png");
    musicSeparator->setPosition(Vec2(Vec2(15 * popupBackground->getContentSize().width / 28,
                                          8 * popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(musicSeparator, 10);

    auto musicOffLabel = MenuItemImage::create("InactiveOff.png", "InactiveOff.png", "InactiveOn.png",
                                               CC_CALLBACK_1(UIConfiguration::musicOffCallback, this));
    musicOffLabel->setPosition(Vec2(Vec2(19 * popupBackground->getContentSize().width / 28,
                                         8 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(musicOffLabel);

    auto sfxLabel = Label::createWithTTF(LocalizedString::create("SPECIAL_EFFECTS")->getCString(), "fonts/BebasNeue.otf", 80);
    sfxLabel->setColor(Color3B(72, 108, 118));
    sfxLabel->setAnchorPoint(Vec2(1, 0.5));
    sfxLabel->setPosition(Vec2(8.5 * popupBackground->getContentSize().width / 28, 5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(sfxLabel);

    auto sfxOnLabel = MenuItemImage::create("ActiveOff.png", "ActiveOff.png", "ActiveOn.png",
                                            CC_CALLBACK_1(UIConfiguration::sfxOnCallback, this));
    sfxOnLabel->setPosition(Vec2(Vec2(11 * popupBackground->getContentSize().width / 28,
                                      5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(sfxOnLabel);

    auto sfxSeparator = Sprite::create("ConfigurationSeparator.png");
    sfxSeparator->setPosition(Vec2(Vec2(15 * popupBackground->getContentSize().width / 28,
                                        5 * popupBackground->getContentSize().height / 16)));
    popupBackground->addChild(sfxSeparator, 10);

    auto sfxOffLabel = MenuItemImage::create("InactiveOff.png", "InactiveOff.png", "InactiveOn.png",
                                             CC_CALLBACK_1(UIConfiguration::sfxOffCallback, this));
    sfxOffLabel->setPosition(Vec2(Vec2(19 * popupBackground->getContentSize().width / 28,
                                       5 * popupBackground->getContentSize().height / 16)));
    soundItems.pushBack(sfxOffLabel);

    if (GameData::getInstance()->getMusic() == true) {
        musicOnLabel->setEnabled(false);
    } else {
        musicOffLabel->setEnabled(false);
    }

    if (GameData::getInstance()->getSFX() == true) {
        sfxOnLabel->setEnabled(false);
    } else {
        sfxOffLabel->setEnabled(false);
    }

    auto menuSound = Menu::createWithArray(soundItems);
    menuSound->setPosition(0, 0);
    popupBackground->addChild(menuSound, 10);

    return true;
}

void UIConfiguration::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    UserDefault::getInstance()->setStringForKey("language", GameData::getInstance()->getLanguage());
    UserDefault::getInstance()->setBoolForKey("music", GameData::getInstance()->getMusic());
    UserDefault::getInstance()->setBoolForKey("sfx", GameData::getInstance()->getSFX());
    UserDefault::getInstance()->flush();
    GameData::getInstance()->setResetGameClicked(false);

    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIConfiguration::menuResetCallback(Ref* pSender)
{
    MenuItemImage* pMenuItem = (MenuItemImage*)(pSender);
    pMenuItem->setEnabled(false);
    Sprite* popupBackground = (Sprite*)(pMenuItem->getParent()->getParent());
    createWarningWindow(popupBackground);
    GameData::getInstance()->setResetGameClicked(true);
}

void UIConfiguration::menuResetNoCallback(Ref* pSender)
{
    MenuItemImage* pMenuItem = (MenuItemImage*)(pSender);
    auto popupBackground = (Sprite*)(pMenuItem->getParent()->getParent()->getParent());
    popupBackground->removeChildByTag(30);
    auto menuButtons = (Menu*)(popupBackground->getChildByTag(20));
    ((MenuItemImage*)(menuButtons->getChildren().at(1)))->setEnabled(true);
    GameData::getInstance()->setResetGameClicked(false);
}

void UIConfiguration::menuResetYesCallback(Ref* pSender)
{
    MenuItemImage* pMenuItem = (MenuItemImage*)(pSender);
    auto popupBackground = (Sprite*)(pMenuItem->getParent()->getParent()->getParent());
    popupBackground->removeChildByTag(30);
    auto menuButtons = (Menu*)(popupBackground->getChildByTag(20));
    ((MenuItemImage*)(menuButtons->getChildren().at(1)))->setEnabled(true);
    GameData::getInstance()->setResetGameClicked(false);
    GameData::getInstance()->resetGameProgress();
}

void UIConfiguration::catFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("cat");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::esFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("es");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::enFlagCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    GameData::getInstance()->setLanguage("en");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::musicOnCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto musicOnLabel = (MenuItem*)pSender;
    musicOnLabel->setEnabled(false);
    auto menuSound = (Menu*)musicOnLabel->getParent();
    auto musicOffLabel = (MenuItem*)menuSound->getChildren().at(1);
    musicOffLabel->setEnabled(true);

    GameData::getInstance()->setMusic(true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
}

void UIConfiguration::musicOffCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto musicOffLabel = (MenuItem*)pSender;
    musicOffLabel->setEnabled(false);
    auto menuSound = (Menu*)musicOffLabel->getParent();
    auto musicOnLabel = (MenuItem*)menuSound->getChildren().at(0);
    musicOnLabel->setEnabled(true);

    GameData::getInstance()->setMusic(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void UIConfiguration::sfxOnCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto sfxOnLabel = (MenuItem*)pSender;
    sfxOnLabel->setEnabled(false);
    auto menuSound = (Menu*)sfxOnLabel->getParent();
    auto sfxOffLabel = (MenuItem*)menuSound->getChildren().at(3);
    sfxOffLabel->setEnabled(true);

    GameData::getInstance()->setSFX(true);
}

void UIConfiguration::sfxOffCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }
    auto sfxOffLabel = (MenuItem*)pSender;
    sfxOffLabel->setEnabled(false);
    auto menuSound = (Menu*)sfxOffLabel->getParent();
    auto sfxOnLabel = (MenuItem*)menuSound->getChildren().at(2);
    sfxOnLabel->setEnabled(true);

    GameData::getInstance()->setSFX(false);
}

void UIConfiguration::createWarningWindow(Sprite* popupBackground)
{
    auto alertBackground = Sprite::create("ConfigurationAlert.png");
    alertBackground->setPosition(Vec2(24 * popupBackground->getContentSize().width / 28, 8 * popupBackground->getContentSize().height / 16));
    
    auto alertLabel = Label::createWithTTF(LocalizedString::create("WARNING")->getCString(), "fonts/BebasNeue.otf", 80);
    alertLabel->setColor(Color3B(255,255,255));
    alertLabel->setPosition(Vec2(alertBackground->getBoundingBox().size.width / 2, 5 * alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertLabel);
    
    auto alertTextLabel = Label::createWithTTF(LocalizedString::create("WARNING_TEXT")->getCString(), "fonts/BebasNeue.otf", 50);
    alertTextLabel->setColor(Color3B(255,255,255));
    alertTextLabel->setMaxLineWidth(325);
    alertTextLabel->setAlignment(TextHAlignment::CENTER);
    alertTextLabel->setPosition(Vec2(alertBackground->getBoundingBox().size.width / 2, 3 * alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertTextLabel);
    
    auto alertConfirmationLabel = Label::createWithTTF(LocalizedString::create("RESET")->getCString() + string(" ?"), "fonts/BebasNeue.otf", 50);
    alertConfirmationLabel->setColor(Color3B(255,255,255));
    alertConfirmationLabel->setPosition(Vec2(1.2 * alertBackground->getBoundingBox().size.width / 4,
                                             alertBackground->getBoundingBox().size.height / 6));
    alertBackground->addChild(alertConfirmationLabel);
    
    Vector<MenuItem*> confirmReset;
    auto confirmResetYes = MenuItemImage::create("ConfigurationResetYes.png", "ConfigurationResetYesPressed.png",
                                                 CC_CALLBACK_1(UIConfiguration::menuResetYesCallback, this));
    confirmResetYes->setPosition(Vec2(Vec2(4 * alertBackground->getBoundingBox().size.width / 6,
                                           alertBackground->getBoundingBox().size.height / 6)));
    confirmReset.pushBack(confirmResetYes);
    auto confirmSeparator = Sprite::create("ConfigurationResetSeparator.png");
    confirmSeparator->setPosition(Vec2(Vec2(4.65 * alertBackground->getBoundingBox().size.width / 6,
                                            alertBackground->getBoundingBox().size.height / 6)));
    alertBackground->addChild(confirmSeparator);
    auto confirmResetNo = MenuItemImage::create("ConfigurationResetNo.png", "ConfigurationResetNoPressed.png",
                                                CC_CALLBACK_1(UIConfiguration::menuResetNoCallback, this));
    confirmResetNo->setPosition(Vec2(Vec2(5.3 * alertBackground->getBoundingBox().size.width / 6,
                                          alertBackground->getBoundingBox().size.height / 6)));
    confirmReset.pushBack(confirmResetNo);
    
    auto menuConfirmReset = Menu::createWithArray(confirmReset);
    menuConfirmReset->setPosition(0, 0);
    alertBackground->addChild(menuConfirmReset, 10);
    
    popupBackground->addChild(alertBackground, 1, 30);
}
