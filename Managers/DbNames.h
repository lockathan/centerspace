#ifndef DbNames_h
#define DbNames_h

#include <string>

class DbNames
{
public:
	class PlayerProfile
	{
	public:
		static const std::string TABLE_NAME;
		static const std::string ID;
		static const std::string NAME;
	};

	class InventoryState
	{
	public:
		static const std::string TABLE_NAME;
		static const std::string ID;
		static const std::string REMAININGCASH;
		static const std::string RANK;
		static const std::string FACTION_ID;

	};
	class CampaignState
	{
	public:
		static const std::string TABLE_NAME;
		static const std::string ID;
		static const std::string PlayerProfile_ID;
		static const std::string FACTION_ID;
		static const std::string RANK;
		static const std::string HUB_ID;
		static const std::string DIFFICULTY;
		static const std::string INVENTORY_ID;
		static const std::string EXPERIENCE;

	};

	class MCU
	{
	public:
		static const std::string TABLE_NAME;
		static const std::string ID;
		static const std::string NAME;
		static const std::string INVENTORY_ID;
		static const std::string TEMPLATE_ID;
		static const std::string ARMOR_ID;
		static const std::string BATTERY_ID;
		static const std::string DRIVE_ID;
		static const std::string REACTOR_ID;
		static const std::string SENSOR_ID;
		static const std::string SHIELD_ID;
		static const std::string TARGETING_SYS_ID;

	};

	class MCUWeapon
	{
	public:
		static const std::string TABLE_NAME;
		static const std::string ID;
		static const std::string MCU_ID;
		static const std::string WEAPONTEMPLATE_ID;
		static const std::string HARDPOINT_NUMBER;
	};

};
#endif