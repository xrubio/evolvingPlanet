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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithSystemFont(LocalizedString::create("CONFIGURATION")->getCString(), "Arial Rounded MT Bold", 180);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - ((visibleSize.height / 8))));
    this->addChild(title, 1);

    auto backButton = MenuItemImage::create(
        "BackButton.png", "BackButtonPressed.png", CC_CALLBACK_1(UIConfiguration::menuBackCallback, this));
    backButton->setPosition(Vec2(origin.x + visibleSize.width - (backButton->getContentSize().width / 2),
                                 origin.y + (backButton->getContentSize().height / 2)));

    auto menu = Menu::createWithItem(backButton);
    menu->setPosition(0, 0);
    this->addChild(menu, 1);

    Vector<MenuItem*> languageItems;
    auto languageLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("LANGUAGE")->getCString(),
                                                                           "Arial Rounded MT Bold", 100));
    languageLabel->setPosition(Vec2(origin.x + visibleSize.width / 4,
                                    origin.y + visibleSize.height - ((3 * visibleSize.height / 8))));
    languageLabel->setEnabled(false);
    languageLabel->setColor(Color3B::WHITE);
    languageItems.pushBack(languageLabel);

    auto catButton = MenuItemImage::create(
        "catFlag.png", "catFlagPressed.png", "catFlagPressed.png", CC_CALLBACK_1(UIConfiguration::catFlagCallback, this));
    catButton->setPosition(Vec2(origin.x + (visibleSize.width / 2),
                                languageLabel->getPosition().y));
    languageItems.pushBack(catButton);

    auto esButton = MenuItemImage::create(
        "esFlag.png", "esFlagPressed.png", "esFlagPressed.png", CC_CALLBACK_1(UIConfiguration::esFlagCallback, this));
    esButton->setPosition(Vec2(catButton->getPosition().x + catButton->getContentSize().width * 2,
                               languageLabel->getPosition().y));
    languageItems.pushBack(esButton);

    auto enButton = MenuItemImage::create(
        "enFlag.png", "enFlagPressed.png", "enFlagPressed.png", CC_CALLBACK_1(UIConfiguration::enFlagCallback, this));
    enButton->setPosition(Vec2(esButton->getPosition().x + esButton->getContentSize().width * 2,
                               languageLabel->getPosition().y));
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
    this->addChild(menuLanguage, 1);

    Vector<MenuItem*> soundItems;
    auto musicLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("MUSIC")->getCString(),
                                                                        "Arial Rounded MT Bold", 100));
    musicLabel->setPosition(Vec2(origin.x + visibleSize.width / 4,
                                 origin.y + visibleSize.height - ((5 * visibleSize.height / 8))));
    musicLabel->setEnabled(false);
    musicLabel->setColor(Color3B::WHITE);
    soundItems.pushBack(musicLabel);

    auto musicOnLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("ON")->getCString(),
                                                                          "Arial Rounded MT Bold", 100),
                                              CC_CALLBACK_1(UIConfiguration::musicOnCallback, this));
    musicOnLabel->setPosition(Vec2(origin.x + (4 * visibleSize.width / 8),
                                   musicLabel->getPosition().y));
    soundItems.pushBack(musicOnLabel);

    auto musicOffLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("OFF")->getCString(),
                                                                           "Arial Rounded MT Bold", 100),
                                               CC_CALLBACK_1(UIConfiguration::musicOffCallback, this));
    musicOffLabel->setPosition(Vec2(origin.x + (6 * visibleSize.width / 8),
                                    musicLabel->getPosition().y));
    soundItems.pushBack(musicOffLabel);

    auto sfxLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("SPECIAL_EFFECTS")->getCString(),
                                                                      "Arial Rounded MT Bold", 100));
    sfxLabel->setPosition(Vec2(origin.x + visibleSize.width / 4,
                               origin.y + visibleSize.height - ((6.5 * visibleSize.height / 8))));
    sfxLabel->setEnabled(false);
    sfxLabel->setColor(Color3B::WHITE);
    soundItems.pushBack(sfxLabel);

    auto sfxOnLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("ON")->getCString(),
                                                                        "Arial Rounded MT Bold", 100),
                                            CC_CALLBACK_1(UIConfiguration::sfxOnCallback, this));
    sfxOnLabel->setPosition(Vec2(origin.x + (4 * visibleSize.width / 8),
                                 sfxLabel->getPosition().y));
    soundItems.pushBack(sfxOnLabel);

    auto sfxOffLabel = MenuItemLabel::create(Label::createWithSystemFont(LocalizedString::create("OFF")->getCString(),
                                                                         "Arial Rounded MT Bold", 100),
                                             CC_CALLBACK_1(UIConfiguration::sfxOffCallback, this));
    sfxOffLabel->setPosition(Vec2(origin.x + (6 * visibleSize.width / 8),
                                  sfxLabel->getPosition().y));
    soundItems.pushBack(sfxOffLabel);

    if (GameData::getInstance()->getMusic() == true) {
        musicOnLabel->setEnabled(false);
        musicOnLabel->setColor(Color3B::GREEN);
    } else {
        musicOffLabel->setEnabled(false);
        musicOffLabel->setColor(Color3B::GREEN);
    }

    if (GameData::getInstance()->getSFX() == true) {
        sfxOnLabel->setEnabled(false);
        sfxOnLabel->setColor(Color3B::GREEN);
    } else {
        sfxOffLabel->setEnabled(false);
        sfxOffLabel->setColor(Color3B::GREEN);
    }

    auto menuSound = Menu::createWithArray(soundItems);
    menuSound->setPosition(0, 0);
    this->addChild(menuSound, 1);

    return true;
}

