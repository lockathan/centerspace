#ifndef CSVFileParser_h
#define CSVFileParser_h

#include <string>
#include <vector>

class CSVFileParser
{
public:
	bool parseFile(std::string fileName, std::vector<std::vector<std::string>>& result);
	~CSVFileParser();
	static CSVFileParser* getSingletonPtr();
protected:
	
private:
	void tokenize(std::string line, std::vector<std::string>& lineResult, char separator);
	static CSVFileParser* mCSVFileParser;
	CSVFileParser();
};
#endif