#ifndef GroupMoveToAttackOrder_h
#define GroupMoveToAttackOrder_h

#include <AI/GroupOrder.h>

class AIGroup;
class BattlefieldGridCell;
class MCUController;
class MCUOrderStatus;

class GroupMoveToAttackOrder : public GroupOrder
{
public:
	GroupMoveToAttackOrder();
	GroupMoveToAttackOrder(AIGroup* owner, MCUController* target);
	~GroupMoveToAttackOrder();
	
	MCUController* getTarget();
	void setTarget(MCUController* target);

private:
	void orderStatusUpdate();
	MCUController* mTarget;
};
#endif