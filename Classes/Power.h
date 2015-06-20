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
   NoPower = -1
};

class Power
{
    string name;
    PowerId id;

public:
    Power( const std::string & name, const PowerId & id, float c, float dur, float cl, float dl, string attr, string t );
    virtual ~Power(){}

    const string & getName() const;
    const PowerId & getId() const;
    float getCooldown(void);
    void setCooldown(float c);
    float getDuration(void);
    void setDuration(float d);
    float getCooldownLeft(void);
    void setCooldownLeft(float c);
    float getDurationLeft(void);
    void setDurationLeft(float d);
    string getAttribute(void);
    void setAttribute(string attr);
    string getType(void);
    void setType(string t);

protected:
    float cooldown;
    float duration;
    float cooldownLeft;
    float durationLeft;
    string attribute;
    string type;
};

#endif /* defined(__simulplay__Power__) */

