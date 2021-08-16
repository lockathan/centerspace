#ifndef GameManager_H
#define GameManager_H
#define OGREKIT_USE_RTSHADER_SYSTEM 1

#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreAL.h>

class GameState;
class InBattleState;
class InBaseState;
class MainMenuState;
class InBaseState;
class CampaignState;
class ProfileManagerState;
class RocketRenderer;
class InputListener;

class GameManager : public Ogre::FrameListener
{
public:
	class EngineUpdateListener
	{
		public:
			virtual void update(Ogre::Real timeSinceLastFrame) = 0;
	};

	~GameManager();

	void startGame();

	void changeState( GameState *gameState );
	void pushState( GameState *gameState );
	void popState();
	void requestShutdown();

	bool setup();

	Ogre::RenderWindow* getRenderWindow();
	Ogre::Root* getRoot();
	// Ogre::FrameListener
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	void updateCoords(float x, float y, float z, std::string type);
	void updateDebugText(std::string text);

	Ogre::Camera* getCamera(std::string name);
	void changeCamera(Ogre::Camera* camera);
	Ogre::SceneManager* getSceneManager();
	GameState* getActiveState();
	
	Ogre::Camera* getFogCamera();
	Ogre::SceneManager* getFogSceneManager();

	void luaDebugReport(const std::string& message, float number);

	OgreAL::SoundManager* getSoundManager();
	static GameManager* getSingletonPtr();
private:
	GameManager();
	GameManager( const GameManager& ) { }
	GameManager & operator = ( const GameManager& );
	
	static GameManager *mGameManager;

	void setupResources();
	
	MainMenuState *mMainMenuState;
	InBaseState *mInBaseState;
	InBattleState *mInBattleState;
	ProfileManagerState* mProfileManagerState;
	GameState *mMissionBriefingState;
	GameState* mActiveState;

	std::vector<GameState*> mStates;

	RocketRenderer *mRocketRenderer;
	InputListener *mInputListener;

	Ogre::Root *mRoot;
	
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneManager;
	std::string mResourcesCfg;
	std::string mPluginsCfg;

	bool mCursorWasVisible;                    // was cursor visible before dialog appeared
	bool mShutDown;
	bool mViewPortCreated;

	std::string mCoords;
	std::string mDebugText;

	std::list<EngineUpdateListener *> mListenerList;
	std::list<EngineUpdateListener *>::iterator mListenerListIterator;

	//fog stuff
	Ogre::SceneManager* mFogSceneManager;
	Ogre::Camera* mFogCamera;
	OgreAL::SoundManager* mSoundManager;

protected:
	virtual bool configure();
	virtual void createFrameListener();
	virtual void createResourceListener();
	virtual void loadResources();

	
	
};
#endif