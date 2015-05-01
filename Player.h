#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include "GameEnums.h"

class Player:public GameObject{
public:
    btScalar w,l,h;
    btVector3 ori,playerLV;
    Player(Ogre::SceneManager* smp,Ogre::String name="player", Ogre::Vector3 startPos=Ogre::Vector3(0,1,0));
    void setPos(Ogre::Vector3 pos);
    void setPos(btVector3 pos);
    btScalar getX();
    btScalar getY();
    btScalar getZ();
    void setLV(btVector3 lvin);
    void setLV(Ogre::Vector3 lvin);

    // PlayerState playerState;
    // WeaponState weapon;
};
