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
#include "Timing.h"
#include <vector>
#include "Achievement.h"
#include <cocos2d.h>

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
    eMobility = 0,
    eReproduction = 1,
    eResistance = 2,
    eTechnology = 3,
    eWarfare = 4,
    eInfluence = 5,
    eExploitation = 6,
    eTrade = 7
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
    std::vector<Goal*> & getGoals(void);
    void addGoal(Goal* g);
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
    std::vector<cocos2d::Point> getRestored(void);
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
    Achievement* getInGameAchievement(void);
    void setInGameAchievement(Achievement *ach);
    bool getPowersUsed(void);
    void setPowersUsed (bool p);
    int getRegenerationRate(void);
    void setRegenerationRate(int r);

    
    vector<string> getCompletedAchievements(void);
    
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

    void setModifiableAttr(int order, int attr) { _modifiableAtt.at(order) = attr; }
    const Levels & getModifiableAttr() const { return _modifiableAtt; }
    // returns a Rect based on center and distance after checking for boundaries
    cocos2d::Rect getArea(const Position & center, int mobility) const;
    
    void clearRestored(void);
 
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
    AttributesLevels _attributesLevels;
    std::vector<Power*> powers;
    std::vector<std::list<Agent*> > _agents;
    std::vector<std::list<Agent*> > _agentsPool;
    std::vector<Act*> actions;
    std::vector<Goal*> goals;
    Agent* _agentsMap[480][320];
    
    //completed achievements to show (key words corresponding to title in achievement_lang)
    vector<string> completedAchievements;

    //Resources exploitment
    int timeExploitedMap[480][320] = { { 0 } };
    bool depletedMap[480][320] = { { false } };
    std::vector<cocos2d::Point> _depletedVector;
    std::vector<cocos2d::Point> _restoredVector;
    bool adaptedMap[480][320] = { { false } };

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
    
    Achievement* _inGameAchievement = nullptr;
    bool _powersUsed = false;

    GameLevel();

    void initializeAgentsPool(void);
    void generateInitialAgents(int type);
    inline void act(void);

    // updates mission goals and _finishedGame state
    void checkGoals();
    // computes the traded agents that type population can have based on Trade attribute and maxAgents
    void computeTraded(int type);
    // computes the influenced agents that type population can have based on Influence attribute, InfluenceBoost and maxAgents
    void computeInfluenced( int type );
    // computes the offspring that type population can have based on Reproduction attribute, ReproductionBoost and maxAgents
    void computeOffspring( int type );
    // sets directions from _agentFutureDirections to _agentDirections if timeSteps has arrived
    void updateDirections(int type);
    // consumes resistance and removes agents from simulation if life <= 0
    void consumeAndRemove(int type);
    // executes all actions for all agents of type (except for reproduction)
    void executeActions(int type);
    // execute Reproduction action
    void reproduce(int type);
    // XRC TODO temporary (move to Agent)
    void checkDeath( std::list<Agent*>::iterator & it);
    void deleteAgent(Agent* agent);
    //check achievements
    void checkAchievements(void);
    //regenerate wood
    void regenerate(void);

    bool _isFinished;
    //cells per step
    int _regenerationRate = 0;
    
};

#endif /* defined(__simulplay__GameLevel__) */
