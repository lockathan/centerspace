#include <Entities/PlayerProfile.h>
#include <Templates/FactionTemplate.h>
#include <Entities/DataProvider.h>
#include <Managers/DbNames.h>
#include <Managers/SQLiteDBManager.h>

PlayerProfile::PlayerProfile()
	:
	mAI(false)
{

}

PlayerProfile::~PlayerProfile()
{

}

std::hash_map<std::string,DbValue> PlayerProfile::getValues()
{
	std::hash_map<std::string,DbValue> values;
	values[DbNames::PlayerProfile::ID].StringValue = SQLiteDBManager::convertLongToString(mId);
	values[DbNames::PlayerProfile::ID].IsNumber = true;

	values[DbNames::PlayerProfile::NAME].StringValue = mName;
	values[DbNames::PlayerProfile::NAME].IsNumber = false;

	return values;
}

void PlayerProfile::setValues(std::hash_map<std::string,std::string> values)
{
	mId = atol(values[DbNames::PlayerProfile::ID].c_str());
	mName = values[DbNames::PlayerProfile::NAME];
}

std::hash_map<std::string, std::string> PlayerProfile::getGuiValues()
{
	std::hash_map<std::string, std::string> result;

	result[FieldNames::Player::PlayerName] = mName;
	result[FieldNames::ItemID] = SQLiteDBManager::convertIntToString(mId);

	return result;
}

bool PlayerProfile::isAI()
{
	return mAI;
}

void PlayerProfile::setIsAI(bool value)
{
	mAI = value;
}
