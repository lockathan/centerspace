#include <Repositories/SoundTemplateRepository.h>
#include <Templates/SoundTemplate.h>
#include <Templates/FactionTemplate.h>

SoundTemplateRepository::SoundTemplateRepository()
{

}

SoundTemplateRepository::~SoundTemplateRepository()
{

}


SoundTemplateRepository* SoundTemplateRepository::mSoundTemplateRepository;

SoundTemplateRepository* SoundTemplateRepository::getSingletonPtr()
{
	if(!mSoundTemplateRepository)
	{
		mSoundTemplateRepository = new SoundTemplateRepository();
	}
	return mSoundTemplateRepository;
}