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
//  UIAchievements.cpp
//  simulplay
//
//  Created by Guillem Laborda on 30/1/15.
//
//

#include "UIAchievements.h"
#include "LocalizedString.h"
#include "GameData.h"

#include <audio/include/SimpleAudioEngine.h>

Scene* UIAchievements::createScene()
{
    auto scene = Scene::create();
    auto layer = UIAchievements::create();
    scene->addChild(layer);
    return scene;
}

bool UIAchievements::init()
{
    if (!Layer::init()) {
        return false;
    }

    glEnable(GL_STENCIL_TEST);

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("gui/MainMenuBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    background->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(background, 0);

    auto title = Sprite::create("gui/MainMenuTitle.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2((2 * visibleSize.width / 25),
        (15 * visibleSize.height / 18)));
    title->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(title, 5, 0);

    auto planet2 = Sprite::create("gui/MainMenuBackgroundPlanet2.png");
    planet2->setScale(1.3);
    planet2->setPosition(Vec2((18 * visibleSize.width / 40),
        (10 * visibleSize.height / 31)));
    planet2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    this->addChild(planet2, 1, 2);

    auto popupBackground = Sprite::create("gui/AchievementsBackground.png");
    popupBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    popupBackground->setPosition(Vec2((visibleSize.width / 2), (7.5 * visibleSize.height / 18)));
    this->addChild(popupBackground, 6);

    Vector<MenuItem*> menuButtons;
    auto backButton = MenuItemImage::create(
        "gui/ProgressMapBackButton.png", "gui/ProgressMapBackButtonPressed.png", CC_CALLBACK_1(UIAchievements::menuBackCallback, this));
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setPosition(Vec2(1 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 50);
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    popupBackground->addChild(menu, 1, 20);

    auto configLabel = Label::createWithTTF(LocalizedString::create("ACHIEVEMENTS"), "fonts/BebasNeue.otf", 100);
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(8 * popupBackground->getContentSize().width / 28, 13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);

    ui::ListView* list = ui::ListView::create();
    list->setDirection(ui::ListView::Direction::VERTICAL);
    list->setTouchEnabled(true);
    auto modelAux = ui::Button::create();
    modelAux->loadTextures("gui/AchievementBackground.png", "gui/AchievementBackgroundPressed.png");
    list->setContentSize(Size(modelAux->getContentSize().width, modelAux->getContentSize().height * 7.75));
    list->setItemsMargin(5);

    for (int i = 0; i < 15; i++) {
        auto model = ui::Button::create();
        model->addTouchEventListener(CC_CALLBACK_2(UIAchievements::showAchievement, this));
        if (i % 2 == 0) {
            model->loadTextures("gui/AchievementBackground.png", "gui/AchievementBackgroundPressed.png");
        }
        else {
            model->loadTextures("gui/AchievementBackground2.png", "gui/AchievementBackgroundPressed.png");
        }
        list->pushBackCustomItem(model);
        auto icon = Sprite::create("gui/AchievementIconOff.png");
        icon->setPosition(Vec2(model->getPositionX() + (icon->getBoundingBox().size.width / 1.5), model->getBoundingBox().size.height / 2));
        model->addChild(icon);
        auto title = Label::createWithTTF("ACHIEVEMENT TITLE", "fonts/BebasNeue.otf", 47);
        title->setColor(Color3B(190, 221, 226));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setPosition(Vec2((icon->getPositionX() / 2) + (icon->getBoundingBox().size.width), 5 * model->getBoundingBox().size.height / 7));
        model->addChild(title);
        auto text = Label::createWithSystemFont("Explanation on how to unlock the achievement", "Corbel", 30);
        text->setColor(Color3B(190, 221, 226));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2((icon->getPositionX() / 2) + (icon->getBoundingBox().size.width), 2 * model->getBoundingBox().size.height / 7));
        model->addChild(text);
    }

    list->setPosition(Vec2(9 * popupBackground->getContentSize().width / 28, 1 * popupBackground->getContentSize().height / 16));

    popupBackground->addChild(list);

    return true;
}

void UIAchievements::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    }

    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void UIAchievements::showAchievement(Ref* pSender, ui::Widget::TouchEventType aType)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (aType == ui::Widget::TouchEventType::ENDED) {
        Vector<MenuItem*> menuButtons;
        auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
            CC_CALLBACK_1(UIAchievements::restoreAchievementsWindow, this));
        darkBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        menuButtons.pushBack(darkBackground);
        auto menu = Menu::createWithArray(menuButtons);
        menu->setPosition(0, 0);
        this->addChild(menu, 10, 100);

        auto popupBackground = Sprite::create("gui/ProgressMapPopupBackground.png");
        popupBackground->setPosition(Vec2(visibleSize.width / 2, 3 * visibleSize.height / 7));
        popupBackground->setScale(GameData::getInstance()->getRaWConversion() * 1.3, GameData::getInstance()->getRaHConversion() * 1.4);
        //popupBackground->setScale(1.3, 1.4);
        this->addChild(popupBackground, 20, 101);
        selectedAchievement = ((ui::Button*)(pSender));
        selectedAchievement->setHighlighted(true);
    }
}

void UIAchievements::restoreAchievementsWindow(Ref* pSender)
{
    selectedAchievement->setHighlighted(false);
    //Menu
    this->removeChildByTag(100);
    //Popup
    this->removeChildByTag(101);
}
