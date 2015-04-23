#include "Level.h"
#include <stdlib.h>

Level::Level(){}

Level::Level(Ogre::SceneManager* smptr)
{
  x=y=z=0;
  smp=smptr;
  levelNode = smp->getRootSceneNode()->createChildSceneNode("Level_node");
  tileset=new Tile(levelNode, smp);
  cout << "\n\nLEVEL MADE: "<<levelNode<<"\n\n";

}

short Level::getTile(int xi, int yi, int zi)
{
  return tile_map[xi + yi*xi + zi*yi*xi]; //linear to 3d storage
}

Ogre::Vector3 Level::getStart()
{
  return start;
}

//create entities and such from level data
void Level::constructLevel(){
  //Ogre::Vector3 offset;//where to put tile
  //std::string tileID="defaultID";

  cout << "\nCONSTRUCTING LEVEL\n";
  for(int i=0; i < x; i++){
    for(int j=0; j < y; j++){
      for(int k=0; k < z; k++){
        tileset->genTile(getTile(i,j,k),i,j,k);

      }
    }
  }
}//void constructLevel()

//simple test platform
int Level::generateLevel(int xi, int yi, int zi)
{
  x=xi;y=yi;z=zi;
  cout << "\nGENERATING LEVEL\n";
  for(int i=0; i < xi;i++){
    for(int j=0; j < yi;j++){
      for(int k=0; k < zi;k++){
        short tile = rand()%13+1;
        tile_map.push_back(tile);
  } } }
  cout << "\n\n LEVEL GENERATED \n\n";
}

Level::~Level(){
  delete(tileset);
}
