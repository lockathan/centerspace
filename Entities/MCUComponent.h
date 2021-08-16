#ifndef MCUComponent_h
#define MCUComponent_h

#include <Ogre.h>
#include <Templates/MCUComponentTemplate.h>
#include <Templates/WeaponTemplate.h>

enum ComponentType
{
	CT_CHASSIS = 0,
	CT_ARMOR = 1,
	CT_BATTERY = 2,
	CT_DRIVE = 3,
	CT_REACTOR = 4,
	CT_SHIELD = 5,
	CT_SENSOR_SYSTEM = 6,
	CT_TARGETING_SYSTEM = 7,
	CT_WEAPON = 8
};

class ArmorTemplate;
class MCUController;
class MCUComponentStatus;
class WeaponHardpoint;

class MCUComponent
{
public:
	MCUComponent();
	MCUComponent(MCUController* controller, ComponentType type, const Ogre::Real& maxHealth, const Ogre::Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate);
	MCUComponent(MCUComponentTemplate* component, MCUController* controller, ComponentType type, const Ogre::Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate);
	MCUComponent(MCUComponentTemplate* component, MCUController* controller, WeaponHardpoint* weapon, const Ogre::Real& armor, MCUComponent* armorComponent, ArmorTemplate* armorTemplate);
	~MCUComponent();
	
	MCUComponentTemplate* getMCUComponentTemplate();
	void setMCUComponentTemplate(MCUComponentTemplate* component);

	MCUController* getMCUController();
	void setMCUController(MCUController* mcu);

	Ogre::Real getHealthRemaining();
	void setHealthRemaining(const Ogre::Real& health);

	Ogre::Real getMaxHealth();
	void setMaxHealth(const Ogre::Real& health);

	Ogre::Real getArmorRemaining();
	void setArmorRemaining(const Ogre::Real& armor);

	Ogre::Real getMaxArmor();
	void setMaxArmor(const Ogre::Real& armor);
	
	ArmorTemplate* getArmorTemplate();
	void setArmorTemplate(ArmorTemplate* armor);

	MCUComponent* getArmorComponent();
	void setArmorComponent(MCUComponent* component);

	MCUComponentStatus* getMCUComponentStatus();
	void setMCUComponentStatus(MCUComponentStatus* status);

	ComponentType getComponentType();
	void setComponentType(ComponentType type);
	
	WeaponHardpoint* getWeaponHardpoint();
	void setWeaponHardpointNumber(WeaponHardpoint* weapon);

	void applyArmorComponentDamage(const Ogre::Real& damage);
	Ogre::Real applyDamage(const Ogre::Real& damage, DamageType type, MCUController* firedBy);

	bool isBroken();
private:
	MCUComponentTemplate* mMCUComponentTemplate;
	MCUComponentStatus* mMCUComponentStatus;
	MCUController* mMCUController;
	Ogre::Real mHealthRemaining;
	Ogre::Real mMaxHealth;
	Ogre::Real mArmorRemaining;
	Ogre::Real mMaxArmor;
	ArmorTemplate* mArmorTemplate;
	MCUComponent* mArmorComponent;
	ComponentType mComponentType;
	WeaponHardpoint* mWeaponHardpoint;
};
#endif