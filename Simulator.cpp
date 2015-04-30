#include "Simulator.h"
#include "GameObject.h"
#include <stdio.h>

using std::cout;

Simulator::Simulator(){
    //initialize bullet world
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver,collisionConfig);
    //create world
    dynamicsWorld->setGravity(btVector3(0,-9.8, 0));//386.09 inches/s^2 = 9.8m/s^2
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    dynamicsWorld->stepSimulation(elapsedTime,maxSubSteps,fixedTimestep);
    //update in ogre
    for(int i=0; i < objList.size();i++){
        objList[i]->updateTransform();
        cout << objList[i]->getName()<<":";
        objList[i]->printpos();

    }
}

btDiscreteDynamicsWorld* Simulator::getWorld(){
    return dynamicsWorld;
}

void Simulator::addObject (GameObject* o) {
  cout << "\nADDING"<< o<<"\n";
    objList.push_back(o);
    cout << "\ngetting body...   \n";
    dynamicsWorld->addRigidBody(o->getBody());
    cout << " ... got body\n";
};
