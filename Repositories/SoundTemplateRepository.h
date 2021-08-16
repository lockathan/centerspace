#ifndef SoundTemplateRepository_h
#define SoundTemplateRepository_h

#include "Ogre.h"
#include <Templates/SoundTemplate.h>
#include <Repositories/Repository.h>

#include <vector>

class SoundTemplateRepository : public Repository<SoundTemplate>
{
public:
	static SoundTemplateRepository* getSingletonPtr();
	~SoundTemplateRepository();

private:
	SoundTemplateRepository();
	static SoundTemplateRepository* mSoundTemplateRepository;
};
#endif