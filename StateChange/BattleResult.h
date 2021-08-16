#ifndef BattleResult_h
#define BattleResult_h

#include <deque>
#include <hash_map>

class PlayerProfile;
class PlayerResult;

class BattleResult
{
public:
	BattleResult();
	BattleResult(float basePay, float bounty);
	~BattleResult();

	std::deque<PlayerResult*>& getPlayerResults();
	void setPlayerResults(std::deque<PlayerResult*>& results);
	void addPlayerResult(PlayerResult* result);

	float getBaseMissionPay();
	void setBaseMissionPay(float value);

	float getBounty();
	void setBounty(float value);

	PlayerResult* getPlayerResult(PlayerProfile* player);

	std::hash_map<std::string,std::string> getGuiValues(PlayerProfile* profile);
private:
	std::deque<PlayerResult*> mPlayerResults;
	float mBaseMissionPay;
	float mBounty;
};
#endif