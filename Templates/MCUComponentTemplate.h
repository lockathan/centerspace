#ifndef MCUComponentTemplate_h
#define MCUComponentTemplate_h

#include <Entities/PersistentEntity.h>
#include <Templates/Template.h>
#include <string>

class FactionTemplate;

class MCUComponentTemplate : public Template
{
public:
	MCUComponentTemplate();
	virtual ~MCUComponentTemplate();

	std::string getDescription();
	void setDescription(std::string desc);

	float getCost();
	void setCost(float cost);

	float getWeight();
	void setWeight(float weight);

	float getMaxHealth();
	void setMaxHealth(float health);

	int getRequiredRank();
	void setRequiredRank(int rank);

	FactionTemplate* getFactionTemplate();
	void setFactionTemplate(FactionTemplate* faction);

	int getSize();
	void setSize(int size);
	
	virtual std::hash_map<std::string, std::string> getGuiValues();
protected:
	float mCost;
	float mWeight;
	float mMaxHealth;
	int mRequiredRank;
	int mSize;
	FactionTemplate* mFactionTemplate;
	std::string mDescription;
};

#endif