#ifndef ProfileManagerState_H
#define ProfileManagerState_H

#include <States/GameState.h>

class PlayerProfile;
class SQLiteDBManager;

class ProfileManagerState : public GameState
{
public:
	~ProfileManagerState();

	void enter();
	void exit();

	void pause(){}
	void resume(){}
	
	void createScene(){}
	void destroyScene(){}
	void update(Ogre::Real timeSinceLastFrame){}

	PlayerProfile* getCurrentPlayer();
	void setCurrentPlayer(PlayerProfile* player);

	void ProcessEvent(Rocket::Core::Event& e);
	void onSelectedElementChanged(DataSourceType source, int id);

	void newProfile();
	void selectProfile(const std::string& profileId);

	static ProfileManagerState* getSingletonPtr();
private:
	ProfileManagerState();
	PlayerProfile* mCurrentPlayer;
	
	SQLiteDBManager* mSQLiteDBManager;
	int mSelectedProfileId;
	static ProfileManagerState* mProfileManagerState;
};
#endif