#include <Managers/SQLiteDBManager.h>
#include <Managers/DbNames.h>
#include <Ogre.h>
#include <sqlite3.h>
#include <States/CampaignState.h>
#include <States/InventoryState.h>
#include <Entities/PlayerProfile.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/WeaponHardpoint.h>
#include <vector>
#include <hash_map>
#include <deque>
#include <string>
#include <sstream>

using namespace Ogre;

SQLiteDBManager::SQLiteDBManager()
{
	mFileName = "CenterSpace.sqlite";
	int rc = sqlite3_open(mFileName.c_str(),&mDB);
	switch(rc)
	{
	case SQLITE_OK:
		createDB();
		break;
	case SQLITE_CANTOPEN:
		break;
	case SQLITE_CORRUPT:
		break;
	case SQLITE_DENY:
		break;
	case SQLITE_FAIL:
		break;
	case SQLITE_IOERR:
		break;
	}
}

SQLiteDBManager::~SQLiteDBManager() 
{
	sqlite3_close(mDB);
}

PlayerProfile* SQLiteDBManager::getPlayerProfile(long id)
{
	

	std::vector<std::string> fields;
	fields.push_back(DbNames::PlayerProfile::ID);
	fields.push_back(DbNames::PlayerProfile::NAME);
	
	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::PlayerProfile::TABLE_NAME, id, fields),fields);

	if(values.size() == 0)
		return 0;
	PlayerProfile* profile = new PlayerProfile();
	profile->setValues(values);

	return profile;
}

std::vector<PlayerProfile*> SQLiteDBManager::getPlayerProfiles()
{
	std::vector<PlayerProfile*> results;
	std::vector<std::hash_map<std::string,std::string>> queryResults;

	std::vector<std::string> fields;
	fields.push_back(DbNames::PlayerProfile::ID);
	fields.push_back(DbNames::PlayerProfile::NAME);

	queryResults = selectFieldsMultipleRecords(buildSelectQueryString(DbNames::PlayerProfile::TABLE_NAME,"1","1",fields),fields);

	std::vector<std::hash_map<std::string,std::string>>::iterator playerIterator;
	
	for(playerIterator = queryResults.begin();playerIterator<queryResults.end(); ++playerIterator)
	{
		PlayerProfile* player = new PlayerProfile();
		player->setValues(*playerIterator);
		results.push_back(player);
	}

	return results;
}

std::deque<MechanizedCombatUnit*> SQLiteDBManager::getMechanizedCombatUnits(InventoryState* inv)
{
	std::deque<MechanizedCombatUnit*> results;
	std::vector<std::hash_map<std::string,std::string>> queryResults;

	std::vector<std::string> fields;
	fields.push_back(DbNames::MCU::ID);
	fields.push_back(DbNames::MCU::INVENTORY_ID);
	fields.push_back(DbNames::MCU::TEMPLATE_ID);
	fields.push_back(DbNames::MCU::ARMOR_ID);
	fields.push_back(DbNames::MCU::BATTERY_ID);
	fields.push_back(DbNames::MCU::DRIVE_ID);
	fields.push_back(DbNames::MCU::REACTOR_ID);
	fields.push_back(DbNames::MCU::SENSOR_ID);
	fields.push_back(DbNames::MCU::SHIELD_ID);
	fields.push_back(DbNames::MCU::TARGETING_SYS_ID);
	fields.push_back(DbNames::MCU::NAME);

	queryResults = selectFieldsMultipleRecords(buildSelectQueryString(DbNames::MCU::TABLE_NAME,DbNames::MCU::INVENTORY_ID,convertLongToString(inv->getId()),fields),fields);

	std::vector<std::hash_map<std::string,std::string>>::iterator mcuIterator;
	
	for(mcuIterator = queryResults.begin();mcuIterator<queryResults.end(); ++mcuIterator)
	{
		MechanizedCombatUnit* mcu = new MechanizedCombatUnit();
		mcu->setValues(*mcuIterator);
		mcu->setWeapons(getWeaponHardpoints(mcu));
		mcu->setInventoryState(inv);
		results.push_back(mcu);
	}

	return results;
}