void UIConfiguration::menuBackCallback(Ref* pSender)
{
    UserDefault::getInstance()->setStringForKey("language", GameData::getInstance()->getLanguage());
    UserDefault::getInstance()->setBoolForKey("music", GameData::getInstance()->getMusic());
    UserDefault::getInstance()->setBoolForKey("sfx", GameData::getInstance()->getSFX());
    UserDefault::getInstance()->flush();

    auto scene = UIMainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::catFlagCallback(Ref* pSender)
{
    auto catButton = (MenuItem*)pSender;
    catButton->setEnabled(false);
    auto menuLanguage = (Menu*)catButton->getParent();
    auto esButton = (MenuItem*)menuLanguage->getChildren().at(2);
    auto enButton = (MenuItem*)menuLanguage->getChildren().at(3);

    esButton->setEnabled(true);
    enButton->setEnabled(true);
    GameData::getInstance()->setLanguage("cat");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::esFlagCallback(Ref* pSender)
{
    auto esButton = (MenuItem*)pSender;
    esButton->setEnabled(false);
    auto menuLanguage = (Menu*)esButton->getParent();
    auto catButton = (MenuItem*)menuLanguage->getChildren().at(1);
    auto enButton = (MenuItem*)menuLanguage->getChildren().at(3);

    catButton->setEnabled(true);
    enButton->setEnabled(true);
    GameData::getInstance()->setLanguage("es");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::enFlagCallback(Ref* pSender)
{
    auto enButton = (MenuItem*)pSender;
    enButton->setEnabled(false);
    auto menuLanguage = (Menu*)enButton->getParent();
    auto catButton = (MenuItem*)menuLanguage->getChildren().at(1);
    auto esButton = (MenuItem*)menuLanguage->getChildren().at(2);

    catButton->setEnabled(true);
    esButton->setEnabled(true);
    GameData::getInstance()->setLanguage("en");
    auto scene = UIConfiguration::createScene();
    Director::getInstance()->replaceScene(scene);
}

void UIConfiguration::musicOnCallback(Ref* pSender)
{
    auto musicOnLabel = (MenuItem*)pSender;
    musicOnLabel->setEnabled(false);
    musicOnLabel->setColor(Color3B::GREEN);
    auto menuSound = (Menu*)musicOnLabel->getParent();
    auto musicOffLabel = (MenuItem*)menuSound->getChildren().at(2);
    musicOffLabel->setEnabled(true);
    musicOffLabel->setColor(Color3B::WHITE);

    GameData::getInstance()->setMusic(true);
}

void UIConfiguration::musicOffCallback(Ref* pSender)
{
    auto musicOffLabel = (MenuItem*)pSender;
    musicOffLabel->setEnabled(false);
    musicOffLabel->setColor(Color3B::GREEN);
    auto menuSound = (Menu*)musicOffLabel->getParent();
    auto musicOnLabel = (MenuItem*)menuSound->getChildren().at(1);
    musicOnLabel->setEnabled(true);
    musicOnLabel->setColor(Color3B::WHITE);

    GameData::getInstance()->setMusic(false);
}

void UIConfiguration::sfxOnCallback(Ref* pSender)
{
    auto sfxOnLabel = (MenuItem*)pSender;
    sfxOnLabel->setEnabled(false);
    sfxOnLabel->setColor(Color3B::GREEN);
    auto menuSound = (Menu*)sfxOnLabel->getParent();
    auto sfxOffLabel = (MenuItem*)menuSound->getChildren().at(5);
    sfxOffLabel->setEnabled(true);
    sfxOffLabel->setColor(Color3B::WHITE);

    GameData::getInstance()->setSFX(true);
}

void UIConfiguration::sfxOffCallback(Ref* pSender)
{
    auto sfxOffLabel = (MenuItem*)pSender;
    sfxOffLabel->setEnabled(false);
    sfxOffLabel->setColor(Color3B::GREEN);
    auto menuSound = (Menu*)sfxOffLabel->getParent();
    auto sfxOnLabel = (MenuItem*)menuSound->getChildren().at(4);
    sfxOnLabel->setEnabled(true);
    sfxOnLabel->setColor(Color3B::WHITE);

    GameData::getInstance()->setSFX(false);
}