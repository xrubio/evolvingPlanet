//
//  Kill.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/11/14.
//
//

#include "Kill.h"
#include "UIGameplayMap.h"

list<Agent*>::reverse_iterator Kill::execute(int type, Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);
    int techVal = agent->getValOfAttribute("TECHNOLOGY");
    float tech = 1;
    switch (techVal) {
        case 1:
            tech = 1.1;
            break;
        case 2:
            tech = 1.3;
            break;
        case 3:
            tech = 1.5;
            break;
        case 4:
            tech = 2;
            break;
        case 5:
            tech = 2.5;
            break;
        default:
            tech = 1;
            break;
    }
    int probKill = agent->getValOfAttribute("HOSTILITY") * tech;
    int mobility = (agent->getValOfAttribute("MOBILITY") * 4) + 1 * tech;
    if ((rand() % 100) < probKill * 10) {
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
}