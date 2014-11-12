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

bool Collect::execute(int type, int indexAgent)
{
    //UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();
    Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);

    GameLevel::getInstance()->setTimeExploited(agent->getPosition()->getX(), agent->getPosition()->getY(),
                                               GameLevel::getInstance()->getTimeExploited(agent->getPosition()->getX(),
                                                                                          agent->getPosition()->getY()) + 1);

    int recollection = agent->getValOfAttribute("RECOLLECTION");
    float efficiency;
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
    }

    Power* p = nullptr;
    Power* pRest = nullptr;
    for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
        if (GameLevel::getInstance()->getPowers().at(i)->getName() == "RecollectionBoost") {
            p = GameLevel::getInstance()->getPowers().at(i);
        }
        if (GameLevel::getInstance()->getPowers().at(i)->getName() == "RestoreLand") {
            pRest = GameLevel::getInstance()->getPowers().at(i);
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
        if (GameLevel::getInstance()->getGoals().at(i)->getAgentType() == type and ((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->getGoalAmount() > 0) {
            int mapSelector = 0;
            if (GameLevel::getInstance()->getDepleted(agent->getPosition()->getX(), agent->getPosition()->getY()) == true) {
                mapSelector = 1;
            } else if (GameLevel::getInstance()->getEnvironmentAdaptation(agent->getPosition()->getX(), agent->getPosition()->getY()) == true) {
                mapSelector = 2;
            }

            ((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->setCurrentAmount(
                ((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->getCurrentAmount() + (GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMapResources(mapSelector, agent->getPosition()->getX(), agent->getPosition()->getY()) * efficiency));
        }
    }
    return false;
}