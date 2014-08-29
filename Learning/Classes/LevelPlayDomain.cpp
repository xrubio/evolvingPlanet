//
//  LevelPlayDomain.cpp
//  simulplay
//
//  Created by Guillem Laborda on 16/07/14.
//
//

#include "LevelPlayDomain.h"
#include "GameData.h"

using namespace std;

LevelPlayDomain::LevelPlayDomain()
{
    timeTakenPosition = clock();
    timeTakenAge = timeTakenPosition;
    positionPlayer.set(0, 1450);
}

void LevelPlayDomain::playLevel(void)
{
    float secondsPosition, secondsAge;
    clock_t currentTime;
    agePlayer = GameData::getInstance()->getAgePlayer();
    while (!finishedLevel) {
        currentTime = clock();
        secondsPosition = (float)(currentTime - timeTakenPosition) / CLOCKS_PER_SEC;
        secondsAge = (float)(currentTime - timeTakenAge) / CLOCKS_PER_SEC;

        if (secondsPosition >= 0.05) {
            if (positionPlayer.x >= 2048 and direction > 0) {
                direction = -direction;
                positionPlayer.y -= 9;
            }
            if (positionPlayer.x <= 0 and direction < 0) {
                direction = -direction;
                positionPlayer.y -= 9;
            }

            positionPlayer.x += velocityMultiplier * direction;
            timeTakenPosition = clock();
        }
        if (secondsAge >= 5 - velocityMultiplier) {
            agePlayer++;
            GameData::getInstance()->setAgePlayer(agePlayer);
            timeTakenAge = clock();
        }
    }
}