#include "Monster.h"

/*
Monster should have a specified range of "sight", if player
walks into range of sight, monster will pick up speed and follow
the player as long as the player is still in range
*/

//Add monsters during createscene()
//Get monster position, divide x/z by 5 and get current tile type

Monster::Monster(Ogre::SceneManager* sceneMgr, Ogre::Vector3 spawn_point, Ogre::Vector3 direction)
{

	//Monster's Vision/Location
	m_seePlayer = false;
	m_lastSeenPosition = Ogre::Vector3::ZERO;
	m_destinationVector = 0;
	m_directionVector = direction;
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

	btVector3 btspawn;
	//Ogre::Vector3 m_spawnPt = Vector3(spawn_point.getX(),spawn_point.getY(),spawn_point.getZ())
	//rootNode->setPosition(m_spawnPt1);
	
	cout << "Monster Spawns at: [" << spawn_point.x << ", " << spawn_point.y << ", " << spawn_point.z << "]\n";
	rootNode->setPosition(spawn_point);
	//btspawn = btVector3(m_spawnPt1.x,m_spawnPt1.y,m_spawnPt1.z);
	btspawn = btVector3(spawn_point.x, spawn_point.y, spawn_point.z);
	position = btspawn;
	setWalkList();

	rootNode->setScale(0.01, 0.01, 0.01);
    //Ogre::Entity* m_entity = sceneMgr->createEntity("robot.mesh");
    rootNode->attachObject(m_entity);

    m_destinationVector = m_walkList.front();
	m_walkList.pop_front();
	m_walkList.push_back(m_destinationVector);

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


    Monster::MONSTER_STATE state = Monster::STATE_WANDER;
    changeState(state);
    // btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);

    // //add other physics constants
    // bodyCI.m_restitution=restitution;
    // bodyCI.m_friction=friction;
    // bodyCI.m_rollingFriction=0.2;//!!!

    // body = new btRigidBody(bodyCI);
    // body->setActivationState(DISABLE_DEACTIVATION);
    // btTransform btt;
    // ms->getWorldTransform(btt);
    // btVector3 org=btt.getOrigin();

    //=======================


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
/*
void Monster::initMonster(Ogre::SceneManager* smp,  spawn_point)
{
	Ogre::SceneManager* sceneMgr = smp;
	rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

	btVector3 btspawn;
	if(spawn_point == 1)
	{
		rootNode->setPosition(m_spawnPt1);
		btspawn = btVector3(m_spawnPt1.x,m_spawnPt1.y,m_spawnPt1.z);
		position=btspawn;
		setWalkList();

	}
	else if (spawn_point == 2)
	{
		rootNode->setPosition(m_spawnPt2);
		btspawn = btVector3(m_spawnPt2.x,m_spawnPt2.y,m_spawnPt2.z);
		position=btspawn;
		setWalkList();

	}
	else //spawn_point == 3
	{
		rootNode->setPosition(m_spawnPt3);
		btspawn = btVector3(m_spawnPt3.x,m_spawnPt3.y,m_spawnPt3.z);
		position=btspawn;
		setWalkList();
		
	}


	rootNode->setScale(0.01, 0.01, 0.01);
    //Ogre::Entity* m_entity = sceneMgr->createEntity("robot.mesh");
    rootNode->attachObject(m_entity);

    m_destinationVector = m_walkList.front();
	m_walkList.pop_front();
	m_walkList.push_back(m_destinationVector);

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



}
*/
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

	Ogre::Vector3 posOgre= Ogre::Vector3(position.getX(),position.getY(),position.getZ());
	//m_distance=(posOgre-m_destinationVector).length();
	//update distance left to travel
	m_distance -= move;

	//check if monster reached destination or hit an object
	
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
        	
        	cout << "\n\n\n@@@@@@@@@@@@@@@@@@ DESTINATION VALID  @@@@@@@@@@@@@@@@@@@@@@@@\n\n\n";
        	//Make next destination the approximate position of chosen tile
        	//Currently hacked the y coordinate to bring ninja's closer to the ground
        	m_destinationVector = Ogre::Vector3(tile_x*-1, m_destinationVector.y, tile_y);
        	
        	//position=btVector3(m_destinationVector.x,m_destinationVector.y,m_destinationVector.z);
    	}

    	//linear velocity
    	m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    	m_distance = m_directionVector.normalise();
    	

    	/* OLD DESTINATION SWITCHING CODE
		m_destinationVector = m_walkList.front(); //update monster's next destination (front of the queue)
		m_walkList.pop_front(); //remove the destination from the queue
		m_walkList.push_back(m_destinationVector); //push destination back at end of queue
		*/ 
		
		
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
		rootNode->translate(m_directionVector * move);
		Ogre::Vector3 cur_pos = rootNode->getPosition();
		position = btVector3(cur_pos.x, cur_pos.y, cur_pos.z);
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

	m_directionVector = m_destinationVector - rootNode->getPosition();
	if(m_directionVector==Ogre::Vector3(0,0,0)){
		cout << "zero norm\n";
		m_directionVector=Ogre::Vector3::ZERO;
	}
	else
		m_distance = m_directionVector.normalise();


	return true;


}

void Monster::killMonster()
{
	m_animState = m_entity->getAnimationState("Death1");
	m_animState->setEnabled(true);

	rootNode->detachObject(m_entity);


}


/* 04/22 - Begin adding monsters to the scene from BaseApplication.cpp */
