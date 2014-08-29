//
//  GameDataTest.cpp
//  simulplay
//
//  Created by Guillem Laborda on 25/07/14.
//
//

#include "../Classes/GameData.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GameDataTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(GameDataGetterSetter)
{

    GameData::getInstance()->setLevelScore(0, 2);

    BOOST_CHECK_EQUAL(2, GameData::getInstance()->getLevelScore(0));
}
