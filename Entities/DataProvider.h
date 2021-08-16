#ifndef DataProvider_h
#define DataProvider_h

#include <hash_map>
#include <deque>
#include <Entities/Enums.h>

class DataSet;
class DataSource;
class DataBoundElement;
class DataProviderElement;
class DataSourceUpdateListener;
class LuaScriptElement;

class FieldNames
{
public:
	static const std::string ItemID;
	class Mission
	{
	public:
		static const std::string MissionName;
		static const std::string Terrain;
		static const std::string SystemName;
		static const std::string LongDescription;
		static const std::string MissionType;
	};
	class Player
	{
	public:
		static const std::string PlayerNumber;
		static const std::string PlayerName;
		static const std::string FactionName;
		static const std::string TeamNumber;
		static const std::string Rank;
		static const std::string Cash;

	};

	class MCUTemplate
	{
	public:
		static const std::string MCUTemplateName;
		static const std::string ArmorName;
		static const std::string BatteryName;
		static const std::string DriveName;
		static const std::string ReactorName;
		static const std::string SensorSystemName;
		static const std::string ShieldName;
		static const std::string TargetingSystemName;
		static const std::string Cost;
		static const std::string Description;
	};

	class WeaponTemplate
	{
	public:
		static const std::string WeaponName;
		static const std::string Range;
		static const std::string Type;
		static const std::string WeaponCost;
		static const std::string WeaponWeight;
		static const std::string WeaponDescription;
		static const std::string ShieldDamage;
		static const std::string PiercingDamage;
		static const std::string ArmorDamage;
		static const std::string MaxAmmo;
		static const std::string EnergyCostPerTurn;
	};

	class ComponentTemplate
	{
	public:
		static const std::string ComponentName;
		static const std::string ComponentCost;
		static const std::string ComponentWeight;
		static const std::string ComponentDescription;
	};

	class MCU
	{
	public:
		static const std::string MCUName;
		static const std::string MCUTemplateName;
		static const std::string ChanceToHitTarget;
		static const std::string MaxReactorPower;
		static const std::string RemainingReactorPower;
		static const std::string MaxBatteryPower;
		static const std::string RemainingBatteryPower;
		static const std::string MaxShieldPower;
		static const std::string RemainingShieldPower;
		static const std::string ArmorName;
		static const std::string BatteryName;
		static const std::string DriveName;
		static const std::string ReactorName;
		static const std::string SensorSystemName;
		static const std::string ShieldName;
		static const std::string TargetingSystemName;
		static const std::string ThumbnailImage;
		static const std::string ProfileImage;
		static const std::string Value;
		static const std::string RemainingHealthPercent;
	};

	class Weapon
	{
	public:
		static const std::string WeaponName;
		static const std::string RemainingAmmo;
		static const std::string EnergyCost;
		static const std::string Accuracy;
		static const std::string Range;
		static const std::string CanFire;
		static const std::string ChanceToHitTarget;
		static const std::string ErrorText;
	};

	class BattleResults
	{
	public:
		static const std::string MoneyEarned;
		static const std::string TotalRepairCosts;
		static const std::string NetProfit;
		static const std::string ExperienceEarned;
		static const std::string Promotion;
	};

	class MCUStatus
	{
	public:
		static const std::string MCUName;
		static const std::string Status;
		static const std::string RepairCost;
	};
};

class DataProvider
{
public:
	~DataProvider();

	DataSource* getDataSource(DataSourceType source);

	void changeSelectedPrimaryElement(DataSourceType source, std::string dataSourceID, int id, bool propagate = true);

	void registerListener(DataBoundElement* listener);
	void initializeAllListeners();
	
	void registerForUpdates(DataProviderElement* listener);
	void registerForUpdates(DataSourceUpdateListener* listener, DataSourceType source);

	void deregister(DataProviderElement* listener);
	void deregister(DataSourceUpdateListener* listener, DataSourceType source);

	void updateDataSourceListeners(DataSourceType source);

	void clearDataSource(DataSourceType source, bool preserveListeners = false);

	static DataProvider* getSingletonPtr();
	static DataSourceType getDataSource(std::string source);
	static std::string getTypeString(DataSourceType type);
	void registerLuaScriptElement(LuaScriptElement* element);
	void deregisterLuaScriptElement(LuaScriptElement* element);

	void setDataProviderElementVisible(DataSourceType source, std::string id, bool visible);
protected:
	void initializeAllListenersImpl();
	int mCurrentId;
	std::hash_map<DataSourceType, DataSource*> mData;
	std::deque<DataProviderElement*> mDataProviderElements;

	DataSourceType mDataSource;
	
	bool mBusyUpdating;
	bool mBufferContainsElements;
	std::deque<DataBoundElement*> mListeners;
	std::deque<DataBoundElement*> mListenerBuffer;

private:
	DataProvider();
	

	static DataProvider* mDataProvider;
};

#endif
