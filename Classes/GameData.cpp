//
//  GameData.cpp
//  simulplay
//
//  Created by Guillem Laborda on 21/07/14.
//
//

#include "GameData.h"

// Global static pointer used to ensure a single instance of the class.
GameData* GameData::m_pInstance = NULL;

/** This function is called to create an instance of the class.
 Calling the constructor publicly is not allowed. The constructor
 is private and is only called by this Instance function.
 */
GameData* GameData::getInstance()
{
    if (!m_pInstance) // Only allow one instance of class to be generated.
    {
        m_pInstance = new GameData;
    }
    return m_pInstance;
}


int GameData::getAgePlayer()
{
    return agePlayer;
}

void GameData::setAgePlayer(int age)
{
    agePlayer = age;
}