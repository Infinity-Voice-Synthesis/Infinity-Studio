#include "Config.h"

std::unique_ptr<Config> Config::_config = std::make_unique<Config>();

Config::Config()
	:componentPath("packages/"),
	configPath("configs/"),
	themePath("themes/"),
	translatePath("translates/"),
	scriptPath("scripts/"),
	srcPath("src/"),
	palettePath("palettes/")
{
	Config::configs = std::make_unique<juce::var>();
	Config::translates = std::make_unique<juce::var>();
	Config::theme = std::make_unique<juce::var>();
	Config::themeSources = std::make_unique<juce::var>();
}

void Config::init(const juce::String& path, const juce::String& branch, const juce::String& package)
{
	Config::_config->path = path;
	Config::_config->branch = branch;
	Config::_config->package = package;
}

void Config::destory()
{
	Config::_config = nullptr;
}

void Config::refreshConfigs()
{
	const juce::String& configPath = Config::_config->getConfigPath();
	const juce::String configFile = configPath + "config.json";

	juce::File cFile(configFile);
	*(Config::_config->configs) = juce::JSON::parse(cFile);

	const juce::String& themePath = Config::_config->getThemeFilePath();
	const juce::String& themeFile = themePath + "config.json";
	const juce::String& themeSrcFile = themePath + "source.json";

	juce::File tFile(themeFile);
	*(Config::_config->theme) = juce::JSON::parse(tFile);

	juce::File tsFile(themeSrcFile);
	*(Config::_config->themeSources) = juce::JSON::parse(tsFile);
}

void Config::refreshTranslates()
{
	const juce::String& translatePath = Config::_config->getTranslatePath();
	const juce::String& lanName = Config::cf("language").toString();
	const juce::String translateFile = translatePath + lanName + ".json";

	juce::File file(translateFile);
	*(Config::_config->translates) = juce::JSON::parse(file);
}

juce::var Config::cf(const juce::String& key)
{
	return Config::get()[key.toStdString().c_str()];
}

juce::String Config::tr(const juce::String& s)
{
	if (s.isEmpty()) {
		return s;
	}
	juce::String&& str = Config::getTrans()[s.toStdString().c_str()].toString();
	if (str.isEmpty()) {
		return s;
	}
	return str;
}

juce::var Config::tm(const juce::String& obj, const juce::String& key)
{
	return Config::getTheme()[obj.toStdString().c_str()][key.toStdString().c_str()];
}

juce::String Config::ts(const juce::String& obj, const juce::String& key)
{
	return Config::getThemeSrc()[obj.toStdString().c_str()][key.toStdString().c_str()].toString();
}

juce::String Config::tsFull(const juce::String& obj, const juce::String& key)
{
	return juce::String(
		Config::_config->getThemeSourcePath() +
		Config::ts(obj, key)
	);
}

juce::var& Config::get()
{
	return *(Config::_config->configs);
}

juce::var& Config::getTrans()
{
	return *(Config::_config->translates);
}

juce::var& Config::getTheme()
{
	return *(Config::_config->theme);
}

juce::var& Config::getThemeSrc()
{
	return *(Config::_config->themeSources);
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

juce::String Config::getThemeFilePath()
{
	return juce::String(
		this->getThemePath() +
		Config::cf("theme").toString() +
		"/"
	);
}

juce::String Config::getThemeSourcePath()
{
	return juce::String(
		Config::_config->getThemeFilePath() +
		Config::_config->srcPath
	);
}

juce::String Config::getThemePalettePath()
{
	return juce::String(
		Config::_config->getThemeFilePath() +
		Config::_config->palettePath
	);
}
