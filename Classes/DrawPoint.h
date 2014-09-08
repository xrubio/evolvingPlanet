//
//  DrawPoint.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__DrawPoint__
#define __simulplay__DrawPoint__

#include "cocos2d.h"
#include <vector>

using namespace cocos2d;
using namespace std;

typedef struct
    {
    Point pt;
    float r;
    float g;
    float b;
} AgentPoint;

class DrawPoint : public Node {

public:
    static DrawPoint* create();

    DrawPoint();
    ~DrawPoint();
    virtual void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);

    void appendPoint(float x, float y, float r = 1, float g = 1, float b = 1);
    void appendPoint(Point pt, float r = 1, float g = 1, float b = 1);
    void erasePoint(int i);

    vector<AgentPoint>* points;

    float scale = 1;

private:
    void onDrawPrimitives(const Mat4& transform, bool transformUpdated);
    CustomCommand _customCommand;
};

#endif /* defined(__simulplay__DrawPoint__) */
