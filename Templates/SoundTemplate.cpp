#include <Templates/SoundTemplate.h>
#include <string>
#include <hash_map>

SoundTemplate::SoundTemplate()
	:
	Template(),
	mFileName(""),
	mLoop(false)
{

}

SoundTemplate::~SoundTemplate()
{

}

	
std::string& SoundTemplate::getFileName()
{
	return mFileName;
}

void SoundTemplate::setFileName(const std::string& fileName)
{
	mFileName = fileName;
}

bool SoundTemplate::loop()
{
	return mLoop;
}

void SoundTemplate::setLoop(bool loop)
{
	mLoop = loop;
}
