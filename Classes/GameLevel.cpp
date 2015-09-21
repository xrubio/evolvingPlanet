/*
 * Copyright (c) 2015
 * MURPHY'S TOAST GAMES
 * 
 * This file is part of Evolving Planet.
 * Evolving Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Evolving Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Evolving Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include "Power.h"
#include "AreaPower.h"
#include "GameData.h"
#include "ExpansionGoal.h"

GameLevel* GameLevel::gamelevelInstance = NULL;

size_t GameLevel::_numAttributes= 7;

GameLevel* GameLevel::getInstance()
{
    if (!gamelevelInstance) // Only allow one instance of class to be generated.
    {
        gamelevelInstance = new GameLevel;
    }
    return gamelevelInstance;
}

GameLevel::GameLevel()
{
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
    return maxAgents[type];
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
    return numInitialAgents[type];
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

int GameLevel::getAgentAttribute(int type, int key)
{
    return _agentAttributes.at(type)[key];
}

void GameLevel::setAgentAttribute(int type, int key, int level)
{
    if(_agentAttributes.size() <= type)
    {
        _agentAttributes.push_back(Levels(_numAttributes,-1));
    }
    _agentAttributes.at(type).at(key) = level;
}

const GameLevel::Levels & GameLevel::getAgentAttributes(int type) const
{
    return _agentAttributes.at(type);
}

bool GameLevel::initAttributesEmpty() const
{
    return _agentAttributesInitialConfig.empty();
}

void GameLevel::setAgentAttributesToInit()
{
    _agentAttributes.clear();
    _agentAttributes.resize(_agentAttributesInitialConfig.size());
    std::copy(_agentAttributesInitialConfig.begin(), _agentAttributesInitialConfig.end(), _agentAttributes.begin());
}

void GameLevel::setAgentAttributesInitToCurrent()
{
    _agentAttributesInitialConfig.clear();
    _agentAttributesInitialConfig.resize(_agentAttributes.size());
    std::copy(_agentAttributes.begin(), _agentAttributes.end(), _agentAttributesInitialConfig.begin());
}

void GameLevel::resetAgentAttributesInitialConfig(void)
{
    _agentAttributesInitialConfig.clear();
}

float GameLevel::getValueAtLevel(int attr, int level) const
{
    return _attributesLevels.at(attr).at(level);
}

void GameLevel::setValueAtLevel(int attr, int level, float value)
{
    _attributesLevels.at(attr).at(level) = value;
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

vector<list<Agent*> > GameLevel::getAgents(void)
{
    return _agents;
}

void GameLevel::setAgents(vector<list<Agent*> > agents)
{
    _agents = agents;
}

void GameLevel::addAgent(Agent* ag)
{
    while (_agents.size() <= ag->getType()) {
        list<Agent*> v;
        _agents.push_back(v);
    }
    _agents[ag->getType()].push_back(ag);
    _agentsMap[ag->getPosition()->getX()][ag->getPosition()->getY()] = ag;
}

list<Agent*>::reverse_iterator GameLevel::deleteAgent(int type, Agent* agent)
{
    int posx = agent->getPosition()->getX();
    int posy = agent->getPosition()->getY();
    //delete _agentsMap[posx][posy]; // = nullptr;
    _agentsMap[posx][posy] = nullptr;
    //_agents.at(type).erase(_agents.at(type).begin() + i);
    list<Agent*>::reverse_iterator it = find(_agents[type].rbegin(), _agents[type].rend(), agent);
    //advance(it, 1);
    _agents[type].erase(--(it.base()));
    _agentsPool[type].push_back(agent);
    //delete agent;
    return it;
}

vector<list<Agent*> > GameLevel::getAgentsPool(void)
{
    return _agentsPool;
}

void GameLevel::popFrontAgentsPool(int type)
{
    _agentsPool[type].pop_front();
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

const LevelState & GameLevel::getFinishedGame() const
{
    return finishedGame;
}

void GameLevel::setFinishedGame(const LevelState & f)
{
    finishedGame = f;
}

unsigned int GameLevel::getTimeSteps(void)
{
    return timeSteps;
}

void GameLevel::setTimeSteps(unsigned int steps)
{
    timeSteps = steps;
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

int GameLevel::getAttributeCost(int type, int key)
{
    return _attributesCost.at(type).at(key);
}

void GameLevel::setAttributeCost(int type, int key, int val)
{
    _attributesCost.at(type).at(key) = val;
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
    return _agentsMap[i][j];
}

int GameLevel::getMaxAllAgents(void)
{
    return maxAllAgents;
}

void GameLevel::setMaxAllAgents(int m)
{
    maxAllAgents = m;
}

vector<cocos2d::Point> GameLevel::getAgentDirections(void)
{
    return agentDirections;
}

void GameLevel::setAgentDirections(vector<cocos2d::Point> ad)
{
    agentDirections = ad;
}

void GameLevel::setAgentDirection(int agentType, cocos2d::Point p)
{
    agentDirections[agentType] = p;
}

vector<vector<pair<int, cocos2d::Point> > > GameLevel::getAgentFutureDirections(void)
{
    return agentFutureDirections;
}

void GameLevel::setAgentFutureDirections(vector<vector<pair<int, cocos2d::Point> > > afd)
{
    agentFutureDirections = afd;
}

void GameLevel::setAgentFutureDirection(int type, int step, cocos2d::Point p)
{
    pair<unsigned int, cocos2d::Point> par(step, p);
    while (agentFutureDirections.size() <= type) {
        vector<pair<int, cocos2d::Point> > v;
        agentFutureDirections.push_back(v);
    }
    agentFutureDirections[type].push_back(par);
}

void GameLevel::setAgentPixelSize(int i)
{
    agentPixelSize = i;
}

int GameLevel::getAgentPixelSize(void)
{
    return agentPixelSize;
}

void GameLevel::setEvolutionPointsFreq(int i)
{
    evolutionPointsFreq = i;
}

int GameLevel::getEvolutionPointsFreq(void)
{
    return evolutionPointsFreq;
}

void GameLevel::playLevel(void)
{
    CCLOG("step;pop;time");
    while (finishedGame == Running)
    {
        if (Timing::getInstance()->act == true) {
            while (gameplayMap->play == false)
                ;
            paint = false;
            clock_t stepTime = clock();
//            CCLOG("Start calc");
            act();
            timeSteps++;
            gameplayMap->setTimeProgressBar(timeSteps);
            if (timeSteps % evolutionPointsFreq == 0) {
                evolutionPoints++;
            }
            paint = true;
            Timing::getInstance()->act = false;  
            
            // number of _agents
            size_t numAgents = 0;
            for (size_t i = 0; i < GameLevel::getInstance()->getAgents().size(); i++)
            {
                numAgents += GameLevel::getInstance()->getAgents()[i].size();
            }

            CCLOG("%d;%d;%f", timeSteps, numAgents, float(clock() - stepTime) / CLOCKS_PER_SEC);
            calcTime = float(clock() - stepTime) / CLOCKS_PER_SEC;
            /*try {
                if (float(clock() - stepTime) / CLOCKS_PER_SEC > 1.27) {
                    throw 2;
                }
            }
            catch (int e) {
                CCLOG("Time Exceeded");
            }*/
        }
    }
    ended = true;
    CCLOG("End of game: %i", finishedGame);
}

