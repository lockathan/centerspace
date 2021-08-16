#ifndef MCUTemplateRepository_h
#define MCUTemplateRepository_h

#include "Ogre.h"
#include <Templates/MCUTemplate.h>
#include <Repositories/Repository.h>
#include <vector>

class MCUTemplateRepository : public Repository<MCUTemplate>
{
public:
	static MCUTemplateRepository* getSingletonPtr();
	~MCUTemplateRepository();

	std::vector<MCUTemplate*> getAvailableMCUTemplates(int factionId, int rank);
private:
	MCUTemplateRepository();
	static MCUTemplateRepository* mMCUTemplateRepository;
};
#endif