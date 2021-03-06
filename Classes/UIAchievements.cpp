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
    
    if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX or CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    {
        this->setKeyboardEnabled(true);
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
    //backButton->setAnchorPoint(Vec2(0, 0.5));
    //backButton->setPosition(Vec2(1 * popupBackground->getContentSize().width / 28, 2 * popupBackground->getContentSize().height / 16));
    backButton->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    backButton->setPosition(Vec2((4 * visibleSize.width / 34), (1.5 * visibleSize.height / 25)));
    auto backLabel = Label::createWithTTF(LocalizedString::create("BACK"), "fonts/BebasNeue.otf", 60 * GameData::getInstance()->getRaConversion());
    backLabel->setColor(Color3B(205, 202, 207));
    backLabel->setPosition(backButton->getContentSize().width / 2, backButton->getContentSize().height / 2);
    backButton->addChild(backLabel);
    menuButtons.pushBack(backButton);

    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    this->addChild(menu, 1, 20);

    auto configLabel = Label::createWithTTF(LocalizedString::create("ACHIEVEMENTS"), "fonts/BebasNeue.otf", 100 * GameData::getInstance()->getRaConversion());
    configLabel->setColor(Color3B(255, 255, 255));
    configLabel->setAnchorPoint(Vec2(1, 0.5));
    configLabel->setPosition(Vec2(8 * popupBackground->getContentSize().width / 28, 13.5 * popupBackground->getContentSize().height / 16));
    popupBackground->addChild(configLabel);
    
    //GET NUM OF ACHIEVEMENTS COMPLETED
    int numAch = 0;
    int achCompleted = 0;
    for (int i = 0; i < GameData::getInstance()->getAchievements().size(); i++)
    {
        for (int j = 0; j < GameData::getInstance()->getAchievements().at(i).size(); j++)
        {
            if (GameData::getInstance()->getAchievements().at(i).at(j)->getCompleted() == true)
            {
                achCompleted++;
            }
            numAch++;
        }
    }
    
    auto labelProgressAchievements = Label::createWithTTF("( " + to_string(achCompleted) + " / " + to_string(numAch) + " )", "fonts/BebasNeue.otf", 80 * GameData::getInstance()->getRaConversion());
    labelProgressAchievements->setAnchorPoint(Vec2(1, 0.5));
    labelProgressAchievements->setPosition(Vec2(configLabel->getPositionX(), configLabel->getPositionY() - configLabel->getContentSize().height));
    labelProgressAchievements->setColor(Color3B::WHITE);
    popupBackground->addChild(labelProgressAchievements);

    ui::ListView* list = ui::ListView::create();
    list->setDirection(ui::ListView::Direction::VERTICAL);
    list->setTouchEnabled(true);
    auto modelAux = ui::Button::create();
    modelAux->loadTextures("gui/AchievementBackground.png", "gui/AchievementBackgroundPressed.png");
    list->setContentSize(Size(modelAux->getContentSize().width, modelAux->getContentSize().height * 7.75));
    list->setItemsMargin(5);
    
    vector< vector<Achievement*> > achs = GameData::getInstance()->getAchievements();
    int k = 0;
    for (int i = 0; i < achs.size(); i++) {
        for (int j = 0; j < achs.at(i).size(); j++)
        {
            auto model = ui::Button::create();
            model->addTouchEventListener(CC_CALLBACK_2(UIAchievements::showAchievement, this));
            if (k % 2 == 0) {
                model->loadTextures("gui/AchievementBackground.png", "gui/AchievementBackgroundPressed.png");
            }
            else {
                model->loadTextures("gui/AchievementBackground2.png", "gui/AchievementBackgroundPressed.png");
            }
            list->pushBackCustomItem(model);
            
            std::stringstream iconPathStr;
            iconPathStr << "gui/achievements/Ach" << achs.at(i).at(j)->getGoalType();
            if(achs.at(i).at(j)->getCompleted())
            {
                iconPathStr << "Unlocked.png";
            }
            else
            {
                iconPathStr << "Locked.png";
            }
            
            auto icon = Sprite::create(iconPathStr.str());
            icon->setPosition(Vec2(model->getPositionX() + (icon->getBoundingBox().size.width / 1.5), model->getBoundingBox().size.height / 2));
            model->addChild(icon);
            string key = to_string(i) + "_" + achs.at(i).at(j)->getGoalType();
            auto title = Label::createWithTTF(LocalizedString::create(string("TITLE_LVL"+key).c_str(), "achievements"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
            title->setColor(Color3B(190, 221, 226));
            title->setAnchorPoint(Vec2(0, 0.5));
            title->setPosition(Vec2((icon->getPositionX() / 2) + (icon->getBoundingBox().size.width),
                                    5 * model->getBoundingBox().size.height / 7));
            model->addChild(title);
            auto text = Label::createWithTTF(LocalizedString::create(string("DESCR_LVL"+key).c_str(), "achievements"), "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
            text->setColor(Color3B(190, 221, 226));
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setPosition(Vec2((icon->getPositionX() / 2) + (icon->getBoundingBox().size.width), 2 * model->getBoundingBox().size.height / 7));
            model->addChild(text);
            model->setName(to_string(i) + "-" + to_string(j));
            model->setEnabled(achs.at(i).at(j)->getCompleted());
            k++;
        }
    }

    list->setPosition(Vec2(9 * popupBackground->getContentSize().width / 28, 1 * popupBackground->getContentSize().height / 16));

    popupBackground->addChild(list);

    return true;
}

void UIAchievements::menuBackCallback(Ref* pSender)
{
    if (GameData::getInstance()->getSFX() == true) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.mp3");
    }

    auto scene = UIMainMenu::createScene();
    auto transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}


void UIAchievements::zoomImageInCallback(Ref* pSender)
{
    auto image = (MenuItemImage*)pSender;
    if (image->getNumberOfRunningActions() != 0)
    {
        return;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Vector<MenuItem*> menuButtons;
    auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
                                                CC_CALLBACK_1(UIAchievements::zoomImageOutCallback, this));
    darkBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    darkBackground->setOpacity(0);
    darkBackground->runAction(FadeIn::create(0.5));
    menuButtons.pushBack(darkBackground);
    auto menu = Menu::createWithArray(menuButtons);
    menu->setPosition(0, 0);
    
    image->runAction(ScaleTo::create(0.5, 1.2 * GameData::getInstance()->getRaConversion()));
    image->setCallback(CC_CALLBACK_1(UIAchievements::zoomImageOutCallback, this));
}

void UIAchievements::zoomImageOutCallback(Ref* pSender)
{
    auto image = (MenuItemImage*)(pSender);
    if (image->getNumberOfRunningActions() != 0)
    {
        return;
    }
    image->runAction(ScaleTo::create(0.5, GameData::getInstance()->getRaConversion()));
    image->setCallback(CC_CALLBACK_1(UIAchievements::zoomImageInCallback, this));
}


void UIAchievements::showAchievement(Ref* pSender, ui::Widget::TouchEventType aType)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (aType == ui::Widget::TouchEventType::ENDED) {
        if (GameData::getInstance()->getSFX() == true) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.mp3");
        }
        
        Vector<MenuItem*> menuButtons;
        auto darkBackground = MenuItemImage::create("gui/ProgressMapDarkBackground.png", "gui/ProgressMapDarkBackground.png",
            CC_CALLBACK_1(UIAchievements::restoreAchievementsWindow, this));
        darkBackground->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
        darkBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        darkBackground->setColor(Color3B::GRAY);
        menuButtons.pushBack(darkBackground);
        auto menu = Menu::createWithArray(menuButtons);
        menu->setPosition(0, 0);
        this->addChild(menu, 10, 100);

        auto popupBackground = Sprite::create("gui/ProgressMapPopupBackground.png");
        popupBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        popupBackground->setScale(GameData::getInstance()->getRaWConversion() * 1.3, GameData::getInstance()->getRaHConversion() * 1.4);
        //popupBackground->setScale(1.3, 1.4);
        this->addChild(popupBackground, 20, 101);
        selectedAchievement = ((ui::Button*)(pSender));
        selectedAchievement->setHighlighted(true);
        
        size_t dash = selectedAchievement->getName().find("-");
        int i = atoi(selectedAchievement->getName().substr(0, dash).c_str());
        int j = atoi(selectedAchievement->getName().substr(dash + 1).c_str());
        auto ach = GameData::getInstance()->getAchievements().at(i).at(j);
        size_t pos = ach->getResource().find("-");
        string resourceType = ach->getResource().substr(0, pos);
        string resource = ach->getResource().substr(pos + 1);
        
        if (resourceType == "IMG")
        {
            string textImage = "";
            //check if there is text
            if (resource.find("-") != string::npos)
            {
                pos = resource.find("-");
                textImage = resource.substr(0, pos);
                resource = resource.substr(pos + 1);
            }
            
            auto contextImage = MenuItemImage::create(resource, resource);// CC_CALLBACK_1(UIAchievements::zoomImageInCallback, this));
            float ratio = std::min(popupBackground->getContentSize().width/contextImage->getContentSize().width, popupBackground->getContentSize().height/contextImage->getContentSize().height);
            CCLOG("ratio: %f frac width %f height: %f", ratio, popupBackground->getContentSize().width/contextImage->getContentSize().width, popupBackground->getContentSize().height/contextImage->getContentSize().height);
            
      
            // occupy all the screen if no text
/*            if (textImage != "")
            {
                contextImage->setScale(0.7f*ratio);
                contextImage->setAnchorPoint(Vec2(0.5, 0.0f));
                contextImage->setPosition(0.5f*popupBackground->getContentSize().width, 0.05f*popupBackground->getContentSize().height);
            }
            else
            {
            */
                contextImage->setScale(0.9f*ratio);
                contextImage->setAnchorPoint(Vec2(0.5, 0.5f));
                contextImage->setPosition(0.5f*popupBackground->getContentSize().width, 0.5f*popupBackground->getContentSize().height);
//            }
            
            auto menuContext = Menu::create(contextImage, NULL);
            menuContext->setPosition(0, 0);
            popupBackground->addChild(menuContext);
            
            if (textImage != "")
            {
                auto textLabel = Label::createWithTTF(LocalizedString::create(textImage.c_str(), "achievements"), "fonts/arial.ttf", 30 * GameData::getInstance()->getRaConversion());
                textLabel->setColor(Color3B(216, 229, 235));
                textLabel->setMaxLineWidth(0.9f*popupBackground->getContentSize().width);
                textLabel->setAnchorPoint(Vec2(0.5, 0.0));
                textLabel->setPosition(Vec2(0.5f*popupBackground->getContentSize().width, 0.05*popupBackground->getContentSize().height));

                auto labelBorder = DrawNode::create();
                float marginWidth = 0.02f*textLabel->getContentSize().width;
                float marginHeight = 0.02f*textLabel->getContentSize().height;
                Vec2 margin(marginWidth, marginHeight);
                Vec2 origin(textLabel->getBoundingBox().origin - margin);
                Vec2 end(textLabel->getBoundingBox().origin + textLabel->getBoundingBox().size + margin);
                labelBorder->drawSolidRect(origin, end, Color4F(Color4B(42,52,55,180)));
                labelBorder->drawRect(origin, end, Color4F(Color4B(55,83,101,255)));

                popupBackground->addChild(labelBorder);
                popupBackground->addChild(textLabel);
            }
        }
        else if (resourceType == "TXT")
        {
            auto contextIntroduction = TextFieldTTF::textFieldWithPlaceHolder(LocalizedString::create(resource.c_str(), "achievements"), Size(visibleSize.width / (1.8 * GameData::getInstance()->getRaWConversion()), visibleSize.height), TextHAlignment::LEFT, "fonts/BebasNeue.otf", 40 * GameData::getInstance()->getRaConversion());
            contextIntroduction->setColorSpaceHolder(Color4B(216, 229, 235, 255));
            contextIntroduction->setPosition(Vec2(popupBackground->getContentSize().width / 2, popupBackground->getContentSize().height / 2));
            popupBackground->addChild(contextIntroduction);
        }
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

void UIAchievements::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        menuBackCallback(this);
    }
}
