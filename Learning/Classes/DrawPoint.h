//
//  DrawPoint.h
//  simulplay
//
//  Created by Guillem Laborda on 28/07/14.
//
//

#ifndef __simulplay__DrawPoint__
#define __simulplay__DrawPoint__

#include "cocos2d.h"
#include "vector"

using namespace cocos2d;

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
    //virtual void draw(void);
    virtual void draw(Renderer* renderer, const kmMat4& transform, bool transformUpdated);

    void appendPoint(float x, float y, float r = 1, float g = 1, float b = 1);
    void appendPoint(Point pt, float r = 1, float g = 1, float b = 1);

    std::vector<AgentPoint>* m_points;

    float scale = 1;

private:
    void onDrawPrimitives(const Mat4& transform, bool transformUpdated);
    CustomCommand _customCommand;
};

#endif /* defined(__simulplay__DrawPoint__) */
