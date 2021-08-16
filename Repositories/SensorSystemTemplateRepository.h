#ifndef SensorSystemTemplateRepository_h
#define SensorSystemTemplateRepository_h

#include "Ogre.h"
#include <Templates/SensorSystemTemplate.h>
#include <Repositories/Repository.h>

class SensorSystemTemplateRepository : public Repository<SensorSystemTemplate>
{
public:
	static SensorSystemTemplateRepository* getSingletonPtr();
	~SensorSystemTemplateRepository();

	
	std::vector<SensorSystemTemplate*> getAvailableTemplates(int factionId, int rank, int size = 0);

private:
	SensorSystemTemplateRepository();
	static SensorSystemTemplateRepository* mSensorSystemTemplateRepository;
};
#endif