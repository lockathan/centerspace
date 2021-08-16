#ifndef SQLiteDBManager_H
#define SQLiteDBManager_H

#include <Ogre.h>
#include <sqlite3.h>
#include <vector>
#include <hash_map>

class PlayerProfile;
class MechanizedCombatUnit;
class WeaponHardpoint;
class CampaignState;
class InventoryState;



class DbValue
{
public:
	bool IsNumber;
	std::string StringValue;
};

class SQLiteDBManager
{
public:
	~SQLiteDBManager();
	
	PlayerProfile* getPlayerProfile(long id);
	std::vector<PlayerProfile*> getPlayerProfiles();
	MechanizedCombatUnit* getMechanizedCombatUnit(long id);
	std::deque<MechanizedCombatUnit*> getMechanizedCombatUnits(InventoryState* inv);
	WeaponHardpoint* getWeaponHardpoint(long id);
	std::vector<WeaponHardpoint*> getWeaponHardpoints(MechanizedCombatUnit* mcu);
	InventoryState* getInventoryState(long inventoryId);
	CampaignState* getCampaignState(long campaignId);
	CampaignState* getCampaignState(long playerProfileId, int factionId);
	std::vector<CampaignState*> getCampaignStates(long playerProfileId);

	void savePlayerProfile(PlayerProfile* profile);
	void saveCampaignState(CampaignState* campaign);
	void saveInventoryState(InventoryState* inventory);
	void saveMCU(MechanizedCombatUnit* mcu);
	void saveWeaponHardpoint(WeaponHardpoint* weapon);

	long addPlayerProfile(PlayerProfile* profile);
	long addCampaignState(CampaignState* campaign);
	long addInventoryState(InventoryState* inventory);
	long addMCU(MechanizedCombatUnit* mcu);
	long addWeaponHardpoint(WeaponHardpoint* weapon);

	void setFileName(std::string name);
	std::string getFileName();

	static std::string convertIntToString(int value);
	static std::string convertLongToString(long value);
	static std::string convertFloatToString(float value);
	static SQLiteDBManager* getSingletonPtr();
private:
	SQLiteDBManager();
	SQLiteDBManager( const SQLiteDBManager& ) { }
	SQLiteDBManager & operator = ( const SQLiteDBManager& );
	
	std::hash_map<std::string, std::string> selectFieldsOneRecord(std::string query, std::vector<std::string> fields);
	std::vector<std::hash_map<std::string,std::string>> selectFieldsMultipleRecords(std::string query, std::vector<std::string> fields);
	std::vector<std::vector<std::string> > query(std::string query);
	long runInsertQuery(std::string query);

	std::string buildSelectQueryString(std::string tableName, long id, std::vector<std::string> columns);
	std::string buildSelectQueryString(std::string tableName, std::string queryField, std::string value, std::vector<std::string> columns);
	std::string buildSelectQueryString(std::string tableName, std::hash_map<std::string, DbValue> queryFields, std::vector<std::string> columns);
	std::string buildUpdateQueryString(std::string tableName, std::string queryField, std::string value, std::hash_map<std::string,DbValue> columns);
	std::string buildInsertQueryString(std::string tableName, std::hash_map<std::string,DbValue> columns);

	void createDB();

	std::string mFileName;

	sqlite3* mDB;
	static SQLiteDBManager *mSQLiteDBManager;

};

#endif