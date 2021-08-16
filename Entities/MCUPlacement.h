#ifndef MCUPlacement_h
#define MCUPlacement_h

#include <Controllers/MCUController.h>

class AIGroup;

struct MCUPlacement
{
public:
	MCUPlacement(int x, int z, MechanizedCombatUnit* mcu) : X(x), Z(z), OwnerAIGroup(0), MCU(mcu), Placed(false) {}
	~MCUPlacement(){if(!OwnerAIGroup) delete MCU;}
	int X;
	int Z;
	AIGroup* OwnerAIGroup;
	MechanizedCombatUnit* MCU;
	bool Placed;
};
#endif