MechanizedCombatUnit* SQLiteDBManager::getMechanizedCombatUnit(long id)
{
	

	std::vector<std::string> fields;
	fields.push_back(DbNames::MCU::ID);
	fields.push_back(DbNames::MCU::INVENTORY_ID);
	fields.push_back(DbNames::MCU::TEMPLATE_ID);
	fields.push_back(DbNames::MCU::ARMOR_ID);
	fields.push_back(DbNames::MCU::BATTERY_ID);
	fields.push_back(DbNames::MCU::DRIVE_ID);
	fields.push_back(DbNames::MCU::REACTOR_ID);
	fields.push_back(DbNames::MCU::SENSOR_ID);
	fields.push_back(DbNames::MCU::SHIELD_ID);
	fields.push_back(DbNames::MCU::TARGETING_SYS_ID);
	fields.push_back(DbNames::MCU::NAME);

	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::MCU::TABLE_NAME, id, fields),fields);
	
	if(values.size() == 0)
		return 0;

	MechanizedCombatUnit* mcu = new MechanizedCombatUnit();

	mcu->setValues(values);
	mcu->setWeapons(getWeaponHardpoints(mcu));
	
	return mcu;
}

std::vector<WeaponHardpoint*> SQLiteDBManager::getWeaponHardpoints(MechanizedCombatUnit* mcu)
{
	std::vector<WeaponHardpoint*> results;
	std::vector<std::hash_map<std::string,std::string>> queryResults;

	std::vector<std::string> fields;
	fields.push_back(DbNames::MCUWeapon::ID);
	fields.push_back(DbNames::MCUWeapon::MCU_ID);
	fields.push_back(DbNames::MCUWeapon::HARDPOINT_NUMBER);
	fields.push_back(DbNames::MCUWeapon::WEAPONTEMPLATE_ID);

	queryResults = selectFieldsMultipleRecords(buildSelectQueryString(DbNames::MCUWeapon::TABLE_NAME,DbNames::MCUWeapon::MCU_ID,convertLongToString(mcu->getId()),fields),fields);

	std::vector<std::hash_map<std::string,std::string>>::iterator weaponIterator;
	
	for(weaponIterator = queryResults.begin();weaponIterator<queryResults.end(); ++weaponIterator)
	{
		WeaponHardpoint* weapon = new WeaponHardpoint();
		weapon->setMCU(mcu);
		weapon->setValues(*weaponIterator);
		results.push_back(weapon);
	}

	return results;
}

WeaponHardpoint* SQLiteDBManager::getWeaponHardpoint(long id)
{
	

	std::vector<std::string> fields;
	fields.push_back(DbNames::MCUWeapon::ID);
	fields.push_back(DbNames::MCUWeapon::MCU_ID);
	fields.push_back(DbNames::MCUWeapon::HARDPOINT_NUMBER);
	fields.push_back(DbNames::MCUWeapon::WEAPONTEMPLATE_ID);
	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::MCUWeapon::TABLE_NAME, id, fields),fields);

	if(values.size() == 0)
		return 0;

	WeaponHardpoint* hardpoint = new WeaponHardpoint();
	hardpoint->setValues(values);

	return hardpoint;
}

InventoryState* SQLiteDBManager::getInventoryState(long inventoryId)
{
	
	std::vector<std::string> fields;
	fields.push_back(DbNames::InventoryState::ID);
	fields.push_back(DbNames::InventoryState::RANK);
	fields.push_back(DbNames::InventoryState::REMAININGCASH);
	fields.push_back(DbNames::InventoryState::FACTION_ID);
	
	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::InventoryState::TABLE_NAME, inventoryId, fields),fields);
	
	if(values.size() == 0)
		return 0;
	InventoryState* inventory = new InventoryState();
	inventory->setValues(values);

	inventory->setMCUs(getMechanizedCombatUnits(inventory));
	
	return inventory;
}

