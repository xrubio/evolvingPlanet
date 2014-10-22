//
//  Agents.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__Agent__
#define __simulplay__Agent__

#include "Position.h"
#include <map>
#include <string>

using namespace std;

class Agent {

public:
    Agent() {};
    Agent(int i, int lf, int t, int posx, int posy);
    Agent(Agent const&) {};
    Agent& operator=(Agent const&);
    ~Agent() {};

    int getId(void);
    int getLife(void);
    void setLife(int lf);
    int getType(void);
    void setType(int t);
    Position* getPosition(void);
    void setPosition(int posx, int posy);
    map<string, int> getAttributes(void);
    void setAttributes(map<string, int> map);
    int getValOfAttribute(string att);
    void setValOfAttribute(string att, int val);

private:
    int id;
    int life;
    int type;
    Position* position;
    map<string, int> attributes;
};

#endif /* defined(__simulplay__Agent__) */
