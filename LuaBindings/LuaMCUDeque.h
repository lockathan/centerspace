#ifndef LuaMCUDeque_h
#define LuaMCUDeque_h

#include <deque>
#include <Entities/MCUPlacement.h>
class MechanizedCombatUnit;

class LuaMCUDeque
{
public:
	LuaMCUDeque();
	LuaMCUDeque(std::deque<MechanizedCombatUnit*>& mcus);
	LuaMCUDeque(std::deque<MCUPlacement*>& mcus);
	~LuaMCUDeque();

	void addMechanizedCombatUnit(MechanizedCombatUnit* mcu);
	void setMCUs(std::deque<MechanizedCombatUnit*>& mcus);
	std::deque<MCUPlacement*>& getMCUPlacements();

	MCUPlacement* getNext();
	bool hasMoreItems();
	void reset();
	unsigned int numItems();

	int getSeedX();
	void setSeedX(int seed);

	int getSeedZ();
	void setSeedZ(int seed);

	bool isCellOccupied(int x, int z);

	int getStrength();
private:
	std::deque<MCUPlacement*> mMCUs;
	std::deque<MCUPlacement*>::iterator mIterator;
	int mSeedX;
	int mSeedZ;
};
#endif