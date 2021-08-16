#include <AI/MCUOrderStatus.h>
#include <AI/AIGroup.h>

MCUOrderStatus::MCUOrderStatus()
	:
	mMCUController(0),
	mStatus(OrderStatus::OS_STARTED)
{

}

MCUOrderStatus::MCUOrderStatus(MCUController* controller)
	:
	mMCUController(controller),
	mStatus(OrderStatus::OS_STARTED)
{

}

MCUOrderStatus::~MCUOrderStatus()
{

}
	
MCUController* MCUOrderStatus::getMCUController()
{
	return mMCUController;
}

void MCUOrderStatus::setMCUController(MCUController* controller)
{
	mMCUController = controller;
}

OrderStatus MCUOrderStatus::getStatus()
{
	return mStatus;
}

void MCUOrderStatus::setStatus(OrderStatus status)
{
	mStatus = status;
}