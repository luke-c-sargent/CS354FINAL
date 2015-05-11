#include "MonsterSound.h"
#include <iostream>

MonsterSound::MonsterSound() {
	//Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    if (!loadMedia())
    {
        printf( "Failed to load media!\n");
    }
}
MonsterSound::~MonsterSound() {
	closeMedia();
}

bool MonsterSound::loadMedia()
{

    //Load music
    aggro_sound = Mix_LoadWAV( "found.wav" );
    if( aggro_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void MonsterSound::monster_aggro()
{
    Mix_PlayChannel( -1, aggro_sound, 0 );
}

void MonsterSound::closeMedia()
{
    Mix_FreeChunk( aggro_sound );
    aggro_sound = NULL;
    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}
