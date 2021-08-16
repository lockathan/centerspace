#include <Entities/PersistentEntity.h>
#include <string>


PersistentEntity::PersistentEntity()
{
	mId = 0;
	mName = "";
}

PersistentEntity::~PersistentEntity()
{

}

long PersistentEntity::getId()
{
	return mId;
}

void PersistentEntity::setId(long id)
{
	mId = id;
}


std::string PersistentEntity::getName()
{
	return mName;
}

void PersistentEntity::setName(std::string name)
{
	mName = name;
}