void GameLevel::resetLevel(void)
{
    //REVISAR MEMORIA, O ESBORRAR INSTANCIA ENLLOC DE FER RESET I ALLIBERAR AL DESTRUCTOR

    currentTime = 0;
    numLevel = 0;
    _agentAttributes.clear();
    powers.clear();
    /*for (int i = 0; i < _agents.size(); i++) {
        for (int j = 0; j < _agents.at(i).size(); j++) {
            delete _agents.at(i).at(j);
        }
    }*/
    _agents.clear();
    _agentsPool.clear();
    addedAgents = 0;
    deletedAgents.clear();
    idCounter = 0;
    agentPixelSize = 1;

    maxAgents.clear();
    numInitialAgents.clear();

    actions.clear();
    goals.clear();
    for (int i = 0; i < 480; i++) {
        for (int j = 0; j < 320; j++) {
            _agentsMap[i][j] = nullptr;
            timeExploitedMap[i][j] = 0;
            depletedMap[i][j] = false;
            adaptedMap[i][j] = false;
        }
    }

    timeSteps = 0;
    timeSpeed = 0;

    evolutionPoints = 10;

    finishedGame = Running;

    currentAgentType = 0;
    maxAllAgents = 0;

    paint = false;
    ended = false;
    prevGoal = 0;
    calcTime = 0;

    agentDirections.clear();
    agentFutureDirections.clear();

    _attributesLevels.clear();
    // all attributes have 6 levels (0 to 5). All of them are set to 0
    for(size_t i=0; i<_numAttributes; i++)
    {
        std::vector<float> r(6, 0);
        _attributesLevels.push_back(r);
    }
}

