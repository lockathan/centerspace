#ifndef Enums_h
#define Enums_h

#include <string>


enum WeaponType
{
	ENERGY = 0,
	BALLISTIC = 1,
	MISSILE = 2,
	NONE = 3
};


enum HardpointType
{
	ENERGY_ONLY = 0,
	BALLISTIC_ONLY = 1,
	MISSILE_ONLY = 2,
	ENERGY_OR_BALLISTIC = 3
};
#endif