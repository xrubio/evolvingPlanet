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
    _timeStep = 0.0f;
    _lastStepExecuted = -1;
    
    // seconds for a time step
    timeval stepTime;
    gettimeofday(&stepTime, nullptr);

    timeval currentTimePart;
    gettimeofday(&currentTimePart, nullptr);
    double secsCurrentTimePart = secs(currentTimePart);

    long int interval = int(1000.0f*float(_secondsPerStep)/20.0f);
    CCLOG("timing start with interval %ld", interval );
    while (GameLevel::getInstance()->getFinishedGame() == Running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        // store current time in currentTimePart
        gettimeofday(&currentTimePart, nullptr);
        double oldPart = secsCurrentTimePart;
        secsCurrentTimePart = secs(currentTimePart);

        // if paused just store last time
        if (!GameLevel::getInstance()->isPlaying())
        {
            gettimeofday(&stepTime, nullptr);
            continue;
        }
        
        if(!act and int(floor(_timeStep)>_lastStepExecuted))
        {
            act = true;
            _lastStepExecuted = int(floor(_timeStep));
        }

        double diff = (secsCurrentTimePart - oldPart)/_secondsPerStep;
        _timeStep += diff;

        for (size_t i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
        {
            Power* p = GameLevel::getInstance()->getPowers().at(i);
            if(!p->isActivated())
            {
                continue;
            }

            // if duration finished then activate cooldown
            if(p->getDurationLeft() > 0)
            {
                p->decreaseDurationLeft(diff);
                // if duration is exhausted then start cooldown
                if(p->getDurationLeft()<=0)
                {
                    p->resetCooldownLeft();
                }
            }
            else if(p->getCooldownLeft()>0)
            {
                p->decreaseCooldownLeft(diff); 
            }
        }
    }
}

