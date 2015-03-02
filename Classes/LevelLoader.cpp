//
//  LevelLoader.cpp
//  simulplay
//
//  Created by Guillem Laborda on 15/09/14.
//
//

#include "LevelLoader.h"
#include "GameLevel.h"
#include "Reproduce.h"
#include "Collect.h"
#include "Deplete.h"
#include "EnvironmentAdaptation.h"
#include "Kill.h"
#include "Die.h"
#include "MultiplierPower.h"
#include "AreaPower.h"
#include "UIGameplayMap.h"
#include "ExpansionGoal.h"
#include "CollectionGoal.h"

#include "cocos2d.h"
#include <iostream>

void LevelLoader::loadXmlFile(string filename)
{

    string dir = "levels/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string ext = ".xml";
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + filename + ext);
    result = doc.load_file((fullPath).c_str());
    //NUM_LEVEL
    GameLevel::getInstance()->setNumLevel(atoi(doc.child_value("NUM_LEVEL")));
    //NAME_LEVEL
    //FILE_MAP
    GameLevel::getInstance()->setMapFilename(doc.child_value("FILE_MAP"));
    //AGENTS
    xml_node ags = doc.child("AGENTS").child("AGENT");
    int maxAllAgents = 0;
    if (strncmp(doc.child("AGENTS").attribute("MAX_ALL").value(), "YES", 3) == 0) {
        maxAllAgents = atoi(doc.child("AGENTS").attribute("MAX_AGENTS").value());
        GameLevel::getInstance()->setMaxAllAgents(maxAllAgents);
    }
    while (ags != nullptr) {
        //TYPE
        int type = atoi(ags.attribute("TYPE").value());
        //NUM_INITIAL_AGENTS
        GameLevel::getInstance()->setNumInitialAgent(type, atoi(ags.child("NUM_INITIAL_AGENTS").child_value()));
        //MAX_AGENTS
        if (maxAllAgents == 0) {
            GameLevel::getInstance()->setMaxAgent(type, atoi(ags.child("MAX_AGENTS").child_value()));
        }
        xml_node atts = ags.child("ATTRIBUTES").child("ATTRIBUTE");

        while (atts != nullptr) {
            GameLevel::getInstance()->setAgentAttribute(type, atts.attribute("NAME").value(), atoi(atts.child("INITIAL_VALUE").child_value()));
            if (type == 0) {
                GameLevel::getInstance()->setAttributesValues(atts.attribute("NAME").value());
            }
            atts = atts.next_sibling("ATTRIBUTE");
        }

        ags = ags.next_sibling("AGENT");
    }

    //POWERS
    xml_node pws = doc.child("POWERS").child("POWER");
    while (pws != nullptr) {
        string nameString = pws.attribute("NAME").value();
        int nameInt = -1;
        //0 = ReproductionBoost, 1 = ResistanceBoost, 2 = RecollectionBoost, 3 = RestoreLand
        if (nameString == "ReproductionBoost") {
            nameInt = 0;
        } else if (nameString == "ResistanceBoost") {
            nameInt = 1;
        } else if (nameString == "RecollectionBoost") {
            nameInt = 2;
        } else if (nameString == "RestoreLand") {
            nameInt = 3;
        }
        int cooldown = atoi(pws.child("COOLDOWN").child_value());
        int duration = atoi(pws.child("DURATION").child_value());
        int durationLeft = 0;
        int cooldownLeft = 0;
        string attribute = pws.child("ATTRIBUTE").child_value();
        string type = pws.child("TYPE").attribute("TYPE_NAME").value();
        if (type == "Multiplier") {
            float multiplier = atof(pws.child("TYPE").child("MULTIPLIER").child_value());
            GameLevel::getInstance()->addPower(new MultiplierPower(nameString, nameInt, cooldown, duration, durationLeft, cooldownLeft, attribute, type, multiplier));
        } else if (type == "Area") {
            float radius = atof(pws.child("TYPE").child("RADIUS").child_value());
            GameLevel::getInstance()->addPower(new AreaPower(nameString, nameInt, cooldown, duration, durationLeft, cooldownLeft, attribute, type, radius));
        }
        pws = pws.next_sibling("POWER");
    }

    //ACTIONS (REPRODUCE ALWAYS BEFORE DIE)
    xml_node acts = doc.child("ACTIONS").child("ACTION");
    while (acts != nullptr) {
        string action = acts.child_value();
        if (action == "Reproduce") {
            GameLevel::getInstance()->addAction(new Reproduce());
        } else if (action == "Collect") {
            GameLevel::getInstance()->addAction(new Collect());
        } else if (action == "Deplete") {
            GameLevel::getInstance()->addAction(new Deplete());
        } else if (action == "EnvironmentAdaptation") {
            GameLevel::getInstance()->addAction(new EnvironmentAdaptation());
        } else if (action == "Kill") {
            GameLevel::getInstance()->addAction(new Kill());
        } else if (action == "Die") {
            GameLevel::getInstance()->addAction(new Die());
        }
        acts = acts.next_sibling("ACTION");
    }

    //GOALS
    int i = 0;
    xml_node goals = doc.child("GOALS").child("GOAL");
    while (goals != nullptr) {
        string type = goals.attribute("TYPE_GOAL").value();
        int agentType = atoi(goals.child("AGENT_TYPE").child_value());
        int minTime = atoi(goals.child("MIN").child_value());
        int maxTime = atoi(goals.child("MAX").child_value());
        int averageTime = atoi(goals.child("AVERAGE").child_value());
        int desviation2Star = atoi(goals.child("DESVIATION_2_STAR").child_value());
        int desviation3Star = atoi(goals.child("DESVIATION_3_STAR").child_value());
        if (type == "Expansion") {
            int color = atoi(goals.child("COLOR_ZONE").child_value());
            GameLevel::getInstance()->addGoal(new ExpansionGoal(agentType, minTime, maxTime, averageTime, desviation2Star, desviation3Star, type, color));
        } else if (type == "Collection") {
            int goalAmount = atoi(goals.child("GOAL_AMOUNT").child_value());
            GameLevel::getInstance()->addGoal(new CollectionGoal(agentType, minTime, maxTime, averageTime, desviation2Star, desviation3Star, type, goalAmount));
        }
        i++;
        goals = goals.next_sibling("GOAL");
    }
}

string LevelLoader::getLevelFileMap(string filename)
{
    string dir = "levels/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string ext = ".xml";
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + filename + ext);
    result = doc.load_file((fullPath).c_str());
    return doc.child_value("FILE_MAP");
}

vector<string> LevelLoader::getGoalTypes(string filename)
{
    string dir = "levels/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string ext = ".xml";
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + filename + ext);
    result = doc.load_file((fullPath).c_str());
    //return doc.child_value("FILE_MAP");
    vector<string> goalTypes;
    xml_node goals = doc.child("GOALS").child("GOAL");
    while (goals != nullptr) {
        string type = goals.attribute("TYPE_GOAL").value();
        if (std::find(goalTypes.begin(), goalTypes.end(), type) == goalTypes.end()) {
            goalTypes.push_back(type);
        }
        goals = goals.next_sibling("GOAL");
    }
    return goalTypes;
}
