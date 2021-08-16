#include <Repositories/BeamProjectileTemplateRepository.h>
#include <Templates/FactionTemplate.h>
#include <Templates/BeamProjectileTemplate.h>

BeamProjectileTemplateRepository::BeamProjectileTemplateRepository()
{

}

BeamProjectileTemplateRepository::~BeamProjectileTemplateRepository()
{

}


BeamProjectileTemplateRepository* BeamProjectileTemplateRepository::mBeamProjectileTemplateRepository;

BeamProjectileTemplateRepository* BeamProjectileTemplateRepository::getSingletonPtr()
{
	if(!mBeamProjectileTemplateRepository)
	{
		mBeamProjectileTemplateRepository = new BeamProjectileTemplateRepository();
	}
	return mBeamProjectileTemplateRepository;
}