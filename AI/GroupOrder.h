#ifndef GroupOrder_h
#define GroupOrder_h

#include <deque>
#include <AI/MCUOrderStatus.h>

class AIGroup;
class MCUController;

class GroupOrder
{
public:
	GroupOrder();
	GroupOrder(AIGroup* owner);
	virtual ~GroupOrder();
	
	AIGroup* getOwner();
	void setOwner(AIGroup* group);

	std::deque<MCUOrderStatus*> getOrderStatuses();
	void setOrderStatuses(std::deque<MCUOrderStatus*> orderStatuses);
	void setOrderStatus(MCUController* controller, OrderStatus status);
	OrderStatus getOrderStatus(MCUController* controller);
	bool allOrdersCompleted();
	bool allOrdersFailed();
	bool allOrdersCompletedOrFailed();
	bool awaitingOrders();
	void removeMCU(MCUController* mcu);
	void resetOrderStatus();
private:
	virtual void orderStatusUpdate() = 0;
	AIGroup* mOwnerAIGroup;
	std::deque<MCUOrderStatus*> mOrderStatuses;
};
#endif