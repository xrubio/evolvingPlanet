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


#include "Tutorial.h"
#include "GameLevel.h"
#include "../libs/pugixml/pugixml.hpp"
#include <string>
#include "Message.h"
#include "LocalizedString.h"

Tutorial::Tutorial() : _current(nullptr)
{
}

Tutorial::~Tutorial()
{
    delete _current;
    _current = nullptr;
    
    MessageList::iterator it = _messages.begin();
    while(it!=_messages.end())
    {
        Message * message = *it;
        it = _messages.erase(it);
        delete message;
        message = nullptr;
    }
}

void Tutorial::loadMessagesForLevel(const pugi::xml_node & node)
{
    int numMessages = node.attribute("numMessages").as_int();

    for(int i=1; i<=numMessages; i++)
    {
        pugi::xml_node message = node.child("message");
        while(message!= nullptr)
        {
            int id = message.attribute("id").as_int();
            if(id!=i)
            {
                message = message.next_sibling("message");
                continue;
            }
            std::string trigger = message.attribute("trigger").value();
            float xPos = message.attribute("x").as_float();
            float yPos = message.attribute("y").as_float();
            float lineBreak = message.attribute("lineWidth").as_float();
            std::string text = LocalizedString::create(message.attribute("text").value(), "tutorial");
            Message * newMessage = 0;
            if(trigger=="next")
            { 
                newMessage = new MessageNext(text, xPos, yPos, lineBreak);
            }
            else if(trigger=="time")
            {
                int step = message.attribute("step").as_int();
                newMessage = new MessageTime(text, xPos, yPos, lineBreak, step);
            }
            // post condition to close message (basic behavior is close it if tap)
            if(!message.attribute("closesWhen").empty())
            {
                std::string condition = message.attribute("closesWhen").value();
                newMessage->setPostCondition(condition);
                if(condition=="tapButton")
                {
                    std::string button = message.attribute("button").value();
                    newMessage->setPostConditionButtonTap(button);
                }
            }

            // add spots (if any)
            pugi::xml_node spot = message.child("spot");
            if(spot!= nullptr)
            {
                float centerX = spot.attribute("centerX").as_float();
                float centerY = spot.attribute("centerY").as_float();
                std::string image = spot.attribute("image").value();
                newMessage->createSpot(centerX, centerY, image);
            }

            _messages.push_back(newMessage);
            message = message.next_sibling("message");
        }
    }
}

bool Tutorial::loadTutorial()
{ 
    string dir = "data/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + "tutorial.xml");
    pugi::xml_document doc;
    pugi::xml_parse_result result;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ssize_t tmpSize;
    const char* xmlData = (const char *)FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &tmpSize);
    result = doc.load(xmlData);
#else
    result = doc.load_file(fullPath.c_str());
#endif
    int numLevel = GameLevel::getInstance()->getNumLevel();
    pugi::xml_node level = doc.child("level");
    CCLOG("checking level %i", numLevel);
    while(level!= nullptr)
    {
        int num = stoi(level.attribute("num").value());
        if(numLevel==num)
        {
            CCLOG("level: %i found!", numLevel);
            loadMessagesForLevel(level);
            return true;
        }
        level = level.next_sibling("level");
    }
    // not available tutorial for level
    CCLOG("level %i not found!", numLevel);
    return false;
}

bool Tutorial::checkNextMessage()
{
    if(_current || _messages.size()==0)
    {
        return false;
    }
    Message * nextMessage = *(_messages.begin());
    if(nextMessage->meetsPreCondition())
    {
        _current = nextMessage;
        _messages.pop_front();
        return true;
    }
    return false;
}

bool Tutorial::isFinished() const
{
    if(_messages.size()==0)
    {
        return true;
    }
    return false;
}

void Tutorial::removeCurrentMessage()
{
    if(!_current)
    {
        return;
    }
    delete _current;
    _current = 0;
}

