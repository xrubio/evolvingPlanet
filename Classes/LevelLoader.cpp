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
#include "AreaPower.h"
#include "UIGameplayMap.h"
#include "ExpansionGoal.h"
#include "CollectionGoal.h"
#include "Power.h"

#include "cocos2d.h"

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
    //AGENTS_PIXEL_SIZE
    GameLevel::getInstance()->setAgentPixelSize(atoi(doc.child_value("AGENTS_PIXEL_SIZE")));
    //EVPOINTS_FREQ
    GameLevel::getInstance()->setEvolutionPointsFreq(atoi(doc.child_value("EVPOINTS_FREQ")));
    //ATTRIBUTES_CONFIG
    xml_node attsConfig = doc.child("ATTRIBUTES_CONFIG").child("ATTRIBUTE");
    while (attsConfig != nullptr)
    {
        string k = attsConfig.attribute("NAME").value();
        std::stringstream test(attsConfig.child_value("VALUES"));
        std::string segment;
        
        int i = 0;
        int attrKey = GameLevel::getInstance()->convertAttStringToInt(k);
        while(std::getline(test, segment, ','))
        {
            GameLevel::getInstance()->setValueAtLevel(attrKey, i, atof(segment.c_str()));
            i++;
        }

       attsConfig = attsConfig.next_sibling("ATTRIBUTE");
    }
    //AGENTS
    int numAgents = doc.child("AGENTS").attribute("NUMTYPES").as_int();

    int maxAllAgents = 0;
    if (strncmp(doc.child("AGENTS").attribute("MAX_ALL").value(), "YES", 3) == 0) {
        maxAllAgents = atoi(doc.child("AGENTS").attribute("MAX_AGENTS").value());
        GameLevel::getInstance()->setMaxAllAgents(maxAllAgents);
    }

    GameLevel::getInstance()->setNumAgentTypes(numAgents);
    for(int i=0; i<numAgents; i++)
    {
        std::stringstream strType;
        strType << i;
        xml_node ags = doc.child("AGENTS").find_child_by_attribute("AGENT", "TYPE", strType.str().c_str());
        if(!ags)
        {
            CCLOG("type: %s not found!!!", strType.str().c_str());
        }
        //NUM_INITIAL_AGENTS
        GameLevel::getInstance()->setNumInitialAgent(i, atoi(ags.child("NUM_INITIAL_AGENTS").child_value()));
        //MAX_AGENTS
        if (maxAllAgents == 0) {
            GameLevel::getInstance()->setMaxAgent(i, atoi(ags.child("MAX_AGENTS").child_value()));
        }
        //ATTRIBUTES
        xml_node atts = ags.child("ATTRIBUTES").child("ATTRIBUTE");

        while (atts != nullptr)
        {
            int attrType = GameLevel::getInstance()->convertAttStringToInt(atts.attribute("NAME").value());
            GameLevel::getInstance()->setAgentAttribute(i, attrType, atoi(atts.child("INITIAL_VALUE").child_value()));
            //temporal
            /*if (type == 0) {
                GameLevel::getInstance()->setAttributesValues(type, atts.attribute("NAME").value());
            }*/
            xml_attribute guiOrder= atts.attribute("GUI");
            if(guiOrder)
            {
                GameLevel::getInstance()->setModifiableAttr(guiOrder.as_int(), attrType);
            }
            atts = atts.next_sibling("ATTRIBUTE");
        }
        //DIRECTIONS
        xml_node drs = ags.child("DIRECTIONS").child("DIRECTION");

        while (drs != nullptr) {
            GameLevel::getInstance()->setAgentFutureDirection(i, atoi(drs.child("STEP").child_value()),
                cocos2d::Point(atoi(drs.child("POSITION").attribute("X").value()),
                                                                  atoi(drs.child("POSITION").attribute("Y").value())));

            drs = drs.next_sibling("DIRECTION");
        }

        ags = ags.next_sibling("AGENT");
    }

    //POWERS
    xml_node pws = doc.child("POWERS").child("POWER");
    while (pws != nullptr) {
        string nameString = pws.attribute("NAME").value();
        PowerId id = NoPower;
        if (nameString == "ReproductionBoost")
        {
            id = ReproductionBoost;
        }
        else if (nameString == "ResistanceBoost")
        {
            id = ResistanceBoost;
        }
        else if (nameString == "RecollectionBoost")
        {
            id = RecollectionBoost;
        }
        else if (nameString == "RestoreLand")
        {
            id = RestoreLand;
        }
        float cost = atoi(pws.child("COST").child_value());
        int cooldown = atoi(pws.child("COOLDOWN").child_value());
        int duration = atoi(pws.child("DURATION").child_value());
        int durationLeft = 0;
        int cooldownLeft = 0;
        string attribute = pws.child("ATTRIBUTE").child_value();
        string type = pws.child("TYPE").attribute("TYPE_NAME").value();
        if (type == "Global") {
            GameLevel::getInstance()->addPower(new Power(nameString, id, cooldown, duration, durationLeft, cooldownLeft, attribute, type, cost));
        }
        else if (type == "Area") {
            float radius = atof(pws.child("TYPE").child("RADIUS").child_value());
            GameLevel::getInstance()->addPower(new AreaPower(nameString, id, cooldown, duration, durationLeft, cooldownLeft, attribute, type, cost, radius));
        }
        pws = pws.next_sibling("POWER");
    }

    //ACTIONS (REPRODUCE ALWAYS BEFORE DIE)
    xml_node acts = doc.child("ACTIONS").child("ACTION");
    while (acts != nullptr) {
        string action = acts.child_value();
        if (action == "Reproduce") {
            GameLevel::getInstance()->addAction(new Reproduce());
        }
        else if (action == "Collect") {
            GameLevel::getInstance()->addAction(new Collect());
        }
        else if (action == "Deplete") {
            GameLevel::getInstance()->addAction(new Deplete());
        }
        else if (action == "EnvironmentAdaptation") {
            GameLevel::getInstance()->addAction(new EnvironmentAdaptation());
        }
        else if (action == "Kill") {
            GameLevel::getInstance()->addAction(new Kill());
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
            GameLevel::getInstance()->addGoal(new ExpansionGoal(agentType, minTime, maxTime, averageTime, desviation2Star, desviation3Star, color));
        }
        else if (type == "Collection") {
            int goalAmount = atoi(goals.child("GOAL_AMOUNT").child_value());
            GameLevel::getInstance()->addGoal(new CollectionGoal(agentType, minTime, maxTime, averageTime, desviation2Star, desviation3Star, goalAmount));
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
