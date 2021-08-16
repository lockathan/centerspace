#ifndef MCUComponentStatus_h
#define MCUComponentStatus_h

#include <Entities/MCUComponent.h>

class MCUComponent;
class MCUComponentTemplate;
class MCUTemplate;
class MCUStatus;

class MCUComponentStatus
{
public:
	MCUComponentStatus();
	MCUComponentStatus(MCUComponent* component,ComponentType type);
	MCUComponentStatus(ComponentType type, float maxHealth, MCUTemplate* mcu);
	~MCUComponentStatus();

	MCUComponentTemplate* getMCUComponentTemplate();
	void setMCUComponentTemplate(MCUComponentTemplate* component);

	float getHealthRemaining();
	void setHealthRemaining(float health);

	ComponentType getComponentType();
	void setComponentType(ComponentType type);

	MCUStatus* getMCUStatus();
	void setMCUStatus(MCUStatus* mcu);

	float getDamagePercent();

	float getRepairCosts();
private:
	MCUComponentTemplate* mMCUComponentTemplate;
	MCUTemplate* mMCUTemplate;
	float mHealthRemaining;
	float mMaxHealth;
	ComponentType mComponentType;
	MCUStatus* mMCUStatus;
};
#endif