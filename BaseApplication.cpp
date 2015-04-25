#include "BaseApplication.h"

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    up(0),
    down(0),
    left(0),
    right(0),
    state(GameState::Main),
    play_button(0),
    quit_button(0),
    mx(0),my(0),theta(0),phi(0)
{
  cameraDir=Ogre::Vector3(0,-1,-1);
  cameraPos=Ogre::Vector3(0,0,0);
  mSensitivity=0.1;
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

void BaseApplication::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6f,0.6f,0.6f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  //level making
  level->constructLevel();

    //lighting
/*
	Ogre::Light * light = mSceneMgr->createLight("light1");
	light->setPosition(Ogre::Vector3(0,500,500));
	light->setDiffuseColour(1.0, 1.0, 1.0);
	light->setSpecularColour(1.0, 0.0, 0.0);*/
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(cameraPos);
    // Look back along -Z
    mCamera->lookAt(cameraPos+cameraDir);
    mCamera->setNearClipDistance(1);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  	mInputContext.mMouse = mMouse;
  	mInputContext.mKeyboard = mKeyboard;

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();
    createFrameListener();
    createMenu();

    return true;
};
//-------------------------------------------------------------------------------------
void BaseApplication::createMenu()
{
    // Setup GUI
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    play_button = mTrayMgr->createButton(OgreBites::TL_CENTER, "Play", "Play");
    quit_button = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Quit");
}
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    //need to process input
    //if(state==Play)
        processInput();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
    }
    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up


    if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    if (state == Play)
    {
    /*    else if(arg.key == OIS::KC_9)
            {
                sounds->pauseMusic();
            }
        else if(arg.key == OIS::KC_M)
            {
                sounds->playEffect(1);
            }
        else if(arg.key == OIS::KC_1)
            {
                sounds->enableSound();
            }*/
        if (arg.key == OIS::KC_P)
        {
            state = Pause;
            mTrayMgr->showCursor();
            play_button = mTrayMgr->createButton(OgreBites::TL_CENTER, "Resume", "Resume");
            quit_button = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Quit");
        }

        else if(arg.key == OIS::KC_W){
             up=true;
        }
        else if(arg.key == OIS::KC_A){
            left=true;
        }
        else if(arg.key == OIS::KC_S){
            down=true;
        }
        else if(arg.key == OIS::KC_D){
            right=true;
        }
    }

    else if (state == Pause)
    {
        if (arg.key == OIS::KC_P)
        {
            mTrayMgr->clearTray(OgreBites::TL_CENTER);
            mTrayMgr->destroyAllWidgets();
            mTrayMgr->hideCursor();
            state = Play;
        }
    }

    //mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
    //mCameraMan->injectKeyUp(arg);

    if(arg.key == OIS::KC_W){
      up=false;
    }
    else if(arg.key == OIS::KC_A){
        left=false;

    }
    else if(arg.key == OIS::KC_S){
        down=false;
    }
    else if(arg.key == OIS::KC_D){
        right=false;
    }

    return true;
}
void BaseApplication::buttonHit (OgreBites::Button *button)
{
    if (button == play_button)
    {
        // Create the scene
        mTrayMgr->clearTray(OgreBites::TL_CENTER);
        mTrayMgr->destroyAllWidgets();
        if (state == Main)
        {
            //generate level
            level=new Level(mSceneMgr);
            level->generateLevel(4,3,1);
            // Create the scene
            createScene();
        }
        mTrayMgr->hideCursor();
        state = Play;
    }
    else if (button == quit_button)
    {
        mShutDown = true;
    }
}
bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if(state==Play){
    mx=(float)arg.state.X.rel;
    my=(float)arg.state.Y.rel;
    }
    if (state == Main || state == Pause){
        mTrayMgr->injectMouseMove(arg);
      }
      //else
      //mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    //mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void BaseApplication::processInput(){
  //move camera with mouse
  phi-=mx*mSensitivity;
  theta-=my*mSensitivity;//to not invert mouse
  float r90=3.1400/2.0; // just shy of 90*
  if(theta>r90){
      cout << "too big at" << theta;
      theta=r90;}
  else if(theta<=-1*r90){
      cout << "too small at" <<theta;
      theta=-1*r90;}

  cameraDir=Ogre::Vector3(sin(phi)*cos(theta),sin(phi)*sin(theta),cos(phi));

  //move camera with keyboard
  float cameraSpeed=0.01;
  if(up){
    cameraPos+=cameraSpeed*cameraDir;
  }
  if(down)
    cameraPos-=cameraSpeed*cameraDir;
  if(left){
    cameraPos.x-=cameraSpeed*cos(phi+90);
    cameraPos.z-=cameraSpeed*sin(phi+90);
  }
  if(right){
      cameraPos.x+=cameraSpeed*cos(phi+90);
      cameraPos.z+=cameraSpeed*sin(phi+90);
  }

  mCamera->setPosition(cameraPos);
  mCamera->lookAt(cameraPos+cameraDir);

  //cout << "t,p: " << theta << " " << phi << "\n";
  cout << "pos: {"<<cameraPos.x<<","<<cameraPos.y<<","<<cameraPos.z<<"}\n";
  cout << "dir: {"<<cameraDir.x<<","<<cameraDir.y<<","<<cameraDir.z<<"}\n";

  //mCamera->rotate(Ogre::Quaternion(Ogre::Degree(0.1),Ogre::Vector3(0,1,0)));
  mx=0;
  my=0;

}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BaseApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
