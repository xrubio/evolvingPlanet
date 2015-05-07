//
//  Collect.cpp
//  simulplay
//
//  Created by Guillem Laborda on 20/10/14.
//
//

#include "Collect.h"
#include "UIGameplayMap.h"
#include "CollectionGoal.h"

list<Agent*>::reverse_iterator Collect::execute(int type, Agent* agent)
{
    //UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();
    //Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    GameLevel::getInstance()->setTimeExploited(agent->getPosition()->getX(), agent->getPosition()->getY(),
        GameLevel::getInstance()->getTimeExploited(agent->getPosition()->getX(),
            agent->getPosition()->getY()) + 1);

    float efficiency = GameLevel::getInstance()->getAttributesValues(type, "RECOLLECTION", agent->getValOfAttribute("RECOLLECTION")) / 100;
    /*float efficiency;
    switch (recollection) {
    case 1:
        efficiency = 0.2;
        break;
    case 2:
        efficiency = 0.3;
        break;
    case 3:
        efficiency = 0.4;
        break;
    case 4:
        efficiency = 0.5;
        break;
    case 5:
        efficiency = 0.7;
        break;
    case 6:
        efficiency = 0.8;
        break;
    case 7:
        efficiency = 1;
        break;
    case 8:
        efficiency = 1.3;
        break;
    case 9:
        efficiency = 1.5;
        break;
    case 10:
        efficiency = 2;
        break;
    default:
        efficiency = 0;
        break;
    }*/

    Power* p = nullptr;
    Power* pRest = nullptr;
    for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
        if (GameLevel::getInstance()->getPowers()[i]->getNameInt() == 2) {
            p = GameLevel::getInstance()->getPowers()[i];
        }
        if (GameLevel::getInstance()->getPowers()[i]->getNameInt() == 3) {
            pRest = GameLevel::getInstance()->getPowers()[i];
        }
    }
    if (p != nullptr and p->getDurationLeft() > 0) {
        efficiency += 0.5;
    }

    if (pRest != nullptr and pRest->getDurationLeft() > 0) {
        //RESTORE LAND
        GameLevel::getInstance()->getUIGameplayMap()->restoreLand();
        pRest->setDurationLeft(0);
        pRest->setCooldownLeft(pRest->getCooldown() + 1);
    }

    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        if (GameLevel::getInstance()->getGoals()[i]->getAgentType() == type and ((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->getGoalAmount() > 0) {
            int mapSelector = 0;
            if (GameLevel::getInstance()->getDepleted(agent->getPosition()->getX(), agent->getPosition()->getY())) {
                mapSelector = 1;
            }
            else if (GameLevel::getInstance()->getEnvironmentAdaptation(agent->getPosition()->getX(), agent->getPosition()->getY())) {
                mapSelector = 2;
            }

            ((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->setCurrentAmount(((CollectionGoal*)GameLevel::getInstance()->getGoals()[i])->getCurrentAmount() + (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(mapSelector, agent->getPosition()->getX(), agent->getPosition()->getY(), 1) * efficiency));
        }
    }
}