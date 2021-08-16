#include <Managers/GameManager.h>
#include <Managers/InGameCamera.h>
#include <Managers/LuaStateManager.h>
#include <Managers/PhysicsManager.h>
#include <Managers/SQLiteDBManager.h>
#include <States/GameState.h>
#include <States/InBaseState.h>
#include <States/InBattleState.h>
#include <States/MainMenuState.h>
#include <States/MissionBriefingState.h>
#include <States/ProfileManagerState.h>
#include <GUI/RocketRenderer.h>
#include <GUI/TextRenderer.h>
#include <OgreSceneManager.h>
#include <Input/InputListener.h>
#include <Repositories/RepositoryManager.h>
#include <Ogre.h>
#include <OgreAL.h>
#include <stdlib.h>
#include <time.h>

using namespace Ogre;

GameManager* GameManager::mGameManager;

GameManager::GameManager() :
	mMainMenuState(0),
	mInBaseState(0),
	mInBattleState(0),
	mProfileManagerState(0),
	mMissionBriefingState(0),
	mActiveState(0),
	mRocketRenderer(0),
	mInputListener(0),
	mRoot(0),
	mWindow(0),
	mSceneManager(0),
	mResourcesCfg(""),
	mPluginsCfg(""),
	mCursorWasVisible(false),
	mShutDown(false),
	mViewPortCreated(false),
	mCoords(""),
	mDebugText(""),
	mFogSceneManager(0),
	mFogCamera(0)
{
	srand (time(NULL));
}

GameManager::~GameManager() 
{
	// Clean up all the states

	

	while( !mStates.empty() ) 
	{
		mStates.back()->exit();
		mStates.pop_back();
	}

	if( mMainMenuState ) 
	{
		delete mMainMenuState;
		mMainMenuState = 0;
	}

	if( mInBaseState ) 
	{
		delete mInBaseState;
		mInBaseState  = 0;
	}

	if( mMissionBriefingState ) 
	{
		delete mMissionBriefingState;
		mMissionBriefingState = 0;
	}

	if( mInBattleState ) 
	{
		delete mInBattleState;
		mInBattleState = 0;
	}
	if( mProfileManagerState )
	{
		delete mProfileManagerState;
		mProfileManagerState = 0;
	}

	if(mInputListener)
		delete mInputListener;
	if(mRocketRenderer)
		delete mRocketRenderer;
	
	if(SQLiteDBManager::getSingletonPtr())
		delete SQLiteDBManager::getSingletonPtr();
	
	if(RepositoryManager::getSingletonPtr())
		delete RepositoryManager::getSingletonPtr();

	if(InGameCamera::getSingletonPtr())
		delete InGameCamera::getSingletonPtr();

	if(mSceneManager)
	{
		//mSceneManager->destroyAllAnimations();
		//mSceneManager->destroyAllBillboardChains();
		//mSceneManager->destroyAllBillboardSets();
		//mSceneManager->destroyAllCameras();
		//mSceneManager->destroyAllEntities();
		//mSceneManager->destroyAllLights();
		//mSceneManager->destroyAllManualObjects();
		//mSceneManager->destroyAllStaticGeometry();
		delete mSceneManager;
		mSceneManager = 0;
	}

	if(mFogSceneManager)
	{
		//mFogSceneManager->destroyAllAnimations();
		//mFogSceneManager->destroyAllBillboardChains();
		//mFogSceneManager->destroyAllBillboardSets();
		//mFogSceneManager->destroyAllCameras();
		//mFogSceneManager->destroyAllEntities();
		//mFogSceneManager->destroyAllLights();
		//mFogSceneManager->destroyAllManualObjects();
		//mFogSceneManager->destroyAllStaticGeometry();
		//delete mFogSceneManager;
		//mFogSceneManager = 0;
	}
	if(mWindow)
		delete mWindow;

}

void GameManager::startGame() {

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
}

bool GameManager::configure()
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	
	mRoot = Ogre::Root::getSingletonPtr();
	if(!mRoot)
		mRoot = new Ogre::Root(mPluginsCfg,"ogre.cfg");

	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		
		
		mWindow = mRoot->initialise(true, "Center Space");

		return true;
	}
	else
	{
		return false;
	}
}

