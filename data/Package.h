#pragma once

#include <string>
#include <list>
#include <map>

class Package
{
	static Package _package;

	Package();
	~Package();

public:
	static Package& getPackage();

	class Param {
	public:
		std::string name;
		double vMax;
		double vMin;
		double vDefault;
	};

	//声库
	std::list<std::string> getLibraryAvailable();
	std::string getLibraryDefault();
	std::string getEngineName(std::string library);
	std::list<std::string> getLibraryTimbre(std::string library);
	std::string getLibraryTimbreDefault(std::string library);
	std::string getLibraryDictionaryDefault(std::string library);

	//引擎
	std::list<std::string> getEngineAvailable();
	std::list<Package::Param> getEngineParam(std::string engine);//ugly!
	std::list<Package::Param> getEngineNoteParam(std::string engine);//ugly!
	bool getEngineSplit(std::string engine);

	//字典
	std::list<std::string> getDictionaryAvailable();
	std::pair<std::map<std::string, int64_t>, bool> getDictionaryPhoneme(std::string dictionary, std::string name);
};

