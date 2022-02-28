#pragma once

#include <JuceHeader.h>

class Config
{
	Config();
	static Config _config;
public:
	static void refreshConfigs();
	static void refreshTranslates();

	static void init(const juce::String& path, const juce::String& branch, const juce::String& package);

	static juce::String tr(const juce::String&& s);

private:
	juce::String getBranchPathName();
	juce::String getPackagePathName();

	juce::String getEditorPath();
	juce::String getConfigPath();
	juce::String getThemePath();
	juce::String getTranslatePath();
	juce::String getScriptPath();

	std::unique_ptr<juce::var> configs;
	std::unique_ptr<juce::var> translates;

	juce::String path;
	juce::String branch;
	juce::String package;

	juce::String componentPath;
	juce::String configPath;
	juce::String themePath;
	juce::String translatePath;
	juce::String scriptPath;
};

