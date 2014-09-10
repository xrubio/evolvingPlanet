//
//  GameLevel.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include "GameLevel.h"
#include "UIGameplayMap.h"
#include "Die.h"
#include "Reproduce.h"

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

void GameLevel::setAgentAttribute(string key, int value)
{
    agentAttributes[key] = value;
}

map<string, int> GameLevel::getAgentAttributes(void)
{
    return agentAttributes;
}

void GameLevel::setAgentAttributes(map<string, int> atts)
{
    agentAttributes.swap(atts);
}

vector<Agent*> GameLevel::getAgents(void)
{
    return agents;
}

void GameLevel::setAgents(vector<Agent*> ags)
{
    agents = ags;
}

void GameLevel::addAgent(Agent* ag)
{
    agents.push_back(ag);
}

void GameLevel::deleteAgent(int i)
{
    agents.erase(agents.begin() + i);
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

void GameLevel::addDeletedAgent(int id)
{
    deletedAgents.push_back(id);
}

void GameLevel::deleteDeletedAgent(int i)
{
    deletedAgents.erase(deletedAgents.begin() + i);
}

int GameLevel::getIdCounter(void)
{
    return idCounter;
}

void GameLevel::setIdCounter(int count)
{
    idCounter = count;
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

int GameLevel::getEvolutionPoints(void)
{
    return evolutionPoints;
}

void GameLevel::setEvolutionPoints(int points)
{
    evolutionPoints = points;
}

int GameLevel::getAttributeCost(string key)
{
    return attributesCost[key];
}

void GameLevel::setAttributeCost(string key, int val)
{
    attributesCost[key] = val;
}

void GameLevel::playLevel(void)
{
    clock_t stepTime = clock();
    while (finishedGame == 0) {
        currentTime = clock();
        if (timeSpeed != 0) {
            float step = ((float)currentTime / CLOCKS_PER_SEC) - ((float)stepTime / CLOCKS_PER_SEC);
            if (step > timeSpeed) {
                cout << step << endl;
                stepTime = clock();
                while (gameplayMap->play == false)
                    ;
                paint = false;
                act();
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
                if (timeSteps % 4 == 0) {
                    evolutionPoints++;
                }
                paint = true;
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

    actions.clear();

    timeSteps = 0;
    timeSpeed = 2.5;

    power1Active = 0;
    power2Active = 0;

    cooldownPower1 = 0;
    cooldownPower2 = 0;

    evolutionPoints = 0;

    //0 = notFinished, 1 = finishedCompleted, 2 = finishedTimeOut, 3 = finished0Agents,
    //4 = finishedBack
    finishedGame = 0;
}

void GameLevel::createLevel(int lvl)
{
    numLevel = lvl;

    actions.push_back(new Die());
    actions.push_back(new Reproduce());

    generateInitialAgents();
    paint = true;
}

void GameLevel::initializeAttributesCost(void)
{
    attributesCost["att1"] = 1;
    attributesCost["att2"] = 1;
    attributesCost["att3"] = 1;
}

void GameLevel::generateInitialAgents(void)
{
    int posy = 28;
    for (int i = 0; i < 10; i++) {
        posy += 2;
        int posx = 25;
        for (int j = 0; j < 10; j++) {
            auto a = new Agent(idCounter, 100, posx, posy);
            a->setAttributes(agentAttributes);
            agents.push_back(a);
            posx += 2;
            idCounter++;
        }
    }
}

void GameLevel::act(void)
{
    deletedAgents.clear();
    addedAgents = 0;

    //int index = 0;
    int dieAgentsSize = (int)agents.size();
    for (int i = dieAgentsSize - 1; i >= 0; i--) {
        //Die
        //Si ha fet modificacions retorna true (en aquest cas, esborrar un agent, per tant no s'ha de reproduir)
        if (actions.at(0)->execute(i) == false) {
            //Reproduce
            //Retorna true si s'ha reproduit
            actions.at(1)->execute(i);
        }
        //index++;
    }
    if (agents.size() == 0) {
        finishedGame = 1;
    }
}

bool GameLevel::validatePosition(int posx, int posy)
{
    //Fora del mapa
    if (posx < 0 or posx > 200 or posy < 0 or posy > 200) {
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