CampaignState* SQLiteDBManager::getCampaignState(long campaignId)
{
	std::vector<std::string> fields;
	fields.push_back(DbNames::CampaignState::ID);
	fields.push_back(DbNames::CampaignState::INVENTORY_ID);
	fields.push_back(DbNames::CampaignState::PlayerProfile_ID);
	fields.push_back(DbNames::CampaignState::FACTION_ID);
	fields.push_back(DbNames::CampaignState::RANK);
	fields.push_back(DbNames::CampaignState::HUB_ID);
	fields.push_back(DbNames::CampaignState::DIFFICULTY);
	fields.push_back(DbNames::CampaignState::EXPERIENCE);


	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::CampaignState::TABLE_NAME,DbNames::CampaignState::ID, convertLongToString(campaignId), fields),fields);
	if(values.size() == 0)
		return 0;

	CampaignState* campaign = new CampaignState(false);
	campaign->setValues(values);
	campaign->setInventoryState(getInventoryState(atol(values[DbNames::CampaignState::INVENTORY_ID].c_str())));
	
	return campaign;
}

CampaignState* SQLiteDBManager::getCampaignState(long playerProfileId, int factionId)
{
	std::vector<std::string> fields;
	fields.push_back(DbNames::CampaignState::ID);
	fields.push_back(DbNames::CampaignState::INVENTORY_ID);
	fields.push_back(DbNames::CampaignState::PlayerProfile_ID);
	fields.push_back(DbNames::CampaignState::FACTION_ID);
	fields.push_back(DbNames::CampaignState::RANK);
	fields.push_back(DbNames::CampaignState::HUB_ID);
	fields.push_back(DbNames::CampaignState::DIFFICULTY);
	fields.push_back(DbNames::CampaignState::EXPERIENCE);

	std::hash_map<std::string, DbValue> queryFields;
	queryFields[DbNames::CampaignState::PlayerProfile_ID].StringValue = convertLongToString(playerProfileId);
	queryFields[DbNames::CampaignState::PlayerProfile_ID].IsNumber = true;

	queryFields[DbNames::CampaignState::FACTION_ID].StringValue = convertIntToString(factionId);
	queryFields[DbNames::CampaignState::FACTION_ID].IsNumber = true;

	std::hash_map<std::string, std::string> values = selectFieldsOneRecord(buildSelectQueryString(DbNames::CampaignState::TABLE_NAME,queryFields, fields),fields);
	
	if(values.size() == 0)
		return 0;

	CampaignState* campaign = new CampaignState(false);
	campaign->setValues(values);
	campaign->setInventoryState(getInventoryState(atol(values[DbNames::CampaignState::INVENTORY_ID].c_str())));
	campaign->setPlayerProfile(getPlayerProfile(playerProfileId));

	return campaign;
}

std::vector<CampaignState*> SQLiteDBManager::getCampaignStates(long playerProfileId)
{
	std::vector<CampaignState*> results;
	std::vector<std::hash_map<std::string,std::string>> queryResults;

	std::vector<std::string> fields;
	fields.push_back(DbNames::CampaignState::ID);
	fields.push_back(DbNames::CampaignState::INVENTORY_ID);
	fields.push_back(DbNames::CampaignState::PlayerProfile_ID);
	fields.push_back(DbNames::CampaignState::DIFFICULTY);
	fields.push_back(DbNames::CampaignState::FACTION_ID);
	fields.push_back(DbNames::CampaignState::HUB_ID);
	fields.push_back(DbNames::CampaignState::RANK);
	fields.push_back(DbNames::CampaignState::EXPERIENCE);

	queryResults = selectFieldsMultipleRecords(buildSelectQueryString(DbNames::CampaignState::TABLE_NAME,DbNames::CampaignState::PlayerProfile_ID,convertLongToString(playerProfileId),fields),fields);

	std::vector<std::hash_map<std::string,std::string>>::iterator campaignIterator;
	
	for(campaignIterator = queryResults.begin();campaignIterator<queryResults.end();++campaignIterator)
	{
		CampaignState* campaign = new CampaignState(false);
		campaign->setValues(*campaignIterator);
		campaign->setInventoryState(getInventoryState(atol((*campaignIterator)[DbNames::CampaignState::INVENTORY_ID].c_str())));
		results.push_back(campaign);
	}

	return results;
}

