#include <Repositories/PhysicalProjectileTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/PhysicalProjectileTemplate.h>

PhysicalProjectileTemplateRepository::PhysicalProjectileTemplateRepository()
{

}

PhysicalProjectileTemplateRepository::~PhysicalProjectileTemplateRepository()
{

}


PhysicalProjectileTemplateRepository* PhysicalProjectileTemplateRepository::mPhysicalProjectileTemplateRepository;

PhysicalProjectileTemplateRepository* PhysicalProjectileTemplateRepository::getSingletonPtr()
{
	if(!mPhysicalProjectileTemplateRepository)
	{
		mPhysicalProjectileTemplateRepository = new PhysicalProjectileTemplateRepository();
	}
	return mPhysicalProjectileTemplateRepository;
}