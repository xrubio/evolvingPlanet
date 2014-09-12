//
//  UIAreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIAreaPower.h"
#include "UIGameplayMap.h"

UIAreaPower::UIAreaPower(string file)
{
    icon = Sprite::create(file);
    cooldown = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getCooldownPower2()),
                                           "Arial Rounded MT Bold", 60);
    cooldown->setColor(Color3B::BLUE);
    cooldown->setVisible(false);
    cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(cooldown);
    area = Sprite::create("BoostResistanceArea.png");
    area->setOpacity(100);
    area->setVisible(false);
}

Sprite* UIAreaPower::getArea(void)
{
    return area;
}

void UIAreaPower::onTouchesBegan(Point touchLocation)
{
    if (GameLevel::getInstance()->getCooldownPower2() == 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
        clicked = true;
        area->setPosition(area->getParent()->convertToNodeSpace(icon->getPosition()));
        area->setVisible(true);
        icon->setColor(Color3B::GRAY);
    }
}

void UIAreaPower::onTouchesMoved(Touch* touchLocation)
{
    if (clicked) {
        Point touchArea = area->getParent()->convertToNodeSpace(Director::getInstance()->convertToGL(touchLocation->getLocationInView()));
        Point touch = Director::getInstance()->convertToGL(touchLocation->getLocationInView());
        if (touchLocation) {
            area->setPosition(touchArea);
        }
    }
}

void UIAreaPower::onTouchesEnded(Point touchLocation)
{
    if (clicked) {
        icon->setColor(Color3B::WHITE);
        if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) == false) {
            GameLevel::getInstance()->setPower2Active(10);
            cooldown->setVisible(true);
        }
    }
    clicked = false;
}

void UIAreaPower::update()
{
    if (clicked == false and GameLevel::getInstance()->getPower2Active() == 0) {
        area->setVisible(false);
    }
    if (GameLevel::getInstance()->getCooldownPower2() > 0) {
        cooldown->setVisible(true);
        cooldown->setString(to_string(GameLevel::getInstance()->getCooldownPower2()));
    } else {
        cooldown->setVisible(false);
    }
}