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
#include "Reproduce.h"
#include "Power.h"
#include "AreaPower.h"
#include "GameData.h"
#include "ExpansionGoal.h"
#include "ProgressAchievement.h"
#include "LevelAchievement.h"

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

std::vector<int> GameLevel::getMaxAgents(void)
{
    return maxAgents;
}

void GameLevel::setMaxAgents(std::vector<int> max)
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

std::vector<int> GameLevel::getNumInitialAgents(void)
{
    return numInitialAgents;
}

void GameLevel::setNumInitialAgents(std::vector<int> ini)
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

int GameLevel::getAgentAttribute(int type, int key)
{
    return _agentAttributes.at(type).at(key);
}

void GameLevel::setAgentAttribute(int type, int key, int level)
{
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

std::vector<Power*> GameLevel::getPowers(void)
{
    return powers;
}

void GameLevel::setPowers(std::vector<Power*> p)
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

std::vector<std::list<Agent*> > GameLevel::getAgents(void)
{
    return _agents;
}

void GameLevel::addAgent(Agent* ag)
{
    while (_agents.size() <= ag->getType()) {
        std::list<Agent*> v;
        _agents.push_back(v);
    }
    _agents.at(ag->getType()).push_back(ag);
    _agentsMap[ag->getPosition().getX()][ag->getPosition().getY()] = ag;
}

void GameLevel::deleteAgent(Agent* agent)
{
    int type = agent->getType();
    int posx = agent->getPosition().getX();
    int posy = agent->getPosition().getY();
    _agentsMap[posx][posy] = nullptr;
    _agentsPool.at(type).push_back(agent);
}

std::vector<std::list<Agent*> > GameLevel::getAgentsPool(void)
{
    return _agentsPool;
}

void GameLevel::popFrontAgentsPool(int type)
{
    _agentsPool.at(type).pop_front();
}

void GameLevel::addAction(Act* act)
{
    actions.push_back(act);
}

void GameLevel::deleteAction(int i)
{
    actions.erase(actions.begin() + i);
}

std::vector<Goal*> & GameLevel::getGoals(void)
{
    return goals;
}

void GameLevel::addGoal(Goal* g)
{
    goals.push_back(g);
}

std::vector<Point> GameLevel::getDeletedAgents(void)
{
    return deletedAgents;
}

void GameLevel::setDeletedAgents(std::vector<Point> v)
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
    return _finishedGame;
}

