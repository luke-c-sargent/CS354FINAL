#include "Monster.h"

/*
Monster should have a specified range of "sight", if player
walks into range of sight, monster will pick up speed and follow
the player as long as the player is still in range
*/

//Add monsters during createscene()
//Get monster position, divide x/z by 5 and get current tile type

Monster::Monster(Ogre::SceneManager* sceneMgr, Ogre::Vector3 spawn_point, Ogre::Vector3 destination)
{

	//Monster's Vision/Location
	m_seePlayer = false;
	m_lastSeenPosition = Ogre::Vector3::ZERO;
	m_destinationVector = 0;
	m_directionVector = Ogre::Vector3::ZERO;
	m_distance = 0;
	m_walkSpeed = 1.0f;

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
	m_entity->setMaterialName("Ninja");

	name="ninja";

	//Copied from Player.cpp

    // restitution=1.0;//avg of allowable extremes

    position = btVector3(0,0,0);
    inertia= btVector3(0,0,0);
    rotation=btQuaternion(0,0,0,1);

    mass=1000.0f;
    // restitution=1.0;
    // friction=0;

    ms = new btDefaultMotionState(btTransform(rotation, position));

	rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

	//Set Monster's spawn position
	btVector3 btspawn;
	//cout << "\nMonster Spawns at: [" << spawn_point.x << ", " << spawn_point.y << ", " << spawn_point.z << "]\n";
	rootNode->setPosition(spawn_point);
	//btspawn = btVector3(m_spawnPt1.x,m_spawnPt1.y,m_spawnPt1.z);
	btspawn = btVector3(spawn_point.x, spawn_point.y, spawn_point.z);
	position = btspawn;
	//setWalkList();

	rootNode->setScale(0.01, 0.01, 0.01);
    rootNode->attachObject(m_entity);

    //Set initial destination
    m_destinationVector = destination;
    
    //linear velocity
    m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    m_distance = m_directionVector.normalise();    
    
	//bullet stuff
	shape = new btBoxShape(btVector3(.5,1.9,.5));
    shape->calculateLocalInertia(mass,inertia);

    ms = new btDefaultMotionState(btTransform(rotation, position));

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);

    //add other physics constants
    bodyCI.m_restitution=restitution;
    bodyCI.m_friction=friction;

    body = new btRigidBody(bodyCI);
    //body->setActivationState(DISABLE_DEACTIVATION);
    btTransform btt;
    ms->getWorldTransform(btt);


    //Change monster state to initiate animation
    Monster::MONSTER_STATE state = Monster::STATE_WANDER;
    changeState(state);

}
/*
void Monster::setWalkList()
{
	int i, next;
	
	for(i = 0; i < 10; i++)
	{
		next = rand() % 3 + 1;
		if (next == 1)
		{
			m_walkList.push_back(m_spawnPt1);			
		}
		else if (next == 2)
		{
			m_walkList.push_back(m_spawnPt2);
		}
		else if (next == 3)
		{
			m_walkList.push_back(m_spawnPt3);
		}
	}
}
*/
void Monster::changeState(MONSTER_STATE state)
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

void Monster::updateMonsters(Level* level, const Ogre::FrameEvent& evt)
{
	//smooth out movement
	Ogre::Real move = m_walkSpeed * evt.timeSinceLastFrame;

	//update distance left to travel
	Ogre::Vector3 posOgre= Ogre::Vector3(position.getX(),position.getY(),position.getZ());
	m_distance=(posOgre-m_destinationVector).length();	
	//m_distance -= move;

	//check if monster reached destination
	if(m_distance <= 0.1f)
	{

		//cout << "\nDestination reached\n";
		rootNode->setPosition(m_destinationVector); //place monster at destination
		position=btVector3(m_destinationVector.x,m_destinationVector.y,m_destinationVector.z);
		
		m_directionVector = Ogre::Vector3::ZERO; //set to 0 so next part of path can be started

		//Choose new destination for Monster
		int tile_x = rand() % level->x*5 + 1;
    	int tile_y = rand() % level->y*5 + 1;
		
		//m_destinationVector = Ogre::Vector3(tile_x*-1, m_destinationVector.y, tile_y);
    	
    	if((short)level->getTile(tile_x/5, tile_y/5) != 0)
    	{
        	
        	//cout << "\n\n\n@@@@@@@@@@@@@@@@@@ DESTINATION VALID  @@@@@@@@@@@@@@@@@@@@@@@@\n\n\n";
        	//Make next destination the approximate position of chosen tile
        	//Currently hacked the y coordinate to bring ninja's closer to the ground
        	m_destinationVector = Ogre::Vector3(tile_x*-1, m_destinationVector.y, tile_y);
        	
        	//position=btVector3(m_destinationVector.x,m_destinationVector.y,m_destinationVector.z);
    	}

    	//linear velocity
    	m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    	m_distance = m_directionVector.normalise();
		
		
		if(m_directionVector==Ogre::Vector3(0,0,0)){
			cout << "zero norm\n";
			m_directionVector=Ogre::Vector3::ZERO;
			m_distance = m_directionVector.normalise();
		}
		//else
			//m_distance = m_directionVector.normalise();
		
		
		//There should be rotation code here?

		Ogre::Vector3 src = rootNode->getOrientation()*Ogre::Vector3::UNIT_X;

		if ((1.0f + src.dotProduct(m_directionVector)) < 0.0001f)
		{
			rootNode->yaw(Ogre::Degree(180));
		}
		else
		{
			Ogre::Quaternion quat = src.getRotationTo(m_directionVector);
			rootNode->rotate(quat);
		}
	}
	else
	{
		//rootNode->translate(m_directionVector * move);
		//Ogre::Vector3 cur_pos = rootNode->getPosition();
		//position = btVector3(cur_pos.x, cur_pos.y, cur_pos.z);
	}
	
	//cout << "\nMonster position: " << rootNode->getPosition().x << " " << rootNode->getPosition().z << "\n";
	//cout << "\nBullet box position: " << position.getX() << " " << position.getZ() << "\n";
	
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

/*
bool Monster::nextLocation()
{
	
	if (m_walkList.empty()) //this should not happen right now
	{
		return false;
	}

	m_destinationVector = m_walkList.front();
	m_walkList.pop_front();
	m_walkList.push_back(m_destinationVector);

	m_directionVector = m_destinationVector - rootNode->getPosition();
	if(m_directionVector==Ogre::Vector3(0,0,0)){
		cout << "zero norm\n";
		m_directionVector=Ogre::Vector3::ZERO;
	}
	else
		m_distance = m_directionVector.normalise();


	return true;


}
*/

void Monster::killMonster()
{
	m_animState = m_entity->getAnimationState("Death1");
	m_animState->setEnabled(true);

	rootNode->detachObject(m_entity);


}

void Monster::changeDestination(Level* level)
{
	int x = rand() % level->x*5 + 1;
	int z = rand() % level->y*5 + 1;

	Ogre::Vector3 destination = Ogre::Vector3(x*-1, -2.2, z); //update y position to constant
	m_destinationVector = destination;
    
    //linear velocity
    m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    m_distance = m_directionVector.normalise();
}

/* 04/22 - Begin adding monsters to the scene from BaseApplication.cpp */