void GameLevel::createLevel(void)
{
    initializeAgentsPool();
    for(size_t i = 0; i < numInitialAgents.size(); i++)
    {
        generateInitialAgents(i);
        agentDirections.push_back(Point(-1, -1));
    }

    paint = true;
}

void GameLevel::initializeAttributesCost(void)
{
    for(size_t i = 0; i < _agentAttributes.size(); i++)
    {
        _attributesCost.push_back(Levels(_numAttributes,-1));
        for(size_t j=0; j<_agentAttributes.at(i).size(); j++)
        {
            //si el valor inicial es diferent de 0, es valor que no modificarà l'usuari
            if (_agentAttributes.at(i).at(j) != 0)
            {
                CCLOG("cost for i: %d j= %d is 0", i, j);
                _attributesCost.at(i).at(j) = 0;
            }
            else
            {
                CCLOG("cost for i: %d j= %d is 1", i, j);
                _attributesCost.at(i).at(j) = 1;
            }
        }
    }
}

void GameLevel::setAttributesToInitialAgents(void)
{
    std::list<Agent*> agents = _agents[0];
    for(std::list<Agent*>::iterator it=agents.begin(); it!=agents.end(); it++)
    {
        (*it)->copyValues(0);
    }
}

void GameLevel::initializeAgentsPool(void)
{
    for (int i = 0; i < maxAgents.size(); i++) {
        list<Agent*> l_agent;
        for (int j = 0; j < maxAgents[i]; j++) {
            l_agent.push_back(new Agent());
        }
        _agentsPool.push_back(l_agent);
    }
}

