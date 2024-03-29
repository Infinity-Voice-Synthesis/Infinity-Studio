#pragma once

#include <JuceHeader.h>

class Config final
{
	static std::unique_ptr<Config> _config;
public:
	Config();

	static void refreshConfigs();
	static void refreshTranslates();

	static void init(const juce::String& path, const juce::String& branch, const juce::String& package);
	static void destory();

	static juce::var cf(const juce::String& key);//配置文件
	static juce::String tr(const juce::String& s);//翻译文件
	static juce::var tm(const juce::String& obj, const juce::String& key);//主题配置文件
	static juce::String ts(const juce::String& obj, const juce::String& key);//主题资源文件
	static juce::String tsFull(const juce::String& obj, const juce::String& key);//主题资源文件完整路径
	static juce::Colour tc(const juce::String& obj, const juce::String& key);//主题颜色

	static juce::var& get();
	static juce::var& getTrans();
	static juce::var& getTheme();
	static juce::var& getThemeSrc();
	static juce::HashMap<juce::String, juce::var>& getThemeColors();

	static juce::String script(const juce::String& name);
	static juce::String scriptDir();
	static juce::String appPath();
	static juce::String appDir();

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

	juce::HashMap<juce::String, juce::var> palettes;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Config)
};

