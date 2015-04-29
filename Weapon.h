#pragma once

#include <Ogre.h>
#include <GameObject.h>
#include "GameEnums.h"
#include <stdio.h>
#include "WeaponSound.h"

using std::cout;

class Weapon{
public:
    Weapon(const int weapon);
    ~Weapon(void);

    bool fire();
    bool reload();
    void switch_weapon();
    void cancel_reload();
    int ammo_left();
    int total_ammo_left();

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