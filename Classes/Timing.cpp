//
//  Timing.cpp
//  simulplay
//
//  Created by Guillem Laborda on 09/01/15.
//
//

#include "Timing.h"

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
    clock_t stepTime = clock();
    clock_t powerTime = clock();

    while (GameLevel::getInstance()->getFinishedGame() == 0) {
        clock_t currentTime = clock();
        if (GameLevel::getInstance()->getTimeSpeed() != 0) {
            float step = ((float)currentTime / CLOCKS_PER_SEC) - ((float)stepTime / CLOCKS_PER_SEC);
            if (step >= GameLevel::getInstance()->getTimeSpeed()) {
                act = true;
                //cout << step << endl;
                //GameLevel::getInstance()->setTimeSteps(GameLevel::getInstance()->getTimeSteps() + 1);
                stepTime = clock();
            }

            if (((float)currentTime / CLOCKS_PER_SEC) - ((float)powerTime / CLOCKS_PER_SEC) >= 0.07) {
                powerTime = clock();
                for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
                    Power* p = GameLevel::getInstance()->getPowers()[i];
                    if (p->getDurationLeft() == p->getDuration()) {
                        p->setCooldownLeft(p->getCooldown());
                    }
                    if (p->getDurationLeft() > 0) {
                        p->setDurationLeft(p->getDurationLeft() - ((1.0 / GameLevel::getInstance()->getTimeSpeed()) / 10.0));
                    }
                    if (p->getCooldownLeft() > 0) {
                        p->setCooldownLeft(p->getCooldownLeft() - ((1.0 / GameLevel::getInstance()->getTimeSpeed()) / 10.0));
                    }
                }
            }
        }
    }
}