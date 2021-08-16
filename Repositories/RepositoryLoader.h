#ifndef RepositoryLoader_h
#define RepositoryLoader_h

#include <Repositories/Repository.h>

class RepositoryLoader
{
public:
	RepositoryLoader(){}
	virtual ~RepositoryLoader(){}
	template<class T>
	void load(const std::string& fileName, Repository<T>* r);
	
};

#endif