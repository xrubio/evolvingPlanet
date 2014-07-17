//
//  LevelPlayDomain.cpp
//  simulplay
//
//  Created by Guillem Laborda on 16/07/14.
//
//

#include "LevelPlayDomain.h"
#include <unistd.h>

using namespace std;

LevelPlayDomain::LevelPlayDomain()
{
    //time(&timeTaken);
    timeTaken = clock();
    positionPlayer.set(0,200);
}

void LevelPlayDomain::playLevel(void)
{
    int delay = 1000;
    float seconds;
    clock_t currentTime;
    while (!finishedLevel)
    {
        //time(&currentTime);  /* get current time; same as: timer = time(NULL)  */
        currentTime = clock();
        //seconds = difftime(currentTime,timeTaken);

        seconds = (float)(currentTime - timeTaken) / CLOCKS_PER_SEC;
        
        //cout << seconds << endl;
        
        if (seconds >= 0.05)
        {
            agePlayer += velocityMultiplier;
            
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
            timeTaken = clock();
        }
    }

}