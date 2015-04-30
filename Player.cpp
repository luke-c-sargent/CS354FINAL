#include <Player.h>
#include <btBulletDynamicsCommon.h>

using std::cout;

//multiplayer stuff

Player::Player(Ogre::SceneManager* smp, Ogre::String inname   ):
  l(25),
  w(25),
  h(50)
  // playerState(PlayerState::NoFire),
  // weapon(Weapon0)

{
    //set scene manager pointer
    sceneMgr=smp;
    playerLV=btVector3(0,0,0);
    //physical constants
    l=1;
    w=1;
    h=2;

    ori=btVector3(0,1,0);

    restitution=1.0;//avg of allowable extremes

    name=inname;

    position = btVector3(ori);
    inertia= btVector3(0,0,0);
    rotation=btQuaternion(0,0,0,1);

    mass=100.0f;
    restitution=1.0;
    friction=0;

    //bind player

    rootNode=sceneMgr->getRootSceneNode()->createChildSceneNode(name + "Node");
    Ogre::Entity * entity=sceneMgr->createEntity(inname,"cube.mesh");
    rootNode->attachObject(entity);

    //purdiness
    entity->setCastShadows(true);
    std::string namest = name;
    if(namest.compare("player")==0)
      entity->setMaterialName("PlayerWire");
    else
    entity->setMaterialName("PlayerWire2");
    rootNode->translate(position.getX(),position.getY(),position.getZ());

    rootNode->scale(w/100,h/100,l/100);

    //bt values
    shape = new btBoxShape(btVector3(w/2.,h/2.,l/2.));
    shape->calculateLocalInertia(mass,inertia);



    ms = new btDefaultMotionState(btTransform(rotation, position));

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);

    //add other physics constants
    bodyCI.m_restitution=restitution;
    bodyCI.m_friction=friction;
    bodyCI.m_rollingFriction=0.2;//!!!

    body = new btRigidBody(bodyCI);
    body->setActivationState(DISABLE_DEACTIVATION);
    btTransform btt;
    ms->getWorldTransform(btt);
    btVector3 org=btt.getOrigin();
}

void Player::setLV(btVector3 lvin){
  playerLV=lvin;
}

void Player::setLV(Ogre::Vector3 lvin){

  playerLV=btVector3(lvin.x,lvin.y,lvin.z);
}

void Player::setPos(Ogre::Vector3 pos){
    position = btVector3(pos.x,pos.y,pos.z);
}

void Player::setPos(btVector3 pos){
  position=pos;
}

btScalar Player::getX(){
    return position.getX();
}
btScalar Player::getY(){
    return position.getY();
}
btScalar Player::getZ(){
    return position.getZ();
}
