
#include "Message.h"
#include "GameLevel.h"

Message::Message( const std::string & text ) : _text(text)
{
}

Message::~Message()
{
}

MessageTime::MessageTime( const std::string & text, const unsigned int & step) : Message(text), _step(step)
{
}

MessageTime::~MessageTime()
{
}

bool MessageTime::meetConditions() const
{
    if(GameLevel::getInstance()->getTimeSteps()==_step)
    {
        return true;
    }
    return false;
}

MessageNext::MessageNext( const std::string & text) : Message(text)
{
}

MessageNext::~MessageNext()
{
}

bool MessageNext::meetConditions() const
{
    // meet conditions is always true when you check this message
    // it happens because the previous one is always closed before checking the conditions of the next
    return true;
}


