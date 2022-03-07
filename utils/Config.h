#pragma once

#include <JuceHeader.h>

class Config
{
	static std::unique_ptr<Config> _config;
public:
	Config();

	static void refreshConfigs();
	static void refreshTranslates();

	static void init(const juce::String& path, const juce::String& branch, const juce::String& package);
	static void destory();

	static juce::var cf(juce::String&& key);
	static juce::String tr(juce::String&& s);
	static juce::var tm(juce::String&& obj, juce::String&& key);
	static juce::String ts(juce::String&& obj, juce::String&& key);
	static juce::String tsFull(juce::String&& obj, juce::String&& key);

	static juce::var& get();
	static juce::var& getTrans();
	static juce::var& getTheme();
	static juce::var& getThemeSrc();

private:
	juce::String getBranchPathName();
	juce::String getPackagePathName();

	juce::String getEditorPath();
	juce::String getConfigPath();
	juce::String getThemePath();
	juce::String getTranslatePath();
	juce::String getScriptPath();

	juce::String getThemeFilePath();
	juce::String getThemeSourcePath();
	juce::String getThemePalettePath();

	std::unique_ptr<juce::var> configs;
	std::unique_ptr<juce::var> translates;

	std::unique_ptr<juce::var> theme;
	std::unique_ptr<juce::var> themeSources;

	juce::String path;
	juce::String branch;
	juce::String package;

	const juce::String componentPath;
	const juce::String configPath;
	const juce::String themePath;
	const juce::String translatePath;
	const juce::String scriptPath;
	const juce::String srcPath;
	const juce::String palettePath;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Config)
};

