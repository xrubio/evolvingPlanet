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
    int mobility = int(float((1.0f + agent->getValOfAttribute("MOBILITY")) * GameLevel::getInstance()->getAgentPixelSize()) * tech);
    mobility *= 2;
    if (probCulture > -1) {
        probCulture = probCulture * tech;
        //CERCAR AGENT EN RADI
        int radi = 3;
        int maxIterations = 9;

        int minRandomX = agent->getPosition()->getX() - radi;
        int maxRandomX = agent->getPosition()->getX() + radi;
        int minRandomY = agent->getPosition()->getY() - radi;
        int maxRandomY = agent->getPosition()->getY() + radi;
        
        int posx = random(minRandomX, maxRandomX);
        int posy = random(minRandomY, maxRandomY);
        bool validTypeAgent = false;
        if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and
            GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
        {
            validTypeAgent = true;
        }
        while (maxIterations > 0 and validTypeAgent == false) {
            posx = random(minRandomX, maxRandomX);
            posy = random(minRandomY, maxRandomY);
            if (GameLevel::getInstance()->getAgentAtMap(posx, posy) != nullptr and
                GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType() != agent->getType())
            {
                validTypeAgent = true;
            }
            maxIterations--;
        }
        
        //SI S'HA TROBAT AGENT I INFLUEIX
        if (maxIterations > 0)
        {
            switch (GameLevel::getInstance()->getAgentAtMap(posx, posy)->getValOfAttribute("CULTURAL_INFLUENCE")) {
                case 1:
                    probCulture = 20;
                    break;
                case 2:
                    probCulture = 30;
                    break;
                case 3:
                    probCulture = 40;
                    break;
                case 4:
                    probCulture = 50;
                    break;
                case 5:
                    probCulture = 60;
                    break;
                case -1:
                    probCulture = -1;
                    break;
                default:
                    probCulture = 10;
                    break;
            }
            probCulture = probCulture * tech;
            if (random(0, 100) < probCulture){
                type = GameLevel::getInstance()->getAgentAtMap(posx, posy)->getType();
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
        int probReproductionVal = agent->getValOfAttribute("REPRODUCTION");
        float probReproduction = 0.1f;
        //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
        switch (probReproductionVal) {
        case 1:
            probReproduction = 0.4f;
            break;
        case 2:
            probReproduction = 0.5f;
            break;
        case 3:
            probReproduction = 0.6f;
            break;
        case 4:
            probReproduction = 0.7f;
            break;
        case 5:
            probReproduction = 0.8f;
        default:
            probReproduction = 0.3f;
            break;
        }
        probReproduction *= tech;

        CCLOG("repr %f mobility %d", probReproduction, mobility);
        Power* p = nullptr;
        for (int i = 0; i < GameLevel::getInstance()->getPowers().size(); i++)
        {
            if (GameLevel::getInstance()->getPowers()[i]->getId() != ReproductionBoost)
            {
                continue;
            }
            p = GameLevel::getInstance()->getPowers()[i];
        }
        if (p != nullptr and p->getDurationLeft() > 0) {
            probReproduction *= 1.5f;
        }
        //srand(time(NULL));
        //if ((rand() % 100) < probReproduction) {
        if (random(0, 100) < 100*probReproduction) {
            int maxIterations = 30;
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
