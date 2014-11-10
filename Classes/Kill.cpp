//
//  Kill.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/11/14.
//
//

#include "Kill.h"
#include "UIGameplayMap.h"

bool Kill::execute(int type, int indexAgent)
{
    Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);
    int probKill = agent->getValOfAttribute("HOSTILITY");
    int mobility = agent->getValOfAttribute("MOBILITY") * 5;
    if ((rand() % 100) < probKill * 100) {
        int maxIterations = 100;
        bool kill = false;
        int posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
        int posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);
        while (maxIterations > 0 and kill == false) {
            if (posx >= 0 and posx < 480 and posy >= 0 and posy < 320) {
                if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != type) {
                    GameLevel::getInstance()->getAgentAtMap(posx, posy)->setLife(0);
                    kill = true;
                }
            }
            posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
            posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);
            maxIterations--;
        }
    }

    return false;
}