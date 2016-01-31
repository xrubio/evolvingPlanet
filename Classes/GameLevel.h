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
    eTrade = 7,
    eTerraform = 8
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

    UIGameplayMap * getUIGameplayMap(void);
    void setUIGameplayMap(UIGameplayMap* gmplmap);
    const std::string & getMapFilename(void) const;
    void setMapFilename(const std::string & filename);
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

    const std::vector<Power*> & getPowers(void) const;
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

    void play( bool play ){ _play = play; }
    bool isPlaying() const { return _play; }

    int getEvolutionPoints(void);
    void setEvolutionPoints(int points);
    int getAttributeCost(int type, int key);
    void setAttributeCost(int type, int key, int val);
    bool getDepleted(int x, int y);
    void setDepletedMap(int x, int y, bool val);
    void addDepletedToVector(int x, int y);
    const std::vector<cocos2d::Point> & getNewDepletedVector(void) const { return _newDepletedVector; }
    void clearNewDepletedVector() { _newDepletedVector.clear(); }
    const std::vector<cocos2d::Point> & getRestored(void) const;
    bool getTerraformed(int x, int y);
    void setTerraformed(int x, int y, bool val);
    const std::vector<cocos2d::Point> & getTerraformedVector(void) const;
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
    bool getPowersUsed(void) const;
    // notify that some power has been used
    void powerUsed();
    int getRegenerationRate(void);
    void setRegenerationRate(int r);
    void setTerraformFactor(float t);

    
    vector<string> getCompletedAchievements(void);
    
    void createLevel(void);
    void setAttributesToInitialAgents(void);
    void setNumAgentTypes(size_t numAgents);

    /** start the level and run it until some condition ends it **/
    void playLevel(void);
    void resetLevel(void);

    // check if posx/posy are within map boundaries
    bool isInsideMap(int posx, int posy) const;
    // check if pos/posy is inside map, in land and empty of agents
    bool validatePosition(int posx, int posy) const;
    
    int convertAttStringToInt(const string & s);
    string convertAttIntToString(int i);

    bool paint = false;
    bool ended = false;

    void setModifiableAttr(int order, int attr) { _modifiableAtt.at(order) = attr; }
    const Levels & getModifiableAttr() const { return _modifiableAtt; }
    // returns a Rect based on center and distance after checking for boundaries
    cocos2d::Rect getArea(const Position & center, int mobility) const;
    
    void clearRestored(void);
    void clearTerraformedVector(void);
    
    // add name to legendNames and color to legendColors to display as mapInfo legend
    void addEntryToLegend( const std::string & name, const cocos2d::Color4B & color) { _legendNames.push_back(name); _legendColors.push_back(color); }
    size_t getLegendSize() const { return _legendNames.size(); }
    const std::string & getLegendName(int index) const { return _legendNames.at(index); }
    const cocos2d::Color4B & getLegendColor(int index) const { return _legendColors.at(index); }
 
    // checks if a Power type is currently in effect
    bool powerIsInEffect( const PowerType & type ) const;
    // checks if an AreaPower type is in effect in pos
    bool powerIsInRadius( const PowerType & type, const Position & pos ) const;

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
    
    // get legend names and colors for displaying legend of mapInfo
    std::vector<std::string> _legendNames;
    std::vector<cocos2d::Color4B> _legendColors;
    
    //completed achievements to show (key words corresponding to title in achievement_lang)
    vector<string> completedAchievements;

    //Resources exploitment
    bool depletedMap[480][320] = { { false } };
    std::vector<cocos2d::Point> _depletedVector;
    // store cells depleted during this step
    std::vector<cocos2d::Point> _newDepletedVector;
    std::vector<cocos2d::Point> _restoredVector;
    bool _terraformedMap[480][320] = { { false } };
    std::vector<cocos2d::Point> _terraformedVector;

    std::vector<cocos2d::Point> deletedAgents;
    int idCounter = 0;

    // true if level is playing, false if not started or in pause
    bool _play;
    
    int evolutionPoints = 10;
    int evolutionPointsFreq = 2;
   
    // state of the game
    LevelState _finishedGame = Running;

    int currentAgentType = 0;

    std::vector<cocos2d::Point> _agentDirections;
    std::vector<std::vector<pair<int, cocos2d::Point> > > _agentFutureDirections;
    
    Achievement* _inGameAchievement = nullptr;
    bool _powersUsed;

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
    // last step of the mission
    int _lastStep;
    // harm reduction factor
    float _terraformFactor = 0;
 
};

#endif /* defined(__simulplay__GameLevel__) */
