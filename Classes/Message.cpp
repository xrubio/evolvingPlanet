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


#include "Message.h"
#include "GameLevel.h"

Message::Message( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth ) : _text(text), _pos(xPos, yPos), _lineWidth(lineWidth), _postCondition("tap"), _meetsPostCondition(false)
{
}

Message::~Message()
{
}

void Message::addSpot( const float & centerX, const float & centerY, const float & radius)
{
    Spot spot(centerX, centerY, radius);
    _spots.push_back(spot);
}

void Message::setPostCondition( const std::string & condition )
{
    _postCondition = condition;
    _meetsPostCondition = false;
}

MessageTime::MessageTime( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth, const unsigned int & step) : Message(text, xPos, yPos, lineWidth), _step(step)
{
}

MessageTime::~MessageTime()
{
}

bool MessageTime::meetsPreCondition() const
{
    if(GameLevel::getInstance()->getTimeSteps()==_step)
    {
        return true;
    }
    return false;
}

MessageNext::MessageNext( const std::string & text, const float & xPos, const float & yPos, const float & lineWidth ) : Message(text, xPos, yPos, lineWidth)
{
}

MessageNext::~MessageNext()
{
}

bool MessageNext::meetsPreCondition() const
{
    // meet conditions is always true when you check this message
    // it happens because the previous one is always closed before checking the conditions of the next
    return true;
}

