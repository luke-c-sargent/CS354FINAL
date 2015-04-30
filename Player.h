#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include "GameEnums.h"

class Player:public GameObject{
public:
    btScalar w,l,h;
    btVector3 ori,playerLV;
    Player(Ogre::SceneManager* smp,Ogre::String name="player");
    void setPos(Ogre::Vector3 pos);
    void setPos(btVector3 pos);
    btScalar getX();
    btScalar getY();
    btScalar getZ();

    // PlayerState playerState;
    // WeaponState weapon;
};