bool GameManager::setup() 
{
	

	bool carryOn = configure();
	if (!carryOn) return false;
	
	setupResources();

	// Set default mipmap level (NB some APIs ignore this)
	//Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();
	
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mInputListener = InputListener::getSingletonPtr();
	mRocketRenderer = RocketRenderer::getSingletonPtr();
	
	mMainMenuState = MainMenuState::getSingletonPtr();
	mInBaseState  = InBaseState::getSingletonPtr();
	mMissionBriefingState = MissionBriefingState::getSingletonPtr();
	mInBattleState = InBattleState::getSingletonPtr();
	mProfileManagerState = ProfileManagerState::getSingletonPtr();
	createFrameListener();

	mListenerList.push_back(mMainMenuState);
	mListenerList.push_back(mMissionBriefingState);
	mListenerList.push_back(mInBattleState);
	mListenerList.push_back(mInBaseState);
	mListenerList.push_back(mInputListener);
	mListenerList.push_back(PhysicsManager::getSingletonPtr());
	mListenerList.push_back(InGameCamera::getSingletonPtr());

	RepositoryManager* repositoryManager = RepositoryManager::getSingletonPtr();
	//repositoryManager->loadData();
	repositoryManager->initialize();
	LuaStateManager::getSingletonPtr()->loadScript(mProfileManagerState, "repositories.lua");
	LuaStateManager::getSingletonPtr()->executeRepositoryLoadFunction(mProfileManagerState, "load");
	repositoryManager->cleanUp();
	new TextRenderer();
	TextRenderer::getSingleton().addTextBox("txtFPS", "0", 12, 10, 100, 20, Ogre::ColourValue::Green);
	TextRenderer::getSingleton().addTextBox("txtCoords", "0", 115, 10, 100, 20, Ogre::ColourValue::Green);
	TextRenderer::getSingleton().addTextBox("txtDebug", "0", 415, 10, 100, 20, Ogre::ColourValue::Green);

	mCoords = "";
	
	mFogSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC,"FogSceneManager");
	mFogCamera = mFogSceneManager->createCamera("FogCamera");
	
	LuaStateManager* mLuaStateManager = LuaStateManager::getSingletonPtr();
	mSoundManager = new OgreAL::SoundManager();
	mRocketRenderer->initMenuSound();
	changeState(mProfileManagerState);
	//LuaStateManager::getSingletonPtr()->setReadyForReset(false);
	
	return true;
}

void GameManager::setupResources() 
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load( "resources.cfg" );

	// Go through all settings in the file
	ConfigFile::SectionIterator itSection = cf.getSectionIterator();

	String sSection, sType, sArch;
	while( itSection.hasMoreElements() ) 
	{
		sSection = itSection.peekNextKey();

		ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
		ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
		while( itSetting != mapSettings->end() ) 
		{
			sType = itSetting->first;
			sArch = itSetting->second;
							
			ResourceGroupManager::getSingleton().addResourceLocation(
				sArch, sType, sSection );

			++itSetting;
		}
	}
}

void GameManager::changeState( GameState *gameState ) 
{
	// Cleanup the current state
	if( !mStates.empty() )
	{
		mStates.back()->exit();
		mStates.pop_back();
	}
	
	// Store and init the new state
	mStates.push_back( gameState );
	//LuaStateManager::getSingletonPtr()->resetState();
	mRocketRenderer->setActiveState(gameState);
	mStates.back()->enter();
	mActiveState = gameState;
	
}

void GameManager::pushState( GameState *gameState ) 
{
	// Pause current state
	if( !mStates.empty() ) 
	{
		mStates.back()->pause();
	}

	// Store and init the new state
	mStates.push_back( gameState );
	mStates.back()->enter();
}

void GameManager::popState() 
{
	// Cleanup the current state
	if( !mStates.empty() )
	{
		mStates.back()->exit();
		mStates.pop_back();
	}

	// Resume previous state
	if( !mStates.empty() )
	{
		mStates.back()->resume();
	}
}

