
#ifndef __Tutorial_h__
#define __Tutorial_h__

/** This class structures the information that needs to be supplied to GameLevel to provide information to the user **/
class Tutorial
{
public:
    Tutorial();
    virtual ~Tutorial();
    /** fill the class using an xml file
      \returns true if the level has an available tutorial, false otherwise
      */
    bool loadTutorial();
};

#endif // __Tutorial_h__

