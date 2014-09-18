//
//  Die.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Die.h"
#include "UIGameplayMap.h"

bool Die::execute(int indexAgent)
{
    UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();
    Agent* agent = GameLevel::getInstance()->getAgents().at(indexAgent);

    int zone = gameplayMap->getValueAtGameplayMapHotSpot(agent->getPosition()->getX(),
                                                         agent->getPosition()->getY());
    int harm;
    switch (zone) {
    case 1:
        harm = 20;
        break;
    case 2:
        harm = 20;
        break;
    case 3:
        harm = 15;
        break;
    case 4:
        harm = 30;
        break;
    default:
        harm = 20;
        break;
    }
    int resistance = agent->getValOfAttribute("RESISTANCE");
    //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
    Power* p = nullptr;
    for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
        if (GameLevel::getInstance()->getPowers().at(i)->getName() == "ResistanceBoost") {
            p = GameLevel::getInstance()->getPowers().at(i);
        }
    }
    if (p != nullptr and p->getDurationLeft() > 0) {
        if (gameplayMap->isInBoostResistanceArea(agent->getPosition()->getX() * float(2048 / 200),
                                                 agent->getPosition()->getY() * float(1536 / 200))) {
            harm /= 2;
        }
    }
    harm = harm - resistance;
    if (harm < 0) {
        harm = 0;
    }
    agent->setLife(agent->getLife() - harm);

    if (agent->getLife() <= 0) {
        GameLevel::getInstance()->addDeletedAgent(agent->getId());
        GameLevel::getInstance()->deleteAgent(indexAgent);
        return true;
    }
    return false;
}
