#include <AI/GroupAttackOrder.h>
#include <Controllers/MCUController.h>
#include <AI/AIGroup.h>

GroupAttackOrder::GroupAttackOrder()
	:
	GroupOrder(),
	mTarget(0)
{

}

GroupAttackOrder::GroupAttackOrder(AIGroup* owner, MCUController* target)
	:
	GroupOrder(owner),
	mTarget(target)
{

}

GroupAttackOrder::~GroupAttackOrder()
{

}

MCUController* GroupAttackOrder::getTarget()
{
	return mTarget;
}

void GroupAttackOrder::setTarget(MCUController* target)
{
	mTarget = target;
}

void GroupAttackOrder::orderStatusUpdate()
{
	//only marked as completed when the target is destroyed
	if(this->allOrdersFailed())
		this->getOwner()->finishedTurn();
	else
		this->getOwner()->executeGroupAttackOrder();
}
