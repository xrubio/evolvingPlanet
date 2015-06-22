
#include "Tutorial.h"
#include "GameLevel.h"
#include "../libs/pugixml/pugixml.hpp"
#include <string>
#include "Message.h"

Tutorial::Tutorial() : _current(0)
{
}

Tutorial::~Tutorial()
{
    if(_current)
    {
        delete _current;
    }
    MessageList::iterator it = _messages.begin();
    while(it!=_messages.end())
    {
        Message * message = *it;
        it = _messages.erase(it);
        delete message;
    }
}

void Tutorial::dummyLoad()
{
    std::string messageStr1("Welcome to the Console. I hope you like it because you'll spend the next weeks looking at it! The satellites we deployed around the planet is giving you real-time updates on the position and state of your simulated species.");
    Message * message1 = new MessageTime(messageStr1, 0);
    std::string messageStr2("The Console shows you a map of the area where the agents have been deployed.");
    Message * message2 = new MessageNext(messageStr2);
    std::string messageStr3("These zones define mountains and arid areas. These regions are difficult to live, so your agents will die faster here.");
    // TODO ZoneAreaLevel a una zona del mapa
    Message * message3 = new MessageNext(messageStr3);

    _messages.push_back(message1);
    _messages.push_back(message2);
    _messages.push_back(message3);
}

void Tutorial::loadMessagesForLevel(const pugi::xml_node & node)
{
    dummyLoad();
}

bool Tutorial::loadTutorial()
{ 
    string dir = "data/";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dir = "";
#endif

    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dir + "tutorial.xml");
    pugi::xml_document doc;
    doc.load_file((fullPath).c_str());

    int numLevel = GameLevel::getInstance()->getNumLevel();
    pugi::xml_node level = doc.child("level");
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
    if(nextMessage->meetConditions())
    {
        _current = nextMessage;
        _messages.pop_front();
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

