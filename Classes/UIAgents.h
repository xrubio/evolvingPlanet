//
//  UIAgents.h
//  simulplay
//
//  Created by Guillem Laborda on 13/08/14.
//
//

#ifndef __simulplay__UIAgents__
#define __simulplay__UIAgents__

#include "UIGameplayMap.h"
#include "UIProgressMap.h"
#include "GameLevel.h"

class UIAgents : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    void menuStartCallback(Ref* pSender);
    void menuMapCallback(Ref* pSender);
    void minusAtt1Callback(Ref* pSender);
    void plusAtt1Callback(Ref* pSender);
    void minusAtt2Callback(Ref* pSender);
    void plusAtt2Callback(Ref* pSender);
    void minusAtt3Callback(Ref* pSender);
    void plusAtt3Callback(Ref* pSender);

    CREATE_FUNC(UIAgents);

private:
    bool gameStarted = true;

    int att1 = 0;
    int att2 = 0;
    int att3 = 0;
};

#endif /* defined(__simulplay__UIAgents__) */
