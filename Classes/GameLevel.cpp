//
//  GameLevel.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include "GameLevel.h"
#include "UIGameplayMap.h"

GameLevel* GameLevel::gamelevelInstance = NULL;

GameLevel* GameLevel::getInstance()
{
    if (!gamelevelInstance) // Only allow one instance of class to be generated.
    {
        gamelevelInstance = new GameLevel;
    }
    return gamelevelInstance;
}

UIGameplayMap* GameLevel::getUIGameplayMap(void)
{
    return gameplayMap;
}

void GameLevel::setUIGameplayMap(UIGameplayMap* gmplmap)
{
    gameplayMap = gmplmap;
}

double GameLevel::getCurrentTime(void)
{
    return currentTime;
}

void GameLevel::setCurrentTime(double time)
{
    currentTime = time;
}

int GameLevel::getNumLevel(void)
{
    return numLevel;
}

void GameLevel::setNumLevel(int lvl)
{
    numLevel = lvl;
}

int GameLevel::getAgentAttribute(string key)
{
    return agentAttributes[key];
}

void GameLevel::setAgentAttributes(string key, int value)
{
    agentAttributes[key] = value;
}

vector<Agent*> GameLevel::getAgents(void)
{
    return agents;
}

void GameLevel::setAgents(vector<Agent*> ags)
{
    agents = ags;
}

int GameLevel::getAddedAgents(void)
{
    return addedAgents;
}

void GameLevel::setAddedAgents(int i)
{
    addedAgents = i;
}

vector<int> GameLevel::getDeletedAgents(void)
{
    return deletedAgents;
}

void GameLevel::setDeletedAgents(vector<int> v)
{
    deletedAgents.swap(v);
}

int GameLevel::getFinishedGame(void)
{
    return finishedGame;
}

void GameLevel::setFinishedGame(int f)
{
    finishedGame = f;
}

int GameLevel::getTimeSteps(void)
{
    return timeSteps;
}

float GameLevel::getTimeSpeed(void)
{
    return timeSpeed;
}

void GameLevel::setTimeSpeed(float speed)
{
    timeSpeed = speed;
}

float GameLevel::getTimeSpeedBeforePause(void)
{
    return timeSpeedBeforePause;
}

void GameLevel::setTimeSpeedBeforePause(float speed)
{
    timeSpeedBeforePause = speed;
}

int GameLevel::getCooldownPower1(void)
{
    return cooldownPower1;
}

void GameLevel::setCooldownPower1(int cd)
{
    cooldownPower1 = cd;
}

int GameLevel::getCooldownPower2(void)
{
    return cooldownPower2;
}

void GameLevel::setCooldownPower2(int cd)
{
    cooldownPower2 = cd;
}

int GameLevel::getPower1Active(void)
{
    return power1Active;
}

void GameLevel::setPower1Active(int cd)
{
    power1Active = cd;
}

int GameLevel::getPower2Active(void)
{
    return power2Active;
}

void GameLevel::setPower2Active(int cd)
{
    power2Active = cd;
}

void GameLevel::playLevel(void)
{
    clock_t stepTime = clock();
    while (finishedGame == 0) {
        currentTime = clock();
        if (timeSpeed != 0) {
            if (((float)currentTime / CLOCKS_PER_SEC) - ((float)stepTime / CLOCKS_PER_SEC) > timeSpeed) {
                stepTime = clock();
                while (gameplayMap->play == false)
                    ;
                paint = false;
                //die();
                //reproduce();
                dieAndReproduce();
                if (power1Active == 5) {
                    cooldownPower1 = 20;
                }
                if (power2Active == 10) {
                    cooldownPower2 = 20;
                }
                if (power1Active > 0) {
                    power1Active--;
                }
                if (power2Active > 0) {
                    power2Active--;
                }
                if (cooldownPower1 > 0) {
                    cooldownPower1--;
                }
                if (cooldownPower2 > 0) {
                    cooldownPower2--;
                }
                timeSteps++;
                paint = true;
                //gameplayMap->updateAgents(agents);
            }
        }
    }
    cout << "End of game: " << finishedGame << endl;
}

void GameLevel::resetLevel(void)
{
    currentTime = 0;
    numLevel = 0;
    agentAttributes.clear();
    powers.clear();
    for (int i = 0; i < agents.size(); i++) {
        delete agents.at(i);
    }
    agents.clear();
    addedAgents = 0;
    deletedAgents.clear();
    idCounter = 0;

    timeSteps = 0;
    timeSpeed = 2.5;

    //0 = notFinished, 1 = finishedCompleted, 2 = finishedTimeOut, 3 = finished0Agents,
    //4 = finishedBack
    finishedGame = 0;
}

void GameLevel::createLevel(int lvl)
{
    numLevel = lvl;

    //aplicar atributs als agents de forma independent a cadascun
    generateInitialAgents();
    paint = true;
}

void GameLevel::generateInitialAgents(void)
{
    int posy = 200;
    for (int i = 0; i < 10; i++) {
        posy += 5;
        int posx = 260;
        for (int j = 0; j < 10; j++) {
            auto a = new Agent(idCounter, 100, posx, posy);
            a->setAttributes(agentAttributes);
            agents.push_back(a);
            posx += 5;
            idCounter++;
        }
    }
}

