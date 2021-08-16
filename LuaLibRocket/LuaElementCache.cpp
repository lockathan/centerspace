#include <LuaLibRocket/LuaElement.h>
#include <LuaLibRocket/LuaDocument.h>
#include <Rocket/Core.h>
#include <LuaLibRocket/LuaElementCache.h>
#include <deque>

LuaElementCache::LuaElementCache()
{
	
}


LuaElementCache::~LuaElementCache()
{

}

void LuaElementCache::addLuaElement(LuaElement* element)
{
	mLuaElements.push_back(element);
}

void LuaElementCache::removeLuaElement(LuaElement* element)
{
	std::deque<LuaElement*>::iterator iter;

	for(iter = mLuaElements.begin(); iter != mLuaElements.end(); ++iter)
	{
		if((*iter) == element)
		{
			mLuaElements.erase(iter);
			return;
		}
	}
}

void LuaElementCache::clearAllLuaElements()
{
	std::deque<LuaElement*>::iterator iter;

	for(iter = mLuaElements.begin(); iter != mLuaElements.end(); )
	{
		delete (*iter);
		iter = mLuaElements.erase(iter);
	}
}

LuaElementCache* LuaElementCache::mLuaElementCache;

LuaElementCache* LuaElementCache::getSingletonPtr()
{
	if(!mLuaElementCache)
	{
		mLuaElementCache = new LuaElementCache();
	}

	return mLuaElementCache;
}
