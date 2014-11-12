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
#include "MultiplierPower.h"
#include "AreaPower.h"
#include "GameData.h"
#include "ExpansionGoal.h"

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

string GameLevel::getMapFilename(void)
{
    return mapFilename;
}

void GameLevel::setMapFilename(string filename)
{
    mapFilename = filename;
}

vector<int> GameLevel::getMaxAgents(void)
{
    return maxAgents;
}

void GameLevel::setMaxAgents(vector<int> max)
{
    maxAgents.swap(max);
}

int GameLevel::getMaxAgent(int type)
{
    return maxAgents.at(type);
}

void GameLevel::setMaxAgent(int type, int max)
{
    maxAgents.insert(maxAgents.begin() + type, max);
}

vector<int> GameLevel::getNumInitialAgents(void)
{
    return numInitialAgents;
}

void GameLevel::setNumInitialAgents(vector<int> ini)
{
    numInitialAgents.swap(ini);
}

int GameLevel::getNumInitialAgent(int type)
{
    return numInitialAgents.at(type);
}

void GameLevel::setNumInitialAgent(int type, int ini)
{
    numInitialAgents.insert(numInitialAgents.begin() + type, ini);
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

int GameLevel::getAgentAttribute(int type, string key)
{
    return agentAttributes.at(type)[key];
}

void GameLevel::setAgentAttribute(int type, string key, int value)
{
    if (agentAttributes.size() <= type) {
        agentAttributes.push_back(map<string, int>());
    }
    agentAttributes.at(type)[key] = value;
}

map<string, int> GameLevel::getAgentAttributes(int type)
{
    return agentAttributes.at(type);
}

void GameLevel::setAgentAttributes(vector<map<string, int> > atts)
{
    agentAttributes.swap(atts);
}

vector<Power*> GameLevel::getPowers(void)
{
    return powers;
}

void GameLevel::setPowers(vector<Power*> p)
{
    powers.swap(p);
}

void GameLevel::addPower(Power* p)
{
    powers.push_back(p);
}

void GameLevel::deletePower(int i)
{
    powers.erase(powers.begin() + i);
}

vector<vector<Agent*> > GameLevel::getAgents(void)
{
    return agents;
}

void GameLevel::setAgents(vector<vector<Agent*> > ags)
{
    agents = ags;
}

void GameLevel::addAgent(Agent* ag)
{
    while (agents.size() <= ag->getType()) {
        vector<Agent*> v;
        agents.push_back(v);
    }
    agents.at(ag->getType()).push_back(ag);
    agentsMap[ag->getPosition()->getX()][ag->getPosition()->getY()] = ag;
}

void GameLevel::deleteAgent(int type, int i)
{
    int posx = agents.at(type).at(i)->getPosition()->getX();
    int posy = agents.at(type).at(i)->getPosition()->getY();
    //delete agentsMap[posx][posy]; // = nullptr;
    agentsMap[posx][posy] = nullptr;
    agents.at(type).erase(agents.at(type).begin() + i);
}

vector<Act*> GameLevel::getActions(void)
{
    return actions;
}

void GameLevel::setActions(vector<Act*> a)
{
    actions.swap(a);
}

void GameLevel::addAction(Act* act)
{
    actions.push_back(act);
}

void GameLevel::deleteAction(int i)
{
    actions.erase(actions.begin() + i);
}

vector<Goal*> GameLevel::getGoals(void)
{
    return goals;
}

void GameLevel::setGoals(vector<Goal*> g)
{
    goals.swap(g);
}

void GameLevel::addGoal(Goal* g)
{
    goals.push_back(g);
}

void GameLevel::deleteGoal(int i)
{
    goals.erase(goals.begin() + i);
}

int GameLevel::getAddedAgents(void)
{
    return addedAgents;
}

void GameLevel::setAddedAgents(int i)
{
    addedAgents = i;
}

vector<Point> GameLevel::getDeletedAgents(void)
{
    return deletedAgents;
}

void GameLevel::setDeletedAgents(vector<Point> v)
{
    deletedAgents.swap(v);
}

void GameLevel::addDeletedAgent(Point p)
{
    deletedAgents.push_back(p);
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

int GameLevel::getEvolutionPoints(void)
{
    return evolutionPoints;
}

void GameLevel::setEvolutionPoints(int points)
{
    evolutionPoints = points;
}

int GameLevel::getAttributeCost(int type, string key)
{
    return attributesCost.at(type)[key];
}

void GameLevel::setAttributeCost(int type, string key, int val)
{
    attributesCost.at(type)[key] = val;
}

int GameLevel::getTimeExploited(int x, int y)
{
    return timeExploitedMap[x][y];
}

void GameLevel::setTimeExploited(int x, int y, int val)
{
    timeExploitedMap[x][y] = val;
}

bool GameLevel::getDepleted(int x, int y)
{
    return depletedMap[x][y];
}

void GameLevel::setDepleted(int x, int y, bool val)
{
    depletedMap[x][y] = val;
}

bool GameLevel::getEnvironmentAdaptation(int x, int y)
{
    return adaptedMap[x][y];
}

void GameLevel::setEnvironmentAdaptation(int x, int y, bool val)
{
    adaptedMap[x][y] = val;
}

int GameLevel::getCurrentAgentType(void)
{
    return currentAgentType;
}

void GameLevel::setCurrentAgentType(int i)
{
    currentAgentType = i;
}

Agent* GameLevel::getAgentAtMap(int i, int j)
{
    return agentsMap[i][j];
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
                for (int i = 0; i < powers.size(); i++) {
                    Power* p = powers.at(i);
                    if (p->getDurationLeft() == p->getDuration()) {
                        p->setCooldownLeft(p->getCooldown());
                    }
                    if (p->getDurationLeft() > 0) {
                        p->setDurationLeft(p->getDurationLeft() - 1);
                    }
                    if (p->getCooldownLeft() > 0) {
                        p->setCooldownLeft(p->getCooldownLeft() - 1);
                    }
                }
                timeSteps++;
                if (timeSteps % 2 == 0) {
                    evolutionPoints++;
                }
                paint = true;
            }
        }
    }
    paint = false;
    cout << "End of game: " << finishedGame << endl;
}

