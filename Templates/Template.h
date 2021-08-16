#ifndef Template_h
#define Template_h

#include <string>

class Template
{
public:
	Template();
	virtual ~Template();
	int getId();
	void setId(int id);
	
	std::string getName();
	void setName(std::string name);
protected:
	int mId;
	std::string mName;
};

#endif