void SQLiteDBManager::savePlayerProfile(PlayerProfile* profile)
{
	query(buildUpdateQueryString(DbNames::PlayerProfile::TABLE_NAME,DbNames::PlayerProfile::ID,convertIntToString(profile->getId()),profile->getValues()));
}

void SQLiteDBManager::saveCampaignState(CampaignState* campaign)
{
	query(buildUpdateQueryString(DbNames::CampaignState::TABLE_NAME,DbNames::CampaignState::ID,convertIntToString(campaign->getId()),campaign->getValues()));
	
	if(campaign->getInventoryState()->getId() <= 0)
		addInventoryState(campaign->getInventoryState());
	else
		saveInventoryState(campaign->getInventoryState());
}

void SQLiteDBManager::saveInventoryState(InventoryState* inventory)
{
	query(buildUpdateQueryString(DbNames::InventoryState::TABLE_NAME,DbNames::InventoryState::ID,convertIntToString(inventory->getId()),inventory->getValues()));

	std::deque<MechanizedCombatUnit*> mcus = inventory->getMCUs();
	std::deque<MechanizedCombatUnit*>::iterator mcuIterator;

	for(mcuIterator = mcus.begin();mcuIterator < mcus.end(); ++mcuIterator)
	{
		if((*mcuIterator)->getId() <= 0)
			addMCU(*mcuIterator);
		else
			saveMCU(*mcuIterator);
	}
}

void SQLiteDBManager::saveMCU(MechanizedCombatUnit* mcu)
{
	query(buildUpdateQueryString(DbNames::MCU::TABLE_NAME,DbNames::MCU::ID,convertIntToString(mcu->getId()),mcu->getValues()));
	
	std::vector<WeaponHardpoint*> weapons = mcu->getWeapons();
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = weapons.begin();weaponIterator < weapons.end(); ++weaponIterator)
	{
		if((*weaponIterator))
		{
			if((*weaponIterator)->getId() <= 0)
				addWeaponHardpoint(*weaponIterator);
			else
				saveWeaponHardpoint(*weaponIterator);
		}
	}
}


void SQLiteDBManager::saveWeaponHardpoint(WeaponHardpoint* weapon)
{
	query(buildUpdateQueryString(DbNames::MCUWeapon::TABLE_NAME,DbNames::MCUWeapon::ID,convertIntToString(weapon->getId()),weapon->getValues()));
}

long SQLiteDBManager::addPlayerProfile(PlayerProfile* profile)
{
	long id = runInsertQuery(buildInsertQueryString(DbNames::PlayerProfile::TABLE_NAME,profile->getValues()));
	profile->setId(id);

	return id;
}

long SQLiteDBManager::addCampaignState(CampaignState* campaign)
{
	long invId = addInventoryState(campaign->getInventoryState());
	std::hash_map<std::string, DbValue> values = campaign->getValues();
	values[DbNames::CampaignState::INVENTORY_ID].StringValue = convertLongToString(invId);
	values[DbNames::CampaignState::INVENTORY_ID].IsNumber = true;
	long id = runInsertQuery(buildInsertQueryString(DbNames::CampaignState::TABLE_NAME,values));
	campaign->setId(id);

	return id;
}

long SQLiteDBManager::addInventoryState(InventoryState* inventory)
{
	long id = runInsertQuery(buildInsertQueryString(DbNames::InventoryState::TABLE_NAME,inventory->getValues()));
	inventory->setId(id);
	std::deque<MechanizedCombatUnit*> mcus = inventory->getMCUs();
	std::deque<MechanizedCombatUnit*>::iterator mcuIterator;

	for(mcuIterator = mcus.begin();mcuIterator < mcus.end(); ++mcuIterator)
	{
		addMCU(*mcuIterator);
	}

	return id;
}

