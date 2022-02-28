#include "Config.h"

Config Config::_config;

Config::Config()
{
	Config::configs = std::make_unique<juce::var>();
	Config::translates = std::make_unique<juce::var>();
}

void Config::init(const juce::String& path, const juce::String& branch, const juce::String& package)
{
	Config::_config.path = path;
	Config::_config.branch = branch;
	Config::_config.package = package;
}

void Config::refreshConfigs()
{
	const juce::String& configPath = Config::_config.getConfigPath();
	const juce::String configFile = configPath + "config.json";

	juce::File file(configFile);
	*(Config::_config.configs.get()) = juce::JSON::parse(file);
}

void Config::refreshTranslates()
{
	const juce::String& translatePath = Config::_config.getTranslatePath();
	const juce::String& lanName = (*(Config::_config.configs.get()))["language"].toString();
	const juce::String translateFile = translatePath + lanName + ".json";

	juce::File file(translateFile);
	*(Config::_config.translates) = juce::JSON::parse(file);
}

juce::String Config::tr(const juce::String&& s)
{
	return (*(Config::_config.translates))[s.toStdString().c_str()].toString();
}

juce::String Config::getBranchPathName()
{
	if (this->branch == "main") {
		return juce::String("");
	}
	else {
		return juce::String(this->branch + "/");
	}
}

juce::String Config::getPackagePathName()
{
	return juce::String(this->package + "/");
}

juce::String Config::getEditorPath()
{
	return juce::String(
		this->path +
		this->componentPath +
		this->getBranchPathName() +
		this->getPackagePathName()
	);
}

juce::String Config::getConfigPath()
{
	return juce::String(
		this->getEditorPath() +
		this->configPath
	);
}

juce::String Config::getThemePath()
{
	return juce::String(
		this->getEditorPath() +
		this->themePath
	);
}

juce::String Config::getTranslatePath()
{
	return juce::String(
		this->getEditorPath() +
		this->translatePath
	);
}

juce::String Config::getScriptPath()
{
	return juce::String(
		this->getEditorPath() +
		this->scriptPath
	);
}