void GameLevel::reproduce(void)
{
    int probReproduction = agentAttributes["att1"];
    int mobility = agentAttributes["att2"] * 10;
    unsigned long size = agents.size();
    addedAgents = 0;
    if (size < 3000) {
        for (int i = 0; i < size; i++) {
            if (probReproduction > 0) {
                int reproduce;
                if (probReproduction == 10) {
                    reproduce = 0;
                } else {
                    reproduce = rand() % (10 - probReproduction) + 0;
                }
                if (reproduce == 0) {
                    int posx = rand() % (2 * mobility) + (agents.at(i)->getPosition()->getX() - mobility);
                    int posy = rand() % (2 * mobility) + (agents.at(i)->getPosition()->getY() - mobility);
                    if (validatePosition(posx, posy)) {
                        auto ag = new Agent(idCounter, 100, posx, posy);
                        //gameplayMap->addAgent(ag);
                        agents.push_back(ag);
                        addedAgents++;
                        idCounter++;
                    }
                }
            }
        }
    }
}

void GameLevel::die(void)
{
    deletedAgents.clear();
    int index = 0;
    for (int i = 0; i < agents.size(); i++) {
        int zone = gameplayMap->getValueAtGameplayMapHotSpot(agents.at(i)->getPosition()->getX(),
                                                             agents.at(i)->getPosition()->getY());
        int harm;
        switch (zone) {
        case 1:
            harm = 15;
            break;
        case 2:
            harm = 15;
            break;
        case 3:
            harm = 10;
            break;
        case 4:
            harm = 40;
            break;
        default:
            harm = 15;
            break;
        }
        agents.at(i)->setLife(agents.at(i)->getLife() - harm);

        if (agents.at(i)->getLife() <= 0) {
            //gameplayMap->deleteAgent(agents.at(i)->getId(), i);
            //delete agents.at(i);
            //deletedAgents.push_back(agents.at(i)->getId());
            deletedAgents.push_back(index);
            agents.erase(agents.begin() + i);
            i--;
        }
        index++;
    }
    if (agents.size() == 0) {
        finishedGame = 3;
    }
}

void GameLevel::dieAndReproduce(void)
{
    deletedAgents.clear();
    addedAgents = 0;

    //int index = 0;
    unsigned long dieAgentsSize = agents.size();
    for (int i = 0; i < dieAgentsSize; i++) {
        int zone = gameplayMap->getValueAtGameplayMapHotSpot(agents.at(i)->getPosition()->getX(),
                                                             agents.at(i)->getPosition()->getY());
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
        int resistance = agents.at(i)->getValOfAttribute("att3");
        if (power2Active > 0) {
            if (gameplayMap->isInBoostResistanceArea(agents.at(i)->getPosition()->getX(), agents.at(i)->getPosition()->getY())) {
                harm /= 2;
            }
        }
        harm = harm - resistance;
        if (harm < 0) {
            harm = 0;
        }
        agents.at(i)->setLife(agents.at(i)->getLife() - harm);

        if (agents.at(i)->getLife() <= 0) {
            //gameplayMap->deleteAgent(agents.at(i)->getId(), i);
            //delete agents.at(i);
            deletedAgents.push_back(agents.at(i)->getId());
            //deletedAgents.push_back(index);
            agents.erase(agents.begin() + i);
            dieAgentsSize--;
            i--;
        }
        //si viu, reproduce
        else {
            if (agents.size() < 3000) {
                int probReproduction = agents.at(i)->getValOfAttribute("att1");
                if (power1Active > 0) {
                    probReproduction += 3;
                }
                int mobility = agents.at(i)->getValOfAttribute("att2") * 10;
                if (probReproduction > 0) {
                    int reproduce;
                    if (probReproduction >= 10) {
                        reproduce = 0;
                    } else {
                        reproduce = rand() % (10 - probReproduction) + 0;
                    }
                    if (reproduce == 0) {
                        int posx = rand() % (2 * mobility) + (agents.at(i)->getPosition()->getX() - mobility);
                        int posy = rand() % (2 * mobility) + (agents.at(i)->getPosition()->getY() - mobility);
                        if (validatePosition(posx, posy)) {
                            auto ag = new Agent(idCounter, 100, posx, posy);
                            ag->setAttributes(agentAttributes);
                            //gameplayMap->addAgent(ag);
                            agents.push_back(ag);
                            addedAgents++;
                            idCounter++;
                        }
                    }
                }
            }
        }
        //index++;
    }
    if (agents.size() == 0) {
        finishedGame = 3;
    }
}

bool GameLevel::validatePosition(int posx, int posy)
{
    //Fora del mapa
    if (posx < 0 or posx > 2048 or posy < 0 or posy > 1536) {
        return false;
    }

    //Aigua o similar
    if (gameplayMap->getValueAtGameplayMapHotSpot(posx, posy) == 0) {
        return false;
    }

    //Hi ha un agent
    for (int i = 0; i < agents.size(); i++) {
        if (agents.at(i)->getPosition()->getX() == posx and agents.at(i)->getPosition()->getY() == posy) {
            return false;
        }
    }
    return true;
}