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
//  GameLevel.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__GameLevel__
#define __simulplay__GameLevel__

#include <map>
#include <string>
#include "Agent.h"
#include "Act.h"
#include "Power.h"
#include "Goal.h"
#include "cocos2d.h"
#include "Timing.h"
#include <vector>

class UIGameplayMap;

using namespace std;

enum LevelState
{
    Running = 0,
    Success = 1,
    GoalFailBefore = 2,
    GoalFailAfter = 3,
    NoAgentsLeft = 4,
    UserCancel = 5
};

enum Attributes
{
    Mobility = 0,
    Reproduction = 1,
    Resistance = 2,
    Technology = 3,
    Hostility = 4,
    CulturalInfluence = 5,
    Adaptation = 6
};

class GameLevel
{  
public:
    // number of attributes for each agent
    static size_t _numAttributes;
    // float values for each level for the set of attributes
    typedef std::vector< std::vector<float> > AttributesLevels;
    // current level for each attribute
    typedef std::vector<int> Levels;
    // current level for each agent type and attribute
    typedef std::vector<Levels> LevelsVector;
    
    static GameLevel* getInstance();

    UIGameplayMap* getUIGameplayMap(void);
    void setUIGameplayMap(UIGameplayMap* gmplmap);
    string getMapFilename(void);
    void setMapFilename(string filename);
    std::vector<int> getMaxAgents(void);
    void setMaxAgents(std::vector<int> max);
    int getMaxAgent(int type);
    void setMaxAgent(int type, int max);
    std::vector<int> getNumInitialAgents(void);
    void setNumInitialAgents(std::vector<int> ini);
    int getNumInitialAgent(int type);
    void setNumInitialAgent(int type, int ini);
    double getCurrentTime(void);
    void setCurrentTime(double time);
    int getNumLevel(void);
    void setNumLevel(int lvl);

    // attributes management
    int getAgentAttribute(int type, int key);
    void setAgentAttribute(int type, int key, int value);

    // returns the current list of attribute levels of an agent type
    const Levels & getAgentAttributes(int type) const;

    // set the list of attribute levels for all the agents to init
    void setAgentAttributesToInit();
    // set the initial list of attribute levels for all the agents to current
    void setAgentAttributesInitToCurrent();
    bool initAttributesEmpty() const;

    // clears the list of attributes for all agents
    void resetAgentAttributesInitialConfig(void);

    // returns value of attribute attr (0 if not present)
    float getValueAtLevel(int attr, int level) const;
    // sets the attribute value at k to v for level i
    void setValueAtLevel(int attr, int level, float value);

    std::vector<Power*> getPowers(void);
    void setPowers(std::vector<Power*> p);
    void addPower(Power* p);
    void deletePower(int i);
    std::vector<std::list<Agent*> > getAgents(void);
    void addAgent(Agent* ag);
    std::vector<std::list<Agent*> > getAgentsPool(void);
    void popFrontAgentsPool(int type);
    void addAction(Act* act);
    void deleteAction(int i);
    std::vector<Goal*> getGoals(void);
    void setGoals(std::vector<Goal*> g);
    void addGoal(Goal* g);
    void deleteGoal(int i);
    int getAddedAgents(void);
    void setAddedAgents(int i);
    std::vector<cocos2d::Point> getDeletedAgents(void);
    void setDeletedAgents(std::vector<cocos2d::Point> v);
    void addDeletedAgent(cocos2d::Point p);
    void deleteDeletedAgent(int i);
    int getIdCounter(void);
    void setIdCounter(int count);
    const LevelState & getFinishedGame() const;
    void setFinishedGame(const LevelState & f);
    unsigned int getTimeSteps(void);
    void setTimeSteps(unsigned int steps);
    float getTimeSpeed(void);
    void setTimeSpeed(float speed);
    float getTimeSpeedBeforePause(void);
    void setTimeSpeedBeforePause(float speed);
    int getEvolutionPoints(void);
    void setEvolutionPoints(int points);
    int getAttributeCost(int type, int key);
    void setAttributeCost(int type, int key, int val);
    int getTimeExploited(int x, int y);
    void setTimeExploited(int x, int y, int val);
    bool getDepleted(int x, int y);
    void setDepleted(int x, int y, bool val);
    bool getEnvironmentAdaptation(int x, int y);
    void setEnvironmentAdaptation(int x, int y, bool val);
    int getCurrentAgentType(void);
    void setCurrentAgentType(int i);
    Agent* getAgentAtMap(int i, int j);
    int getMaxAllAgents(void);
    void setMaxAllAgents(int m);
    void setAgentDirection(int agentType, cocos2d::Point p);
    
