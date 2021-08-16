#include <LuaLibRocket/LuaElement.h>
#include <LuaLibRocket/LuaDocument.h>
#include <LuaLibRocket/LuaElementCache.h>
#include <GUI/DataBoundElement.h>
#include <GUI/DataBoundListElement.h>
#include <GUI/DataBoundListItemElement.h>
#include <Rocket/Core.h>
#include <Rocket/Core/String.h>
#include <string>

LuaElement::LuaElement(Rocket::Core::Element* element)
	:
	mElement(element)
{
	LuaElementCache::getSingletonPtr()->addLuaElement(this);
}


LuaElement::~LuaElement()
{
	
}

Rocket::Core::Element* LuaElement::getRocketElement()
{
	return mElement;
}

std::string LuaElement::getId()
{
	return mElement->GetId().CString();
}

void LuaElement::setId(const std::string& id)
{
	mElement->SetId(id.c_str());
}

std::string LuaElement::getTagName()
{
	return mElement->GetTagName().CString();
}

std::string LuaElement::getName()
{
	return mElement->GetAttribute<Rocket::Core::String>("name", "").CString();
}

void LuaElement::setName(const std::string& name)
{
	mElement->SetAttribute("name", name);
}

bool LuaElement::isClassSet(const std::string& className)
{
	return mElement->IsClassSet(className.c_str());
}

void LuaElement::setClass(const std::string& className, bool enable)
{
	mElement->SetClass(className.c_str(), enable);
}

std::string LuaElement::getProperty(const std::string& name)
{
	return mElement->GetProperty(name.c_str())->ToString().CString();
}

void LuaElement::setProperty(const std::string& name, const std::string& value)
{
	mElement->SetProperty(name.c_str(), value.c_str());
}

void LuaElement::removeProperty(const std::string& name)
{
	mElement->RemoveProperty(name.c_str());
}

std::string LuaElement::getAttribute(const std::string& name)
{
	if(hasAttribute(name))
		return mElement->GetAttribute<Rocket::Core::String>(name.c_str(), "").CString();

	return "";
}

bool LuaElement::hasAttribute(const std::string& name)
{
	return mElement->HasAttribute(name.c_str());
}

void LuaElement::setAttribute(const std::string& name, const std::string& value)
{
	mElement->SetAttribute<Rocket::Core::String>(name.c_str(), value.c_str());
}

void LuaElement::removeAttribute(const std::string& name)
{
	mElement->RemoveAttribute(name.c_str());
}

void LuaElement::focus()
{
	mElement->Focus();
}

void LuaElement::blur()
{
	mElement->Blur();
}

void LuaElement::click()
{
	mElement->Click();
}

LuaDocument* LuaElement::getOwnerDocument()
{
	return new LuaDocument(mElement->GetOwnerDocument());
}

LuaElement* LuaElement::getParentNode()
{
	return new LuaElement(mElement->GetParentNode());
}

LuaElement* LuaElement::getNextSibling()
{
	return new LuaElement(mElement->GetNextSibling());
}

LuaElement* LuaElement::getPreviousSibling()
{
	return new LuaElement(mElement->GetPreviousSibling());
}

LuaElement* LuaElement::getFirstChild()
{
	return new LuaElement(mElement->GetFirstChild());
}

LuaElement* LuaElement::getLastChild()
{
	return new LuaElement(mElement->GetLastChild());
}

LuaElement* LuaElement::getChild(int index)
{
	return new LuaElement(mElement->GetChild(index));
}

int LuaElement::getNumChildren(bool include_non_dom_elements)
{
	return mElement->GetNumChildren(include_non_dom_elements);
}

int LuaElement::getItemId()
{
	Rocket::Core::Element* parent;
	int itemId = mElement->GetAttribute<int>("itemid", -1);

	if(itemId == -1)
	{
		parent = mElement->GetParentNode();
		while(parent)
		{
			if(parent->GetTagName() == "databoundlistitem")
			{
				DataBoundListItemElement* listItem = dynamic_cast<DataBoundListItemElement*>(parent);
				
				if(listItem)
				{
					itemId = listItem->getItemID();
				}
			}
			parent = parent->GetParentNode();
		}
	}
	return itemId;
}

bool LuaElement::hasChildNodes()
{
	return mElement->HasChildNodes();
}

void LuaElement::removeChild(LuaElement element)
{
	mElement->RemoveChild(element.getRocketElement());
}
	

std::string	LuaElement::getInnerRML()
{
	Rocket::Core::String content;
	mElement->GetInnerRML(content);
	return content.CString();
}

void LuaElement::setInnerRML(const std::string &rml)
{
	mElement->SetInnerRML(rml.c_str());
}

LuaElement* LuaElement::getElementById(const std::string &id)
{
	Rocket::Core::Element* element = mElement->GetElementById(id.c_str());
	if(element)
		return new LuaElement(element);
	return 0;
}

LuaElement* LuaElement::getElementByName(const std::string& name)
{
	if(getName() == name)
		return new LuaElement(mElement);

	for(unsigned int i = 0; i < mElement->GetNumChildren(); ++i)
	{
		Rocket::Core::Element* child(0);
		child = mElement->GetChild(i);
		if(child)
		{
			if(child->GetAttribute<Rocket::Core::String>("name", "").CString() == name.c_str())
			{
				return new LuaElement(child);
			}
		}
	}

	return 0;
}
