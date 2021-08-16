#ifndef MainMenuState_H
#define MainMenuState_H

#include "GameState.h"
#include <Ogre.h>

class SQLiteDBManager;

class MainMenuState : 
  public GameState,
  public Ogre::FrameListener
{
public:
	~MainMenuState(){}

	void enter();
	void exit();

	void pause();
	void resume();
	void update( Ogre::Real timeSinceLastFrame );
	void mouseMoved(const OIS::MouseEvent& arg);
	void keyPressed(const OIS::KeyEvent& arg);
	void keyReleased(const OIS::KeyEvent& arg);

	void ProcessEvent(Rocket::Core::Event& event);
	void loadSinglePlayer();

	void createScene(){} // Override me!
	void destroyScene(){}
	static MainMenuState* getSingletonPtr();
	
	void loadCampaignByName(const std::string& faction);

	void onSelectedElementChanged(DataSourceType source, int id);
private:
	MainMenuState();
	MainMenuState( const MainMenuState& ) { }
	MainMenuState & operator = ( const MainMenuState& );

	SQLiteDBManager* mSQLiteDBManager;
	static MainMenuState    *mMainMenuState;

};

#endif