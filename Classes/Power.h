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
//  Power.h
//  simulplay
//
//  Created by Guillem Laborda on 18/09/14.
//
//

#ifndef __simulplay__Power__
#define __simulplay__Power__

#include <string>

using namespace std;

enum PowerId
{
   ReproductionBoost = 0,
   ResistanceBoost = 1,
   RecollectionBoost = 2,
   RestoreLand = 3,
   InfluenceBoost = 4,
   NoPower = -1
};

class Power
{
    string name;
    PowerId id;

public:
    Power( const std::string & name, const PowerId & id, float c, float dur, const std::string & t, float _cost);
    virtual ~Power(){}

    const string & getName() const;
    const PowerId & getId() const;
    float getCooldown(void) const;
    void setCooldown(float c);
    float getDuration(void) const;
    float getCooldownLeft(void) const;
    void setCooldownLeft(float c);
    float getLastCooldownLeft() const { return _lastCooldownLeft; }
    float getDurationLeft(void) const;
    float getLastDurationLeft() const { return _lastDurationLeft; }
    void setDurationLeft(float d);
    string getType(void);
    void setType(string t);
    float getCost(void);
    void setCost(float c);

    // a power is activated if it has been pressed and is either in effect or in cooldown 
    void activate();
    bool isActivated() const { return _activated; }

protected:
    float _cooldown;
    float _duration;
    float _cooldownLeft;
    float _durationLeft;
    // used to identify transitions from effect to cooldown
    float _lastDurationLeft;
    // used to identify transitions from cooldown to available
    float _lastCooldownLeft;

    string type;
    float cost;
    bool _activated;
    
};

#endif /* defined(__simulplay__Power__) */

