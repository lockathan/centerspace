#ifndef GroupAttackOrder_h
#define GroupAttackOrder_h

#include <AI/GroupOrder.h>

class AIGroup;
class MCUController;

class GroupAttackOrder : public GroupOrder
{
public:
	GroupAttackOrder();
	GroupAttackOrder(AIGroup* owner, MCUController* target);
	~GroupAttackOrder();
	
	MCUController* getTarget();
	void setTarget(MCUController* target);
private:
	void orderStatusUpdate();
	MCUController* mTarget;
};
#endif