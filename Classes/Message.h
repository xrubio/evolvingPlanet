
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
    /** position of message **/
    Vec2 _pos;
    /** width of line of text **/
    int _lineWidth;
public:
    Message( const std::string & text = "no text", const float & xPos = 0, const float & yPos = 0, const int & lineWidth = 100);
    virtual ~Message();

    const std::string & text() const { return _text; } 
    /** \returns true if conditions to show Message are all green **/
    virtual bool meetConditions() const = 0;

};

/** Message pops up when a given time step has been reached **/
class MessageTime : public Message
{
    unsigned int _step;
public:
    MessageTime( const std::string & text, const float & xPos, const float & yPos, const int & lineWidth, const unsigned int & step);
    virtual ~MessageTime();

    bool meetConditions() const;
};

/** Message pops up when last message has been closed **/
class MessageNext : public Message
{
public:    
    MessageNext( const std::string & text, const float & xPos, const float & yPos, const int & lineWidth);
    virtual ~MessageNext();

    bool meetConditions() const;
};

#endif // __Message_h__

