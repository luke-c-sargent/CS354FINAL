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
  sTile={0,1,0,0,0,0,0,0};
  eTile={0,0,1,0,0,0,0,0};
  wTile={0,0,0,1,0,0,0,0};
  n_eTile={1,0,1,0,0,0,0,0};
  n_wTile={1,0,0,1,0,0,0,0};
  s_eTile={0,1,1,0,0,0,0,0};
  s_wTile={0,1,0,1,0,0,0,0};

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
    break;
   case(3):
    bindTileEntity(sTile, tileID,offset);
    break;
   case(4):
    bindTileEntity(eTile, tileID,offset);
    break;
   case(5):
     bindTileEntity(wTile, tileID,offset);
     break;
   case(6):
    bindTileEntity(n_eTile, tileID,offset);
    break;
  case(7):
   bindTileEntity(n_wTile, tileID,offset);
   break;
   case(8):
    bindTileEntity(s_eTile, tileID,offset);
    break;
    case(9):
     bindTileEntity(s_wTile, tileID,offset);
     break;
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
  entity->setMaterialName("PlayerWire");

  //create ceiling

  //create walls
  if(t.n){  makeWall("n", tilename + "_north_wall",tileNode,cubeScale);  }
  if(t.s){  makeWall("s", tilename + "_south_wall",tileNode,cubeScale);  }
  if(t.e){  makeWall("e", tilename + "_east_wall",tileNode,cubeScale);  }
  if(t.w){  makeWall("w", tilename + "_west_wall",tileNode,cubeScale);  }
/*  if(t.w){  makeWall();  }
*/
  tileNode->translate(tileOffset);
}

void Tile::makeWall(string pos, string name,
                    Ogre::SceneNode * tn, Ogre::Vector3 cs){ //rotation

  Ogre::Vector3 tv;
  Ogre::Quaternion rq;
  Ogre::Vector3 xv,yv,zv;
  xv=Ogre::Vector3(1,0,0);
  yv=Ogre::Vector3(0,1,0);
  zv=Ogre::Vector3(0,0,1);

  if(!pos.compare("n")){
    tv=Ogre::Vector3(0,0,((float)z/2.0)-((float)WALLSIZE/2.0));
    rq=Ogre::Quaternion(Ogre::Degree(90),xv);
  }
  else if(!pos.compare("s")){
    tv=Ogre::Vector3(0,0,-1*((float)z/2.0)+((float)WALLSIZE/2.0));
    rq=Ogre::Quaternion(Ogre::Degree(90),xv);
  }
  else if(!pos.compare("e")){
    tv=Ogre::Vector3(-1*((float)x/2.0)+((float)WALLSIZE/2.0),0,0);
    rq=Ogre::Quaternion(Ogre::Degree(90),zv);
  }
  else if(!pos.compare("w")){
    tv=Ogre::Vector3(((float)x/2.0)-((float)WALLSIZE/2.0),0,0);
    rq=Ogre::Quaternion(Ogre::Degree(90),zv);
  }


  Ogre::SceneNode* node=tn->createChildSceneNode(name+"_node");
  Ogre::Entity* entity = smp->createEntity(name,"cube.mesh");
  node->attachObject(entity);
  entity->setMaterialName("PlayerWire2");
  node->scale(cs);
  node->translate(tv);
  node->rotate(rq);
}
