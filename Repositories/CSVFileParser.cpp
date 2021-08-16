#include <Repositories/CSVFileParser.h>
#include <Ogre.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Ogre;
using namespace std;

CSVFileParser::CSVFileParser()
{

}

CSVFileParser::~CSVFileParser()
{

}

bool CSVFileParser::parseFile(std::string fileName,std::vector<std::vector<std::string>>& result)
{
	DataStreamListPtr list = ResourceGroupManager::getSingletonPtr()->openResources(fileName);
	DataStreamList::iterator listIter;
	int count = list->size();
	for(listIter = list->begin(); listIter != list->end(); ++listIter)
	{
		if((*listIter)->isReadable())
		{
			(*listIter)->seek(0);

			std::vector<std::string> lineResult;
		

			while(!(*listIter)->eof())
			{
			
				String line = (*listIter)->getLine(true);
				tokenize(line,lineResult, ';');
				result.push_back(lineResult);
				lineResult.clear();
			}

			(*listIter)->close();
		}
	}
	
	return true;
	/*
	DataStreamPtr ds = ResourceGroupManager::getSingletonPtr()->openResource(fileName);
	if(ds->isReadable())
	{
		ds->seek(0);

		std::vector<std::string> lineResult;
		

		while(!ds->eof())
		{
			
			String line = ds->getLine(true);
			tokenize(line,lineResult, ';');
			result.push_back(lineResult);
			lineResult.clear();
		}

		ds->close();

		return true;
	}
	
	
	return false;
	*/
}

void CSVFileParser::tokenize(std::string line, std::vector<std::string>& lineResult, char separator)
{
	std::stringstream ss(line);
	std::string item;
	while(std::getline(ss, item, separator)) 
	{
		lineResult.push_back(item);
	}
}

CSVFileParser* CSVFileParser::mCSVFileParser;

CSVFileParser* CSVFileParser::getSingletonPtr()
{
	if(!mCSVFileParser)
	{
		mCSVFileParser = new CSVFileParser();
	}

	return mCSVFileParser;
}
