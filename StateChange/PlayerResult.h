#ifndef PlayerResult_h
#define PlayerResult_h

#include <deque>

enum PlayerOutcome
{
	PO_PLAYING = 0,
	PO_VICTORY = 1,
	PO_DEFEAT = 2
};

class MechanizedCombatUnit;
class MCUStatus;
class PlayerProfile;

class PlayerResult
{
public:
	PlayerResult();
	PlayerResult(PlayerProfile* profile, int team);
	~PlayerResult();

	PlayerOutcome getPlayerOutcome();
	void setPlayerOutcome(PlayerOutcome outcome);

	PlayerProfile* getPlayerProfile();
	void setPlayerProfile(PlayerProfile* player);

	std::deque<MCUStatus*>& getMCUStatuses();
	void setMCUStatuses(std::deque<MCUStatus*>& statuses);
	void addMCUStatus(MCUStatus* status);

	int getTeam();
	void setTeam(int t);

	int getKills();
	void setKills(int kills);
	void addKill();

	bool gainedPromotion();
	void setGainedPromotion(bool value);

	MCUStatus* getMCUStatus(MechanizedCombatUnit* mcu);
	
	float getTotalRepairCosts();
private:
	PlayerOutcome mPlayerOutcome;
	PlayerProfile* mPlayerProfile;
	std::deque<MCUStatus*> mMCUStatuses;
	bool mGainedPromotion;
	int mTeam;
	int mKills;

};
#endif