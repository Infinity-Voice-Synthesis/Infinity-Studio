#include "Config.h"

Config Config::_config;

std::unique_ptr<juce::var> Config::configs;
std::unique_ptr<juce::var> Config::translates;

juce::String Config::path;
juce::String Config::branch;
juce::String Config::package;

juce::String Config::componentPath = "packages/";

juce::String Config::configPath = "configs/";
juce::String Config::themePath = "themes/";
juce::String Config::translatePath = "translates/";
juce::String Config::scriptPath = "scripts/";

Config::Config()
{
	Config::configs = std::make_unique<juce::var>();
	Config::translates = std::make_unique<juce::var>();
}

void Config::init(const juce::String& path, const juce::String& branch, const juce::String& package)
{
	Config::path = path;
	Config::branch = branch;
	Config::package = package;
}

void Config::refreshConfigs()
{
	const juce::String& configPath = Config::getConfigPath();
	const juce::String configFile = configPath + "config.json";

	juce::File file(configFile);
	*(Config::configs.get()) = juce::JSON::parse(file);
}

void Config::refreshTranslates()
{
	const juce::String& translatePath = Config::getTranslatePath();
	const juce::String& lanName = (*(Config::configs.get()))["language"].toString();
	const juce::String translateFile = translatePath + lanName + ".json";

	juce::File file(translateFile);
	*(Config::translates) = juce::JSON::parse(file);
}

juce::String Config::tr(const juce::String&& s)
{
	return (*(Config::translates))[s.toStdString().c_str()].toString();
}

juce::String Config::getBranchPathName()
{
	if (Config::branch == "main") {
		return juce::String("");
	}
	else {
		return juce::String(Config::branch + "/");
	}
}

juce::String Config::getPackagePathName()
{
	return juce::String(Config::package + "/");
}

juce::String Config::getEditorPath()
{
	return juce::String(
		Config::path +
		Config::componentPath +
		Config::getBranchPathName() +
		Config::getPackagePathName()
	);
}

juce::String Config::getConfigPath()
{
	return juce::String(
		Config::getEditorPath() +
		Config::configPath
	);
}

juce::String Config::getThemePath()
{
	return juce::String(
		Config::getEditorPath() +
		Config::themePath
	);
}

juce::String Config::getTranslatePath()
{
	return juce::String(
		Config::getEditorPath() +
		Config::translatePath
	);
}

juce::String Config::getScriptPath()
{
	return juce::String(
		Config::getEditorPath() +
		Config::scriptPath
	);
}
