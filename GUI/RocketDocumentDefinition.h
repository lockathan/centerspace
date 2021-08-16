#ifndef RocketDocumentDefinition_h
#define RocketDocumentDefinition_h

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>

class RocketDocumentDefinition
{
public:
	RocketDocumentDefinition(){}
	RocketDocumentDefinition(Rocket::Core::String id, Rocket::Core::String name, bool modal = false);
	bool Modal;
	Rocket::Core::String ID;
	Rocket::Core::String Filename;
};

#endif
