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
