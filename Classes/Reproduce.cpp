//
//  Reproduce.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Reproduce.h"
#include "UIGameplayMap.h"

bool Reproduce::execute(int indexAgent)
{
    Agent* agent = GameLevel::getInstance()->getAgents().at(indexAgent);
    if (GameLevel::getInstance()->getAgents().size() < GameLevel::getInstance()->getMaxAgents()) {
        int probReproduction = agent->getValOfAttribute("REPRODUCTION");
        //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
        Power* p = nullptr;
        for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
            if (GameLevel::getInstance()->getPowers().at(i)->getName() == "ReproductionBoost") {
                p = GameLevel::getInstance()->getPowers().at(i);
            }
        }
        if (p != nullptr and p->getDurationLeft() > 0) {
            probReproduction += 3;
        }
        int mobility = agent->getValOfAttribute("MOBILITY");
        if (probReproduction > 0) {
            int reproduce;
            if (probReproduction >= 10) {
                reproduce = 0;
            } else {
                reproduce = rand() % (10 - probReproduction) + 0;
            }
            if (reproduce == 0) {
                int posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
                int posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);
                if (GameLevel::getInstance()->validatePosition(posx, posy)) {
                    auto ag = new Agent(GameLevel::getInstance()->getIdCounter(), 100, posx, posy);
                    ag->setAttributes(GameLevel::getInstance()->getAgentAttributes());
                    GameLevel::getInstance()->addAgent(ag);
                    GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
                    GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
                    return true;
                }
            }
        }
    }
    return false;
}