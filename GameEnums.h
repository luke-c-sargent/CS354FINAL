#ifndef __gameenums_h__
#define __gameenums_h__

enum GameState{
	Main = 0,
	Store,
	Play,
	Pause,
	End
};

enum WeaponState{
	Weapon0 = 0,
	Weapon1,
	Weapon2
};

enum PlayerState{
	NoFire = 0,
	Fire,
	Reload
};

#endif