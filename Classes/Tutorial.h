
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

    /** fake level loaded for testing purposes **/
    void dummyLoad();

    // create _messages from the level located at node 
    void loadMessagesForLevel(const pugi::xml_node & node);
public:
    Tutorial();
    virtual ~Tutorial();
    /** fill the class using an xml file
      \returns true if the level has an available tutorial, false otherwise
      */
    bool loadTutorial();

    /** check if next message on list can be shown. \returns true if it's possible **/
    bool checkNextMessage();
    void removeCurrentMessage();
    const Message * getCurrentMessage() const{return _current;}
};

#endif // __Tutorial_h__

