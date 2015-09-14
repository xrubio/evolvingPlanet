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
//  WaveNode.cpp
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 21/7/15.
//
//

#include "WaveNode.h"


#define kVertCreationBlockSize 100

bool WaveNode::init(int size){
    
    if (!Node::init()) {
        return false;
    }
    
    dynamicVertCount = 0;
    dynamicVertIndex = 0;
    
    dynamicVerts = nullptr;
    
    // Must define what shader program OpenGL ES 2.0 should use.
    // The instance variable shaderProgram exists in the CCNode class in Cocos2d 2.0.
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    //self.shaderProgram = [[CCShaderCache sharedShaderCache] programForKey:kCCShader_PositionColor];
    CCLOG("SET");
    glDrawMode = kDrawTriangleStrip; // Default draw mode for this class.
    dynamicVertColors = new Color4B[size];
    
    return true;
}

void WaveNode::setReadyToDrawDynamicVerts(bool shouldDraw) {
    shouldDrawDynamicVerts = shouldDraw;
}

// Called to release the memory of the dynamic verts and reset this class to its default state.
void WaveNode::clearDynamicDrawArray(void) {
    shouldDrawDynamicVerts = false;
    
    if (dynamicVerts != nullptr) {
        free(dynamicVerts);
        free(dynamicVertColors);
        dynamicVerts = nullptr;
        dynamicVertColors = nullptr;
        dynamicVertCount = 0;
        dynamicVertIndex = 0;
    }
}

// Adds a vertex point with the zVertex p.z set to zero and assignes its color.
void WaveNode::addToDynamicVerts2D(Point vert, Color4B color) {
    WavePoint p;
    p.x = vert.x;
    p.y = vert.y;
    p.z = 0;
    addToDynamicVerts3D(p, color);
}

// Adds a 3D vertex point to the dynamicVerts array and the color of that vert to the dynaicVertColors array.
void WaveNode::addToDynamicVerts3D(WavePoint vert, Color4B vertexColor) {
    
    // Create vertex blocks in sizes of 100 so that memory allocation only needs to
    // be done 1/kVertCreationBlockSize times as often as the verts are added.
    
    int remainder = dynamicVertCount % kVertCreationBlockSize;
    int vertBlockCount = dynamicVertCount / kVertCreationBlockSize + 1;
    if (remainder == 0) {
        dynamicVerts = (WavePoint*)realloc(dynamicVerts, sizeof(WavePoint) * kVertCreationBlockSize * vertBlockCount);
        //dynamicVertColors = (Color4B*)realloc(dynamicVertColors, sizeof(Color4B) * kVertCreationBlockSize * vertBlockCount);
    }
    
    // Increment so that the index always points to what will be the next added vert/color pair.
    dynamicVertColors[dynamicVertIndex] = vertexColor;
    dynamicVerts[dynamicVertIndex++] = vert;
    
    //  NSLog(@"created vert:(%.2f,%.2f,%.2f) withColor:r:%d,g:%d,b:%d,a:%d", vert.x, vert.y, vert.z,
    //  vertexColor.r, vertexColor.b, vertexColor.b, vertexColor.a);
    
    dynamicVertCount = dynamicVertIndex;
}

void WaveNode::draw(Renderer* renderer, const Mat4& transform, uint32_t flags) {
    
    _customCommand.init(1);
    _customCommand.func = CC_CALLBACK_0(WaveNode::onDrawPrimitives, this, transform, false);
    renderer->addCommand(&_customCommand);
}

void WaveNode::onDrawPrimitives(const Mat4 &transform, bool transformUpdated)
{
    // Only draw if this class has the verticies and colors to be drawn setup and ready to be drawn.
    if (shouldDrawDynamicVerts == true) {
        glLineWidth(4);
        
        //ccGLEnableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR );
        glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
        
        // Tell OpenGL ES 2.0 to use the shader program assigned in the init of this node.
        this->getGLProgram()->use();
        this->getGLProgram()->setUniformsForBuiltins();
        
        //[self.shaderProgram setUniformForModelViewProjectionMatrix];
        
        // Pass the verticies to draw to OpenGL
        glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, dynamicVerts);
        
        // Pass the colors of the vertices to draw to OpenGL
        glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, dynamicVertColors);
        
        // Choose which draw mode to use.
        switch (glDrawMode) {
            case kDrawTriangleStrip:
                glDrawArrays(GL_TRIANGLE_STRIP, 0, dynamicVertCount);
                break;
                
            case kDrawLines:
                glDrawArrays(GL_LINE_STRIP, 0, dynamicVertCount);
                break;
                
            case kDrawPoints:
                glDrawArrays(GL_POINTS, 0, dynamicVertCount);
                break;
                
            case kDrawTriangleFan:
                glDrawArrays(GL_TRIANGLE_FAN, 0, dynamicVertCount);
                break;
                
            default:
                glDrawArrays(GL_TRIANGLE_STRIP, 0, dynamicVertCount);
                break;
        }
    }
}

// Static method to generate a 3d vertex.
WavePoint WaveNode::h3px(float x, float y, float z){
    WavePoint p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}
