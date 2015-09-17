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
#include <vector>
#include "Agent.h"
#include "Act.h"
#include "Power.h"
#include "Goal.h"
#include "cocos2d.h"
#include "Timing.h"

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

class GameLevel
{  
public:
    // float values for each level for the set of attributes
    typedef std::map<std::string, std::vector<float> > AttributesLevelsMap;
    typedef std::map<std::string, int > LevelsMap;
    typedef std::vector<LevelsMap> LevelsMapVector;
    
    static GameLevel* getInstance();

    UIGameplayMap* getUIGameplayMap(void);
    void setUIGameplayMap(UIGameplayMap* gmplmap);
    string getMapFilename(void);
    void setMapFilename(string filename);
    vector<int> getMaxAgents(void);
    void setMaxAgents(vector<int> max);
    int getMaxAgent(int type);
    void setMaxAgent(int type, int max);
    vector<int> getNumInitialAgents(void);
    void setNumInitialAgents(vector<int> ini);
    int getNumInitialAgent(int type);
    void setNumInitialAgent(int type, int ini);
    double getCurrentTime(void);
    void setCurrentTime(double time);
    int getNumLevel(void);
    void setNumLevel(int lvl);

    // attributes management
    int getAgentAttribute(int type, const string & key);
    void setAgentAttribute(int type, const string & key, int value);

    // returns the current list of attribute levels of an agent type
    const LevelsMap & getAgentAttributes(int type) const;

    // set the list of attribute levels for all the agents to init
    void setAgentAttributesToInit();
    // set the initial list of attribute levels for all the agents to current
    void setAgentAttributesInitToCurrent();
    bool initAttributesEmpty() const;

    // clears the list of attributes for all agents
    void resetAgentAttributesInitialConfig(void);

    // returns value of attribute attr (0 if not present)
    float getValueAtLevel(const string & attr, int level) const;
    // creates the attribute and set all values to 0
    void createAttributeLevels(const string & attr);
    // sets the attribute value at k to v for level i
    void setValueAtLevel(const string & attr, int level, float value);

    vector<Power*> getPowers(void);
    void setPowers(vector<Power*> p);
    void addPower(Power* p);
    void deletePower(int i);
    vector<list<Agent*> > getAgents(void);
    void setAgents(vector<list<Agent*> > agents);
    void addAgent(Agent* ag);
    list<Agent*>::reverse_iterator deleteAgent(int type, Agent* agent);
    vector<list<Agent*> > getAgentsPool(void);
    void popFrontAgentsPool(int type);
    vector<Act*> getActions(void);
    void setActions(vector<Act*> a);
    void addAction(Act* act);
    void deleteAction(int i);
    vector<Goal*> getGoals(void);
    void setGoals(vector<Goal*> g);
    void addGoal(Goal* g);
    void deleteGoal(int i);
    int getAddedAgents(void);
    void setAddedAgents(int i);
    vector<cocos2d::Point> getDeletedAgents(void);
    void setDeletedAgents(vector<cocos2d::Point> v);
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
    int getAttributeCost(int type, string key);
    void setAttributeCost(int type, string key, int val);
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
    vector<cocos2d::Point> getAgentDirections(void);
    void setAgentDirections(vector<cocos2d::Point> ad);
    void setAgentDirection(int agentType, cocos2d::Point p);
    
    vector<vector<pair<int, cocos2d::Point> > > getAgentFutureDirections(void);
    void setAgentFutureDirections(vector<vector<pair<int, cocos2d::Point> > > afd);
    void setAgentFutureDirection(int type, int step, cocos2d::Point p);
    void setAgentPixelSize(int i);
    int getAgentPixelSize(void);
    void setEvolutionPointsFreq(int i);
    int getEvolutionPointsFreq(void);
    
    
    void createLevel(void);
    void initializeAttributesCost(void);
    void setAttributesToInitialAgents(void);

    /** start the level and run it until some condition ends it **/
    void playLevel(void);
    void resetLevel(void);

    bool validatePosition(int posx, int posy);

    bool paint = false;
    bool ended = false;
    float calcTime = 0;
    int prevGoal = 0;

private:
    static GameLevel* gamelevelInstance;
    UIGameplayMap* gameplayMap;

    string mapFilename;
    //VECTOR POSITION = TYPE
    vector<int> maxAgents;
    vector<int> numInitialAgents;
    int maxAllAgents = 0;
    int agentPixelSize = 1;

    clock_t currentTime = 0;
    int numLevel;
    LevelsMapVector _agentAttributes;
    LevelsMapVector _agentAttributesInitialConfig;
    LevelsMapVector attributesCost;
    //temporal
    AttributesLevelsMap _attributesLevels;
    vector<Power*> powers;
    vector<list<Agent*> > _agents;
    vector<list<Agent*> > _agentsPool;
    vector<Act*> actions;
    vector<Goal*> goals;
    Agent* _agentsMap[480][320];

    //Resources exploitment
    int timeExploitedMap[480][320] = { { 0 } };
    bool depletedMap[480][320] = { { false } };
    bool adaptedMap[480][320] = { { false } };

    int addedAgents = 0;
    vector<cocos2d::Point> deletedAgents;
    int idCounter = 0;

    unsigned int timeSteps = 0;
    float timeSpeed = 0;
    float timeSpeedBeforePause = 1.265;
    
    int evolutionPoints = 10;
    int evolutionPointsFreq = 2;
    
    LevelState finishedGame = Running;

    int currentAgentType = 0;

    vector<cocos2d::Point> agentDirections;
    vector<vector<pair<int, cocos2d::Point> > > agentFutureDirections;

    GameLevel(){};

    void initializeAgentsPool(void);
    void generateInitialAgents(int type);
    inline void act(void);


};

#endif /* defined(__simulplay__GameLevel__) */
