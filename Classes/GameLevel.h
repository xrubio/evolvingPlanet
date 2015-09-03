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
    GoalFail = 2,
    NoAgentsLeft = 3,
    UserCancel = 4
};

class GameLevel
{  
public:
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
    int getAgentAttribute(int type, string key);
    void setAgentAttribute(int type, string key, int value);
    map<string, int> getAgentAttributes(int type);
    void setAgentAttributes(vector<map<string, int> > atts);
    vector<map<string, int> > getAgentAllAttributes(void);
    vector<map<string, int> > getAgentAttributesInitialConfig(void);
    void setAgentAttributesInitialConfig(vector<map<string, int> > atts);
    void resetAgentAttributesInitialConfig(void);
    //temporal
    int getAttributesValues(int type, string k, int v);
    vector<map<string, vector<int> > > getAttributesValues(void);
    void setAttributesValues(int type, string k);
    void setAttributesValues(int type, string k, int i, int v);

    vector<Power*> getPowers(void);
    void setPowers(vector<Power*> p);
    void addPower(Power* p);
    void deletePower(int i);
    vector<list<Agent*> > getAgents(void);
    void setAgents(vector<list<Agent*> > ags);
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
    vector<map<string, int> > agentAttributes;
    vector<map<string, int> > agentAttributesInitialConfig;
    vector<map<string, int> > attributesCost;
    //temporal
    vector<map<string, vector<int> > > attributesValues;
    vector<Power*> powers;
    vector<list<Agent*> > agents;
    vector<list<Agent*> > agentsPool;
    vector<Act*> actions;
    vector<Goal*> goals;
    Agent* agentsMap[480][320];

    //Resources exploitment
    int timeExploitedMap[480][320] = { { 0 } };
    bool depletedMap[480][320] = { { false } };
    bool adaptedMap[480][320] = { { false } };

    int addedAgents = 0;
    vector<cocos2d::Point> deletedAgents;
    int idCounter = 0;

    unsigned int timeSteps = 0;
    float timeSpeed = 0;
    float timeSpeedBeforePause = 2.5;

    int evolutionPoints = 10;
    
    LevelState finishedGame = Running;

    int currentAgentType = 0;

    vector<cocos2d::Point> agentDirections;
    vector<vector<pair<unsigned int, cocos2d::Point> > > agentFutureDirections;

    GameLevel(){};

    void initializeAgentsPool(void);
    void generateInitialAgents(int type);
    inline void act(void);


};

#endif /* defined(__simulplay__GameLevel__) */
