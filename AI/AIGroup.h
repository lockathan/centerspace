#ifndef AIGroup_h
#define AIGroup_h

#include <deque>
#include <Entities/MCUPlacement.h>

class BattlefieldGridCell;
class GroupAttackOrder;
class GroupMoveOrder;
class GroupMoveToAttackOrder;
class LuaMCUDeque;
class MCUController;
class MechanizedCombatUnit;
class PlayerBattleState;

class AIGroup
{
public:
	AIGroup(int id);
	virtual ~AIGroup();

	int getAverageMCUXPos();
	int getAverageMCUZPos();

	std::deque<MCUController*> getMCUs();
	void setMCUs(std::deque<MCUController*> mcus);

	BattlefieldGridCell* getCurrentGoal();
	void setCurrentGoal(BattlefieldGridCell* goal);

	bool hasMoreMCUControllers();
	MCUController* getNextMCUController();
	void removeMCU(MCUController* mcu);

	LuaMCUDeque* getLuaMCUDeque();
	std::deque<MCUPlacement*>& getMCUPlacements();
	void addMCUPlacement(MechanizedCombatUnit* mcu, int x, int z);
	void setMCUPlacements(std::deque<MCUPlacement*>& placements);

	PlayerBattleState* getPlayerBattleState();
	void setPlayerBattleState(PlayerBattleState* state);

	void addMCUController(MCUController* mcu);

	bool hasFinishedTurn();
	void startTurn();

	bool hasAttackOrder();
	GroupAttackOrder* getGroupAttackOrder();
	void setGroupAttackOrder(GroupAttackOrder* order);
	void giveGroupAttackOrder(MCUController* target);
	void executeGroupAttackOrder();
	void executeGroupAttackOrderIfNecessary();
	void finishedGroupAttackOrder(bool issueNewOrders = true);

	bool hasMoveOrder();
	GroupMoveOrder* getGroupMoveOrder();
	void setGroupMoveOrder(GroupMoveOrder* order);
	void giveGroupMoveOrder(BattlefieldGridCell* dest);
	void finishedGroupMoveOrder(bool issueNewOrders = true);

	bool hasMoveToAttackOrder();
	GroupMoveToAttackOrder* getGroupMoveToAttackOrder();
	void setGroupMoveToAttackOrder(GroupMoveToAttackOrder* order);
	void giveGroupMoveToAttackOrder(MCUController* target);
	void finishedGroupMoveToAttackOrder(bool issueNewOrders = true);

	bool hasOrders();
	bool hasLowMovementEnergy();
	bool hasFiredAllWeapons();
	bool canFireUpon(MCUController* target);

	void finishedTurn();
	void notifyOfMovementChange(MCUController* controller);
	void notifyOfMovementFailureDueToBlockage(MCUController* controller, MCUController* blockage, BattlefieldGridCell* destination);

	MCUController* getClosestVisibleEnemyMCU();
	
	float getShortWeaponRange();
	float getLongWeaponRange();

	void loopFinished();

	int getId();
private:
	int mId;
	std::deque<MCUController*> mMCUs;
	std::deque<MCUController*>::iterator mMCUIterator;
	std::deque<MCUPlacement*> mInitialPlacements;

	float mShortWeaponRange;
	float mLongWeaponRange;

	BattlefieldGridCell* mCurrentGoal;

	GroupAttackOrder* mGroupAttackOrder;
	GroupMoveOrder* mGroupMoveOrder;
	GroupMoveToAttackOrder* mGroupMoveToAttackOrder;
	PlayerBattleState* mPlayerBattleState;
	LuaMCUDeque* mLuaMCUDeque;
	bool mFinishedTurn;
};
#endif