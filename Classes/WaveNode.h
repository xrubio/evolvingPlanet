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
//  WaveNode.h
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 21/7/15.
//
//

#ifndef __EvolvingPlanet__WaveNode__
#define __EvolvingPlanet__WaveNode__

#include "cocos2d.h"

using namespace cocos2d;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} WavePoint;

typedef enum tDrawMode {
    kDrawTriangleStrip,
    kDrawTriangleFan,
    kDrawPoints,
    kDrawLines,
    
}tDrawMode;

class WaveNode : public Node {
public:
    
    virtual bool init(int size);
    
    WaveNode(){};
    WavePoint h3px(float x, float y, float z);
    void addToDynamicVerts2D(Point vert, Color4B color);
    void addToDynamicVerts3D(WavePoint vert, Color4B color);
    void setReadyToDrawDynamicVerts(bool isReadyToDraw);
    void clearDynamicDrawArray();
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    void onDrawPrimitives(const Mat4 &transform, bool transformUpdated);
    
//private:
    
    bool shouldDrawDynamicVerts;
    
    // Dynamic Verts
    WavePoint* dynamicVerts;
    
    // Color of each vert
    Color4B* dynamicVertColors;
    
    int dynamicVertCount;
    int dynamicVertIndex;
    
    tDrawMode glDrawMode;
    
    CustomCommand _customCommand;
    
};

#endif /* defined(__EvolvingPlanet__WaveNode__) */
