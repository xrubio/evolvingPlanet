
#ifndef __Message_h__
#define __Message_h__

#include <string>

/** Popup note for the tutorial **/
class Message
{
    std::string _text;
public:
    Message( const std::string & text = "no text");
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
    MessageTime( const std::string & text, const unsigned int & step);
    virtual ~MessageTime();

    bool meetConditions() const;
};

/** Message pops up when last message has been closed **/
class MessageNext : public Message
{
public:    
    MessageNext( const std::string & text);
    virtual ~MessageNext();

    bool meetConditions() const;
};

#endif // __Message_h__

