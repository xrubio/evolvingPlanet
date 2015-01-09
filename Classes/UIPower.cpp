//
//  UIPower.cpp
//  simulplay
//
//  Created by Guillem Laborda on 12/09/14.
//
//

#include "UIPower.h"

void UIPower::setPosition(float x, float y)
{
    icon->setPosition(x, y);
    //cooldown->setPosition(icon->getContentSize().width / 2, icon->getContentSize().height / 2);
}

Power* UIPower::getPower(void)
{
    return power;
}

bool UIPower::getClicked(void)
{
    return clicked;
}

void UIPower::setClicked(bool b)
{
    clicked = b;
}

Sprite* UIPower::getIcon()
{
    return icon;
}
