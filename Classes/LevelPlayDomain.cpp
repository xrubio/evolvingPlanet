//
//  LevelPlayDomain.cpp
//  simulplay
//
//  Created by Guillem Laborda on 16/07/14.
//
//

#include "LevelPlayDomain.h"
#include <unistd.h>
#include "GameData.h"

using namespace std;

LevelPlayDomain::LevelPlayDomain()
{
    //time(&timeTaken);
    timeTakenPosition = clock();
    timeTakenAge = timeTakenPosition;
    positionPlayer.set(0,200);
}

void LevelPlayDomain::playLevel(void)
{
    float secondsPosition, secondsAge;
    clock_t currentTime;
    agePlayer = GameData::getInstance()->getAgePlayer();
    while (!finishedLevel)
    {
        //time(&currentTime);  /* get current time; same as: timer = time(NULL)  */
        currentTime = clock();
        //seconds = difftime(currentTime,timeTaken);

        secondsPosition = (float)(currentTime - timeTakenPosition) / CLOCKS_PER_SEC;
        secondsAge = (float)(currentTime - timeTakenAge) / CLOCKS_PER_SEC;
        
        //cout << seconds << endl;
        
        if (secondsPosition >= 0.05)
        {            
            if (positionPlayer.x >= 2048 and direction > 0)
            {
                direction = -direction;
            }
            if (positionPlayer.x <= 0 and direction < 0)
            {
                direction = -direction;
            }
            
            positionPlayer.x += velocityMultiplier*direction;
            //cout << agePlayer << " " << positionPlayer.x << " " << positionPlayer.y << endl;
            //time(&timeTaken);
            timeTakenPosition = clock();
        }
        if (secondsAge >= 2)
        {
            agePlayer += velocityMultiplier;
            GameData::getInstance()->setAgePlayer(agePlayer);
            timeTakenAge = clock();
            cout << "AGE: " << agePlayer << endl;
        }
    }

}