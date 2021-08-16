#ifndef RocketDocuments_h
#define RocketDocuments_h

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include "RocketDocumentDefinition.h"

class RocketDocuments
{
public:
	class ProfileMenu
	{
	public:
		static const RocketDocumentDefinition ProfileMain;
	};
	class MainMenu
	{
	public:
		static const RocketDocumentDefinition Main;
		static const RocketDocumentDefinition SinglePlayer;
		static const RocketDocumentDefinition Campaign;
		static const RocketDocumentDefinition Multiplayer;
		static const RocketDocumentDefinition Options;

	};

	class InBase
	{
	public:
		static const RocketDocumentDefinition Main;
		static const RocketDocumentDefinition MissionSelect;
		static const RocketDocumentDefinition PostBattle;

	};

	class MCUBay
	{
	public:
		static const RocketDocumentDefinition Main;
		static const RocketDocumentDefinition MCUPurchase;
		static const RocketDocumentDefinition MCUSelectEdit;
		static const RocketDocumentDefinition MCUEdit;
		static const RocketDocumentDefinition MCUComponentSelect;

	};

	class InBattle
	{
	public:
		static const RocketDocumentDefinition InBattleHUD;
		static const RocketDocumentDefinition InBattleMenu;
		static const RocketDocumentDefinition BetweenTurnsHUD;
	};
};

#endif
