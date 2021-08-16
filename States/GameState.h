#ifndef GameState_H
#define GameState_H

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <States/GameState.h>
#include <Entities/DataProvider.h>
#include <Entities/Enums.h>
#include <GUI/RocketRenderer.h>
#include <Input/InputListener.h>
#include <OgreFrameListener.h>
#include <deque>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>

class lua_State;

class GameState : 
	public GameManager::EngineUpdateListener, 
	public InputListener::Listener, 
	public Rocket::Core::EventListener
{
public:
	virtual ~GameState();

	virtual void enter()  = 0;
	virtual void exit()   = 0;

	virtual void pause()  = 0;
	virtual void resume() = 0;

	void changeState( GameState *state );
	void pushState( GameState *state );
	void popState();
	void requestShutdown();
	bool isEnabled();
	void setEnabled(bool enabled);
	virtual void createScene() = 0; // Override me!
	virtual void destroyScene() = 0;
	virtual void ProcessEvent(Rocket::Core::Event& event){}
	virtual void onSelectedElementChanged(DataSourceType source, int id){}
	virtual void mouseMoved(const OIS::MouseEvent& arg){}
	virtual void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id){}
	virtual void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id){}
	virtual void keyPressed(const OIS::KeyEvent& arg){}
	virtual void keyReleased(const OIS::KeyEvent& arg){}
	
	Ogre::SceneNode* getCamNode();
	std::deque<std::string>& getLoadedScripts();
	lua_State* getLuaState();
protected:
	GameState();

	Ogre::Camera* getCamera(std::string name);
	RocketRenderer *mRocketRenderer;
	GameManager* mGameManager;

	Ogre::SceneManager* mSceneManager;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCamNode;
	bool mIsEnabled;
	std::deque<std::string> mLoadedLuaScripts;
	lua_State* mLuaState;
private:
	GameState( const GameState& ) { }
	GameState & operator = ( const GameState& );
	
	
};
#endif