void GameManager::requestShutdown() 
{
	mShutDown = true;
}


//-------------------------------------------------------------------------------------
void GameManager::createFrameListener()
{
	//Set initial mouse clipping size
	mInputListener->windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mInputListener);

	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void GameManager::createResourceListener()
{

}
//-------------------------------------------------------------------------------------
void GameManager::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");

	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool GameManager::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;

	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;
	
	mInputListener->update(evt.timeSinceLastFrame);

	for(mListenerListIterator = mListenerList.begin();mListenerListIterator != mListenerList.end();++mListenerListIterator)
	{
		(*mListenerListIterator)->update(evt.timeSinceLastFrame);
	}

	return true;
}

//-------------------------------------------------------------------------------------
bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	TextRenderer::getSingleton().setText("txtFPS", SQLiteDBManager::convertFloatToString(mWindow->getAverageFPS()) );
	//TextRenderer::getSingleton().setText("txtCoords", mCoords );
	//TextRenderer::getSingleton().setText("txtDebug", mDebugText );

	if(mWindow->isClosed())
		return false;

	if(mShutDown)
	{
		return false;
	}

	mInputListener->update(evt.timeSinceLastFrame);

	for(mListenerListIterator = mListenerList.begin();mListenerListIterator != mListenerList.end();++mListenerListIterator)
	{
		(*mListenerListIterator)->update(evt.timeSinceLastFrame);
	}

	return true;
}

void GameManager::updateCoords(float x, float y, float z, std::string type)
{
	mCoords = "X:" + SQLiteDBManager::convertFloatToString(x) + " Y:" + SQLiteDBManager::convertFloatToString(y) + " Z:" + SQLiteDBManager::convertFloatToString(z) + "Type:" + type;
}

void GameManager::updateDebugText(std::string text)
{
	mDebugText = text;
}

Ogre::RenderWindow* GameManager::getRenderWindow()
{
	return mWindow;
}


Ogre::Root* GameManager::getRoot()
{
	return mRoot;
}

Ogre::SceneManager* GameManager::getSceneManager()
{
	return mSceneManager;
}

void GameManager::changeCamera(Ogre::Camera* camera)
{
	
	Viewport* vp = camera->getViewport();
	if(!vp)
	{
		vp = mWindow->addViewport(camera);
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	}
	else
	{
		vp->setCamera(camera);
	}	

	camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	
}

Camera* GameManager::getCamera(String name)
{
	if(mSceneManager->hasCamera(name))
	{
		Camera* cam = mSceneManager->getCamera(name);
		if(cam->getParentSceneNode())
		{
			if(cam->getParentSceneNode()->getName() != "CamNode")
			{
				SceneNode* mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CamNode", Ogre::Vector3(0, 0, 0));
				mCamNode->attachObject(cam);
			}
		}
		else
		{
			SceneNode* mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CamNode", Ogre::Vector3(0, 0, 0));
			mCamNode->attachObject(cam);
		}
		cam->getViewport()->setCamera(cam);
		
		return cam;
	}

	Camera* cam = mSceneManager->createCamera(name);
	Viewport* vp = mWindow->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	SceneNode* mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CamNode", Ogre::Vector3(0, 0, 0));
	mCamNode->attachObject(cam);
	return cam;
}

GameState* GameManager::getActiveState()
{
	return mActiveState;
}

Ogre::Camera* GameManager::getFogCamera()
{
	return mFogCamera;
}

Ogre::SceneManager* GameManager::getFogSceneManager()
{
	return mFogSceneManager;
}

void GameManager::luaDebugReport(const std::string& message, float number)
{
	mDebugText = message + " " + SQLiteDBManager::convertFloatToString(number);
	LogManager::getSingletonPtr()->getDefaultLog()->logMessage(mDebugText);
}

OgreAL::SoundManager* GameManager::getSoundManager()
{
	return mSoundManager;
}

GameManager* GameManager::getSingletonPtr() 
{
	if( !mGameManager ) 
	{
		mGameManager = new GameManager();
	}

	return mGameManager;
}
