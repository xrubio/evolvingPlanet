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
//  Timing.cpp
//  simulplay
//
//  Created by Guillem Laborda on 09/01/15.
//
//

#include "Timing.h"
#include "UIGameplayMap.h"

Timing* Timing::timingInstance = nullptr;

float Timing::_secondsPerStep = 0.8f;

Timing* Timing::getInstance()
{
    if (!timingInstance) // Only allow one instance of class to be generated.
    {
        timingInstance = new Timing;
    }
    return timingInstance;
}
    
double Timing::secs( const timeval & time) const
{
    return time.tv_sec + double(time.tv_usec)/1000000.0f;
}

void Timing::start(void)
{
    CCLOG("timing start");
    
    // seconds for a time step
    timeval stepTime;
    gettimeofday(&stepTime, nullptr);

    // no idea
    timeval stepTimePart;
    gettimeofday(&stepTimePart, nullptr);

    timeval currentTimePart;

    while (GameLevel::getInstance()->getFinishedGame() == Running)
    {
        // if paused just store last time
        if (!GameLevel::getInstance()->isPlaying())
        {
            gettimeofday(&stepTime, nullptr);
            gettimeofday(&stepTimePart, nullptr);
            continue;
        }
        // store current time in currentTimePart
        gettimeofday(&currentTimePart, nullptr);

        double secsCurrentTimePart = secs(currentTimePart);
        float step = secsCurrentTimePart - secs(stepTime);

        // if seconds per step has passed and last step was already computed
        if(step >= _secondsPerStep and !act)
        {
            act = true;
            gettimeofday(&stepTime, nullptr);
        }

        // if powertime already passed
        for (size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
        {
            Power* p = GameLevel::getInstance()->getPowers().at(i);
            if(!p->isActivated())
            {
                continue;
            }
            CCLOG("power %s is activated", p->getName().c_str());

            // if duration finished then activate cooldown
            if(p->getDurationLeft() >= 0)
            {
                p->setDurationLeft(p->getDurationLeft() - ((1.0 / _secondsPerStep) / 10.0));
            }
            else
            {
                CCLOG("cooldown at: %f new value: %f", p->getCooldownLeft());
                p->setCooldownLeft(p->getCooldownLeft() - ((1.0 / _secondsPerStep) / 10.0));
                CCLOG("new value: %f", p->getCooldownLeft());
            }
        }

        float stepPart = secsCurrentTimePart - secs(stepTimePart);
        if (stepPart >= _secondsPerStep / 10.0)
        {
            GameLevel::getInstance()->getUIGameplayMap()->setTimeProgressBar(GameLevel::getInstance()->getUIGameplayMap()->getTimeProgressBar() + (0.09));
            gettimeofday(&stepTimePart, nullptr);
        }
    }
}