void GameLevel::resetLevel(void)
{
    //REVISAR MEMORIA, O ESBORRAR INSTANCIA ENLLOC DE FER RESET I ALLIBERAR AL DESTRUCTOR

    currentTime = 0;
    numLevel = 0;
    agentAttributes.clear();
    powers.clear();
    /*for (int i = 0; i < agents.size(); i++) {
        for (int j = 0; j < agents.at(i).size(); j++) {
            delete agents.at(i).at(j);
        }
    }*/
    agents.clear();
    addedAgents = 0;
    deletedAgents.clear();
    idCounter = 0;

    maxAgents.clear();
    numInitialAgents.clear();

    actions.clear();
    goals.clear();
    for (int i = 0; i < 480; i++) {
        for (int j = 0; j < 320; j++) {
            agentsMap[i][j] = nullptr;
            timeExploitedMap[i][j] = 0;
            depletedMap[i][j] = false;
            adaptedMap[i][j] = false;
        }
    }

    timeSteps = 0;
    timeSpeed = 2.5;

    evolutionPoints = 0;

    //0 = notFinished, 1 = finishedCompleted, 2 = finishedTimeOut, 3 = finished0Agents,
    //4 = finishedBack
    finishedGame = 0;

    currentAgentType = 0;
}

void GameLevel::createLevel(void)
{
    for (int i = 0; i < numInitialAgents.size(); i++) {
        generateInitialAgents(i);
    }
    paint = true;
}

