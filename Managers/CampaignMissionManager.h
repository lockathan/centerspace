#ifndef CampaignMissionManager_H
#define CampaignMissionManager_H

#include <Entities/RandomMission.h>
#include <vector>
#include <deque>
#include <hash_map>

class CampaignState;
class DataSet;
class FactionTemplate;
class HubTemplate;
class PlayerProfile;
class Mission;
class RandomMissionGenerator;

class CampaignMissionManager
{
public:
	~CampaignMissionManager();

	CampaignState* getCampaignState();
	void setCampaignState(CampaignState* campaign);

	FactionTemplate* setFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	void loadRandomMission(int id, PlayerProfile* player);
	void loadRandomMission(Mission* mission, PlayerProfile* player);

	void generateRandomMissions();

	DataSet* getRandomMissionDataSet();

	static CampaignMissionManager* getSingletonPtr();
protected:
	static CampaignMissionManager* mCampaignMissionManager;
	std::deque<Mission*> mRandomMissions;
	
	CampaignState* mCampaignState;
	FactionTemplate* mFactionTemplate;
	RandomMissionGenerator* mRandomMissionGenerator;
private:
	CampaignMissionManager();
	CampaignMissionManager( const CampaignMissionManager& ) { }
	CampaignMissionManager & operator = ( const CampaignMissionManager& );

};
#endif