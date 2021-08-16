#include <LuaBindings/LuaMCUDeque.h>
#include <Entities/MechanizedCombatUnit.h>
#include <Entities/MCUPlacement.h>
#include <Templates/MCUTemplate.h>
#include <deque>

LuaMCUDeque::LuaMCUDeque()
	:
	mSeedX(0),
	mSeedZ(0)
{

}

LuaMCUDeque::LuaMCUDeque(std::deque<MechanizedCombatUnit*>& mcus)
	:
	mSeedX(0),
	mSeedZ(0)
{
	setMCUs(mcus);
}

LuaMCUDeque::LuaMCUDeque(std::deque<MCUPlacement*>& mcus)
	:
	mMCUs(mcus),
	mSeedX(0),
	mSeedZ(0)
{
	mIterator = mMCUs.begin();
}

LuaMCUDeque::~LuaMCUDeque()
{

}
	
MCUPlacement* LuaMCUDeque::getNext()
{
	MCUPlacement* result = (*mIterator);

	++mIterator;
	return result;
}

void LuaMCUDeque::addMechanizedCombatUnit(MechanizedCombatUnit* mcu)
{
	mMCUs.push_back(new MCUPlacement(0,0,mcu));
	mIterator = mMCUs.begin();
}

void LuaMCUDeque::setMCUs(std::deque<MechanizedCombatUnit*>& mcus)
{
	std::deque<MechanizedCombatUnit*>::iterator iter;

	for(iter = mcus.begin(); iter != mcus.end(); ++iter)
	{
		MCUPlacement* placement = new MCUPlacement(0,0,(*iter));
		mMCUs.push_back(placement);
	}
	mIterator = mMCUs.begin();
}

std::deque<MCUPlacement*>& LuaMCUDeque::getMCUPlacements()
{
	return mMCUs;
}

bool LuaMCUDeque::hasMoreItems()
{
	return mIterator != mMCUs.end();
}

void LuaMCUDeque::reset()
{
	mIterator = mMCUs.begin();
}

unsigned int LuaMCUDeque::numItems()
{
	return mMCUs.size();
}

int LuaMCUDeque::getSeedX()
{
	return mSeedX;
}

void LuaMCUDeque::setSeedX(int seed)
{
	mSeedX = seed;
}

int LuaMCUDeque::getSeedZ()
{
	return mSeedZ;
}

void LuaMCUDeque::setSeedZ(int seed)
{
	mSeedZ = seed;
}


bool LuaMCUDeque::isCellOccupied(int x, int z)
{
	std::deque<MCUPlacement*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->X == x && (*iter)->Z == z && (*iter)->Placed)
			return true;
	}
	return false;
}

int LuaMCUDeque::getStrength()
{
	int baseStrength(0);

	std::deque<MCUPlacement*>::iterator iter;

	for(iter = mMCUs.begin(); iter != mMCUs.end(); ++iter)
	{
		if((*iter)->MCU->getMCUTemplate()->getMCUType() == MCUType::MT_LIGHT)
		{
			baseStrength+= 1;
		}

		if((*iter)->MCU->getMCUTemplate()->getMCUType() == MCUType::MT_MEDIUM)
		{
			baseStrength+= 2;
		}

		if((*iter)->MCU->getMCUTemplate()->getMCUType() == MCUType::MT_HEAVY)
		{
			baseStrength+= 3;
		}
	}

	return baseStrength;
}
