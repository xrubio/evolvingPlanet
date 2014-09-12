//
//  UIMultiplierPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIMultiplierPower.h"
#include "UIGameplayMap.h"

UIMultiplierPower::UIMultiplierPower(string file)
{
    icon = Sprite::create(file);
    cooldown = Label::createWithSystemFont(to_string(GameLevel::getInstance()->getCooldownPower1()),
                                           "Arial Rounded MT Bold", 60);
    cooldown->setColor(Color3B::MAGENTA);
    cooldown->setVisible(false);
    cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
    icon->addChild(cooldown);
}

void UIMultiplierPower::onTouchesBegan(Point touchLocation)
{
    if (GameLevel::getInstance()->getCooldownPower1() == 0 and GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation)) {
        clicked = true;
        icon->setScale(1.25);
    }
}

void UIMultiplierPower::onTouchesMoved(Touch* touchLocation)
{
}

void UIMultiplierPower::onTouchesEnded(Point touchLocation)
{
    icon->setScale(1);
    if (GameLevel::getInstance()->getUIGameplayMap()->selectSpriteForTouch(icon, touchLocation) and clicked) {
        //Activar boost reproduction un cop s'ha tocat i soltat a sobre la imatge que toca
        icon->setColor(Color3B::GRAY);
        GameLevel::getInstance()->setPower1Active(5);
        cooldown->setVisible(true);
    }
}

void UIMultiplierPower::update()
{
    if (GameLevel::getInstance()->getPower1Active() == 0) {
        icon->setColor(Color3B::WHITE);
    }
    if (GameLevel::getInstance()->getCooldownPower1() > 0) {
        cooldown->setVisible(true);
        cooldown->setString(to_string(GameLevel::getInstance()->getCooldownPower1()));
    } else {
        cooldown->setVisible(false);
    }
}