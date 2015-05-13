#include "Weapon.h"

Weapon::Weapon(const int weapon)
{

	float bulletsize = 0.1;


	//physics geom

	mass = 100;
	reloadTimer = new Ogre::Timer();
	fireTimer = new Ogre::Timer();
	switchTimer = new Ogre::Timer();
	weaponsound = new WeaponSound(0);
	reloadBool = false;
	switchtime = 500;
	bulletSize=0.1;
	bulletSpeed=10.0;
	count=0;
	switch (weapon){
		case WeaponState::Weapon0:
		{
			name = "Weak";
			power = 1;
			ammo = 20;
			total_ammo = 2000;
			ammo_cap = 20;
			total_ammo_cap = 2000;
			firetime = 100;
			reloadtime = 3500;
			bulletSize=0.1;
			mass=10;
			break;
		}
		case WeaponState::Weapon1:
		{
			name = "Medium";
			power = 2;
			ammo = 10;
			total_ammo = 100;
			ammo_cap = 10;
			total_ammo_cap = 100;
			firetime = 1000;
			reloadtime = 3500;
			bulletSize=0.2;
			mass=100;
			break;
		}
		case WeaponState::Weapon2:
		{
			name = "Strong";
			power = 3;
			ammo = 5;
			total_ammo = 50;
			ammo_cap = 5;
			total_ammo_cap = 50;
			firetime = 1500;
			reloadtime = 3500;
			bulletSize=0.3;
			mass=1000;
			break;
		}
	}
	temp_firetime = firetime;
	temp_switchtime = switchtime;
	temp_reloadtime = reloadtime;
	col_shape=new btSphereShape(bulletSize);
}

void Weapon::setSMP(Ogre::SceneManager* smp){
	sceneMgr=smp;
}

Weapon::~Weapon()
{
	delete reloadTimer;
	delete fireTimer;
}

void Weapon::reset_weapon()
{
	ammo = ammo_cap;
	total_ammo = total_ammo_cap;
}

int Weapon::ammo_left()
{
	return ammo;
}

int Weapon::total_ammo_left()
{
	return total_ammo;
}


int Weapon::fire(void)
{
	if (fireTimer->getMilliseconds() >= temp_firetime && switchTimer->getMilliseconds() >= temp_switchtime)
	{
		temp_firetime = firetime;
		temp_switchtime = switchtime;
		if (ammo == 0)
		{
			return Weapon::reload();
		}
		else
		{
			// FIRE
			ammo = ammo - 1;
			total_ammo = total_ammo - 1;
			weaponsound->fire();
			fireTimer->reset();
			return 1;
			//spawnBullet();
		}
	}
	return 2;
}

void Weapon::pause()
{
	temp_firetime = temp_firetime - fireTimer->getMilliseconds();
	temp_switchtime = temp_switchtime - fireTimer->getMilliseconds();
	temp_reloadtime = temp_reloadtime - fireTimer->getMilliseconds();
	weaponsound->pause();
	// cout << "\n pausetime: " << temp_reloadtime << "\n";
}

void Weapon::unpause()
{
	reloadTimer->reset();
	fireTimer->reset();
	switchTimer->reset();
	weaponsound->unpause();
}

void Weapon::cancel_reload(void)
{
	reloadBool = false;
	weaponsound->switch_weapon();
}

Bullet* Weapon::spawnBullet(btVector3 playerPos, btVector3 dir, Simulator* simulator){

	Bullet * b = new Bullet(this,sceneMgr,simulator,playerPos, dir,std::to_string(count));
	count++;
	if(count<0)
		count=0;
	return b;
}

bool Weapon::reload(void)
{
	// cout << "\n can we reload? " << reloadTimer->getMilliseconds() << " vs " << temp_reloadtime;
	if (ammo == ammo_cap || total_ammo <= 0)
		return true;
	else if (reloadBool == false && switchTimer->getMilliseconds() >= temp_switchtime)
	{
		temp_switchtime = switchtime;
		weaponsound->reload();
		reloadTimer->reset();
		reloadBool = true;
	}
	else if (reloadTimer->getMilliseconds() >= temp_reloadtime)
	{
		temp_reloadtime = reloadtime;
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

void Weapon::switch_weapon(void)
{
	switchTimer->reset();
}
