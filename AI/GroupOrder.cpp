#include <AI/GroupOrder.h>
#include <AI/MCUOrderStatus.h>
#include <AI/AIGroup.h>
#include <States/PlayerBattleState.h>
#include <deque>

GroupOrder::GroupOrder()
	:
	mOwnerAIGroup(0)
{

}

GroupOrder::GroupOrder(AIGroup* owner)
	:
	mOwnerAIGroup(owner)
{
	std::deque<MCUController*> mcus = mOwnerAIGroup->getMCUs();
	std::deque<MCUController*>::iterator iter;

	for(iter = mcus.begin(); iter != mcus.end(); ++iter)
	{
		MCUOrderStatus* status = new MCUOrderStatus((*iter));
		mOrderStatuses.push_back(status);
	}
	mOwnerAIGroup->getPlayerBattleState()->notifyOfNewOrders(mOwnerAIGroup, this);
}

GroupOrder::~GroupOrder()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter))
		{
			delete (*iter);
			(*iter) = 0;
		}
	}
}

AIGroup* GroupOrder::getOwner()
{
	return mOwnerAIGroup;
}

void GroupOrder::setOwner(AIGroup* group)
{
	mOwnerAIGroup = group;
	
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter))
		{
			delete (*iter);
			(*iter) = 0;
		}
	}

	std::deque<MCUController*> mcus = mOwnerAIGroup->getMCUs();
	std::deque<MCUController*>::iterator mcuIter;

	for(mcuIter = mcus.begin(); mcuIter != mcus.end(); ++iter)
	{
		MCUOrderStatus* status = new MCUOrderStatus((*mcuIter));
		mOrderStatuses.push_back(status);
	}
}

std::deque<MCUOrderStatus*> GroupOrder::getOrderStatuses()
{
	return mOrderStatuses;
}

void GroupOrder::setOrderStatuses(std::deque<MCUOrderStatus*> orderStatuses)
{
	mOrderStatuses = orderStatuses;
}

void GroupOrder::setOrderStatus(MCUController* controller, OrderStatus status)
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getMCUController() == controller)
		{
			(*iter)->setStatus(status);
			break;
		}
	}
	orderStatusUpdate();
}

OrderStatus GroupOrder::getOrderStatus(MCUController* controller)
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getMCUController() == controller)
			return (*iter)->getStatus();
	}

	return OrderStatus::OS_STARTED;
}

bool GroupOrder::allOrdersCompleted()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getStatus() != OrderStatus::OS_COMPLETED)
			return false;
	}

	return true;
}

bool GroupOrder::allOrdersFailed()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getStatus() != OrderStatus::OS_FAILED)
			return false;
	}

	return true;
}

bool GroupOrder::allOrdersCompletedOrFailed()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getStatus() != OrderStatus::OS_FAILED && (*iter)->getStatus() != OrderStatus::OS_COMPLETED)
			return false;
	}

	return true;
}

bool GroupOrder::awaitingOrders()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getStatus() == OrderStatus::OS_STARTED)
			return true;
	}

	return false;
}

void GroupOrder::removeMCU(MCUController* mcu)
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		if((*iter)->getMCUController() == mcu)
		{
			delete (*iter);
			mOrderStatuses.erase(iter);
			return;
		}
	}
}

void GroupOrder::resetOrderStatus()
{
	std::deque<MCUOrderStatus*>::iterator iter;
	for(iter = mOrderStatuses.begin(); iter != mOrderStatuses.end(); ++iter)
	{
		(*iter)->setStatus(OrderStatus::OS_STARTED);
	}
}
