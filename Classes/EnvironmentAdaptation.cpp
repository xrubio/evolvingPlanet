//
//  EnvironmentAdaptation.cpp
//  simulplay
//
//  Created by Guillem Laborda on 24/10/14.
//
//

#include "EnvironmentAdaptation.h"
#include "UIGameplayMap.h"

bool EnvironmentAdaptation::execute(int type, int indexAgent)
{
    Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    int adaptation = agent->getValOfAttribute("ADAPTATION");
    switch (adaptation) {
    case 1:
        adaptation = 70;
        break;
    case 2:
        adaptation = 65;
        break;
    case 3:
        adaptation = 60;
        break;
    case 4:
        adaptation = 50;
        break;
    case 5:
        adaptation = 40;
        break;
    case 6:
        adaptation = 30;
        break;
    case 7:
        adaptation = 25;
        break;
    case 8:
        adaptation = 15;
        break;
    case 9:
        adaptation = 10;
        break;
    case 10:
        adaptation = 0;
        break;
    default:
        adaptation = 100;
        break;
    }

    //JUGAR AMB EL TEMPS QUE HA ESTAT OCUPAT L'ESPAI I LA PROBBILITAT
    if (GameLevel::getInstance()->getTimeExploited(agent->getPosition()->getX(), agent->getPosition()->getY()) > 2
        and (rand() % 100) < adaptation) {
        GameLevel::getInstance()->setEnvironmentAdaptation(agent->getPosition()->getX(), agent->getPosition()->getY(), true);
    }

    return false;
}