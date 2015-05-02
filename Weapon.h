#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include "GameEnums.h"
#include <stdio.h>
#include "WeaponSound.h"
#include <btBulletDynamicsCommon.h>

using std::cout;

struct Bullet;

class Weapon{
public:
    Weapon(const int weapon);
    ~Weapon(void);

    GameObject * playerptr;
    Ogre::String name;
    bool fire();
    bool reload();
    void switch_weapon();
    void cancel_reload();
    int ammo_left();
    int total_ammo_left();
    Ogre::SceneManager* sceneMgr;
    Bullet* spawnBullet(btVector3 playerPos, btVector3 dir,Simulator* simulator, int count);

    void setSMP(Ogre::SceneManager* smp);

    float bulletSpeed;
    float bulletSize;
    btCollisionShape* col_shape;
protected:

    WeaponSound * weaponsound;
    Ogre::Timer* fireTimer;
    Ogre::Timer* reloadTimer;
    Ogre::Timer* switchTimer;
    Ogre::Real firetime;
    Ogre::Real reloadtime;
    Ogre::Real switchtime;
    // Ogre::Real lastCPUtime;
    Ogre::Real power;
    int ammo_cap;
    int total_ammo_cap;
    int ammo;
    int total_ammo;
    bool reloadBool;




};

struct Bullet:public GameObject{

  Bullet(Weapon* weap,Ogre::SceneManager* smp, Simulator* sim,btVector3 pos, btVector3 dir, Ogre::String count){
    //std::cout <<dir.getX()<<","<<dir.getY()<<","<<dir.getZ()<<"\n";
    sceneMgr=smp;
    simulator=sim;
    position=pos;
    direction=dir;
    weapon=weap;
    age=0;
    rotation=btQuaternion(0,0,0,1);

    //physics stuffs
    inertia= btVector3(0,0,0);
    mass=0.01;
    weapon->col_shape->calculateLocalInertia(mass,inertia);
    ms = new btDefaultMotionState(btTransform(rotation, position));
    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);
    body = new btRigidBody(bodyCI);
    sim->addObject(this);

    name=weapon->name+"_bullet_"+count;
    rootNode=sceneMgr->getRootSceneNode()->createChildSceneNode(name +Ogre::String()+ "Node");
    Ogre::Entity * entity=sceneMgr->createEntity(name,"sphere.mesh");
    rootNode->attachObject(entity);
    float ratio = weapon->bulletSize/200.0;
    rootNode->scale(ratio,ratio,ratio);
    rootNode->translate(position.getX(),position.getY(),position.getZ());
  }

  btVector3 linvel(){
    return (weapon->bulletSpeed*direction);
  }

  Weapon * weapon;
  btVector3 direction;
  float age;

};
