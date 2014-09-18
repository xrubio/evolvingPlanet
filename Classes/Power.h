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

    string getName(void);
    void setName(string n);
    int getCooldown(void);
    void setCooldown(int c);
    int getDuration(void);
    void setDuration(int d);
    int getCooldownLeft(void);
    void setCooldownLeft(int c);
    int getDurationLeft(void);
    void setDurationLeft(int d);
    string getAttribute(void);
    void setAttribute(string attr);
    string getType(void);
    void setType(string t);

protected:
    string name;
    int cooldown;
    int duration;
    int cooldownLeft;
    int durationLeft;
    string attribute;
    string type;
};

#endif /* defined(__simulplay__Power__) */
