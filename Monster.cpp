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
	m_destinationVector = 0;
	m_directionVector = Ogre::Vector3::NEGATIVE_UNIT_Z;
	m_distance = 0;
	m_walkSpeed = 1.0;

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

void Monster::initMonster(Ogre::SceneManager* smp, int spawn_point)
{
	Ogre::SceneManager* sceneMgr = smp;
	m_node = sceneMgr->getRootSceneNode()->createChildSceneNode();

	if(spawn_point == 1)
	{
		m_node->setPosition(m_spawnPt1);
		
		m_walkList.push_back(m_spawnPt2);
		m_walkList.push_back(m_spawnPt3);
		m_walkList.push_back(m_spawnPt1);

	} 
	else if (spawn_point == 2)
	{
		m_node->setPosition(m_spawnPt2);
		
		m_walkList.push_back(m_spawnPt3);
		m_walkList.push_back(m_spawnPt1);
		m_walkList.push_back(m_spawnPt2);	
	}
	else //spawn_point == 3
	{
		m_node->setPosition(m_spawnPt3);
		
		m_walkList.push_back(m_spawnPt1);
		m_walkList.push_back(m_spawnPt2);
		m_walkList.push_back(m_spawnPt3);
	}
	
	
	m_node->setScale(0.01, 0.01, 0.01);
    //Ogre::Entity* m_entity = sceneMgr->createEntity("robot.mesh");
    m_node->attachObject(m_entity);

    m_destinationVector = m_walkList.front();
	m_walkList.pop_front();
	m_walkList.push_back(m_destinationVector);
	
}

void Monster::changeState(MONSTER_STATE state, const Ogre::FrameEvent& evt)
{
	
	if(state == STATE_WANDER)
	{
		m_animState = m_entity->getAnimationState("Walk");

	}
	else if (state == STATE_CHILL)
	{
		m_animState = m_entity->getAnimationState("Idle1");
		//zero out the m_walkList so he stops moving around
	}
	
	//m_animState = m_entity->getAnimationState("Walk");
	
	m_animState->setLoop(true);
	m_animState->setEnabled(true);
	
	//m_animState->addTime(evt.timeSinceLastFrame);

}

void Monster::updateMonsters(const Ogre::FrameEvent& evt)
{
	//cout << "\n@@@@@@@@@@@@@         HELLO         @@@@@@@@@@@@@\n";
	//smooth out movement
	Ogre::Real move = m_walkSpeed * evt.timeSinceLastFrame;

	//update distance left to travel
	m_distance -= move;

	//check if monster reached destination
	if(m_distance <= 0.0f)
	{
		
		m_node->setPosition(m_destinationVector); //place monster at destination
		m_directionVector = Ogre::Vector3::ZERO; //set to 0 so next part of path can be started

		m_destinationVector = m_walkList.front(); //update monster's next destination (front of the queue)
		m_walkList.pop_front(); //remove the destination from the queue
		m_walkList.push_back(m_destinationVector); //push destination back at end of queue
		
		m_directionVector = m_destinationVector - m_node->getPosition(); //set new direction vector
		m_distance = m_directionVector.normalise();

		//There should be rotation code here?
		
		Ogre::Vector3 src = m_node->getOrientation()*Ogre::Vector3::UNIT_X;

		if ((1.0f + src.dotProduct(m_directionVector)) < 0.0001f) 
		{
			m_node->yaw(Ogre::Degree(180));						
		}
		else
		{
			Ogre::Quaternion quat = src.getRotationTo(m_directionVector);
			m_node->rotate(quat);
		}
	}
	else
	{
		m_node->translate(m_directionVector * move);
	}
	m_animState->addTime(evt.timeSinceLastFrame);
}
/*
void Monster::moveMonster()
{
	if(m_directionVector == Ogre::Vector3::ZERO)
	{
		if(nextLocation())
		{
	    	m_animState = m_entity->getAnimationState("Walk");
   	 		m_animState->setLoop(true);
    		m_animState->setEnabled(true);		
		}
	}
	else
	{
		Ogre::Real move = m_walkSpeed * evt.timeSinceLastFrame;
  		m_distance -= move;
	}
}
*/
bool Monster::nextLocation()
{
	if (m_walkList.empty()) //this should not happen right now
	{
		return false;
	}

	m_destinationVector = m_walkList.front();
	m_walkList.pop_front();
	m_walkList.push_back(m_destinationVector);
	
	m_directionVector = m_destinationVector - m_node->getPosition();
	m_distance = m_directionVector.normalise();
	
	return true;


}

void Monster::killMonster()
{
	m_animState = m_entity->getAnimationState("Death1");
	m_animState->setEnabled(true);

	m_node->detachObject(m_entity);


}

/* 04/22 - Begin adding monsters to the scene from BaseApplication.cpp */