//
//  Reproduce.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Reproduce.h"
#include "UIGameplayMap.h"

bool Reproduce::execute(int type, int indexAgent)
{
    Agent* agent = GameLevel::getInstance()->getAgents().at(type).at(indexAgent);
    if (GameLevel::getInstance()->getAgents().at(type).size() < GameLevel::getInstance()->getMaxAgent(agent->getType())) {
        int probReproduction = agent->getValOfAttribute("REPRODUCTION");
        //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio

        int mobility = agent->getValOfAttribute("MOBILITY") * 5;
        switch (probReproduction) {
        case 1:
            probReproduction = 20;
            break;
        case 2:
            probReproduction = 25;
            break;
        case 3:
            probReproduction = 30;
            break;
        case 4:
            probReproduction = 35;
            break;
        case 5:
            probReproduction = 40;
            break;
        case 6:
            probReproduction = 45;
            break;
        case 7:
            probReproduction = 55;
            break;
        case 8:
            probReproduction = 65;
            break;
        case 9:
            probReproduction = 80;
            break;
        case 10:
            probReproduction = 100;
            break;
        default:
            probReproduction = 0;
            break;
        }
        Power* p = nullptr;
        for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
            if (GameLevel::getInstance()->getPowers().at(i)->getName() == "ReproductionBoost") {
                p = GameLevel::getInstance()->getPowers().at(i);
            }
        }
        if (p != nullptr and p->getDurationLeft() > 0) {
            probReproduction += 30;
        }
        //srand(time(NULL));
        if ((rand() % 100) < probReproduction) {
            int maxIterations = 1000;
            int posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
            int posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);
            while (maxIterations > 0 and GameLevel::getInstance()->validatePosition(posx, posy) == false) {
                posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
                posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);
                maxIterations--;
            }
            if (maxIterations > 0) {

                //INFLUENCIA CULTURAL - CALCULAR TIPUS
                int type = agent->getType();
                if (agent->getValOfAttribute("CULTURAL_INFLUENCE") > 0) {
                }

                auto ag = new Agent(GameLevel::getInstance()->getIdCounter(), 100, type, posx, posy);
                ag->setAttributes(GameLevel::getInstance()->getAgentAttributes(type));
                GameLevel::getInstance()->addAgent(ag);
                GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
                GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
                return true;
            }
        }
    }
    return false;
}