long SQLiteDBManager::addMCU(MechanizedCombatUnit* mcu)
{
	long id = runInsertQuery(buildInsertQueryString(DbNames::MCU::TABLE_NAME,mcu->getValues()));
	mcu->setId(id);
	std::vector<WeaponHardpoint*> weapons = mcu->getWeapons();
	std::vector<WeaponHardpoint*>::iterator weaponIterator;

	for(weaponIterator = weapons.begin();weaponIterator < weapons.end(); ++weaponIterator)
	{
		if((*weaponIterator))
			addWeaponHardpoint(*weaponIterator);
	}
	return id;
}

long SQLiteDBManager::addWeaponHardpoint(WeaponHardpoint* weapon)
{
	long id = runInsertQuery(buildInsertQueryString(DbNames::MCUWeapon::TABLE_NAME,weapon->getValues()));
	weapon->setId(id);
	return id;
}

std::string SQLiteDBManager::buildSelectQueryString(std::string tableName, long id, std::vector<std::string> columns)
{
	return buildSelectQueryString(tableName, "ID", convertLongToString(id),columns);
}

std::string SQLiteDBManager::buildSelectQueryString(std::string tableName, std::string queryField, std::string value, std::vector<std::string> columns)
{
	std::string query = "SELECT ";
	std::vector<std::string>::iterator columnIterator;

	for(columnIterator = columns.begin();columnIterator < columns.end(); ++columnIterator)
	{
		if(columnIterator != columns.begin())
			query+= ", ";

		query+= (*columnIterator) + " ";
	}
	
	query+= " FROM " + tableName;

	query+= " WHERE " + queryField + " = " + value + ";";

	return query;
}

std::string SQLiteDBManager::buildSelectQueryString(std::string tableName, std::hash_map<std::string, DbValue> queryFields, std::vector<std::string> columns)
{
	std::string query = "SELECT ";
	std::vector<std::string>::iterator columnIterator;

	for(columnIterator = columns.begin();columnIterator < columns.end(); ++columnIterator)
	{
		if(columnIterator != columns.begin())
			query+= ", ";

		query+= (*columnIterator) + " ";
	}
	
	query+= " FROM " + tableName;

	query+= " WHERE ";

	std::hash_map<std::string, DbValue>::iterator queryIterator;

	for(queryIterator = queryFields.begin();queryIterator!= queryFields.end();++queryIterator)
	{
		if(queryIterator != queryFields.begin())
			query+= "AND ";

		query+= (*queryIterator).first + " = ";
		if((*queryIterator).second.IsNumber)
			query+= (*queryIterator).second.StringValue + " ";
		else
			query+= " '" + (*queryIterator).second.StringValue + "' ";
	}

	return query;
}

std::string SQLiteDBManager::buildUpdateQueryString(std::string tableName, std::string queryField, std::string value, std::hash_map<std::string,DbValue> columns)
{
	std::string query = "UPDATE " + tableName + " SET ";
	std::hash_map<std::string,DbValue>::iterator columnIterator;

	for(columnIterator = columns.begin();columnIterator != columns.end(); ++columnIterator)
	{
		if(columnIterator != columns.begin())
			query+= ", ";

		query+= (*columnIterator).first + " = ";
		if(!(*columnIterator).second.IsNumber)
			query+= "'" + (*columnIterator).second.StringValue + "' ";
		else
			query+= (*columnIterator).second.StringValue + " ";
	}

	query+= " WHERE " + queryField + " = " + value + ";";

	return query;
}

std::string SQLiteDBManager::buildInsertQueryString(std::string tableName, std::hash_map<std::string,DbValue> columns)
{
	std::string query = "INSERT INTO " + tableName + " ( ";
	std::hash_map<std::string,DbValue>::iterator columnIterator;

	bool first = true;

	for(columnIterator = columns.begin();columnIterator != columns.end(); ++columnIterator)
	{
		//do not insert into the ID column
		if((*columnIterator).first == "ID")
			continue;

		if(!first)
			query+= " , ";
		else
			first = false;

		

		query+= (*columnIterator).first;
	}
	query += " ) VALUES ( ";

	first = true;

	for(columnIterator = columns.begin();columnIterator != columns.end(); ++columnIterator)
	{
		if((*columnIterator).first == "ID")
			continue;

		if(!first)
			query+= " , ";
		else
			first = false;

		
		if((*columnIterator).second.IsNumber)
			query+= (*columnIterator).second.StringValue + " ";
		else
			query+= "'" + (*columnIterator).second.StringValue + "' ";
	}

	query+=" ); ";

	return query;
}

