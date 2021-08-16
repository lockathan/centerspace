#ifndef InventoryState_h
#define InventoryState_h

#include "PlayerState.h"
#include <States/MCUBayState.h>
#include <Entities/PersistentEntity.h>
#include <vector>
#include <hash_map>
#include <deque>

class MechanizedCombatUnit;
class FactionTemplate;
class DbValue;
class SQLiteDBManager;

class InventoryState : public PlayerState, public PersistentEntity
{
public:
	InventoryState();
	~InventoryState();

	void enter();
	void exit();
	
	void load();
	void save();

	void addMCU(MechanizedCombatUnit* mcu);
	void removeMCU(MechanizedCombatUnit* mcu);
	void setMCUs(std::deque<MechanizedCombatUnit*> mcus);
	std::deque<MechanizedCombatUnit*> getMCUs();
	MechanizedCombatUnit* getMCU(long id);

	int getRank();
	void setRank(int rank);

	float getRemainingCash();
	void setRemainingCash(float cash);

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	void changeMCUComponent(MCUBayState::ComponentType componentType,long mcuId, int componentId, int hardpoint = -1);
	void changeMCUComponent(MCUBayState::ComponentType componentType, MechanizedCombatUnit* mcu, MCUComponentTemplate* component, int hardpoint = -1);

	virtual std::hash_map<std::string,DbValue> getValues();
	virtual void setValues(std::hash_map<std::string,std::string> values);	
private:
	int mRank;
	float mRemainingCash;


	FactionTemplate* mFactionTemplate;
	std::deque<MechanizedCombatUnit*> mMCUs;
	std::hash_map<std::string, int> mMCUSequenceNumbers;

	SQLiteDBManager* mSQLiteDBManager;
};
#endif