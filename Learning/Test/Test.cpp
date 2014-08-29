//
//  Test.cpp
//  simulplay
//
//  Created by Guillem Laborda on 25/07/14.
//
//

#include "../Classes/LevelPlayDomain.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LevelPlayDomainTest
#include <boost/test/unit_test.hpp>

int add(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_CASE(universeInOrder)
{
    BOOST_CHECK(add(2, 2) == 4);
}