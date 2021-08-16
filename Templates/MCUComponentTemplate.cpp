#include <Templates/MCUComponentTemplate.h>
#include <Entities/DataProvider.h>

#include <Managers/SQLiteDBManager.h>

MCUComponentTemplate::MCUComponentTemplate()
	:
	mCost(0),
	mWeight(0),
	mMaxHealth(0),
	mRequiredRank(0),
	mSize(0),
	mFactionTemplate(0),
	mDescription("")
{

}

MCUComponentTemplate::~MCUComponentTemplate()
{

}

std::string MCUComponentTemplate::getDescription()
{
	return mDescription;
}


void MCUComponentTemplate::setDescription(std::string desc)
{
	mDescription = desc;
}


float MCUComponentTemplate::getCost()
{
	return mCost;
}

void MCUComponentTemplate::setCost(float cost)
{
	mCost = cost;
}


float MCUComponentTemplate::getWeight()
{
	return mWeight;
}

void MCUComponentTemplate::setWeight(float weight)
{
	mWeight = weight;
}

float MCUComponentTemplate::getMaxHealth()
{
	return mMaxHealth;
}

void MCUComponentTemplate::setMaxHealth(float health)
{
	mMaxHealth = health;
}

int MCUComponentTemplate::getRequiredRank()
{
	return mRequiredRank;
}

void MCUComponentTemplate::setRequiredRank(int rank)
{
	mRequiredRank = rank;
}

FactionTemplate* MCUComponentTemplate::getFactionTemplate()
{
	return mFactionTemplate;
}

void MCUComponentTemplate::setFactionTemplate(FactionTemplate* faction)
{
	mFactionTemplate = faction;
}

int MCUComponentTemplate::getSize()
{
	return mSize;
}

void MCUComponentTemplate::setSize(int size)
{
	mSize = size;
}

std::hash_map<std::string, std::string> MCUComponentTemplate::getGuiValues()
{
	std::hash_map<std::string, std::string> result;

	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString( this->getId() );
	result[FieldNames::ComponentTemplate::ComponentCost] = SQLiteDBManager::convertFloatToString( mCost);
	result[FieldNames::ComponentTemplate::ComponentDescription] = mDescription;
	result[FieldNames::ComponentTemplate::ComponentName] = mName;
	result[FieldNames::ComponentTemplate::ComponentWeight] = SQLiteDBManager::convertFloatToString( mWeight );
	
	return result;
}
