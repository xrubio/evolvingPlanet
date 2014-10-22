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
    for (int i = 0; i < GameLevel::getInstance()->getGoals().size(); i++) {
        if (GameLevel::getInstance()->getGoals().at(i)->getAgentType() == type and ((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->getGoalAmount() > 0) {
            ((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->setCurrentAmount(((CollectionGoal*)GameLevel::getInstance()->getGoals().at(i))->getCurrentAmount() + 1);
        }
    }
    return false;
}