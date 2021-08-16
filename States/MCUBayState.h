#ifndef MCUBayState_h
#define MCUBayState_h

#include "GameState.h"

class PlayerProfile;
class SQLiteDBManager;
class InventoryState;
class MechanizedCombatUnit;
class MCUComponentTemplate;
class MCUTemplate;
class WeaponTemplate;
class WeaponHardpoint;

class MCUBayState : public GameState 
{
public:

	enum ComponentType
	{
		CT_ARMOR = 0,
		CT_BATTERY,
		CT_DRIVE,
		CT_REACTOR,
		CT_SENSOR,
		CT_SHIELD,
		CT_TARGET,
		CT_WEAPON,
		CT_NONE
	};

	~MCUBayState();

	void enter();
	void exit();

	void pause();
	void resume();
	void update( Ogre::Real timeSinceLastFrame );
	void createScene();
	void destroyScene();

	PlayerProfile* getPlayerProfile();
	void setPlayerProfile(PlayerProfile* player);

	InventoryState* getInventoryState();
	void setInventoryState(InventoryState* inventory);

	
	void loadMCUPurchaseSelectScreen();
	void loadMCUEditSelectScreen();
	void loadMCUEditScreen(long mcuID);

	void changeSelectedComponent(int componentId);

	ComponentType RequestedComponentType;
	int RequestedComponentSize;
	WeaponHardpoint* RequestedWeaponHardpoint;
	long CurrentMCUID;

	void ProcessEvent(Rocket::Core::Event& event);
	
	void onSelectedElementChanged(DataSourceType source, int id);
	
	std::hash_map<std::string,std::string> getGuiValues();

	static MCUBayState* getSingletonPtr();
	
private:
	MCUBayState();
	MCUBayState( const MCUBayState& ) { }
	MCUBayState & operator = ( const MCUBayState& );

	InventoryState* mInventoryState;
	PlayerProfile* mPlayerProfile;
	MechanizedCombatUnit* mSelectedMCU;
	MCUTemplate* mSelectedMCUTemplate;

	bool mEditingWeapon;
	WeaponHardpoint* mWeaponHardpoint;
	int mComponentId;
	SQLiteDBManager* mSQLiteDBManager;
	static MCUBayState    *mMCUBayState;
};
#endif