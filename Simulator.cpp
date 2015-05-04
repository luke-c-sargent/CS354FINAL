#include "Simulator.h"
#include "GameObject.h"
#include "Weapon.h"
#include <stdio.h>
#include <vector>

using std::vector;

using std::cout;

struct ContactCallback:public btCollisionWorld::ContactResultCallback
{
  GameObject* ptr, *a,*b;
  ContactCallback(GameObject* ptri) {ptr=ptri;}

  btScalar addSingleResult(btManifoldPoint& cp,
      const btCollisionObjectWrapper* colObj0Wrap,
      int partId0,
      int index0,
      const btCollisionObjectWrapper* colObj1Wrap,
      int partId1,
      int/*t*/ index1)
  {
      //cout << a->getName() << " hit "<<b->getName()<< "\n";
      //return 666;
    b->hit=true;
  }
  void setAB(GameObject* ai,GameObject* bi){
    a=ai;
    b=bi;
  }
};

Simulator::Simulator(){
    //initialize bullet world
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver,collisionConfig);
    //create world
    dynamicsWorld->setGravity(btVector3(0,-100, 0));//386.09 inches/s^2 = 9.8m/s^2

    //contact callback context
    ccp=new ContactCallback(objList[0]);

}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    cout << objList.size()<<"\n";
    //cout << "about to step\n";
    dynamicsWorld->stepSimulation(elapsedTime,maxSubSteps,fixedTimestep);
    //cout << "stepped\n";

    GameObject * a = objList[1];//level
    GameObject * p = objList[0];//player
    p->printpos();
    //update in ogre
    vector<int> deadBullets;
    for(int i=0; i < objList.size();i++){//step through all objects
        if((objList[i]->getName()).compare("bullet")==0){
          ccp->setAB(a,objList[i]);
          dynamicsWorld->contactPairTest(a->getBody(),objList[i]->getBody(),*ccp);

          if(((Bullet*)objList[i])->hit)
            deadBullets.push_back(i);
          else
            objList[i]->updateTransform();
        }
        else {
          objList[i]->updateTransform();
        }

        //cout << objList[i]->getName()<<":";
        //objList[i]->printpos();

    }
    //cout << "for list one ended\n";
    for(int i=deadBullets.size()-1; i >=0;i--){
      Bullet * go=(Bullet*)objList[deadBullets[i]];
      //delete objList[deadBullets[i]];
      go->getSMP()->destroySceneNode(go->getNode());
      objList.erase(objList.begin()+deadBullets[i]);
    }
    //cout << "end stepsim\n";
}

btDiscreteDynamicsWorld* Simulator::getWorld(){
    return dynamicsWorld;
}

void Simulator::addObject (GameObject* o) {
  //cout << "\nADDING"<< o<<"\n";
    objList.push_back(o);
    //cout << "\ngetting body...   \n";
    dynamicsWorld->addRigidBody(o->getBody());
    //cout << " ... got body\n";
}

GameObject * Simulator::getObjectPtr(int i){
        return objList[i];
}

int Simulator::getObjectListSize(){
  return objList.size();
}