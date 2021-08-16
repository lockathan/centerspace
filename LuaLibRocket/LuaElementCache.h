#ifndef LuaElementCache_h
#define LuaElementCache_h

#include <deque>

class LuaElement;

class LuaElementCache
{
public:
	~LuaElementCache();

	void addLuaElement(LuaElement* element);
	void removeLuaElement(LuaElement* element);
	void clearAllLuaElements();

	static LuaElementCache* getSingletonPtr();
private:
	static LuaElementCache* mLuaElementCache;
	LuaElementCache();
	std::deque<LuaElement*> mLuaElements;
};

#endif
