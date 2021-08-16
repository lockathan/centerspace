#ifndef PersistentEntity_h
#define PersistentEntity_h

#include <Ogre.h>
#include <hash_map>
#include <string>

class DbValue;



class PersistentEntity
{
public:
	PersistentEntity();
	virtual ~PersistentEntity();
	virtual std::hash_map<std::string,DbValue> getValues() = 0;
	virtual void setValues(std::hash_map<std::string,std::string> values) = 0;
	
	long getId();
	void setId(long id);
	
	std::string getName();
	void setName(std::string name);

protected:
	long mId;
	std::string mName;
};
#endif