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
                int probCulture = agent->getValOfAttribute("CULTURAL_INFLUENCE");
                if (probCulture > 0) {

                    //CONTAR AGENTS ALREDEDOR SEGONS TIPUS
                    vector<int> numAgentsPerType;
                    for (int i = 0; i < GameLevel::getInstance()->getMaxAgents().size(); i++) {
                        numAgentsPerType.push_back(0);
                    }
                    for (int i = -mobility / 2; i < mobility / 2; i++) {
                        for (int j = -mobility / 2; j < mobility / 2; j++) {
                            int posx = agent->getPosition()->getX() + i;
                            int posy = agent->getPosition()->getY() + j;
                            if (posx >= 0 and posy >= 0 and posx < 480 and posy < 320) {
                                if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr) {
                                    numAgentsPerType[GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType()] = numAgentsPerType[GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType()] + 1;
                                }
                            }
                        }
                    }

                    //CALCULAR PROBABILITATS EN FUNCIO DEL NUMERO DAGENTS DE CADA TIPUS
                    vector<float> probPerType;
                    float probMid = 0;
                    float probTotal = 0;
                    for (int i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
                        probPerType.push_back(GameLevel::getInstance()->getAgentAttributes(i)["CULTURAL_INFLUENCE"]);
                        probMid += GameLevel::getInstance()->getAgentAttributes(i)["CULTURAL_INFLUENCE"];
                    }
                    for (int i = 0; i < probPerType.size(); i++) {
                        probPerType[i] = (probPerType[i] / probMid) * numAgentsPerType.at(i);
                        probTotal += probPerType[i];
                    }
                    for (int i = 0; i < probPerType.size(); i++) {
                        if (i == 0) {
                            probPerType[i] = (probPerType[i] / probTotal) * 100;
                        } else {
                            probPerType[i] = ((probPerType[i] / probTotal) * 100) + probPerType[i - 1];
                        }
                    }
                    int p = rand() % 100;
                    bool exit = false;
                    for (int i = 0; i < probPerType.size() and exit == false; i++) {
                        if (i == 0 and p < probPerType.at(i)) {
                            type = i;
                            exit = true;
                        } else if (p < probPerType.at(i) and p >= probPerType.at(i - 1)) {
                            type = i;
                            exit = true;
                        }
                    }
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