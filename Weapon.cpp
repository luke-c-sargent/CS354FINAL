#include "Weapon.h"

Weapon::Weapon(const int weapon)
{
	reloadTimer = new Ogre::Timer();
	fireTimer = new Ogre::Timer();
	reloadBool = false;
	switch (weapon){
		case WeaponState::Weapon0:
		{
			power = 1;
			ammo = 20;
			total_ammo = 2000;
			ammo_cap = 20;
			total_ammo_cap = 2000;
			firetime = 100;
			reloadtime = 2000;
			break;
		}
		case WeaponState::Weapon1:
		{
			power = 2;
			ammo = 10;
			total_ammo = 100;
			ammo_cap = 10;
			total_ammo_cap = 100;
			firetime = 500;
			reloadtime = 2500;
			break;
		}
		case WeaponState::Weapon2:
		{
			power = 3;
			ammo = 5;
			total_ammo = 50;
			ammo_cap = 5;
			total_ammo_cap = 50;
			firetime = 1000;
			reloadtime = 3000;
			break;
		}
	}
}

Weapon::~Weapon()
{
	delete reloadTimer;
	delete fireTimer;
}

int Weapon::ammo_left()
{
	return ammo;
}

int Weapon::total_ammo_left()
{
	return total_ammo;
}


bool Weapon::fire(void)
{
	if (ammo == 0)
	{
		return Weapon::reload();
	}
	else if (fireTimer->getMilliseconds() >= firetime)
	{
		// FIRE
		ammo = ammo - 1;
		total_ammo = total_ammo - 1;
		fireTimer->reset();
	}
	return true;
}

bool Weapon::reload(void)
{
	if (reloadBool == false)
	{
		reloadTimer->reset();
		reloadBool = true;
	}
	else if (ammo == ammo_cap || total_ammo <= 0)
		return true;
	else if (reloadTimer->getMilliseconds() >= reloadtime)
	{
		reloadBool = false;
		reloadTimer->reset();
		if (total_ammo >= ammo_cap)
			ammo = ammo_cap;
		else
			ammo = total_ammo;
		return true;
	}
	return false;
}