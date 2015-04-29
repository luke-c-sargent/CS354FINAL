#pragma once

#include "GameObject.h"
// #include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

using std::cout;

class WeaponSound{
public:  
	WeaponSound(const int weapon);
	virtual ~WeaponSound();
	bool loadMedia();
	void closeMedia();
	void fire();
	void reload();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The music that will be played
	Mix_Chunk *gunshot = NULL;
	Mix_Chunk *reload_sound = NULL;
};
