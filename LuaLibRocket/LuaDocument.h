#ifndef LuaDocument_h
#define LuaDocument_h


#include <LuaLibRocket/LuaElement.h>
namespace Rocket
{
	namespace Core
	{
		class ElementDocument;
	}
}

class LuaDocument : public LuaElement
{
public:
	LuaDocument(Rocket::Core::ElementDocument* document);
	~LuaDocument();

	Rocket::Core::ElementDocument* getDocument();
private:
	Rocket::Core::ElementDocument* mDocument;
};

#endif