    void setAgentFutureDirections(std::vector<std::vector<pair<int, cocos2d::Point> > > afd);
    void setAgentFutureDirection(int type, int step, cocos2d::Point p);
    void setAgentPixelSize(int i);
    int getAgentPixelSize(void);
    void setEvolutionPointsFreq(int i);
    int getEvolutionPointsFreq(void);
    
    
    void createLevel(void);
    void setAttributesToInitialAgents(void);
    void setNumAgentTypes(size_t numAgents);

    /** start the level and run it until some condition ends it **/
    void playLevel(void);
    void resetLevel(void);

    bool validatePosition(int posx, int posy);
    
    int convertAttStringToInt(const string & s);
    string convertAttIntToString(int i);

    bool paint = false;
    bool ended = false;
    float calcTime = 0;
    int prevGoal = 0;

    void setModifiableAttr(int order, int attr) { _modifiableAtt.at(order) = attr; }
    const Levels & getModifiableAttr() const { return _modifiableAtt; }

private:
    static GameLevel* gamelevelInstance;
    UIGameplayMap* gameplayMap;

    string mapFilename;
    //VECTOR POSITION = TYPE
    std::vector<int> maxAgents;
    std::vector<int> numInitialAgents;
    int maxAllAgents = 0;
    int agentPixelSize = 1;

    clock_t currentTime = 0;
    int numLevel;
    LevelsVector _agentAttributes;
    LevelsVector _agentAttributesInitialConfig;
    LevelsVector _attributesCost;
    // list of attributes that can be modified by user
    Levels _modifiableAtt;
    //temporal
    AttributesLevels _attributesLevels;
    std::vector<Power*> powers;
    std::vector<std::list<Agent*> > _agents;
    std::vector<std::list<Agent*> > _agentsPool;
    std::vector<Act*> actions;
    std::vector<Goal*> goals;
    Agent* _agentsMap[480][320];

    //Resources exploitment
    int timeExploitedMap[480][320] = { { 0 } };
    bool depletedMap[480][320] = { { false } };
    bool adaptedMap[480][320] = { { false } };

    int addedAgents = 0;
    std::vector<cocos2d::Point> deletedAgents;
    int idCounter = 0;

    unsigned int timeSteps = 0;
    float timeSpeed = 0;
    float timeSpeedBeforePause = 1.265;
    
    int evolutionPoints = 10;
    int evolutionPointsFreq = 2;
   
    // state of the game
    LevelState _finishedGame = Running;

    int currentAgentType = 0;

    std::vector<cocos2d::Point> _agentDirections;
    std::vector<std::vector<pair<int, cocos2d::Point> > > _agentFutureDirections;

    GameLevel();

    void initializeAgentsPool(void);
    void generateInitialAgents(int type);
    inline void act(void);

    // updates mission goals and _finishedGame state
    void checkGoals();
    // computes the offspring that type population can have based on Reproduction attribute, ReproductionBoost and maxAgents
    void computeOffspring( int type );
    // sets directions from _agentFutureDirections to _agentDirections if timeSteps has arrived
    void updateDirections(int type);
    // consumes resistance and removes agents from simulation if life <= 0
    void consumeAndRemove(int type);
    // executes all actions for all agents of type
    void executeActions(int type);
    // XRC TODO temporary (move to Agent)
    std::list<Agent*>::iterator checkDeath( std::list<Agent*>::iterator & it);
    void deleteAgent(Agent* agent);
};

#endif /* defined(__simulplay__GameLevel__) */