void GameLevel::setFinishedGame(const LevelState & f)
{
    _finishedGame = f;
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

void GameLevel::setAgentDirection(int agentType, cocos2d::Point p)
{
    _agentDirections.at(agentType) = p;
}

void GameLevel::setAgentFutureDirection(int type, int step, cocos2d::Point p)
{
    pair<unsigned int, cocos2d::Point> par(step, p);
    while (_agentFutureDirections.size() <= type) {
        std::vector<pair<int, cocos2d::Point> > v;
        _agentFutureDirections.push_back(v);
    }
    _agentFutureDirections.at(type).push_back(par);
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

vector<string> GameLevel::getCompletedAchievements(void)
{
    return completedAchievements;
}

void GameLevel::playLevel(void)
{
    CCLOG("step;pop;time");
    while (_finishedGame == Running)
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
            
            // number of agents of type 0
            size_t numAgents = _agents.at(0).size();
            // alternate summary statistic: total number of agents
            /*
            for (size_t i = 0; i < _agents.size(); i++)
            {
                numAgents += _agents.at(i).size();
            }
            */

            CCLOG("%d;%zu;%f", timeSteps, numAgents, float(clock() - stepTime) / CLOCKS_PER_SEC);
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
    
    if (_finishedGame != UserCancel)
    {
        checkAchievements();
    }
    
    ended = true;
    CCLOG("End of game: %i", _finishedGame);
}

void GameLevel::resetLevel(void)
{
    //REVISAR MEMORIA, O ESBORRAR INSTANCIA ENLLOC DE FER RESET I ALLIBERAR AL DESTRUCTOR

    currentTime = 0;
    numLevel = 0;
    _agentAttributes.clear();
    _modifiableAtt.clear();
    _modifiableAtt.resize(3);
    std::fill(_modifiableAtt.begin(), _modifiableAtt.end(), -1);
    powers.clear();
    _agents.clear();
    _agentsPool.clear();
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

    _finishedGame = Running;

    currentAgentType = 0;
    maxAllAgents = 0;

    paint = false;
    ended = false;
    calcTime = 0;

    _agentDirections.clear();
    _agentFutureDirections.clear();

    _attributesLevels.clear();
    // all attributes have 6 levels (0 to 5). All of them are set to 0
    for(size_t i=0; i<_numAttributes; i++)
    {
        std::vector<float> r(6, 0);
        _attributesLevels.push_back(r);
    }
    
    completedAchievements.clear();
}

void GameLevel::createLevel(void)
{
    initializeAgentsPool();
    for(size_t i = 0; i < numInitialAgents.size(); i++)
    {
        generateInitialAgents(int(i));
        _agentDirections.push_back(Point(-1, -1));
    }

    paint = true;
}

void GameLevel::setAttributesToInitialAgents(void)
{
    std::list<Agent*> agents = _agents.at(0);
    for(std::list<Agent*>::iterator it=agents.begin(); it!=agents.end(); it++)
    {
        (*it)->copyValues(0);
    }
}

void GameLevel::initializeAgentsPool(void)
{
    for (int i = 0; i < maxAgents.size(); i++) {
        std::list<Agent*> l_agent;
        for (int j = 0; j < maxAgents.at(i); j++) {
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
        for (int y = 1; y <= 320; y++) {
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
    while (i < numInitialAgents.at(type))
    {
        int posx = random(minX, maxX);
        int posy = random(minY, maxY);
        if(!validatePosition(posx, posy) or !(gameplayMap->getValueAtGameplayMap(1, posx, posy, 0) == type))
        {
            continue;
        }
        Agent* a = _agentsPool.at(type).front();
        _agentsPool.at(type).pop_front();
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

bool compareDists(const Agent * L, const Agent * R, const Point & spot)
{       
   // compute chebyshev distance
   float Ldist = max(abs(L->getPosition().getX()-spot.x), abs(L->getPosition().getY()-spot.y));
   float Rdist = max(abs(R->getPosition().getX()-spot.x), abs(R->getPosition().getY()-spot.y));
   return Ldist<Rdist;
};

class DistSorter
{
    const Point & _spot;
public:
    DistSorter(const Point & spot) : _spot(spot){}
    bool operator()(const Agent * a1, const Agent * a2)
    {
        return compareDists(a1, a2, _spot);
    }
};


void GameLevel::computeOffspring( int type )
{
    // definir número d'agents que han de crear-se
    size_t numAgents = _agents.at(type).size();
    float probReproduction = getValueAtLevel(Reproduction, _agentAttributes.at(type).at(Reproduction));

    // ReproductionBoost only affects user's population
    if(type==0)
    {
        Power* p = nullptr;
        for (size_t i = 0; i < getPowers().size(); i++)
        {
            if (getPowers().at(i)->getId() != ReproductionBoost)
            {
                continue;
            }
            p = getPowers().at(i);
        }
        if (p != nullptr and p->getDurationLeft() > 0)
        {
            probReproduction = getValueAtLevel(Reproduction, 5);
        }
    }

    int newAgents = int((float)numAgents*probReproduction);
    // check diff between current agents and max agents, and see if it's lower than newAgents
    int value = min(newAgents, int(getMaxAgent(type)-_agents.at(type).size()));
    Agent::_numOffspring.at(type) = RandomHelper::random_int(int(value*0.5f), value);
}

void GameLevel::checkGoals()
{
    int goalToCheck = -1;
    // goalToCheck is the first goal not completed (they are sequential)
    for(size_t j=0; j<goals.size(); j++)
    {
        if(!goals.at(j)->getCompleted())
        {
            goalToCheck = int(j);
            break;
        }
    }
    
    bool nextGoalAchieved = false;
    for(std::list<Agent*>::iterator it=_agents.at(0).begin(); it!=_agents.at(0).end(); it++)
    {
        // as soon as one agent completes the goal then stop checks
        if(goals.at(goalToCheck)->checkGoal(0, *it))
        {
            nextGoalAchieved = true;
            break;
        }
    }
    
    if(nextGoalAchieved)
    {
        // update gui
        gameplayMap->moveGoalPopup(goalToCheck);
        
        // check if this was the last goal
        if(goalToCheck==(goals.size()-1))
        {
            // compute final score
            int finalScore = 0;
            for(size_t j=0; j<goals.size(); j++)
            {
                finalScore += goals.at(j)->getScore();
            }
            int averagedScore = int(finalScore/goals.size());
            CCLOG("FINAL SCORE: %d", averagedScore);
            GameData::getInstance()->setLevelScore(numLevel, averagedScore);
            _finishedGame = Success;
            return;
        }
    }
    
    // mission finished due to extinction
    bool noAgentsLeft = _agents.at(0).empty();
    if(_finishedGame==Running and noAgentsLeft)
    {
        _finishedGame = NoAgentsLeft;
    }
}

void GameLevel::updateDirections(int type)
{       
    // check if NPC agents arrived to destination
    if(type!=0 && _agentDirections.at(type).x!=-1)
    {
        cocos2d::Point pos(-1,-1);
        for(std::list<Agent*>::iterator it=_agents.at(type).begin(); it!=_agents.at(type).end(); it++)
        {
            pos.x = (*it)->getPosition().getX();
            pos.y = (*it)->getPosition().getY();
            const cocos2d::Point & goal = _agentDirections.at(type);
            float distance = pos.getDistance(goal);
            if(distance<5.0f)
            {
                _agentDirections.at(type) = Point(-1,-1);
                break;
            }
        }
    }

    //CHECK DIRECTION
    if(_agentFutureDirections.empty() or _agentFutureDirections.at(type).empty())
    {
        return;
    }
    if(timeSteps != _agentFutureDirections.at(type).at(0).first)
    {
        return;
    }
    _agentDirections.at(type) = _agentFutureDirections.at(type).at(0).second;
    _agentFutureDirections.at(type).erase(_agentFutureDirections.at(type).begin());
}

void GameLevel::checkDeath( std::list<Agent*>::iterator & it)
{   
    Agent * agent = *it;
    UIGameplayMap* gameplayMap = GameLevel::getInstance()->getUIGameplayMap();

    float harm = gameplayMap->getValueAtGameplayMap(0, agent->getPosition().getX(), agent->getPosition().getY(), 0);
    float resistance = agent->getValue(Resistance);

    //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
    if(agent->getType()==0)
    {
        Power* p = nullptr;
        for (size_t i = 0; i < getPowers().size(); i++)
        {
            if(getPowers().at(i)->getId() != ResistanceBoost)
            {
                continue;
            }
           p = getPowers().at(i);
        }
        if(p != nullptr and p->getDurationLeft() > 0)
        {
            if (gameplayMap->isInBoostResistanceArea(agent->getPosition().getX() * float(2048.0 / 480.0), ((1536 - 1365) / 2) + (agent->getPosition().getY() * float(1365.0 / 320.0))))
            {
                resistance = getValueAtLevel(Resistance, 5);
            }
        }
    }

    harm = max(0.0f, harm/resistance);
    agent->setLife(agent->getLife() - int(harm));

    if (agent->getLife() <= 0)
    {
        addDeletedAgent(Point(agent->getPosition().getX(), agent->getPosition().getY()));
        GameLevel::getInstance()->deleteAgent(agent);
        it = _agents.at(agent->getType()).erase(it);
    }
    else
    {
        it++;
    }
}

void GameLevel::consumeAndRemove(int type)
{
    // primera passada per comprovar goals i matar agents iaios
    std::list<Agent*> & agents = _agents.at(type);
    std::list<Agent*>::iterator it = agents.begin();

    while(it!=agents.end())
    {
        checkDeath(it);
    }
}

void GameLevel::executeActions(int type)
{
    std::list<Agent*> & agents = _agents.at(type);
    for(std::list<Agent*>::iterator it=agents.begin(); it!=agents.end(); it++)
    {
        for(size_t j=0; j<actions.size(); j++)
        {
            actions.at(j)->execute(*it);
        }
    }
}

void GameLevel::act(void)
{
    if(_finishedGame!=Running)
    {
        return;
    }

    deletedAgents.clear();

    for(size_t k = 0; k < _agents.size(); k++)
    {
        updateDirections(int(k));
        consumeAndRemove(int(k));
        computeOffspring(int(k));

        // reorder based on distance to fingerSpot (closest first)
        const Point & fingerSpot = _agentDirections.at(k);
        if(fingerSpot.x != -1)
        {
            _agents.at(k).sort(DistSorter(fingerSpot));
        }
        executeActions(int(k));
    }
    
    checkGoals();
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
    
void GameLevel::setNumAgentTypes(size_t numAgents)
{
    Agent::_numOffspring.clear();
    _agentAttributes.clear();
    _attributesCost.clear();
    for(size_t i=0; i<numAgents; i++)
    {
        _agentAttributes.push_back(Levels(_numAttributes, 0));
        _attributesCost.push_back(Levels(_numAttributes, 1));
        Agent::_numOffspring.push_back(0);
    }
}

void GameLevel::checkAchievements(void)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    CCLOG("WARNING: Achievements implemented only for IOS");
    return;
#endif
        
    vector<Achievement*> progressAchs = GameData::getInstance()->getAchievements(0);
    vector<Achievement*> levelAchs = GameData::getInstance()->getAchievements(numLevel);
    
    for (int i = 0; i < progressAchs.size(); i++)
    {
        if (progressAchs.at(i)->getCompleted() == false)
        {
            if (((ProgressAchievement*)progressAchs.at(i))->checkAchievement(progressAchs.at(i)->getGoalType(), 0) == true)
            {
                string key = to_string(0) + "_" + progressAchs.at(i)->getGoalType();
                completedAchievements.push_back(key);
                UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
            }
        }
    }

    for (int i = 0; i < levelAchs.size(); i++)
    {
        if (levelAchs.at(i)->getCompleted() == false)
        {
            if (((LevelAchievement*)levelAchs.at(i))->checkAchievement(levelAchs.at(i)->getGoalType(), numLevel) == true)
            {
                string key = to_string(numLevel) + "_" + levelAchs.at(i)->getGoalType();
                completedAchievements.push_back(key);
                UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
            }
        }
    }
    CCLOG("COMPLETED %lu ACHIEVEMENTS", completedAchievements.size());
}

