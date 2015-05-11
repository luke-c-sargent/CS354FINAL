#pragma once

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <vector>

//#include "Monster.h"

class GameObject;
struct ContactCallback;

class Simulator{
protected:
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    ContactCallback* ccp;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld; //inherits from CollisionWorld
    btConstraintSolver* mConstraintsolver;
    Ogre::SceneManager* sceneMgr;
    std::deque<GameObject*> objList;
public:
    Simulator();
    void stepSimulation(const Ogre::FrameEvent& evt, const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep);
    bool checkHit(int o);
    btDiscreteDynamicsWorld* getWorld();
    void addObject(GameObject* o);
    GameObject * getObjectPtr(int i);
    int getObjectListSize();
    void clearObjectList();

    btCollisionWorld::ClosestRayResultCallback RayCallBack(btVector3 start, btVector3 end);
};
