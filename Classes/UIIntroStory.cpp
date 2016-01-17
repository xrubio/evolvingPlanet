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
#include "LocalizedString.h"

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
  
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto descent = Sprite::create("misc/intro/planet.png");
    descent->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height));
    descent->setScale(GameData::getInstance()->getRaWConversion());
    descent->setAnchorPoint(Vec2(0.5f, 1.0f));
    descent->setName("descent");
    this->addChild(descent, 5);
    descent->runAction(Sequence::create(FadeIn::create(1.0f), DelayTime::create(1.0f), MoveBy::create(7.0f, Vec2(0, GameData::getInstance()->getRaWConversion()*descent->getContentSize().height-visibleSize.height)), DelayTime::create(3.0f), FadeOut::create(2.0f), NULL));


    auto messageLabel = Label::createWithTTF(LocalizedString::create("INTRO_1"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    messageLabel->setColor(Color3B(230, 230, 230));
    messageLabel->enableShadow();
    messageLabel->setMaxLineWidth(800);
    messageLabel->setOpacity(0);
    messageLabel->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    messageLabel->setPosition(Vec2(0.05f*descent->getContentSize().width, 0.05f*descent->getContentSize().height));
    messageLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    messageLabel->runAction(Sequence::create(DelayTime::create(10.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), FadeOut::create(2.0f), NULL));
    descent->addChild(messageLabel);

    auto chamber = Sprite::create("misc/intro/chamber_1.png");
    chamber->setPosition(Vec2(0.5f*Director::getInstance()->getVisibleSize().width, 0.4f*Director::getInstance()->getVisibleSize().height));
    chamber->setName("chamber");
    chamber->setScale(GameData::getInstance()->getRaWConversion());
    chamber->setOpacity(0);
    this->addChild(chamber, 6);
    chamber->runAction(Sequence::create(DelayTime::create(15.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), FadeOut::create(3.0f), NULL));

    auto chamber2 = Sprite::create("misc/intro/CamaraFront.png");
    chamber2->setPosition(Vec2(0.5f*Director::getInstance()->getVisibleSize().width, 0.4f*Director::getInstance()->getVisibleSize().height));
    chamber2->setName("chamber2");
    chamber2->setScale(GameData::getInstance()->getRaWConversion());
    chamber2->setOpacity(0);
    this->addChild(chamber2, 6);
    chamber2->runAction(Sequence::create(DelayTime::create(20.0f), FadeIn::create(3.0f), MoveBy::create(5.0f, Vec2(-0.25f*Director::getInstance()->getVisibleSize().width,0)), DelayTime::create(3.0f), FadeOut::create(1.0f), NULL));

    auto messageLabel2 = Label::createWithTTF(LocalizedString::create("INTRO_2"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    messageLabel2->setColor(Color3B(230, 230, 230));
    messageLabel2->enableShadow();
    messageLabel2->setMaxLineWidth(800);
    messageLabel2->setOpacity(0);
    messageLabel2->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    messageLabel2->setPosition(Vec2(0.6f*visibleSize.width, 0.7*visibleSize.height));
    messageLabel2->runAction(Sequence::create(DelayTime::create(28.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), FadeOut::create(2.0f), NULL));
    this->addChild(messageLabel2);

    // thid scene: transport at kepler
    auto transport = Sprite::create("misc/intro/transport_cut.jpg");
    float conversion = visibleSize.width/transport->getContentSize().width;
    transport->setScale(conversion);
    transport->setPosition(Vec2(1.0f*visibleSize.width, 0.0f));
    transport->setAnchorPoint(Vec2(0.0f, 0.0f));
    transport->setName("transport");
    this->addChild(transport, 5);
    transport->runAction(Sequence::create(DelayTime::create(33.0f), MoveTo::create(6.0f, Vec2(0, 0.0f)), DelayTime::create(3.0f), FadeOut::create(2.0f), NULL));

    auto messageLabel3 = Label::createWithTTF(LocalizedString::create("INTRO_3"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    messageLabel3->setColor(Color3B(230, 230, 230));
    messageLabel3->enableShadow();
    messageLabel3->setMaxLineWidth(800);
    messageLabel3->setOpacity(0);
    messageLabel3->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    messageLabel3->setPosition(Vec2(0.3f*visibleSize.width, 0.9*visibleSize.height));
    messageLabel3->runAction(Sequence::create(DelayTime::create(39.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), FadeOut::create(2.0f), NULL));
    this->addChild(messageLabel3);

    auto transport2 = Sprite::create("misc/intro/transport.jpg");
    transport2->setOpacity(0);
    transport2->setScale(GameData::getInstance()->getRaWConversion());
    transport2->setPosition(Vec2(0.5f*visibleSize.width, 0.0f)); //.5f*visibleSize.height));
    transport2->setAnchorPoint(Vec2(0.5f, 0.0f));
    transport2->setName("transport2");
    this->addChild(transport2, 5);
    transport2->runAction(Sequence::create(DelayTime::create(41.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), MoveBy::create(6.0f, Vec2(0.0f, 0.7f*(visibleSize.height-transport2->getContentSize().height))), DelayTime::create(2.0f), FadeOut::create(2.0f), NULL));
    
    auto messageLabel4 = Label::createWithTTF(LocalizedString::create("INTRO_4"), "fonts/BebasNeue.otf", 50 * GameData::getInstance()->getRaConversion());
    messageLabel4->setColor(Color3B(230, 230, 230));
    messageLabel4->enableShadow();
    messageLabel4->setMaxLineWidth(800);
    messageLabel4->setOpacity(0);
    messageLabel4->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());
    messageLabel4->setPosition(Vec2(0.2f*transport2->getContentSize().width, 0.3*transport2->getContentSize().height));
    messageLabel4->runAction(Sequence::create(DelayTime::create(46.0f), FadeIn::create(2.0f), DelayTime::create(2.0f), FadeOut::create(2.0f), NULL));
    transport2->addChild(messageLabel4);

    auto logo = Sprite::create("misc/intro/logo.png");
    logo->setOpacity(0);
    logo->setScale(GameData::getInstance()->getRaWConversion());
    logo->setPosition(Vec2(0.5f*visibleSize.width, 0.7f*visibleSize.height));
    logo->setName("logo");
    this->addChild(logo, 5);
    logo->runAction(Sequence::create(DelayTime::create(50.0f), FadeIn::create(2.0f), DelayTime::create(4.0f), CallFunc::create(this, callfunc_selector(UIIntroStory::toMap)), NULL));
    
    //createAnimatedLabel("INTRO_1", Vec2(0.6f,0.8f), Vec2(0.15, 0), 1.0f);
    /*
    createAnimatedLabel("INTRO_2", Vec2(0.15f,0.8f), Vec2(0.15, 0), 3.0f);
    createAnimatedLabel("INTRO_3", Vec2(0.8f,0.8f), Vec2(-0.15, 0), 5.0f);
    createAnimatedLabel("INTRO_4", Vec2(0.8f,0.3f), Vec2(-0.15, 0), 7.0f);
    */
    
    return true;
}

void UIIntroStory::toMap() //(float delta)
{
    auto scene = UIProgressMap::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, scene));
}

void UIIntroStory::createAnimatedLabel( const std::string & keyString, const Vec2 & ratioPos, const Vec2 & ratioMove, const float & delay )
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto messageLabel = Label::createWithTTF(LocalizedString::create(keyString.c_str()), "fonts/arial.ttf", 40 * GameData::getInstance()->getRaConversion());

    //messageLabel->setName("tutorial");
    messageLabel->setColor(Color3B(230, 230, 230));
    messageLabel->enableShadow();
    messageLabel->setMaxLineWidth(800);
    messageLabel->setOpacity(0);
    messageLabel->setScale(GameData::getInstance()->getRaWConversion(), GameData::getInstance()->getRaHConversion());

    Vec2 position = Vec2(visibleSize.width*ratioPos.x, visibleSize.height*ratioPos.y);
    messageLabel->setPosition(position);
  
    float marginWidth = 0.02f*messageLabel->getContentSize().width;
    float marginHeight = 0.02f*messageLabel->getContentSize().height;
    Vec2 margin(marginWidth, marginHeight);
    
    Vec2 origin(messageLabel->getBoundingBox().origin - margin);
    Vec2 end(messageLabel->getBoundingBox().origin + messageLabel->getBoundingBox().size + margin);

    auto labelBorder = DrawNode::create();
    //labelBorder->setName("labelBorderTutorial");
    labelBorder->drawSolidRect(origin, end, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    labelBorder->drawRect(origin, end, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    labelBorder->setOpacity(0);
   
    this->addChild(messageLabel, 10);
    this->addChild(labelBorder, 10);
    
    messageLabel->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(1.0), MoveBy::create(2.5, Vec2(visibleSize.width*ratioMove.x, visibleSize.height*ratioMove.y)), DelayTime::create(3.0f), FadeOut::create(1.0f), NULL));
    labelBorder->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(1.0), MoveBy::create(2.5, Vec2(visibleSize.width*ratioMove.x, visibleSize.height*ratioMove.y)), DelayTime::create(3.0f), FadeOut::create(1.0f), NULL));
}

