#include <AI/GroupMoveOrder.h>
#include <Entities/BattlefieldGridCell.h>
#include <AI/AIGroup.h>

GroupMoveOrder::GroupMoveOrder()
	:
	GroupOrder(),
	mDestination(0)
{

}

GroupMoveOrder::GroupMoveOrder(AIGroup* owner, BattlefieldGridCell* dest)
	:
	GroupOrder(owner),
	mDestination(dest)
{

}

GroupMoveOrder::~GroupMoveOrder()
{
	
}

BattlefieldGridCell* GroupMoveOrder::getDestination()
{
	return mDestination;
}

void GroupMoveOrder::setDestination(BattlefieldGridCell* dest)
{
	mDestination = dest;
}

void GroupMoveOrder::orderStatusUpdate()
{
	if(this->allOrdersCompletedOrFailed())
	{
		this->getOwner()->finishedGroupMoveOrder();
	}
}
