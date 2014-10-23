//
//  Deplete.cpp
//  simulplay
//
//  Created by Guillem Laborda on 23/10/14.
//
//

#include "Deplete.h"
#include "UIGameplayMap.h"

bool Deplete::execute(int type, int indexAgent)
{
    Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    int sustainability = agent->getValOfAttribute("SUSTAINABILITY");
    switch (sustainability) {
    case 1:
        sustainability = 70;
        break;
    case 2:
        sustainability = 65;
        break;
    case 3:
        sustainability = 60;
        break;
    case 4:
        sustainability = 50;
        break;
    case 5:
        sustainability = 40;
        break;
    case 6:
        sustainability = 30;
        break;
    case 7:
        sustainability = 25;
        break;
    case 8:
        sustainability = 15;
        break;
    case 9:
        sustainability = 10;
        break;
    case 10:
        sustainability = 0;
        break;
    default:
        sustainability = 100;
        break;
    }

    //JUGAR AMB EL TEMPS QUE HA ESTAT OCUPAT L'ESPAI I LA PROBBILITAT
    if (GameLevel::getInstance()->getTimeExploited(agent->getPosition()->getX(), agent->getPosition()->getY()) > 2
        and (rand() % 100) < sustainability) {
        GameLevel::getInstance()->setDepleted(agent->getPosition()->getX(), agent->getPosition()->getY(), true);
    }

    return false;
}