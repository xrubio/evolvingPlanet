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
#include "Die.h"
#include "MultiplierPower.h"
#include "AreaPower.h"
#include "UIGameplayMap.h"

#include "cocos2d.h"
#include <iostream>

void LevelLoader::loadXmlFile(string filename)
{
    string ext = ".xml";
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename + ext);
    result = doc.load_file((fullPath).c_str());
    //NUM_LEVEL
    //NAME_LEVEL
    //FILE_MAP
    GameLevel::getInstance()->setMapFilename(doc.child_value("FILE_MAP"));
    //INITIAL_ZONE

    //AGENTS
    //MAX_AGENTS
    GameLevel::getInstance()->setMaxAgents(atoi(doc.child("AGENTS").child_value("MAX_AGENTS")));
    //LIFE
    //GameLevel::getInstance()->setMaxAgents(atoi(doc.child("AGENTS").child_value("LIFE")));

    //ATTRIBUTES
    xml_node atts = doc.child("ATTRIBUTES").child("ATTRIBUTE");
    while (atts != nullptr) {
        GameLevel::getInstance()->setAgentAttribute(atts.attribute("NAME").value(), 0);
        atts = atts.next_sibling("ATTRIBUTE");
    }

    //POWERS
    xml_node pws = doc.child("POWERS").child("POWER");
    while (pws != nullptr) {
        string name = pws.attribute("NAME").value();
        int cooldown = atoi(pws.child("COOLDOWN").child_value());
        int duration = atoi(pws.child("DURATION").child_value());
        int durationLeft = 0;
        int cooldownLeft = 0;
        string attribute = pws.child("ATTRIBUTE").child_value();
        string type = pws.child("TYPE").attribute("TYPE_NAME").value();
        if (type == "Multiplier") {
            float multiplier = atof(pws.child("TYPE").child("MULTIPLIER").child_value());
            GameLevel::getInstance()->addPower(new MultiplierPower(name, cooldown, duration, durationLeft, cooldownLeft, attribute, type, multiplier));
        } else if (type == "Area") {
            float radius = atof(pws.child("TYPE").child("RADIUS").child_value());
            GameLevel::getInstance()->addPower(new AreaPower(name, cooldown, duration, durationLeft, cooldownLeft, attribute, type, radius));
        }
        pws = pws.next_sibling("POWER");
    }

    //ACTIONS (REPRODUCE ALWAYS BEFORE DIE)
    xml_node acts = doc.child("ACTIONS").child("ACTION");
    while (acts != nullptr) {
        string action = acts.child_value();
        if (action == "Reproduce") {
            GameLevel::getInstance()->addAction(new Reproduce());
        } else if (action == "Die") {
            GameLevel::getInstance()->addAction(new Die());
        }
        acts = acts.next_sibling("ACTION");
    }
    
    //GOALS
    int i = 0;
    string checkpoint = "CHECKPOINT_";
    xml_node goals = doc.child("GOALS").child((checkpoint+to_string(i+1)).c_str());
    while (goals != nullptr) {
        int minTime = atoi(goals.child("MIN").child_value());
        int maxTime = atoi(goals.child("MAX").child_value());;
        int averageTime = atoi(goals.child("AVERAGE").child_value());;
        GameLevel::getInstance()->addGoal(new Goal(minTime,maxTime,averageTime));
        i++;
        goals = goals.next_sibling((checkpoint+to_string(i+1)).c_str());
    }
}
