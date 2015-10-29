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

Timing* Timing::getInstance()
{
    if (!timingInstance) // Only allow one instance of class to be generated.
    {
        timingInstance = new Timing;
    }
    return timingInstance;
}

void Timing::start(void)
{
    //clock_t stepTime = clock();
    timeval stepTime;
    gettimeofday(&stepTime, nullptr);
    timeval stepTimePart;
    timeval currentTimePart;
    gettimeofday(&stepTimePart, nullptr);

    //clock_t powerTime = clock();
    timeval powerTime;
    gettimeofday(&powerTime, nullptr);

    while (GameLevel::getInstance()->getFinishedGame() == Running) {
        //clock_t currentTime = clock();
        gettimeofday(&currentTimePart, nullptr);
        if (GameLevel::getInstance()->getTimeSpeed() > 0.0) {
            //float step = ((float)currentTime / CLOCKS_PER_SEC) - ((float)stepTime / CLOCKS_PER_SEC);
            float step = (currentTimePart.tv_sec + (currentTimePart.tv_usec / 1000000.0)) - (stepTime.tv_sec + (stepTime.tv_usec / 1000000.0));
            //float stepPart = ((float)currentTimePart / CLOCKS_PER_SEC) - ((float)stepTimePart / CLOCKS_PER_SEC);
            float stepPart = (currentTimePart.tv_sec + (currentTimePart.tv_usec / 1000000.0)) - (stepTimePart.tv_sec + (stepTimePart.tv_usec / 1000000.0));
            if (step >= GameLevel::getInstance()->getTimeSpeed() and step > GameLevel::getInstance()->calcTime + 0.4 and act == false and GameLevel::getInstance()->paint == true and GameLevel::getInstance()->getUIGameplayMap()->play == true) {
                act = true;
                //CCLOG("Time: %f %i", step, act);
                //GameLevel::getInstance()->setTimeSteps(GameLevel::getInstance()->getTimeSteps() + 1);
                //stepTime = clock();
                gettimeofday(&stepTime, nullptr);
            }

            //if (((float)currentTime / CLOCKS_PER_SEC) - ((float)powerTime / CLOCKS_PER_SEC) >= 0.07) {
            if ((currentTimePart.tv_sec + (currentTimePart.tv_usec / 1000000.0)) - (powerTime.tv_sec + (powerTime.tv_usec / 1000000.0)) >= 0.07) {
                //powerTime = clock();
                gettimeofday(&powerTime, nullptr);
                for (size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
                    Power* p = GameLevel::getInstance()->getPowers().at(i);
                    if (p->getDurationLeft() == p->getDuration()) {
                        p->setCooldownLeft(p->getCooldown());
                    }
                    if (p->getDurationLeft() > 0) {
                        p->setDurationLeft(p->getDurationLeft() - ((1.0 / GameLevel::getInstance()->getTimeSpeed()) / 10.0));
                    }
                    if (p->getCooldownLeft() > 0 and p->getDurationLeft() < 0.1) {
                        p->setCooldownLeft(p->getCooldownLeft() - ((1.0 / GameLevel::getInstance()->getTimeSpeed()) / 10.0));
                    }
                }
            }
            if (stepPart >= GameLevel::getInstance()->getTimeSpeed() / 10.0) {
                //CCLOG("stepPart %i", stepPart);
                GameLevel::getInstance()->getUIGameplayMap()->setTimeProgressBar(GameLevel::getInstance()->getUIGameplayMap()->getTimeProgressBar() + (0.09));
                //stepTimePart = clock();
                gettimeofday(&stepTimePart, nullptr);
            }
        }
        else {
            //stepTime = clock();
            gettimeofday(&stepTime, nullptr);
            //stepTimePart = clock();
            gettimeofday(&stepTimePart, nullptr);
        }
    }
}
