
#include "Message.h"
#include "GameLevel.h"

Message::Message( const std::string & text, const float & xPos, const float & yPos, const int & lineWidth ) : _text(text), _pos(xPos, yPos), _lineWidth(lineWidth)
{
}

Message::~Message()
{
}

MessageTime::MessageTime( const std::string & text, const float & xPos, const float & yPos, const int & lineWidth, const unsigned int & step) : Message(text, xPos, yPos, lineWidth), _step(step)
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

MessageNext::MessageNext( const std::string & text, const float & xPos, const float & yPos, const int & lineWidth ) : Message(text, xPos, yPos, lineWidth)
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