std::vector<std::vector<std::string> > SQLiteDBManager::query(std::string query)
{
	sqlite3_stmt *statement;
	std::vector<std::vector<std::string> > results;
	 
	if(sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
	    int cols = sqlite3_column_count(statement);
	    int result = 0;
	    while(true)
	    {
	        result = sqlite3_step(statement);
	             
	        if(result == SQLITE_ROW)
	        {
	            std::vector<std::string> values;
	            for(int col = 0; col < cols; ++col)
	            {
	                values.push_back((char*)sqlite3_column_text(statement, col));
	            }
	            results.push_back(values);
	        }
	        else
	        {
	            break;  
	        }
	    }
	        
	    sqlite3_finalize(statement);
	}
	     
	std::string error = sqlite3_errmsg(mDB);
	//log error
	     
	return results; 
}

long SQLiteDBManager::runInsertQuery(std::string query)
{
	sqlite3_stmt *statement;
	std::vector<std::vector<std::string> > results;
	 
	if(sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
	    int cols = sqlite3_column_count(statement);
	    int result = sqlite3_step(statement);

	    sqlite3_finalize(statement);
	}
	     
	std::string error = sqlite3_errmsg(mDB);
	//log error
	     
	return sqlite3_last_insert_rowid(mDB); 
}

std::hash_map<std::string, std::string> SQLiteDBManager::selectFieldsOneRecord(std::string query,std::vector<std::string> fields)
{
	std::hash_map<std::string, std::string> result;
	sqlite3_stmt *statement;

	if(sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
	    int cols = sqlite3_column_count(statement);
	             
	    if(sqlite3_step(statement) == SQLITE_ROW)
	    {
	            
	        for(int col = 0; col < cols; ++col)
	        {
	            result[fields[col]]= (char*)sqlite3_column_text(statement, col);
	        }
	    }
	        
	    sqlite3_finalize(statement);
	}
	     
	std::string error = sqlite3_errmsg(mDB);
	return result;
}

std::vector<std::hash_map<std::string,std::string>> SQLiteDBManager::selectFieldsMultipleRecords(std::string query,std::vector<std::string> fields)
{
	std::vector<std::hash_map<std::string,std::string>> result;
	result.resize(20);
	sqlite3_stmt *statement;

	if(sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
	    int cols = sqlite3_column_count(statement);
	    int row = 0;
		int recordNumber = 0;
	    while(true)
	    {
	        row = sqlite3_step(statement);
	             
	        if(row == SQLITE_ROW)
	        {
				for(int col = 0; col < cols; ++col)
	            {
					result[recordNumber][fields[col]] = (char*)sqlite3_column_text(statement, col);	
				}
				recordNumber++;
	        }
	        else
	        {
	            break;  
	        }
	    }
		result.resize(recordNumber);
	        
	    sqlite3_finalize(statement);
	}
	     
	std::string error = sqlite3_errmsg(mDB);

	result.shrink_to_fit();
	return result;
}

