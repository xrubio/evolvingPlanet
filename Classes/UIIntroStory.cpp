//
//  UIIntroStory.cpp
//  EvolvingPlanet
//
//  Created by Guillem Laborda on 14/1/16.
//
//

#include "UIIntroStory.h"
#include "GameData.h"
#include "UIProgressMap.h"

Scene* UIIntroStory::createScene()
{
    auto scene = Scene::create();
    auto layer = UIIntroStory::create();
    scene->addChild(layer);
    return scene;
}

bool UIIntroStory::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto camaraBack = Sprite::create("misc/CamaraBack.jpg");
    camaraBack->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    camaraBack->setScale(GameData::getInstance()->getRaWConversion());
    camaraBack->setName("camaraBack");
    this->addChild(camaraBack, 0);
    
    auto camaraHexagon = Sprite::create("misc/CamaraHexagon.png");
    camaraHexagon->setPosition(Vec2(9 * Director::getInstance()->getVisibleSize().width / 19,  6 * Director::getInstance()->getVisibleSize().height / 11));
    camaraHexagon->setName("camaraHexagon");
    camaraHexagon->setScale(GameData::getInstance()->getRaWConversion());
    this->addChild(camaraHexagon, 1);
    camaraHexagon->runAction(Sequence::create(Spawn::create(Repeat::create(RotateBy::create(0.3, -270), 3), ScaleTo::create(0.9, 1.5), NULL),Spawn::create(Repeat::create(RotateBy::create(0.25, 360), 4), ScaleTo::create(1.0, 0.5), NULL), FadeOut::create(0.5), NULL));
    
    auto camaraFront = ProgressTimer::create(Sprite::create("misc/CamaraFront.png"));
    camaraFront->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    camaraFront->setPercentage(0.0);
    camaraFront->setName("camaraFront");
    camaraFront->setScale(GameData::getInstance()->getRaWConversion());
    this->addChild(camaraFront, 1);
    
    auto radialTimer = ProgressFromTo::create(3.0, 0, 66);
    auto radialTimer2 = ProgressFromTo::create(0.2, 66, 90);
    camaraFront->setMidpoint(Vec2(0, 0));
    camaraFront->setBarChangeRate(Vec2(0, 1));
    camaraFront->setType(ProgressTimer::Type::BAR);
    camaraFront->runAction(Sequence::create(DelayTime::create(1.3), radialTimer, radialTimer2, NULL));
    
    auto particleSmoke = ParticleSystemQuad::create("audio/CamaraParticle.plist");
    particleSmoke->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 0.0));
    auto moveParticle = MoveTo::create(5.0, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 4));
    this->addChild(particleSmoke, 5);
    particleSmoke->runAction(moveParticle);
    particleSmoke->setScale(GameData::getInstance()->getRaWConversion());
    
    auto camaraDoor = Sprite::create("misc/CamaraDoor.png");
    camaraDoor->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));//5*Director::getInstance()->getVisibleSize().height/24));
    camaraDoor->setScale(GameData::getInstance()->getRaWConversion());
    camaraDoor->setOpacity(0);
    camaraDoor->setAnchorPoint(Vec2(0.5, 0.66));
    camaraDoor->setName("camaraDoor");
    this->addChild(camaraDoor, 3);
    auto moveDoor = Spawn::create(FadeIn::create(1.5), MoveTo::create(3.0, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 0.66)), NULL);
    camaraDoor->runAction(Sequence::create(DelayTime::create(1.3), moveDoor, NULL));
    
    this->schedule(SEL_SCHEDULE(&UIIntroStory::update), 4.0);
    
    return true;
}

void UIIntroStory::update(float delta)
{
    auto * camaraFront = this->getChildByName("camaraFront");
    auto * camaraDoor = this->getChildByName("camaraDoor");
    if(camaraFront and camaraFront->getNumberOfRunningActions() == 0 and camaraDoor and camaraDoor->getNumberOfRunningActions()==0)
    {
        auto scene = UIProgressMap::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
    }
}