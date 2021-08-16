#include <LuaLibRocket/LuaDocument.h>
#include <LuaLibRocket/LuaElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <string>

LuaDocument::LuaDocument(Rocket::Core::ElementDocument* document) 
	: 
	LuaElement(document),
	mDocument(document)
{

}


LuaDocument::~LuaDocument()
{

}

Rocket::Core::ElementDocument* LuaDocument::getDocument()
{
	return mDocument;
}

