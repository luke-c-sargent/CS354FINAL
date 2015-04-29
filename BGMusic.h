#pragma once

#include "GameObject.h"
// #include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

using std::cout;

class BGMusic{
public:  
	BGMusic();
	virtual ~BGMusic();
	bool loadMedia();
	void closeMedia();
	void playOrPause();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The music that will be played
	Mix_Music *bgmusic = NULL;
};
