#ifndef PhysicalProjectileTemplateRepository_h
#define PhysicalProjectileTemplateRepository_h

#include "Ogre.h"
#include <Templates/PhysicalProjectileTemplate.h>
#include <Repositories/Repository.h>

class PhysicalProjectileTemplateRepository : public Repository<PhysicalProjectileTemplate>
{
public:
	static PhysicalProjectileTemplateRepository* getSingletonPtr();
	~PhysicalProjectileTemplateRepository();

private:
	PhysicalProjectileTemplateRepository();
	static PhysicalProjectileTemplateRepository* mPhysicalProjectileTemplateRepository;
};
#endif