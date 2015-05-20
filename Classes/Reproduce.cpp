//
//  Reproduce.cpp
//  simulplay
//
//  Created by Guillem Laborda on 10/09/14.
//
//

#include "Reproduce.h"
#include "UIGameplayMap.h"

list<Agent*>::reverse_iterator Reproduce::execute(int typeAgent, Agent* agent)
{
    //Agent* agent = GameLevel::getInstance()->getAgents().at(typeAgent).at(indexAgent);

    //INFLUENCIA CULTURAL - CALCULAR TIPUS
    int type = agent->getType();
    int probCulture = agent->getValOfAttribute("CULTURAL_INFLUENCE");
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
    int mobility = (agent->getValOfAttribute("MOBILITY") * 4) + 1 * tech;
    if (probCulture > -1) {
        probCulture = probCulture * tech;
        //CONTAR AGENTS ALREDEDOR SEGONS TIPUS
        vector<int> numAgentsPerType;
        for (int i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
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
        float probMid = 0.0;
        float probTotal = 0.0;
        for (int i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
            probPerType.push_back(float(GameLevel::getInstance()->getAgentAttributes(i)["CULTURAL_INFLUENCE"]));
            probMid += float(GameLevel::getInstance()->getAgentAttributes(i)["CULTURAL_INFLUENCE"]);
        }
        for (int i = 0; i < probPerType.size(); i++) {
            probPerType[i] = (probPerType[i] / probMid) * float(numAgentsPerType[i]);
            probTotal += probPerType[i];
        }
        for (int i = 0; i < probPerType.size(); i++) {
            if (i == 0) {
                probPerType[i] = (probPerType[i] / probTotal) * 100.0;
            }
            else {
                probPerType[i] = ((probPerType[i] / probTotal) * 100.0) + probPerType[i - 1];
            }
        }
        int p = rand() % 100;
        bool exit = false;
        for (int i = 0; i < probPerType.size() and exit == false; i++) {
            if (i == 0 and p < probPerType[i]) {
                type = i;
                exit = true;
            }
            else if (p < probPerType[i] and p >= probPerType[i - 1]) {
                type = i;
                exit = true;
            }
        }
    }

    //REPRODUIR-SE NORMAL (SI HI HA INLFUENCIA CULTURAL I EL TIPUS ES EL MATEIX TAMBE)
    bool maxReached = false;
    if (GameLevel::getInstance()->getMaxAllAgents() > 0) {
        int numAgents = 0;
        for (int i = 0; i < GameLevel::getInstance()->getAgents().size(); i++) {
            numAgents += GameLevel::getInstance()->getAgents()[i].size();
        }
        maxReached = numAgents >= GameLevel::getInstance()->getMaxAllAgents();
    }
    else {
        maxReached = GameLevel::getInstance()->getAgents()[typeAgent].size() >= GameLevel::getInstance()->getMaxAgent(agent->getType());
    }
    if (type == agent->getType() and maxReached == false) {
        /*int probReproduction = GameLevel::getInstance()->getAttributesValues(typeAgent, "REPRODUCTION", agent->getValOfAttribute("REPRODUCTION"));*/
        int probReproduction = agent->getValOfAttribute("REPRODUCTION");
        //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
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
            probReproduction = 40;
            break;
        case 5:
            probReproduction = 50;
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
            probReproduction = 10;
            break;
        }
        probReproduction = probReproduction * tech;
        Power* p = nullptr;
        for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++) {
            if (GameLevel::getInstance()->getPowers()[i]->getNameInt() == 0) {
                p = GameLevel::getInstance()->getPowers()[i];
            }
        }
        if (p != nullptr and p->getDurationLeft() > 0) {
            probReproduction += 30;
        }
        //srand(time(NULL));
        //if ((rand() % 100) < probReproduction) {
        if (random(0, 100) < probReproduction) {
            int maxIterations = 10;
            Point fingerSpot = GameLevel::getInstance()->getAgentDirections()[typeAgent];
            /*if (agent->getType() != 0) {
                fingerSpot.x = -1;
                fingerSpot.y = -1;
            }*/
            int minRandomX = agent->getPosition()->getX() - mobility;
            int maxRandomX = agent->getPosition()->getX() + mobility;
            int minRandomY = agent->getPosition()->getY() - mobility;
            int maxRandomY = agent->getPosition()->getY() + mobility;
            //INDICACIO DE DIRECCIO AMB EL DIT
            if (fingerSpot.x > -1 and fingerSpot.y > -1) {
                if (fingerSpot.x < agent->getPosition()->getX()) {
                    //A L'ESQUERRA
                    maxRandomX = agent->getPosition()->getX();
                }
                else if (fingerSpot.x > agent->getPosition()->getX()) {
                    //A LA DRETA
                    minRandomX = agent->getPosition()->getX();
                }
                if (fingerSpot.y < agent->getPosition()->getY()) {
                    //A BAIX
                    maxRandomY = agent->getPosition()->getY();
                }
                else if (fingerSpot.y > agent->getPosition()->getY()) {
                    //A DALT
                    minRandomY = agent->getPosition()->getY();
                }
            }

            /*int posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
            int posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);*/
            int posx = random(minRandomX, maxRandomX);
            int posy = random(minRandomY, maxRandomY);
            while (maxIterations > 0 and GameLevel::getInstance()->validatePosition(posx, posy) == false) {
                /*posx = rand() % (2 * mobility) + (agent->getPosition()->getX() - mobility);
                posy = rand() % (2 * mobility) + (agent->getPosition()->getY() - mobility);*/
                posx = random(minRandomX, maxRandomX);
                posy = random(minRandomY, maxRandomY);
                maxIterations--;
            }
            if (maxIterations > 0) {
                //auto ag = new Agent(GameLevel::getInstance()->getIdCounter(), 100, type, posx, posy);
                Agent* ag = GameLevel::getInstance()->getAgentsPool()[type].front();
                GameLevel::getInstance()->popFrontAgentsPool(type);
                ag->setId(GameLevel::getInstance()->getIdCounter());
                ag->setLife(100);
                ag->setType(type);
                ag->setPosition(posx, posy);
                ag->setAttributes(GameLevel::getInstance()->getAgentAttributes(type));
                GameLevel::getInstance()->addAgent(ag);
                GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
                GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
            }
        }
    }
    //SI TIPUS DIFERENT -> INFLUENCIA CULTURAL PROVOCA QUE LAGENT ES CONVERTEIXI I NO ES REPRODUEIXI
    else if (type != agent->getType()) {
        auto ag = new Agent(agent->getId(), agent->getLife(), type, agent->getPosition()->getX(), agent->getPosition()->getY());
        ag->setAttributes(GameLevel::getInstance()->getAgentAttributes(type));
        GameLevel::getInstance()->addAgent(ag);
        agent->setLife(0);
        //GameLevel::getInstance()->setAddedAgents(GameLevel::getInstance()->getAddedAgents() + 1);
        //GameLevel::getInstance()->setIdCounter(GameLevel::getInstance()->getIdCounter() + 1);
    }
}