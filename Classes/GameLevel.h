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

class UIGameplayMap;

using namespace std;

class GameLevel {
public:
    static GameLevel* getInstance();

    UIGameplayMap* getUIGameplayMap(void);
    void setUIGameplayMap(UIGameplayMap* gmplmap);
    double getCurrentTime(void);
    void setCurrentTime(double time);
    int getNumLevel(void);
    void setNumLevel(int lvl);
    int getAgentAttribute(string key);
    void setAgentAttribute(string key, int value);
    map<string, int> getAgentAttributes(void);
    void setAgentAttributes(map<string, int> atts);
    vector<Agent*> getAgents(void);
    void setAgents(vector<Agent*> ags);
    void addAgent(Agent* ag);
    void deleteAgent(int i);
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
    int getCooldownPower1(void);
    void setCooldownPower1(int cd);
    int getCooldownPower2(void);
    void setCooldownPower2(int cd);
    int getPower1Active(void);
    void setPower1Active(int cd);
    int getPower2Active(void);
    void setPower2Active(int cd);
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

    clock_t currentTime = 0;
    int numLevel = 0;
    map<string, int> agentAttributes;
    map<string, int> attributesCost;
    map<string, int> powers;
    vector<Agent*> agents;
    vector<Act*> actions;

    int addedAgents = 0;
    vector<int> deletedAgents;
    int idCounter = 0;

    int timeSteps = 0;
    float timeSpeed = 2.5;
    float timeSpeedBeforePause = 2.5;

    int power1Active = 0;
    int power2Active = 0;

    int cooldownPower1 = 0;
    int cooldownPower2 = 0;

    int evolutionPoints = 0;

    //0 = notFinished, 1 = finishedCompleted, 2 = finishedTimeOut, 3 = finished0Agents,
    //4 = finishedBack
    int finishedGame = 0;

    GameLevel() {};
    GameLevel(GameLevel const&) {};
    GameLevel& operator=(GameLevel const&);

    void generateInitialAgents(void);
    void act(void);
};

#endif /* defined(__simulplay__GameLevel__) */
