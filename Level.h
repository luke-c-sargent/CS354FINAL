#pragma once

#include <OgreRoot.h>
#include <vector>
#include <stdio.h>

#include "Tile.h"
//#include "LevelTemplate"

using std::vector;
using std::cout;

//class Tile;
//class LevelTemplate;

class Level
{

  int x,y,z;
  //static class variables

  static vector<Tile*> tile_palette;//actual tiles
  Ogre::SceneManager* smp;
  Ogre::SceneNode * levelNode;

  Tile *tileset;

  //instance variables
  vector<short> tile_map;//3d tile index

  Ogre::Vector3 start;
  //LevelTemplate* levelTemplate;

public:
  Level();
  Level(Ogre::SceneManager* smptr);
  ~Level();
  short getTile(int, int, int);
  Ogre::Vector3 getStart();
  void constructLevel();
  int generateLevel(int xi=1, int yi=1, int zi=1);
};
