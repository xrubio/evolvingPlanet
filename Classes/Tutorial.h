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

#ifndef __Tutorial_h__
#define __Tutorial_h__

#include <list>

namespace pugi
{
    class xml_node;
}

class Message;

/** This class structures the information that needs to be supplied to GameLevel to provide information to the user **/
class Tutorial
{
    typedef std::list<Message*> MessageList;
    std::list<Message*> _messages;
    Message * _current;

    // create _messages from the level located at node 
    void loadMessagesForLevel(const pugi::xml_node & node);
public:
    Tutorial();
    virtual ~Tutorial();
    /** fill the class using an xml file
      \returns true if the level has an available tutorial, false otherwise
      */
    bool loadTutorial();
    // return true if no more messages need to be shown
    bool isFinished() const;

    /** check if next message on list can be shown. \returns true if it's possible **/
    bool checkNextMessage();
    void removeCurrentMessage();
    void removeAllMessages();
    Message * getCurrentMessage() {return _current;}
};

#endif // __Tutorial_h__

