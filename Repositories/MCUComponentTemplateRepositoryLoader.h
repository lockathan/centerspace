#ifndef MCUComponentTemplateRepositoryLoader_h
#define MCUComponentTemplateRepositoryLoader_h

class MCUComponentTemplate;
#include <vector>
#include <string>

class MCUComponentTemplateRepositoryLoader
{
public:
	void load(std::vector<std::string>::iterator& itemIterator, MCUComponentTemplate* item);
	static MCUComponentTemplateRepositoryLoader* getSingletonPtr();
private:
	MCUComponentTemplateRepositoryLoader(){}
	static MCUComponentTemplateRepositoryLoader* mMCUComponentTemplateRepositoryLoader;
};
#endif