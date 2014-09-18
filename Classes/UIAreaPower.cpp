//
//  UIAreaPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIAreaPower.h"
#include "UIGameplayMap.h"

UIAreaPower::UIAreaPower(Power* p)
{
    power = p;
    string filename = p->getName() + "Button" + ".png";
    icon = Sprite::create(filename);
    cooldown = Label::createWithSystemFont(to_string(power->getCooldownLeft()), "Arial Rounded MT Bold", 60);
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
    if (power->getCooldownLeft() == 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
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
            power->setDurationLeft(power->getDuration());
            cooldown->setVisible(true);
        }
    }
    clicked = false;
}

void UIAreaPower::update()
{
    if (clicked == false and power->getDurationLeft() == 0) {
        area->setVisible(false);
    }
    if (power->getCooldownLeft() > 0) {
        cooldown->setVisible(true);
        cooldown->setString(to_string(power->getCooldownLeft()));
    } else {
        cooldown->setVisible(false);
    }
}