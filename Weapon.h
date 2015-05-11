#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include "GameEnums.h"
#include <stdio.h>
#include "WeaponSound.h"
#include "BulletSound.h"
#include <btBulletDynamicsCommon.h>

using std::cout;

struct Bullet;

class Weapon{
public:
    Weapon(const int weapon);
    ~Weapon(void);

    int count;
    GameObject * playerptr;
    Ogre::String name;
    int fire();
    bool reload();
    void switch_weapon();
    void cancel_reload();
    int ammo_left();
    int total_ammo_left();
    Ogre::SceneManager* sceneMgr;
    Bullet* spawnBullet(btVector3 playerPos, btVector3 dir,Simulator* simulator);

    void setSMP(Ogre::SceneManager* smp);

    float bulletSpeed;
    float bulletSize;
    btCollisionShape* col_shape;
    float mass;
    Ogre::Real power;
    
protected:

    WeaponSound * weaponsound;
    Ogre::Timer* fireTimer;
    Ogre::Timer* reloadTimer;
    Ogre::Timer* switchTimer;
    Ogre::Real firetime;
    Ogre::Real reloadtime;
    Ogre::Real switchtime;
    // Ogre::Real lastCPUtime;    
    int ammo_cap;
    int total_ammo_cap;
    int ammo;
    int total_ammo;
    bool reloadBool;




};

struct Bullet:public GameObject{

  Bullet(Weapon* weap,Ogre::SceneManager* smp, Simulator* sim,btVector3 pos, btVector3 dir, Ogre::String count){
    std::cout <<dir.getX()<<","<<dir.getY()<<","<<dir.getZ()<<"\n";
    sceneMgr=smp;
    simulator=sim;
    position=pos;
    direction=dir;
    weapon=weap;
    age=0;
    rotation=btQuaternion(0,0,0,1);
    hit=false;

    //ADD WEAPON STRENGTH
    power = weapon->power;
    //physics stuffs
    inertia= btVector3(0,0,0);
    bulletsound = new BulletSound();
    mass=weapon->mass;
    shape=weapon->col_shape;
    weapon->col_shape->calculateLocalInertia(mass,inertia);
    ms = new btDefaultMotionState(btTransform(rotation, position));
    shape->setMargin(0);
    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);
    body = new btRigidBody(bodyCI);
    sim->addObject(this);
    body->setGravity(btVector3(0,0,0));

    name="bullet";
    Ogre::String outname = weapon->name+"_"+name+"_"+count;
    rootNode=sceneMgr->getRootSceneNode()->createChildSceneNode(outname +Ogre::String()+ "Node");
    Ogre::Entity * entity=sceneMgr->createEntity(outname,"sphere.mesh");
    rootNode->attachObject(entity);
    float ratio = weapon->bulletSize/200.0;
    rootNode->scale(ratio,ratio,ratio);
    rootNode->translate(position.getX(),position.getY(),position.getZ());
  }

  ~Bullet()
  {
    //cout << "\ndis4.5\n";
    sceneMgr->destroySceneNode(rootNode);
    delete body;
    delete ms;
    //cout << "\ndis5\n";
  }

  btVector3 linvel(){
    return (weapon->bulletSpeed*direction);
  }

  Weapon * weapon;
  btVector3 direction;
  float age;
  BulletSound * bulletsound;
};
