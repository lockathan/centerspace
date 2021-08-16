#include <AI/GroupMoveToAttackOrder.h>
#include <AI/MCUOrderStatus.h>
#include <Controllers/MCUController.h>
#include <Entities/BattlefieldGridCell.h>
#include <AI/AIGroup.h>

GroupMoveToAttackOrder::GroupMoveToAttackOrder()
	:
	GroupOrder(),
	mTarget(0)
{

}
	
GroupMoveToAttackOrder::GroupMoveToAttackOrder(AIGroup* owner, MCUController* target)
	:
	GroupOrder(owner),
	mTarget(target)
{

}

GroupMoveToAttackOrder::~GroupMoveToAttackOrder()
{

}

MCUController* GroupMoveToAttackOrder::getTarget()
{
	return mTarget;
}

void GroupMoveToAttackOrder::setTarget(MCUController* target)
{
	mTarget = target;
}

void GroupMoveToAttackOrder::orderStatusUpdate()
{
	if(this->allOrdersCompletedOrFailed())
	{
		this->getOwner()->finishedGroupMoveToAttackOrder();
	}
}