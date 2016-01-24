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
//  LevelAchievement.cpp
//  simulplay
//
//  Created by Guillem Laborda on 5/3/15.
//
//

#include "LevelAchievement.h"

LevelAchievement::LevelAchievement(const std::string & icon, const std::string & resource, const std::string & goalType, int level, bool completed, bool occult) : Achievement(icon, resource, goalType, level, completed, occult)
{
}

bool LevelAchievement::checkAchievement(const std::string & typeAch, int level)
{
    if (GameData::getInstance()->getFirstTimeLevelCompleted() == level)
    {
        if (typeAch == "Accomplished")
        {
            if (GameData::getInstance()->getLevelScore(level) > 0)
            {
                _completed = true;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
        if (typeAch == "Perfect")
        {
            if (GameData::getInstance()->getLevelScore(level) == 3)
            {
                _completed = true;
                return true;
            }
        }
        else if (typeAch == "EPsLeft")
        {
            if((GameData::getInstance()->getLevelScore(level) > 0) && (GameLevel::getInstance()->getEvolutionPoints() >= _variable))
            {
                _completed = true;
                return true;
            }
        }
        else if (typeAch == "NoBoosts")
        {
            CCLOG("score %d power used: %d", GameData::getInstance()->getLevelScore(level), GameLevel::getInstance()->getPowersUsed());
            if((GameData::getInstance()->getLevelScore(level) > 0) && (GameLevel::getInstance()->getPowersUsed() == false))
            {
                _completed = true;
                return true;
            }
        }
    
    return false;
}

bool LevelAchievement::checkInGameAchievement( const std::string & typeAch, int level, int agentColorCode)
{
    if (typeAch == "Discovery")
    {
            // as soon as one agent completes the achievement then stop checks
            // achievement is completed if the agent is within the color coded zone for the achievement
            if(agentColorCode == _variable)
            {
                _completed = true;
                return true;
            }
    }
    return false;
}

