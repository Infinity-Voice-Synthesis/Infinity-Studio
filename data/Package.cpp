#include "Package.h"

Package Package::_package;

Package& Package::getPackage()
{
	return Package::_package;
}

Package::Package()
{

}

Package::~Package()
{

}

std::list<std::string> Package::getLibraryAvailable()
{
	return std::list<std::string>();
}

std::string Package::getLibraryDefault()
{
	return std::string();
}

std::string Package::getEngineName(const std::string& library)
{
	return std::string();
}

std::list<std::string> Package::getLibraryTimbre(const std::string& library)
{
	return std::list<std::string>();
}

std::string Package::getLibraryTimbreDefault(const std::string& library)
{
	return std::string();
}

std::string Package::getLibraryDictionaryDefault(const std::string& library)
{
	return std::string();
}

//

std::list<std::string> Package::getEngineAvailable()
{
	return std::list<std::string>();
}

std::list<Package::Param> Package::getEngineParam(const std::string& engine)
{
	return std::list<Package::Param>();
}

std::list<Package::Param> Package::getEngineNoteParam(const std::string& engine)
{
	return std::list<Package::Param>();
}

bool Package::getEngineSplit(std::string engine)
{
	return false;
}

//

std::list<std::string> Package::getDictionaryAvailable()
{
	return std::list<std::string>();
}

std::pair<std::map<std::string, int64_t>, bool> Package::getDictionaryPhoneme(const std::string& dictionary, const std::string& name)
{
	return std::make_pair(std::map<std::string, int64_t>(), false);
}