void GameLevel::generateInitialAgents(int type)
{
    //FIND RECTANGLE
    int minX = 479;
    int maxX = 0;
    int minY = 319;
    int maxY = 0;
    for (int x = 0; x < 480; x++) {
        for (int y = 0; y < 320; y++) {
            if (gameplayMap->getValueAtGameplayMap(1, x, y, 0) == type) {
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
    while (i < numInitialAgents[type]) {
        int posx = random(minX, maxX); //rand() % maxX + minX;
        int posy = random(minY, maxY); //rand() % maxY + minY;
        if (GameLevel::getInstance()->validatePosition(posx, posy) and gameplayMap->getValueAtGameplayMap(1, posx, posy, 0) == type) {
            //auto a =new Agent(idCounter, 100, type, posx, posy);
            Agent* a = _agentsPool[type].front();
            _agentsPool[type].pop_front();
            a->setId(idCounter);
            a->setLife(100);
            a->setType(type);
            a->setPosition(posx, posy);
            a->copyValues(type);
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
    //CCLOG("Num agents: %i", _agents[0].size());
    //CCLOG("Pool: %i", _agentsPool[0].size());
    //CCLOG("Num agents + Pool: %i", _agents[0].size() + _agentsPool[0].size());
    for (size_t k = 0; k < _agents.size() and finishedGame == Running; k++) {
        //CHECK DIRECTION
        if (agentFutureDirections.empty() == false and agentFutureDirections[k].empty() == false) {
            //CCLOG("check direction %i", agentFutureDirections[k][0].first);
            if (timeSteps == agentFutureDirections[k][0].first) {
                agentDirections[k] = agentFutureDirections[k][0].second;
                agentFutureDirections[k].erase(agentFutureDirections[k].begin());
            }
        }

        list<Agent*>::reverse_iterator end = _agents[k].rbegin();
        while (end != _agents[k].rend() and finishedGame == Running) {
            if ((*end)->getLife() > 0) {
                for (size_t j = 0; j < actions.size() - 1; j++) {
                    actions[j]->execute(k, *end);
                }
                //Check goal d'expansió només de addedAgents ?? mes eficient, com diferenciar tipus goal
                for (size_t  j = 0; j < goals.size(); j++) {
                    if (goals[j]->getCompleted() == false) {
                        goals[j]->checkGoal(k, *end);
                    }
                }
            }
            //MORIR, SEMPRE ULTIMA ACCIO, DESPRES DE COMPROVAR GOALS
            int sizeBefore = (int)_agents[k].size();
            list<Agent*>::reverse_iterator rit = actions[actions.size() - 1]->execute(k, *end);
            if (sizeBefore > _agents[k].size()) {
                end = rit;
            }
            else {
                end++;
            }

            //ALL GOALS COMPLETED ??
            bool failed = false;
            int finalScore = 0;
            for (size_t j = 0; j < goals.size() and failed == false; j++) {
                if (goals[j]->getCompleted() == false) {
                    if (prevGoal != j) {
                        gameplayMap->moveGoalPopup(j);
                        prevGoal = j;
                    }
                    failed = true;
                }
                else {
                    finalScore += goals[j]->getScore();
                }
            }
            if (failed == false and goals.empty() == false) {
                CCLOG("FINAL SCORE: %i", (finalScore/int(goals.size())));
                GameData::getInstance()->setLevelScore(numLevel, finalScore / goals.size());
                finishedGame = Success;
            }
        }
    }

    bool noAgentsLeft = _agents[0].empty();
    if(finishedGame==Running and noAgentsLeft)
    {
        finishedGame = NoAgentsLeft;
    }
}

bool GameLevel::validatePosition(int posx, int posy)
{
    //Fora del mapa
    if (posx < 0 or posx >= 480 or posy < 0 or posy >= 320) {
        return false;
    }
    //Aigua o similar
    if (gameplayMap->getValueAtGameplayMap(0, posx, posy, 0) == 0) {
        return false;
    }
    //Hi ha un agent
    if (_agentsMap[posx][posy] != nullptr) {
        return false;
    }
    return true;
}

int GameLevel::convertAttStringToInt(const string & s)
{
    int ret = -1;

    if (s == "MOBILITY")
    {
        ret = Mobility;
    }
    else if (s == "REPRODUCTION")
    {
        ret = Reproduction;
    }
    else if (s == "RESISTANCE")
    {
        ret = Resistance;
    }
    else if (s == "TECHNOLOGY")
    {
        ret = Technology;
    }
    else if (s == "HOSTILITY")
    {
        ret = Hostility;
    }
    else if (s == "CULTURAL_INFLUENCE")
    {
        ret = CulturalInfluence;
    }
    else if (s == "ADAPTATION")
    {
        ret = Adaptation;
    }
    
    return ret;
}

string GameLevel::convertAttIntToString(int i)
{
    string ret = "";
    
    switch (i) {
        case Mobility:
            ret = "MOBILITY";
            break;
        case Reproduction:
            ret = "REPRODUCTION";
            break;
        case Resistance:
            ret = "RESISTANCE";
            break;
        case Technology:
            ret = "TECHNOLOGY";
            break;
        case Hostility:
            ret = "HOSTILITY";
            break;
        case CulturalInfluence:
            ret = "CULTURAL_INFLUENCE";
            break;
        case Adaptation:
            ret = "ADAPTATION";
            break;
        default:
            ret = "";
            break;
    }
    
    return ret;
}

