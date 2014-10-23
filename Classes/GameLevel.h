//
//  GameLevel.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__GameLevel__
#define __simulplay__GameLevel__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Agent.h"
#include "Act.h"
#include "Power.h"
#include "Goal.h"
#include "cocos2d.h"

class UIGameplayMap;

using namespace std;

class GameLevel {
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
    int getAgentAttribute(string key);
    void setAgentAttribute(string key, int value);
    map<string, int> getAgentAttributes(void);
    void setAgentAttributes(map<string, int> atts);
    vector<Power*> getPowers(void);
    void setPowers(vector<Power*> p);
    void addPower(Power* p);
    void deletePower(int i);
    vector<vector<Agent*> > getAgents(void);
    void setAgents(vector<vector<Agent*> > ags);
    void addAgent(Agent* ag);
    void deleteAgent(int type, int i);
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
    int getFinishedGame(void);
    void setFinishedGame(int f);
    int getTimeSteps(void);
    void setTimeSteps(int steps);
    float getTimeSpeed(void);
    void setTimeSpeed(float speed);
    float getTimeSpeedBeforePause(void);
    void setTimeSpeedBeforePause(float speed);
    int getEvolutionPoints(void);
    void setEvolutionPoints(int points);
    int getAttributeCost(string key);
    void setAttributeCost(string key, int val);
    int getTimeExploited(int x, int y);
    void setTimeExploited(int x, int y, int val);
    bool getDepleted(int x, int y);
    void setDepleted(int x, int y, bool val);

    void createLevel(void);
    void initializeAttributesCost(void);
    void playLevel(void);
    void resetLevel(void);

    bool validatePosition(int posx, int posy);

    bool paint = false;

private:
    static GameLevel* gamelevelInstance;
    UIGameplayMap* gameplayMap;

    string mapFilename;
    //VECTOR POSITION = TYPE
    vector<int> maxAgents;
    vector<int> numInitialAgents;

    clock_t currentTime = 0;
    int numLevel;
    map<string, int> agentAttributes;
    map<string, int> attributesCost;
    vector<Power*> powers;
    vector<vector<Agent*> > agents;
    vector<Act*> actions;
    vector<Goal*> goals;
    Agent* agentsMap[480][320];

    //Resources exploitment
    int timeExploitedMap[480][320] = { 0 };
    bool depletedMap[480][320] = { false };

    int addedAgents = 0;
    vector<cocos2d::Point> deletedAgents;
    int idCounter = 0;

    int timeSteps = 0;
    float timeSpeed = 2.5;
    float timeSpeedBeforePause = 2.5;

    int evolutionPoints = 0;

    //0 = notFinished, 1 = finishedCompleted, 2 = finishedTimeOut (goal no completed), 3 = finished0Agents,
    //4 = finishedBack
    int finishedGame = 0;

    GameLevel() {};
    GameLevel(GameLevel const&) {};
    GameLevel& operator=(GameLevel const&);

    void generateInitialAgents(int type);
    void act(void);
};

#endif /* defined(__simulplay__GameLevel__) */
