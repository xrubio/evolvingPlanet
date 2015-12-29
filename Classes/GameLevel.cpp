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
#include "Reproduce.h"
#include "Power.h"
#include "AreaPower.h"
#include "GameData.h"
#include "ProgressAchievement.h"
#include "LevelAchievement.h"

GameLevel* GameLevel::gamelevelInstance = NULL;

size_t GameLevel::_numAttributes = 9;

GameLevel* GameLevel::getInstance()
{
    if (!gamelevelInstance) // Only allow one instance of class to be generated.
    {
        gamelevelInstance = new GameLevel;
    }
    return gamelevelInstance;
}

GameLevel::GameLevel() : _play(false)
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

const std::string & GameLevel::getMapFilename() const
{
    return mapFilename;
}

void GameLevel::setMapFilename(const std::string & filename)
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

const std::vector<Power*> & GameLevel::getPowers(void) const
{
    return powers;
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

bool GameLevel::getDepleted(int x, int y)
{
    return depletedMap[x][y];
}

void GameLevel::setDepletedMap(int x, int y, bool val)
{
    depletedMap[x][y] = val;
}

void GameLevel::addDepletedToVector(int x, int y)
{
    _depletedVector.push_back(Point(x, y));
}

std::vector<cocos2d::Point> GameLevel::getRestored(void)
{
    return _restoredVector;
}

bool GameLevel::getTerraformed(int x, int y)
{
    return _terraformedMap[x][y];
}

void GameLevel::setTerraformed(int x, int y, bool val)
{
    _terraformedMap[x][y] = val;
    if (val)
    {
        _terraformedVector.push_back(Point(x, y));
    }
}

std::vector<cocos2d::Point> GameLevel::getTerraformedVector(void)
{
    return _terraformedVector;
}

int GameLevel::getCurrentAgentType(void)
{
    return currentAgentType;
}

void GameLevel::setCurrentAgentType(int i)
{
    currentAgentType = i;
}

cocos2d::Rect GameLevel::getArea(const Position & center, int mobility) const
{
    // map size is 480x320
    int minX = std::max(0, center.getX() - mobility);
    int maxX = std::min(479, center.getX() + mobility);
    int minY = std::max(0, center.getY() - mobility);
    int maxY = std::min(319, center.getY() + mobility);
    return cocos2d::Rect(minX, minY, maxX-minX, maxY-minY);
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

Achievement* GameLevel::getInGameAchievement(void)
{
    return _inGameAchievement;
}

void GameLevel::setInGameAchievement(Achievement *ach)
{
    _inGameAchievement = ach;
}

bool GameLevel::getPowersUsed(void)
{
    return _powersUsed;
}

void GameLevel::setPowersUsed (bool p)
{
    _powersUsed = p;
}

int GameLevel::getRegenerationRate(void)
{
    return _regenerationRate;
}

void GameLevel::setRegenerationRate(int r)
{
    _regenerationRate = r;
}

void GameLevel::setTerraformFactor(float t)
{
    _terraformFactor = t;
}

vector<string> GameLevel::getCompletedAchievements(void)
{
    return completedAchievements;
}

void GameLevel::playLevel(void)
{
    CCLOG("step;floatStep;pop;time");
    _lastStep = goals.back()->getMaxTime();
    CCLOG("mission will end in step: %d", _lastStep);
    while (_finishedGame == Running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (Timing::getInstance()->act == true) {
            paint = false;
            clock_t stepTime = clock();
            pthread_mutex_lock(&gameplayMap->gameLevelMutex);
            act();
            pthread_mutex_unlock(&gameplayMap->gameLevelMutex);
            if(int(Timing::getInstance()->getTimeStep()) % evolutionPointsFreq == 0) {
                evolutionPoints++;
            }
            paint = true;
            Timing::getInstance()->act = false;  
            CCLOG("%d;%f;%zu;%f", int(Timing::getInstance()->getTimeStep()), Timing::getInstance()->getTimeStep(), _agents.at(0).size(), float(clock() - stepTime) / CLOCKS_PER_SEC);
            CCLOG("RESOURCES: Wood %d, Mineral %d", Agent::_resourcesPool.at(0).at(0), Agent::_resourcesPool.at(0).at(1));
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
            depletedMap[i][j] = false;
            _terraformedMap[i][j] = false;
        }
    }

    evolutionPoints = 10;

    _finishedGame = Running;

    currentAgentType = 0;
    maxAllAgents = 0;

    paint = false;
    ended = false;

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
    _inGameAchievement = nullptr;
    _regenerationRate = 0;
    _depletedVector.clear();
    _restoredVector.clear();
    _terraformedVector.clear();
    
    _legendNames.clear();
    _legendColors.clear();
        
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
            auto a = new Agent();
            l_agent.push_back(a);
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
            if (gameplayMap->getValueAtGameplayMap(1, x, y) == type) {
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
        if(!validatePosition(posx, posy) or !(gameplayMap->getValueAtGameplayMap(1, posx, posy) == type))
        {
            continue;
        }
        Agent* a = _agentsPool.at(type).front();
        _agentsPool.at(type).pop_front();
        a->setId(idCounter);
        // mean of 150, but each agent is slightly different
        a->setLife(cocos2d::RandomHelper::random_int(125,175));
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


bool GameLevel::powerIsInEffect( const PowerType & type ) const
{
    for(size_t i = 0; i < getPowers().size(); i++)
    {
        Power * power = getPowers().at(i);
        if(getPowers().at(i)->getType()!=type)
        {
            continue;
        }
        if(power->isInEffect())
        {
            return true;
        }
        return false;
    }
    return false;
}

void GameLevel::computeInfluenced( int type )
{
    // definir número d'agents que han de crear-se
    size_t numAgents = _agents.at(type).size();
    float probInfluence = getValueAtLevel(eInfluence, _agentAttributes.at(type).at(eInfluence));

    // InfluenceBoost only affects user's population
    if(type==0)
    {
        if(powerIsInEffect(InfluenceBoost))
        {
            probInfluence = getValueAtLevel(eInfluence, 5);
        }
    }

    int newAgents = int((float)numAgents*probInfluence);
    // check diff between current agents and max agents, and see if it's lower than newAgents
    int value = min(newAgents, int(getMaxAgent(type)-_agents.at(type).size()));
    Agent::_numInfluenced.at(type) = RandomHelper::random_int(int(value*0.5f), value);
}

bool GameLevel::powerIsInRadius( const PowerType & type, const Position & pos ) const
{

    for(size_t i = 0; i < getPowers().size(); i++)
    {
        Power * power = getPowers().at(i);
        if(getPowers().at(i)->getType()!=type)
        {
            continue;
        }

        AreaPower * areaPower = (AreaPower*)power;
        return areaPower->isInRadius(pos);
    }
    return false;
}

void GameLevel::computeTraded(int type)
{
    // definir número d'agents que han de crear-se
    size_t numAgents = _agents.at(type).size();
    float probTrade = getValueAtLevel(eTrade, _agentAttributes.at(type).at(eTrade));
    
    int newAgents = int((float)numAgents*probTrade);
    // check diff between current agents and max agents, and see if it's lower than newAgents
    int value = min(newAgents, int(getMaxAgent(type)-_agents.at(type).size()));
    Agent::_numTraded.at(type) = RandomHelper::random_int(int(value*0.5f), value);
}

void GameLevel::computeOffspring( int type )
{
    // definir número d'agents que han de crear-se
    size_t numAgents = _agents.at(type).size();
    float probReproduction = getValueAtLevel(eReproduction, _agentAttributes.at(type).at(eReproduction));


    // ReproductionBoost only affects user's population
    if(type==0)
    {
        if(powerIsInEffect(ReproductionBoost))
        {
            probReproduction = getValueAtLevel(eReproduction, 5);
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
    if(int(Timing::getInstance()->getTimeStep()) != _agentFutureDirections.at(type).at(0).first)
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

    float harm = gameplayMap->getValueAtGameplayMap(0, agent->getPosition().getX(), agent->getPosition().getY());
    float resistance = agent->getValue(eResistance);
    
    //Mirar al mapa de poders de GameLevel si hi es, sino no fer la accio
    if(agent->getType()==0)
    {
        if(powerIsInEffect(ResistanceBoost) and powerIsInRadius(ResistanceBoost, agent->getPosition()))
        {
            resistance = getValueAtLevel(eResistance, 5);
        }
    }

    harm = max(0.0f, harm/resistance);
    
    //Terraformed
    if (_terraformedMap[agent->getPosition().getX()][agent->getPosition().getY()] == true and _terraformFactor > 0.0)
    {
        harm = harm * _terraformFactor;
    }
    
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

void GameLevel::reproduce(int type)
{  
    // Reproduction is always the first action in the list
    std::list<Agent*> & agents = _agents.at(type);
    for(std::list<Agent*>::iterator it=agents.begin(); it!=agents.end(); it++)
    {
        actions.at(0)->execute(*it);
    }
}

void GameLevel::executeActions(int type)
{
    std::list<Agent*> & agents = _agents.at(type);
    for(std::list<Agent*>::iterator it=agents.begin(); it!=agents.end(); it++)
    {
        // skip action 0 (reproduce)
        for(size_t j=1; j<actions.size(); j++)
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
        int agentType = (int)k;
        updateDirections(agentType);
        consumeAndRemove(agentType);
        computeOffspring(agentType);

        // reorder based on distance to fingerSpot (closest first)
        const Point & fingerSpot = _agentDirections.at(k);
        if(fingerSpot.x != -1)
        {
            _agents.at(k).sort(DistSorter(fingerSpot));
        }
        // reproduce and compute if there's room for converting influenced agents
        reproduce(agentType);
        computeInfluenced(agentType);
        computeTraded(agentType);
        if(fingerSpot.x != -1)
        {
            _agents.at(k).sort(DistSorter(fingerSpot));
        }
        executeActions(agentType);
    }
    if (_regenerationRate > 0)
    {
        regenerate();
    }
        
    //DISCOVERY ACHIEVEMENT
    if (_inGameAchievement != nullptr)
    {
        bool completed = false;
        for(std::list<Agent*>::iterator it=_agents.at(0).begin(); it!=_agents.at(0).end() and completed == false; it++)
        {
            int agentColorCode = GameLevel::getInstance()->getUIGameplayMap()->getValueAtGameplayMap(1, (*it)->getPosition().getX(),  (*it)->getPosition().getY());
            completed = ((LevelAchievement*)_inGameAchievement)->checkInGameAchievement(_inGameAchievement->getGoalType(), numLevel, agentColorCode);
        }
        if (completed)
        {
            string key = to_string(numLevel) + "_" + _inGameAchievement->getGoalType();
            UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
            GameLevel::getInstance()->getUIGameplayMap()->drawInGameAchievementWindow = true;
        }
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
    if (gameplayMap->getValueAtGameplayMap(0, posx, posy) == 0) {
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
        ret = eMobility;
    }
    else if (s == "REPRODUCTION")
    {
        ret = eReproduction;
    }
    else if (s == "RESISTANCE")
    {
        ret = eResistance;
    }
    else if (s == "TECHNOLOGY")
    {
        ret = eTechnology;
    }
    else if (s == "WARFARE")
    {
        ret = eWarfare;
    }
    else if (s == "INFLUENCE")
    {
        ret = eInfluence;
    }
    else if (s == "EXPLOITATION")
    {
        ret = eExploitation;
    }
    else if (s == "TRADE")
    {
        ret = eTrade;
    }
    else if (s == "TERRAFORM")
    {
        ret = eTerraform;
    }
    
    return ret;
}

string GameLevel::convertAttIntToString(int i)
{
    string ret = "";
    
    switch (i) {
        case eMobility:
            ret = "MOBILITY";
            break;
        case eReproduction:
            ret = "REPRODUCTION";
            break;
        case eResistance:
            ret = "RESISTANCE";
            break;
        case eTechnology:
            ret = "TECHNOLOGY";
            break;
        case eWarfare:
            ret = "WARFARE";
            break;
        case eInfluence:
            ret = "INFLUENCE";
            break;
        case eExploitation:
            ret = "EXPLOITATION";
            break;
        case eTrade:
            ret = "TRADE";
            break;
        case eTerraform:
            ret = "TERRAFORM";
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
    Agent::_numInfluenced.clear();
    Agent::_numTraded.clear();
    Agent::_resourcesPool.clear();
    Agent::_resourcesPoolMax.clear();
    _agentAttributes.clear();
    _attributesCost.clear();
    for(size_t i=0; i<numAgents; i++)
    {
        _agentAttributes.push_back(Levels(_numAttributes, 0));
        _attributesCost.push_back(Levels(_numAttributes, 1));
        Agent::_numOffspring.push_back(0);
        Agent::_numInfluenced.push_back(0);
        Agent::_numTraded.push_back(0);
        vector<int> resources (3, 0);
        Agent::_resourcesPool.push_back(resources);
    }
    Agent::_resourcesPoolMax.push_back(0);
    Agent::_resourcesPoolMax.push_back(0);
}

void GameLevel::checkAchievements(void)
{
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

void GameLevel::regenerate(void)
{
    std::random_shuffle(_depletedVector.begin(), _depletedVector.end());
    int init = int(_depletedVector.size()) - 1;
    for (int i = init; i >= 0 and i > init - _regenerationRate; i--)
    {
        int x = int(_depletedVector.at(i).x);
        int y = int(_depletedVector.at(i).y);
        //check neighbour cells are depleted (left, right, top, bottom) TODO not checking if the cells are wood
        if ((x > 0 and depletedMap[x - 1][y] == false) or  (x < 479 and depletedMap[x + 1][y] == false) or (y > 0 and depletedMap[x][y - 1] == false) or (y < 319 and depletedMap[x][y + 1] == false))
        {
            depletedMap[x][y] = false;
            _restoredVector.push_back(Point(x, y));
            _depletedVector.erase(_depletedVector.begin() + i);
        }
    }
}

void GameLevel::clearRestored(void)
{
    _restoredVector.clear();
}

void GameLevel::clearTerraformedVector(void)
{
    _terraformedVector.clear();
}

