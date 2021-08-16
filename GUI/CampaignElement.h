#ifndef CampaignElement_h
#define CampaignElement_h

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <Rocket/Controls/DataSource.h>
#include <vector>

class SQLiteDBManager;
class PlayerProfile;
class CampaignState;
class MainMenuState;
class FactionTemplate;

class CampaignElement : public Rocket::Core::Element
{
public:
	CampaignElement(const Rocket::Core::String& tag);
	~CampaignElement();

	void ProcessEvent(Rocket::Core::Event& ev);
protected:
	CampaignState* mCampaignState;
	MainMenuState* mMainMenuState;
	FactionTemplate* mFactionTemplate;	
	SQLiteDBManager* mSQLiteDBManager;	

	long mPlayerProfileId;

	bool mInitialized;
	void buildElements();

};

#endif
