#ifndef CampaignState_h
#define CampaignState_h

#include "PlayerState.h"
#include <Entities/PersistentEntity.h>
#include <States/BattleResultInterpreter.h>
#include <vector>

class BattleResult;
class MechanizedCombatUnit;
class InventoryState;
class PlayerProfile;
class PlayerResult;
class FactionTemplate;
class FactionHubTemplate;
class RankTemplate;
class DbValue;

class CampaignState : public PlayerState, public PersistentEntity, public BattleResultInterpreter
{
public:
	CampaignState( bool createInventoryState = true );

	~CampaignState();

	void enter();
	void exit();

	void load();
	void save();
	
	void reset();

	PlayerProfile* getPlayerProfile();
	void setPlayerProfile(PlayerProfile* profile);

	RankTemplate* getRankTemplate();
	void setRankTemplate(RankTemplate* rank);

	int getRank();
	void setRank(int rank);

	InventoryState* getInventoryState();
	void setInventoryState(InventoryState* state);

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	int getDifficulty();
	void setDifficulty(int difficulty);
	
	int getExperience();
	void setExperience(int exp);
	void addExperience(int exp);

	FactionHubTemplate* getFactionHubTemplate();
	void setFactionHubTemplate(FactionHubTemplate* hub);

	std::hash_map<std::string,DbValue> getValues();
	void setValues(std::hash_map<std::string,std::string> values);

	bool displayBattleResults();
	void setDisplayBattleResults(bool value);

	BattleResult* getBattleResult();
	PlayerResult* getPlayerResult();
	void interpretResult(BattleResult* result);

	std::hash_map<std::string,std::string> getGuiValues();
private:

	InventoryState* mInventoryState;
	PlayerProfile* mPlayerProfile;
	FactionTemplate* mFactionTemplate;
	FactionHubTemplate* mFactionHubTemplate;
	BattleResult* mBattleResult;
	RankTemplate* mRankTemplate;

	bool mDisplayBattleResults;
	int mDifficulty;
	int mRank;
	int mExperience;
};
#endif