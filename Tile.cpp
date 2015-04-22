#include "Tile.h"
#include <string>

const std::string Tile::wallMesh="cube.mesh";

Tile::Tile(){
  levelNode=(Ogre::SceneNode *)2;
}

Tile::Tile(Ogre::SceneNode * lvlptr, Ogre::SceneManager * smptr):
  levelNode(lvlptr),
  smp(smptr)
{
  defaultTile={0,0,0,0,0,0,0,0};
  nTile={1,0,0,0,0,0,0,0};
  cout << "\n\nTILE MADE"<<levelNode<<"\n\n";
}

void Tile::genTile(int id, int xi, int yi, int zi){


  //0: empty tile:
  string tileID = std::to_string(xi)+",";
  tileID += std::to_string(yi)+",";
  tileID += std::to_string(zi);



  //calculate positional offset of tile--x and y plane is floor
  Ogre::Vector3 offset=Ogre::Vector3(xi*5,zi*5,yi*5);

  cout << "generating " << tileID <<"="<<id<<" at ";
  cout << offset.x << "," << offset.y << "," << offset.z <<"\n";

  switch(id){
    case(0):
      break;
    case(1):
      bindTileEntity(defaultTile, tileID,offset);
      break;
   case(2):
    bindTileEntity(nTile, tileID,offset);
    default:
      break;
  }
  //1: no wall
}

void Tile::bindTileEntity(TileInput t, std::string id, Ogre::Vector3 tileOffset){
  //cube transform to rectangular prism floor centered at origin
  cout << "BINDING TILE ";
  cout << id;
  cout <<"\n";
  float cubescale=TSIZE/100.0;// 5m rooms/ 100unit cube
  float xscale=cubescale;
  float yscale=cubescale*(WALLSIZE/TSIZE);
  float zscale=cubescale;

  Ogre::Vector3 cubeScale=Ogre::Vector3(xscale,yscale,zscale);
  Ogre::Quaternion rq;

  Ogre::Vector3 floorTranslate=Ogre::Vector3(0,-((float)y/2.0)+((float)WALLSIZE/2.0),0);
  Ogre::Vector3 ceilingTranslate=floorTranslate*(-1);

  //create root tile entity
  std::string tilename = "Tile_";
  tilename += id;

  Ogre::SceneNode* tileNode = levelNode->createChildSceneNode(tilename + "_node");

  //create floor
  Ogre::SceneNode* fNode=tileNode->createChildSceneNode(tilename+"_f_node");
  Ogre::Entity* entity = smp->createEntity(tilename+"_floor","cube.mesh");
  fNode->attachObject(entity);

  fNode->scale(cubeScale);
  fNode->translate(floorTranslate);
  cout << "entity material\n";
  entity->setMaterialName("PlayerWire2");

  //create ceiling

  //create walls
  //if(t.n){  makeWall("n", tilename + "_north_wall",tileNode,cubeScale);  }
  /*
  if(t.s){  makeWall();  }
  if(t.e){  makeWall();  }
  if(t.w){  makeWall();  }
*/
  tileNode->translate(tileOffset);
}

void Tile::makeWall(string pos, string name,
                    Ogre::SceneNode * tn, Ogre::Vector3 cs){ //rotation

  Ogre::Vector3 tv;
  Ogre::Vector3 rv;
  if(!pos.compare("n")){
    tv=Ogre::Vector3(0,0,-((float)z/2.0)+((float)WALLSIZE/2.0));
  }

  Ogre::SceneNode* node=tn->createChildSceneNode(name+"_node");
  Ogre::Entity* entity = smp->createEntity(name,"cube.mesh");
  node->attachObject(entity);
  node->scale(cs);
}
