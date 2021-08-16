#ifndef AIGroupTemplateRepository_h
#define AIGroupTemplateRepository_h

#include "Ogre.h"
#include <Templates/AIGroupTemplate.h>
#include <Repositories/Repository.h>
#include <vector>

class AIGroupTemplateRepository : public Repository<AIGroupTemplate>
{
public:
	static AIGroupTemplateRepository* getSingletonPtr();
	~AIGroupTemplateRepository();

	std::deque<AIGroupTemplate*> getAvailableAIGroupTemplates(int factionId, int rank);
	std::deque<AIGroupTemplate*> getAvailableAIGroupTemplates(int factionId, int rank, AIGroupTemplate::AIGroupType type);
	AIGroupTemplate* getRandomAIGroupTemplate(int factionId, int rank);

private:
	AIGroupTemplateRepository();
	static AIGroupTemplateRepository* mAIGroupTemplateRepository;
};
#endif