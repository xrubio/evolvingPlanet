//
//  UIMultiplierPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIMultiplierPower.h"
#include "UIGameplayMap.h"

UIMultiplierPower::UIMultiplierPower(Power* p) : UIPower(p)
{
    icon = Sprite::create("PowerBackgroundButton.png");
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    auto button = Sprite::create(p->getName() + "Button" + ".png");
    button->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(button, 2, 0);
    auto actionTimer = ProgressTimer::create(Sprite::create(p->getName() + "ActionButton" + ".png"));
    actionTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    actionTimer->setType(ProgressTimer::Type::RADIAL);
    icon->addChild(actionTimer, 1, 1);
    auto cooldownTimer = ProgressTimer::create(Sprite::create("PowerCooldownButton.png"));
    cooldownTimer->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    cooldownTimer->setType(ProgressTimer::Type::BAR);
    cooldownTimer->setBarChangeRate(Vec2(0, 1));
    cooldownTimer->setMidpoint(Vec2(0, 0));
    cooldownTimer->setVisible(false);
    icon->addChild(cooldownTimer, 3, 2);
    //cooldown = Label::createWithSystemFont(to_string(power->getCooldownLeft()), "Arial Rounded MT Bold", 60);
    //cooldown->setColor(Color3B::MAGENTA);
    //cooldown->setVisible(false);
    //cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    //icon->addChild(cooldown, 3);
}

void UIMultiplierPower::onTouchesBegan(Point touchLocation)
{
    if (power->getCooldownLeft() <= 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
        clicked = true;
        icon->setScale(0.8 * GameData::getInstance()->getRaWConversion(), 0.8 * GameData::getInstance()->getRaHConversion());
        auto button = (Sprite*)icon->getChildByTag(0);
        button->setColor(Color3B::GRAY);
    }
}

void UIMultiplierPower::onTouchesMoved(Touch* touchLocation)
{
}

void UIMultiplierPower::onTouchesEnded(Point touchLocation)
{
    icon->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) and clicked) {
        //Activar boost reproduction un cop s'ha tocat i soltat a sobre la imatge que toca
        //icon->setColor(Color3B::GRAY);
        power->setDurationLeft(power->getDuration());
        //cooldown->setVisible(true);
    }
    auto button = (Sprite*)icon->getChildByTag(0);
    button->setColor(Color3B::WHITE);
    clicked = false;
    actionTime = 0.0;
}

void UIMultiplierPower::update(float delta)
{
    ProgressTimer* actionTimer = (ProgressTimer*)icon->getChildByTag(1);
    ProgressTimer* cooldownTimer = (ProgressTimer*)icon->getChildByTag(2);

    /*if (clicked == false and power->getDurationLeft() > 0) {
        actionTime += delta;
        if (actionTime > power->getDuration()) {
            actionTime = 0.0;
        }
    }*/

    actionTimer->setPercentage((power->getDurationLeft() / power->getDuration()) * 100.0);

    //actionTimer->setPercentage((float(power->getDurationLeft())) / float(power->getDuration()) * 100.0);

    if (power->getCooldownLeft() > 0) {
        //cooldown->setVisible(true);
        //cooldown->setString(to_string(power->getCooldownLeft()));
        cooldownTimer->setVisible(true);
        cooldownTimer->setPercentage(float(power->getCooldownLeft()) / float(power->getCooldown()) * 100);
    }
    else {
        actionTimer->setPercentage(100.0);
        actionTime = 0.0;
        //cooldown->setVisible(false);
        cooldownTimer->setVisible(false);
    }
}
