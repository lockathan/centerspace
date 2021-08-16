#ifndef LuaElement_h
#define LuaElement_h

#include <Rocket/Core.h>

class LuaDocument;

class LuaElement
{
public:
	LuaElement(Rocket::Core::Element* element);
	virtual ~LuaElement();

	Rocket::Core::Element* getRocketElement();

	std::string getId();
	void setId(const std::string& id);

	std::string getTagName();

	std::string getName();
	void setName(const std::string& name);

	bool isClassSet(const std::string& className);
	void setClass(const std::string& className, bool enable);
	
	std::string getProperty(const std::string& name);
	void setProperty(const std::string& name, const std::string& value);
	void removeProperty(const std::string& name);

	std::string getAttribute(const std::string& name);
	bool hasAttribute(const std::string& name);
	void setAttribute(const std::string& name, const std::string& value);
	void removeAttribute(const std::string& name);

	void focus();
	void blur();
	void click();

	LuaDocument* getOwnerDocument();
	LuaElement* getParentNode();
	LuaElement* getNextSibling();
	LuaElement* getPreviousSibling();
	LuaElement* getFirstChild();
	LuaElement* getLastChild();
	LuaElement* getChild(int index);
	int getNumChildren(bool include_non_dom_elements = false);
	int getItemId();
	bool hasChildNodes();
	void removeChild(LuaElement element);
	

	std::string	getInnerRML();
	void setInnerRML(const std::string &rml);

	LuaElement* getElementById(const std::string &id);
	LuaElement* getElementByName(const std::string& name);

private:
	Rocket::Core::Element* mElement;
};

#endif
