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
	static juce::String getBranchPathName();
	static juce::String getPackagePathName();

	static juce::String getEditorPath();
	static juce::String getConfigPath();
	static juce::String getThemePath();
	static juce::String getTranslatePath();
	static juce::String getScriptPath();

	static std::unique_ptr<juce::var> configs;
	static std::unique_ptr<juce::var> translates;

	static juce::String path;
	static juce::String branch;
	static juce::String package;

	static juce::String componentPath;
	static juce::String configPath;
	static juce::String themePath;
	static juce::String translatePath;
	static juce::String scriptPath;
};

