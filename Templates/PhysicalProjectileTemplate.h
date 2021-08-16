#ifndef PhysicalProjectileTemplate_h
#define PhysicalProjectileTemplate_h

#include "Ogre.h"
#include <Templates/ProjectileTemplate.h>
#include <Controllers/MCUController.h>

class WeaponTemplate;

class PhysicalProjectileTemplate : public ProjectileTemplate
{
public:
	PhysicalProjectileTemplate();
	~PhysicalProjectileTemplate();

	std::string getMeshName();
	void setMeshName(const std::string& name);

	std::string getMaterialName();
	void setMaterialName(const std::string& name);

	float getMovementSpeed();
	void setMovementSpeed(const float& speed);

	float getWeight();
	void setWeight(const float& weight);

	bool isSelfPropelled();
	void setSelfPropelled(bool value);

	bool isArching();
	void setArching(bool value);

	void instantiate(MCUController* mcu, const FireInstruction& instruction);

private:
	std::string mMeshName;
	std::string mMaterialName;

	float mMovementSpeed;
	float mWeight;

	bool mSelfPropelled;
	bool mArching;	
};


#endif