#ifndef PlayerProfile_h
#define PlayerProfile_h

#include <Ogre.h>
#include <Entities/PersistentEntity.h>

class FactionTemplate;
class DbValue;



class PlayerProfile : public PersistentEntity
{
public:
	PlayerProfile() ;
	~PlayerProfile();

	std::hash_map<std::string,DbValue> getValues();
	void setValues(std::hash_map<std::string,std::string> values);

	std::hash_map<std::string, std::string> getGuiValues();

	bool isAI();
	void setIsAI(bool value);
private:
	bool mAI;
};
#endif