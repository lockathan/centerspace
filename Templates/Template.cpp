#include <Templates/Template.h>

Template::Template() 
	:
	mId(0),
	mName("")
{

}

Template::~Template()
{

}

int Template::getId()
{
	return mId;
}

void Template::setId(int id)
{
	mId = id;
}

std::string Template::getName()
{
	return mName;
}

void Template::setName(std::string name)
{
	mName = name;
}
