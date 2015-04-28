#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include <OgreVector3.h>
#include <deque>
#include <OgreFrameListener.h>
#include <stdio.h>

using std::cout;

class Monster
{
public:
	
	//Monster state
	enum MONSTER_STATE{

		STATE_CHILL = 0,
		STATE_WANDER,
		STATE_ATTACK,
		STATE_EVADE
	};



	Monster(Ogre::SceneManager* sceneMgr);
	void initMonster(Ogre::SceneManager* smp, int spawn_point);
	void changeState(MONSTER_STATE state, const Ogre::FrameEvent& evt);
	void moveMonster();
	bool nextLocation();
	void updateMonsters(const Ogre::FrameEvent& evt);
	void killMonster();
	

	//Monster Spawn Points 
	/*	MIGHT MOVE THIS TO DIFFERENT Location	*/
	Ogre::Vector3 m_spawnPt1 = Ogre::Vector3(0, -2.2, -2);
	Ogre::Vector3 m_spawnPt2 = Ogre::Vector3(0, -2.2, 10);
	Ogre::Vector3 m_spawnPt3 = Ogre::Vector3(16, -2.2, 10);


	//Monster's Ogre Implementation Variables
	Ogre::Entity* m_entity;
	Ogre::SceneNode* m_node;
	Ogre::AnimationState* m_animState;

	//Monster's Vision/Location
	bool m_seePlayer; //player is in monster's range/line of sight
	Ogre::Vector3 m_lastSeenPosition; //last position monster saw the player in	
	Ogre::Vector3 m_destinationVector; //monster's current destination
	Ogre::Vector3 m_directionVector; //direction the robot is facing/moving
	Ogre::Real m_distance; //distance left until monster reaches current destination
	Ogre::Real m_walkSpeed;
	std::deque<Ogre::Vector3> m_walkList; //locations that Monster wants to go to

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