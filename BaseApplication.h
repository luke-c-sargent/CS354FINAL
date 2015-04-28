#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

//#include <CEGUI/CEGUI.h>
//#include <CEGUI/RendererModules/Ogre/Renderer.h> //from 0.8 it's just Ogre/Renderer.h

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include "GameEnums.h"
#include "Monster.h"

#include <string.h>

#include "Level.h"
#include "Weapon.h"
#include <stdio.h>

using std::cout;

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication(void);
    ~BaseApplication(void);
    void go(void);

protected:
    bool setup();
    bool configure(void);
    void chooseSceneManager(void);
    void createCamera(void);
    void createFrameListener(void);
    void createScene(void);
    void destroyScene(void);
    void createMenu(void);
    void createViewports(void);
    void setupResources(void);
    void createResourceListener(void);
    void loadResources(void);

    // Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root *mRoot;

    //ye olde camera styff
    Ogre::Camera* mCamera;
    Ogre::Vector3 cameraPos;
    Ogre::Vector3 cameraDir;

    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::OverlaySystem *mOverlaySystem;

    // OgreBites
    OgreBites::InputContext mInputContext;
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;

    // GUI Stuff
    OgreBites::ParamsPanel* scoreboard;    // displays scores
    OgreBites::Button* play_button;
    OgreBites::Button* store_button;
    OgreBites::Button* quit_button;

    // States
    GameState state;
    PlayerState playerState;
    WeaponState weapon;

    virtual void buttonHit (OgreBites::Button *button) override;


    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    float mx, my, theta, phi, mSensitivity; // theta up/down, phi l/r
    bool up,down,left,right;

    void processInput();

    //Level
    Level * level;

    // Weapons
    Weapon * weapon1;
    Weapon * weapon2;
    Weapon * weapon3;
    Weapon ** equippedweapon;

    //Monster Code

    int judgement_day;
    int num_monsters;
    int spawn_point; //will be between 1 - # of spawn pts
    Monster* monster_list[3]; //change to # of monster variable


    //============

};
#endif // #ifndef __BaseApplication_h_
