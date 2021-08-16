#ifndef MCUStatus_h
#define MCUStatus_h

#include <deque>

enum MCUCondition
{
	MC_INTACT = 0,
	MC_DAMAGED = 1,
	MC_DESTROYED = 2
};

class MechanizedCombatUnit;
class MCUComponentStatus;
class PlayerResult;

#include <hash_map>

class MCUStatus
{
public:
	MCUStatus();
	MCUStatus(MechanizedCombatUnit* mcu);
	~MCUStatus();

	MCUCondition getMCUCondition();
	void setMCUCondition(MCUCondition condition);

	MechanizedCombatUnit* getMechanizedCombatUnit();
	void setMechanizedCombatUnit(MechanizedCombatUnit* mcu);

	std::deque<MCUComponentStatus*>& getComponentStatuses();
	void setComponentStatuses(std::deque<MCUComponentStatus*>& statuses);
	void addComponentStatus(MCUComponentStatus* status);

	PlayerResult* getPlayerResult();
	void setPlayerResult(PlayerResult* player);

	float getTotalRepairCosts();

	std::hash_map<std::string,std::string> getGuiValues();
private:
	MCUCondition mMCUCondition;
	MechanizedCombatUnit* mMechanizedCombatUnit;
	std::deque<MCUComponentStatus*> mMCUComponentStatuses;
	PlayerResult* mPlayerResult;
};
#endif