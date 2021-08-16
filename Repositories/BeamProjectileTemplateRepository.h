#ifndef BeamProjectileTemplateRepository_h
#define BeamProjectileTemplateRepository_h

#include "Ogre.h"
#include <Templates/BeamProjectileTemplate.h>
#include <Repositories/Repository.h>

class BeamProjectileTemplateRepository : public Repository<BeamProjectileTemplate>
{
public:
	static BeamProjectileTemplateRepository* getSingletonPtr();
	~BeamProjectileTemplateRepository();

private:
	BeamProjectileTemplateRepository();
	static BeamProjectileTemplateRepository* mBeamProjectileTemplateRepository;
};
#endif