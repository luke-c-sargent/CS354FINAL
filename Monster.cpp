#include "Monster.h"

/*
Monster should have a specified range of "sight", if player 
walks into range of sight, monster will pick up speed and follow
the player as long as the player is still in range
*/

Monster::Monster(Ogre::SceneManager* sceneMgr)
{
	//Monster's Vision/Location
	m_seePlayer = false;
	m_lastSeenPosition = Ogre::Vector3::ZERO;
	m_currentDestination = 0;
	m_directionVector = Ogre::Vector3::NEGATIVE_UNIT_Z;

	//Monster's Timers
	m_attackTimeout = 1000.0;
	m_pursuitTimeout = 10000.0;
	m_damageTimeout = 200.0;
	m_damaged = false;

	//Monster's Data
	m_health = 100.0;
	m_range = 4000.0;
	m_attackRange = 100.0;

	m_entity = sceneMgr->createEntity("ninja.mesh");

/*
	movingSpeed(70.0),
	mDirection(Ogre::Vector3::ZERO),
	mDestination(Ogre::Vector3::ZERO),
	mAnimationState(0),
	mEntity(0),
	mNode(0),
	
	range(10),
	collision(false),
	health(100)

	//set scene manager pointer
    sceneMgr=smp;

    //Bind monster
	rootNode=sceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0, 25.0));
    mEntity = sceneMgr->createEntity("robot.mesh");
    rootNode->attachObject(entity);

*/

}

void Monster::initMonster(Ogre::SceneManager* smp)
{
	Ogre::SceneManager* sceneMgr = smp;
	m_node = sceneMgr->getRootSceneNode()->createChildSceneNode();
	m_node->setPosition(m_spawnPt3);
	m_node->setScale(0.01, 0.01, 0.01);
    //Ogre::Entity* m_entity = sceneMgr->createEntity("robot.mesh");
    m_node->attachObject(this->m_entity);
	
}

/* 04/22 - Begin adding monsters to the scene from BaseApplication.cpp */