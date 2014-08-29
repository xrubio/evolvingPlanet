//
//  DrawPoint.cpp
//  simulplay
//
//  Created by Guillem Laborda on 28/07/14.
//
//

#include "DrawPoint.h"

DrawPoint* DrawPoint::create()
{
    DrawPoint* draw = new DrawPoint();
    draw->autorelease();
    return draw;
}

DrawPoint::DrawPoint()
{
    m_points = new std::vector<AgentPoint>;
}

DrawPoint::~DrawPoint()
{
    delete m_points;
}

void DrawPoint::onDrawPrimitives(const Mat4& transform, bool transformUpdated)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);

    int c = m_points->size();
    for (int i = 0; i < c; i++) {
        AgentPoint pt = m_points->at(i);
        //glColor4f(pt.r, pt.g, pt.b, 1);
        DrawPrimitives::setDrawColor4F(pt.r, pt.g, pt.b, 1);
        //ccDrawPoint(pt.pt);
        DrawPrimitives::drawSolidCircle(pt.pt, 4, 0, 16, 1, 1);
        //DrawPrimitives::drawPoint(pt.pt);
    }
}

void DrawPoint::draw(Renderer* renderer, const kmMat4& transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder + 1);
    _customCommand.func = CC_CALLBACK_0(DrawPoint::onDrawPrimitives, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
    CCLOG("Draw");

    /*int c = m_points->size();
    for (int i = 0; i < c; i++) {
        AgentPoint pt = m_points->at(i);
        //glColor4f(pt.r, pt.g, pt.b, 1);
        DrawPrimitives::setDrawColor4F(pt.r, pt.g, pt.b, 0);
        //ccDrawPoint(pt.pt);
        DrawPrimitives::drawPoint(pt.pt);
        ccDrawPoint(pt.pt);
        DrawPrimitives::drawLine(Point(0, 0), Point(30, 35));
    }
    DrawPrimitives::drawCircle(Point(250, 250), 10, 60, 3, false, 2, 1);*/
}

void DrawPoint::appendPoint(float x, float y, float r, float g, float b)
{
    appendPoint(cocos2d::ccp(x, y), r, g, b);
}

void DrawPoint::appendPoint(Point pt, float r, float g, float b)
{
    AgentPoint dp;
    dp.pt = pt;
    dp.r = r;
    dp.g = g;
    dp.b = b;
    m_points->push_back(dp);
}