//
//  DrawPoint.cpp
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#include "DrawPoint.h"

DrawPoint* DrawPoint::create()
{
    auto draw = new DrawPoint();
    return draw;
}

DrawPoint::DrawPoint()
{
    points = new std::vector<AgentPoint>;
}

DrawPoint::~DrawPoint()
{
    delete points;
}

void DrawPoint::onDrawPrimitives(const Mat4& transform, bool transformUpdated)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);

    unsigned long c = points->size();
    for (int i = 0; i < c; i++) {
        AgentPoint pt = points->at(i);
        //glColor4f(pt.r, pt.g, pt.b, 1);
        DrawPrimitives::setDrawColor4F(pt.r, pt.g, pt.b, 0.2);
        //ccDrawPoint(pt.pt);
        DrawPrimitives::drawSolidCircle(pt.pt, 320, 0, 64, 1, 1);
        //DrawPrimitives::drawPoint(pt.pt);
    }
}

void DrawPoint::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder + 1);
    _customCommand.func = CC_CALLBACK_0(DrawPoint::onDrawPrimitives, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void DrawPoint::appendPoint(float x, float y, float r, float g, float b)
{
    appendPoint(Vec2(x, y), r, g, b);
}

void DrawPoint::appendPoint(Point pt, float r, float g, float b)
{
    AgentPoint dp;
    dp.pt = pt;
    dp.r = r;
    dp.g = g;
    dp.b = b;
    points->push_back(dp);
}

void DrawPoint::erasePoint(int i)
{
    if (i < points->size()) {
        points->erase(points->begin() + i);
    }
}