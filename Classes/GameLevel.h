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

class UIGameplayMap;

using namespace std;

class GameLevel {
public:
    static GameLevel* getInstance();

    UIGameplayMap* getUIGameplayMap(void);
    void setUIGameplayMap(UIGameplayMap* gmplmap);
    string getMapFilename(void);
    void setMapFilename(string filename);
    int getMaxAgents(void);
    void setMaxAgents(int max);
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
    vector<Agent*> getAgents(void);
    void setAgents(vector<Agent*> ags);
    void addAgent(Agent* ag);
    void deleteAgent(int i);
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
    vector<int> getDeletedAgents(void);
    void setDeletedAgents(vector<int> v);
    void addDeletedAgent(int id);
    void deleteDeletedAgent(int id);
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

    void createLevel(int lvl);
    void initializeAttributesCost(void);
    void playLevel(void);
    void resetLevel(void);

    bool validatePosition(int posx, int posy);

    bool paint = false;

private:
    static GameLevel* gamelevelInstance;
    UIGameplayMap* gameplayMap;

    string mapFilename;
    int maxAgents;

    clock_t currentTime = 0;
    int numLevel;
    map<string, int> agentAttributes;
    map<string, int> attributesCost;
    vector<Power*> powers;
    vector<Agent*> agents;
    vector<Act*> actions;
    vector<Goal*> goals;

    int addedAgents = 0;
    vector<int> deletedAgents;
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

    void generateInitialAgents(void);
    void act(void);
    void checkGoals(void);
};

#endif /* defined(__simulplay__GameLevel__) */
