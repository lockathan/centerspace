#ifndef InBaseState_h
#define InBaseState_h

#include "GameState.h"

class FactionTemplate;
class PlayerProfile;
class CampaignState;
class SQLiteDBManager;
class CampaignMissionManager;

enum MissionType
{
	RANDOM_MISSION = 0,
	STORY_MISSION = 1
};

class InBaseState : public GameState 
{
public:
	~InBaseState() { }

	void enter();
	void exit();

	void pause();
	void resume();
	void update( Ogre::Real timeSinceLastFrame );
	void createScene();
	void destroyScene();

	FactionTemplate* getFactionTemplate();
	PlayerProfile* getPlayerProfile();
	CampaignState* getCampaignState();

	void setFactionTemplate(FactionTemplate* faction);
	void setPlayerProfile(PlayerProfile* player);
	void setCampaignState(CampaignState* campaign);

	void loadCampaignState(long playerProfileId, int factionId);
	
	void loadMainBaseScreen();
	
	void startMission();

	void ProcessEvent(Rocket::Core::Event& event);

	void onSelectedElementChanged(DataSourceType source, int id);

	static InBaseState* getSingletonPtr();
	
private:
	InBaseState();
	InBaseState( const InBaseState& ) { }
	InBaseState & operator = ( const InBaseState& );

	MissionType mSelectedMissionType;
	int mSelectedMissionID;

	FactionTemplate* mFactionTemplate;
	PlayerProfile* mPlayerProfile;
	CampaignState* mCampaignState;
	CampaignMissionManager* mCampaignMissionManager;

	SQLiteDBManager* mSQLiteDBManager;
	static InBaseState    *mInBaseState;
};
#endif