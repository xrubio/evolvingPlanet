//
//  GameDataTest.cpp
//  simulplay
//
//  Created by Guillem Laborda on 25/07/14.
//
//

#include "../Classes/Agent.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AgentTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(AgentGetterSetter)
{
    Agent* ag = new Agent(0, 100, 0, 0);
    BOOST_CHECK_EQUAL(0, ag->getId());
}
