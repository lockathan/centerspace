#ifndef DataLoader_h
#define DataLoader_h

class DataLoader
{
public:
	DataLoader(){}
	virtual ~DataLoader(){}
	void load(const std::string& fileName );
	
};

#endif