void SQLiteDBManager::createDB()
{
	sqlite3_stmt *statement;
	std::string error;
	std::string query = "CREATE TABLE IF NOT EXISTS " + DbNames::PlayerProfile::TABLE_NAME;
	query+= " (" + DbNames::PlayerProfile::ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " + DbNames::PlayerProfile::NAME + " TEXT);";

	int result;
	result = sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0);

	if(!result == SQLITE_OK)
	{
		error = sqlite3_errmsg(mDB);
		return;
	}
	else
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	query = "CREATE TABLE IF NOT EXISTS " + DbNames::InventoryState::TABLE_NAME;
	query+= " (" + DbNames::InventoryState::ID + " INTEGER PRIMARY KEY AUTOINCREMENT";
	query+= "," + DbNames::InventoryState::REMAININGCASH + ", " + DbNames::InventoryState::RANK + ", " + DbNames::InventoryState::FACTION_ID;
	query+=");";

	result = sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0);

	if(!result == SQLITE_OK)
	{
		error = sqlite3_errmsg(mDB);
		return;
	}
	else
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}


	query = "CREATE TABLE IF NOT EXISTS " + DbNames::CampaignState::TABLE_NAME; 
	query+= " (" + DbNames::CampaignState::ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " + DbNames::CampaignState::PlayerProfile_ID + " INTEGER, ";
	query+= DbNames::CampaignState::FACTION_ID + " INTEGER, " + DbNames::CampaignState::RANK + " INTEGER, " + DbNames::CampaignState::HUB_ID + " , ";
	query+= DbNames::CampaignState::INVENTORY_ID + " INTEGER, ";
	query+= DbNames::CampaignState::DIFFICULTY + " INTEGER, ";
	query+= DbNames::CampaignState::EXPERIENCE + " INTEGER, ";

	query+= "FOREIGN KEY(" + DbNames::CampaignState::PlayerProfile_ID + ") REFERENCES " + DbNames::PlayerProfile::TABLE_NAME + "(" + DbNames::PlayerProfile::ID + ")";
	query+= ");";

	result = sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0);
	if(!result == SQLITE_OK)
	{
		error = sqlite3_errmsg(mDB);
		return;
	}
	else
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	query = "CREATE TABLE IF NOT EXISTS " + DbNames::MCU::TABLE_NAME;
	query+= "(" + DbNames::MCU::ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " + DbNames::MCU::INVENTORY_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::TEMPLATE_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::ARMOR_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::BATTERY_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::DRIVE_ID + " INTEGER, "; 
	query+= "" + DbNames::MCU::REACTOR_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::SENSOR_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::SHIELD_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::TARGETING_SYS_ID + " INTEGER, ";
	query+= "" + DbNames::MCU::NAME + " TEXT ";
	query+= ", FOREIGN KEY(" + DbNames::MCU::INVENTORY_ID + ") REFERENCES " + DbNames::CampaignState::TABLE_NAME + "(" + DbNames::CampaignState::ID + ")";
	query+= ");";

	result = sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0);
	if(!result == SQLITE_OK)
	{
		error = sqlite3_errmsg(mDB);
		return;
	}
	else
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	query = "CREATE TABLE IF NOT EXISTS " + DbNames::MCUWeapon::TABLE_NAME;
	query+= "(" + DbNames::MCUWeapon::ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " + DbNames::MCUWeapon::MCU_ID + " INTEGER, ";
	query += "" + DbNames::MCUWeapon::WEAPONTEMPLATE_ID + " INTEGER, " + DbNames::MCUWeapon::HARDPOINT_NUMBER + " INTEGER "; 
	query += ",FOREIGN KEY(" + DbNames::MCUWeapon::MCU_ID + ") REFERENCES " + DbNames::MCU::TABLE_NAME + "(" + DbNames::MCU::ID + "));";
	
	result = sqlite3_prepare_v2(mDB, query.c_str(), -1, &statement, 0);
	if(!result == SQLITE_OK)
	{
		error = sqlite3_errmsg(mDB);
		return;
	}
	else
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

}

void SQLiteDBManager::setFileName(String name)
{
	mFileName = name;
}

String SQLiteDBManager::getFileName()
{
	return mFileName;
}

std::string SQLiteDBManager::convertIntToString(int value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

std::string SQLiteDBManager::convertLongToString(long value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

std::string SQLiteDBManager::convertFloatToString(float value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

SQLiteDBManager* SQLiteDBManager::mSQLiteDBManager;

SQLiteDBManager* SQLiteDBManager::getSingletonPtr()
{
	if(!mSQLiteDBManager)
	{
		mSQLiteDBManager = new SQLiteDBManager();
	}
	return mSQLiteDBManager;
}