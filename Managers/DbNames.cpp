#include <Managers/DbNames.h>


const std::string DbNames::PlayerProfile::TABLE_NAME= "PlayerProfiles";
const std::string DbNames::PlayerProfile::ID= "ID";
const std::string DbNames::PlayerProfile::NAME= "Name";

const std::string DbNames::InventoryState::TABLE_NAME= "InventoryStates";
const std::string DbNames::InventoryState::ID= "ID";
const std::string DbNames::InventoryState::REMAININGCASH= "RemainingCash";
const std::string DbNames::InventoryState::RANK= "Rank";
const std::string DbNames::InventoryState::FACTION_ID= "FactionID";

const std::string DbNames::CampaignState::TABLE_NAME= "CampaignStates";
const std::string DbNames::CampaignState::ID= "ID";
const std::string DbNames::CampaignState::PlayerProfile_ID= "PlayerProfileID";
const std::string DbNames::CampaignState::FACTION_ID= "FactionID";
const std::string DbNames::CampaignState::RANK= "Rank";
const std::string DbNames::CampaignState::HUB_ID= "HubID";
const std::string DbNames::CampaignState::DIFFICULTY= "Difficulty";
const std::string DbNames::CampaignState::INVENTORY_ID = "Inventory_ID";
const std::string DbNames::CampaignState::EXPERIENCE = "Experience";

const std::string DbNames::MCU::TABLE_NAME= "MCUs";
const std::string DbNames::MCU::ID= "ID";
const std::string DbNames::MCU::NAME= "Name";
const std::string DbNames::MCU::INVENTORY_ID= "Inventory_ID";
const std::string DbNames::MCU::TEMPLATE_ID= "TemplateID";
const std::string DbNames::MCU::ARMOR_ID= "ArmorID";
const std::string DbNames::MCU::BATTERY_ID= "BatteryID";
const std::string DbNames::MCU::DRIVE_ID= "DriveID";
const std::string DbNames::MCU::REACTOR_ID= "ReactorID";
const std::string DbNames::MCU::SENSOR_ID= "SensorID";
const std::string DbNames::MCU::SHIELD_ID= "ShieldID";
const std::string DbNames::MCU::TARGETING_SYS_ID = "TargetSysID";

const std::string DbNames::MCUWeapon::TABLE_NAME= "MCUWeapons";
const std::string DbNames::MCUWeapon::ID= "ID";
const std::string DbNames::MCUWeapon::MCU_ID= "MCU_ID";
const std::string DbNames::MCUWeapon::WEAPONTEMPLATE_ID= "WeaponTemplateID";
const std::string DbNames::MCUWeapon::HARDPOINT_NUMBER= "HardpointNumber";