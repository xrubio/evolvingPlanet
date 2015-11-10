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

#ifndef __Message_h__
#define __Message_h__

#include <string>
#include <math/Vec2.h>
#include <vector>

using namespace cocos2d;

class Spot
{
public:    
    float _centerX;
    float _centerY;
    /** image to show on screen **/
    std::string _image;
    
    Spot( const float & centerX, const float & centerY, const std::string & image ) : _centerX(centerX), _centerY(centerY), _image(image){}
};

/** Popup note for the tutorial **/
class Message
{
private:    
    /** text to show on screen **/
    std::string _text;
    /** position of message (from 0 to 1 of screen) **/
    Vec2 _pos;
    /** percentage of visible size used for width of line of text **/
    float _lineWidth;
    Spot * _spot;
    
    // condition to close the message
    std::string _postCondition;
    // button name to be tapped in order to close the message when _postCondition is "tapButton"
    std::string _postConditionButtonTap;
    // 
    bool _meetsPostCondition;
public:
    Message( const std::string & text = "no text", const float & xPos = 0.5, const float & yPos = 0.5, const float & lineWidth = 0.5);
    virtual ~Message();

    const std::string & text() const {return _text;} 
    const float & lineWidth() const {return _lineWidth;}
    const Vec2 & pos() const {return _pos;}
    void createSpot( const float & centerX, const float & centerY, const std::string & image);
    /** \returns true if conditions to show Message are all achieved**/
    virtual bool meetsPreCondition() const = 0;
    /** \returns true if conditions to close Message are all achieved**/
    bool meetsPostCondition() const{return _meetsPostCondition;}
    void postConditionAchieved() { _meetsPostCondition = true;}

    const Spot * getSpot()const{return _spot;}

    void setPostCondition( const std::string & condition );
    void setPostConditionOk( bool postConditionOk );
    const std::string & getPostCondition() const{return _postCondition;}
    void setPostConditionButtonTap( const std::string & buttonTap ){_postConditionButtonTap = buttonTap;}
    const std::string & getPostConditionButtonTap() const{return _postConditionButtonTap;}
};

/** Message pops up when a given time step has been reached **/
class MessageTime : public Message
{
    unsigned int _step;
public:
    MessageTime( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth, const unsigned int & step);
    virtual ~MessageTime();

    bool meetsPreCondition() const;
};

/** Message pops up when last message has been closed **/
class MessageNext : public Message
{
public:    
    MessageNext( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth);
    virtual ~MessageNext();

    bool meetsPreCondition() const;
};

#endif // __Message_h__

