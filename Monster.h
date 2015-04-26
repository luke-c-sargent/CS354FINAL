#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include <OgreVector3.h>

class Monster
{
public:
	
	//Monster state
	enum{

		STATE_WANDER = 0,
		STATE_ATTACK,
		STATE_EVADE
	};



	Monster(Ogre::SceneManager* sceneMgr);
	void initMonster(Ogre::SceneManager* smp);
	
	Ogre::Entity* m_entity;
	Ogre::SceneNode* m_node;

	Ogre::Vector3 m_spawnPt1 = Ogre::Vector3(0, -2.2, -2);
	Ogre::Vector3 m_spawnPt2 = Ogre::Vector3(0, -2.2, 10);
	Ogre::Vector3 m_spawnPt3 = Ogre::Vector3(16, -2.2, 10);
	
	//Monster's Vision/Location
	bool m_seePlayer; //player is in monster's range/line of sight
	Ogre::Vector3 m_lastSeenPosition; //last position monster saw the player in	
	int m_currentDestination;
	Ogre::Vector3 m_directionVector;

	//Monster's Timers
	Ogre::Timer* m_attackTimer;
	Ogre::Timer* m_pursuitTimer;
	Ogre::Timer* m_damageTimer;
	Ogre::Real m_attackTimeout;
	Ogre::Real m_pursuitTimeout;
	Ogre::Real m_damageTimeout;
	bool m_damaged;

	//Monster's data
	Ogre::Real m_health;
	Ogre::Real m_range; //if player comes within this distance from monster, pursuit is triggered
	Ogre::Real m_attack; //strength
	Ogre::Real m_attackRange; //monster physically attacks when within this range


/*
	Ogre::Real mDistance; 
	Ogre::Real movingSpeed;
	Ogre::Vector3 mDirection; //Direction of target
	Ogre::Vector3 mDestination;  //Used as end destination (player, constantly updated)
	Ogre::AnimationState* mAnimationState; //possibly not needed
	
	Ogre::SceneNode* mNode;
	
	
	int range; //Radius of monster's range
	bool collision; //Monster to Player/Monster collision
	int health;
*/
	
};
