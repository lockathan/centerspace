#include "RocketDocumentDefinition.h"
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>

RocketDocumentDefinition::RocketDocumentDefinition(Rocket::Core::String id, Rocket::Core::String name, bool modal)
{
	ID = id;
	Filename = name;
	Modal = modal;
}