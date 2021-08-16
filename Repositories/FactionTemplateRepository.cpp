#include <Repositories/FactionTemplateRepository.h>

FactionTemplateRepository::FactionTemplateRepository()
{

}

FactionTemplateRepository::~FactionTemplateRepository()
{

}

FactionTemplateRepository* FactionTemplateRepository::mFactionTemplateRepository;

FactionTemplateRepository* FactionTemplateRepository::getSingletonPtr()
{
	if(!mFactionTemplateRepository)
	{
		mFactionTemplateRepository = new FactionTemplateRepository();
	}
	return mFactionTemplateRepository;
}