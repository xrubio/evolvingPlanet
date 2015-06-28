
#ifndef __Message_h__
#define __Message_h__

#include <string>
#include <math/Vec2.h>

using namespace cocos2d;

/** Popup note for the tutorial **/
class Message
{
    /** text so show on screen **/
    std::string _text;
    /** position of message (from 0 to 1 of screen) **/
    Vec2 _pos;
    /** percentage of visible size used for width of line of text **/
    float _lineWidth;
public:
    Message( const std::string & text = "no text", const float & xPos = 0.5, const float & yPos = 0.5, const float & lineWidth = 0.5);
    virtual ~Message();

    const std::string & text() const {return _text;} 
    const float & lineWidth() const {return _lineWidth;}
    const Vec2 & pos() const {return _pos;}
    /** \returns true if conditions to show Message are all green **/
    virtual bool meetConditions() const = 0;

};

/** Message pops up when a given time step has been reached **/
class MessageTime : public Message
{
    unsigned int _step;
public:
    MessageTime( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth, const unsigned int & step);
    virtual ~MessageTime();

    bool meetConditions() const;
};

/** Message pops up when last message has been closed **/
class MessageNext : public Message
{
public:    
    MessageNext( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth);
    virtual ~MessageNext();

    bool meetConditions() const;
};

#endif // __Message_h__