void GameLevel::initializeAttributesCost(void)
{
    for (int i = 0; i < agentAttributes.size(); i++) {
        attributesCost.push_back(map<string, int>());
        for (map<string, int>::const_iterator it = agentAttributes.at(i).begin(); it != agentAttributes.at(i).end(); it++) {
            //si el valor inicial es diferent de 0, es valor que no modificarà l'usuari
            if (agentAttributes.at(i)[it->first] != 0) {
                attributesCost.at(i)[it->first] = 0;
            } else {
                attributesCost.at(i)[it->first] = 1;
            }
        }
    }
}

void GameLevel::generateInitialAgents(int type)
{
    //FIND RECTANGLE
    int minX = 500;
    int maxX = 0;
    int minY = 500;
    int maxY = 0;
    for (int x = 0; x < 480; x++) {
        for (int y = 0; y < 320; y++) {
            if (gameplayMap->getValueAtGameplayMapHotSpot(1, x, y) == type) {
                if (minX > x)
                    minX = x;
                if (maxX < x)
                    maxX = x;
                if (minY > y)
                    minY = y;
                if (maxY < y)
                    maxY = y;
            }
        }
    }

    int i = 0;
    while (i < numInitialAgents.at(type)) {
        int posx = rand() % maxX + minX;
        int posy = rand() % maxY + minY;
        if (gameplayMap->getValueAtGameplayMapHotSpot(1, posx, posy) == type and GameLevel::getInstance()->validatePosition(posx, posy)) {
            auto a = new Agent(idCounter, 100, type, posx, posy);
            a->setAttributes(agentAttributes.at(type));
            addAgent(a);
            idCounter++;
            i++;
        }
    }
}

void GameLevel::act(void)
{
    deletedAgents.clear();
    addedAgents = 0;

    for (int k = 0; k < agents.size(); k++) {
        int dieAgentsSize = (int)agents.at(k).size();
        for (int i = dieAgentsSize - 1; i >= 0; i--) {
            if (agents.at(k).at(i)->getLife() > 0) {
                for (int j = 0; j < actions.size() - 1; j++) {
                    actions.at(j)->execute(k, i);
                }
                //Check goal d'expansió només de addedAgents ?? mes eficient, com diferenciar tipus goal
                for (int j = 0; j < goals.size(); j++) {
                    if (goals.at(j)->getCompleted() == false) {
                        goals.at(j)->checkGoal(k, i);
                    }
                }
            }
            //MORIR, SEMPRE ULTIMA ACCIO, DESPRES DE COMPROVAR GOALS
            actions.at(actions.size() - 1)->execute(k, i);

            //ALL GOALS COMPLETED ??
            bool failed = false;
            int finalScore = 0;
            for (int j = 0; j < goals.size() and failed == false; j++) {
                if (goals.at(j)->getCompleted() == false) {
                    if (prevGoal != j) {
                        gameplayMap->moveGoalPopup(j);
                        prevGoal = j;
                    }
                    failed = true;
                } else {
                    finalScore += goals.at(j)->getScore();
                }
            }
            if (failed == false and goals.size() > 0) {
                cout << "FINAL SCORE: " << finalScore / goals.size() << endl;
                GameData::getInstance()->setLevelScore(numLevel, finalScore / goals.size());
                finishedGame = 1;
            }
        }
    }
    bool noAgentsLeft = true;
    for (int i = 0; i < agents.size(); i++) {
        if (agents.at(i).size() > 0) {
            noAgentsLeft = false;
        }
    }

    if (finishedGame == 0 and noAgentsLeft) {
        finishedGame = 3;
    }
}

bool GameLevel::validatePosition(int posx, int posy)
{
    //Fora del mapa
    if (posx < 0 or posx >= 480 or posy < 0 or posy >= 320) {
        return false;
    }
    //Aigua o similar
    if (gameplayMap->getValueAtGameplayMapHotSpot(0, posx, posy) == 0) {
        return false;
    }
    //Hi ha un agent
    if (agentsMap[posx][posy] != nullptr) {
        return false;
    }
    return true;
}