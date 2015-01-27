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

class Power {

public:
    Power() {};
    Power(Power const&) {};
    Power& operator=(Power const&);

    string getNameString(void);
    void setNameString(string n);
    int getNameInt(void);
    void setNameInt(int n);
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
    string nameString;
    //0 = ReproductionBoost, 1 = ResistanceBoost, 2 = RecollectionBoost, 3 = RestoreLand
    int nameInt;
    float cooldown;
    float duration;
    float cooldownLeft;
    float durationLeft;
    string attribute;
    string type;
};

#endif /* defined(__simulplay__Power__) */
