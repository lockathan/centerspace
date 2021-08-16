#ifndef MCUOrderStatus_h
#define MCUOrderStatus_h

class MCUController;

enum OrderStatus
{
	OS_STARTED = 0,
	OS_IN_PROGRESS,
	OS_COMPLETED,
	OS_FAILED
};

class MCUOrderStatus
{
public:
	MCUOrderStatus();
	MCUOrderStatus(MCUController* controller);
	~MCUOrderStatus();
	
	MCUController* getMCUController();
	void setMCUController(MCUController* controller);

	OrderStatus getStatus();
	void setStatus(OrderStatus status);
private:
	MCUController* mMCUController;
	OrderStatus mStatus;

};
#endif