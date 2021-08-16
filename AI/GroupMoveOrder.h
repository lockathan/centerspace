#ifndef GroupMoveOrder_h
#define GroupMoveOrder_h

#include <AI/GroupOrder.h>

class AIGroup;
class BattlefieldGridCell;

class GroupMoveOrder : public GroupOrder
{
public:
	GroupMoveOrder();
	GroupMoveOrder(AIGroup* owner, BattlefieldGridCell* dest);
	~GroupMoveOrder();
	
	BattlefieldGridCell* getDestination();
	void setDestination(BattlefieldGridCell* dest);

private:
	void orderStatusUpdate();
	BattlefieldGridCell* mDestination;
};
#endif