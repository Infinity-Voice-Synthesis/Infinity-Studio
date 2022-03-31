#pragma once

#include "Lua/lua.hpp"
#include <JuceHeader.h>

class DMH final
{
	static std::function<void(const juce::String&)> console_mesFunction;
	static std::function<void(const juce::String&)> console_assFunction;
	static std::function<void()> console_clsFunction;

public:
	static void reg_mesFunctions(
		std::function<void(const juce::String&)> console_mesFunction,
		std::function<void(const juce::String&)> console_assFunction,
		std::function<void()> cconsole_lsFunction
	);

public:
	static int test(lua_State* state);
	//static int getProjectTime(lua_State* state);
	//static int setProjectBeat(lua_State* state);
	//static int getProjectBeat(lua_State* state);
	//static int setProjectTempo(lua_State* state);
	//static int getProjectTempo(lua_State* state);
	//static int setProjectSRate(lua_State* state);
	//static int getProjectSRate(lua_State* state);
	//static int setProjectBit(lua_State* state);
	//static int getProjectBit(lua_State* state);
	//static int setProjectQuantize(lua_State* state);
	//static int getProjectQuantize(lua_State* state);
	//static int setProjectEditor(lua_State* state);
	//static int getProjectEditor(lua_State* state);
	//static int addProjectAuthor(lua_State* state);
	//static int getProjectAuthors(lua_State* state);
	////项目属性

	//static int addTrack(lua_State* state);
	//static int removeTrack(lua_State* state);
	//static int countTrack(lua_State* state);
	////轨道数量

	//static int setTrackName(lua_State* state);
	//static int getTrackName(lua_State* state);
	//static int setTrackColor(lua_State* state);
	//static int getTrackColor(lua_State* state);
	//static int setTrackMute(lua_State* state);
	//static int getTrackMute(lua_State* state);
	//static int setTrackSolo(lua_State* state);
	//static int getTrackSolo(lua_State* state);
	//static int setTrackGain(lua_State* state);
	//static int getTrackGain(lua_State* state);
	//static int setTrackPan(lua_State* state);
	//static int getTrackPan(lua_State* state);
	//static int setTrackMix(lua_State* state);
	//static int getTrackMix(lua_State* state);
	////轨道属性

	//static int addContainer(lua_State* state);
	//static int removeContainer(lua_State* state);
	//static int countContainer(lua_State* state);
	////容器数量

	//static int setContainerPlace(lua_State* state);
	//static int getContainerStartBeat(lua_State* state);
	//static int getContainerStartTick(lua_State* state);
	//static int getContainerLength(lua_State* state);
	//static int setContainerPattern(lua_State* state);
	//static int getContainerPattern(lua_State* state);
	////容器属性

	//static int addPattern(lua_State* state);
	//static int removePattern(lua_State* state);
	//static int countPattern(lua_State* state);
	////样式数量

	//static int setPatternName(lua_State* state);
	//static int getPatternName(lua_State* state);
	//static int setPatternFile(lua_State* state);
	//static int getPatternFile(lua_State* state);
	//static int clearPatternFile(lua_State* state);
	//static int setPatternDeviation(lua_State* state);
	//static int getPatternDeviation(lua_State* state);
	//static int setPatternLibrary(lua_State* state);
	//static int getPatternLibrary(lua_State* state);
	//static int setPatternDictionary(lua_State* state);
	//static int getPatternDictionary(lua_State* state);
	//static int setPatternTimbreA(lua_State* state);
	//static int getPatternTimbreA(lua_State* state);
	//static int setPatternTimbreB(lua_State* state);
	//static int getPatternTimbreB(lua_State* state);
	////样式属性

	//static int addNote(lua_State* state);
	//static int removeNote(lua_State* state);
	//static int countNote(lua_State* state);
	////音符数量

	//static int setNotePlace(lua_State* state);
	//static int getNoteStartBeat(lua_State* state);
	//static int getNoteStartTick(lua_State* state);
	//static int getNoteLength(lua_State* state);
	//static int getNotePitch(lua_State* state);
	//static int setNoteName(lua_State* state);
	//static int getNoteName(lua_State* state);
	//static int setNoteTimbre(lua_State* state);
	//static int getNoteTimbre(lua_State* state);
	//static int setNotePhonemes(lua_State* state);
	//static int getNotePhonemes(lua_State* state);
	//static int setNoteFlags(lua_State* state);
	//static int getNoteFlags(lua_State* state);
	//static int getNoteConsonant(lua_State* state);
	////音符属性

	//static int countNoteParam(lua_State* state);
	////音符参数数量

	//static int getNoteParamName(lua_State* state);
	//static int getNoteParamMax(lua_State* state);
	//static int getNoteParamMin(lua_State* state);
	//static int getNoteParamDefault(lua_State* state);
	//static int setNoteParamValue(lua_State* state);
	//static int getNoteParamValue(lua_State* state);
	////音符参数属性

	//static int countParam(lua_State* state);
	////参数数量

	//static int getParamName(lua_State* state);
	//static int getParamMax(lua_State* state);
	//static int getParamMin(lua_State* state);
	//static int getParamDefault(lua_State* state);
	//static int setParamColor(lua_State* state);
	//static int getParamColor(lua_State* state);
	////参数属性

	//static int addParamPattern(lua_State* state);
	//static int removeParamPattern(lua_State* state);
	//static int countParamPattern(lua_State* state);
	////参数样式数量

	//static int setParamPatternPlace(lua_State* state);
	//static int getParamPatternStartBeat(lua_State* state);
	//static int getParamPatternStartTick(lua_State* state);
	//static int getParamPatternLength(lua_State* state);
	////参数样式属性

	//static int addPoint(lua_State* state);
	//static int addPointUnsafe(lua_State* state);
	//static int removePoint(lua_State* state);
	//static int countPoint(lua_State* state);
	////点数量

	//static int setPointPlace(lua_State* state);
	//static int setPointPlaceUnsafe(lua_State* state);
	//static int getPointX(lua_State* state);
	//static int getPointY(lua_State* state);
	////点属性

private:
	/*static void loadPreScript(lua_State* state, std::string name);
	static void loadPostScript(lua_State* state, std::string name);
	static void callPreScript(lua_State* state, int nargs);
	static void callPostScript(lua_State* state, int nargs);
	static std::string scriptPath(std::string name);*/
};

