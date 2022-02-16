#include "DMH.h"

std::function<void(QString&)> DMH::console_mesFunction;
std::function<void(QString&)> DMH::console_assFunction;
std::function<void()> DMH::console_clsFunction;

void DMH::reg_mesFunctions(
	std::function<void(QString&)> console_mesFunction,
	std::function<void(QString&)> console_assFunction,
	std::function<void()> console_clsFunction
)
{
	DMH::console_mesFunction = console_mesFunction;
	DMH::console_assFunction = console_assFunction;
	DMH::console_clsFunction = console_clsFunction;
}

int DMH::test(lua_State* state)
{
	double a1 = luaL_checknumber(state, 1);
	double a2 = luaL_checknumber(state, 2);
	double a3 = luaL_checknumber(state, 3);

	lua_getglobal(state, "IUtils");

	lua_pushstring(state, "test");
	lua_gettable(state, -2);

	lua_pushnumber(state, a1);
	lua_pushnumber(state, a2);
	lua_pushnumber(state, a3);

	if (lua_pcall(state, 3, 0, NULL) != 0) {
		lua_error(state);
	}
	return 0;
}

//int DMH::getProjectTime(lua_State* state)
//{
//
//}
//
//int DMH::setProjectBeat(lua_State* state)
//{
//
//}
//
//int DMH::getProjectBeat(lua_State* state)
//{
//
//}
//
//int DMH::setProjectTempo(lua_State* state)
//{
//
//}
//
//int DMH::getProjectTempo(lua_State* state)
//{
//
//}
//
//int DMH::setProjectSRate(lua_State* state)
//{
//
//}
//
//int DMH::getProjectSRate(lua_State* state)
//{
//
//}
//
//int DMH::setProjectBit(lua_State* state)
//{
//
//}
//
//int DMH::getProjectBit(lua_State* state)
//{
//
//}
//
//int DMH::setProjectQuantize(lua_State* state)
//{
//
//}
//
//int DMH::getProjectQuantize(lua_State* state)
//{
//
//}
//
//int DMH::setProjectEditor(lua_State* state)
//{
//
//}
//
//int DMH::getProjectEditor(lua_State* state)
//{
//
//}
//
//int DMH::addProjectAuthor(lua_State* state)
//{
//
//}
//
//int DMH::getProjectAuthors(lua_State* state)
//{
//
//}
//
////项目属性
//
//int DMH::addTrack(lua_State* state)
//{
//
//}
//
//int DMH::removeTrack(lua_State* state)
//{
//
//}
//
//int DMH::countTrack(lua_State* state)
//{
//
//}
//
////轨道数量
//
//int DMH::setTrackName(lua_State* state)
//{
//
//}
//
//int DMH::getTrackName(lua_State* state)
//{
//
//}
//
//int DMH::setTrackColor(lua_State* state)
//{
//
//}
//
//int DMH::getTrackColor(lua_State* state)
//{
//
//}
//
//int DMH::setTrackMute(lua_State* state)
//{
//
//}
//
//int DMH::getTrackMute(lua_State* state)
//{
//
//}
//
//int DMH::setTrackSolo(lua_State* state)
//{
//
//}
//
//int DMH::getTrackSolo(lua_State* state)
//{
//
//}
//
//int DMH::setTrackGain(lua_State* state)
//{
//
//}
//
//int DMH::getTrackGain(lua_State* state)
//{
//
//}
//
//int DMH::setTrackPan(lua_State* state)
//{
//
//}
//
//int DMH::getTrackPan(lua_State* state)
//{
//
//}
//
//int DMH::setTrackMix(lua_State* state)
//{
//
//}
//
//int DMH::getTrackMix(lua_State* state)
//{
//
//}
//
////轨道属性
//
//int DMH::addContainer(lua_State* state)
//{
//
//}
//
//int DMH::removeContainer(lua_State* state)
//{
//
//}
//
//int DMH::countContainer(lua_State* state)
//{
//
//}
//
////容器数量
//
//int DMH::setContainerPlace(lua_State* state)
//{
//
//}
//
//int DMH::getContainerStartBeat(lua_State* state)
//{
//
//}
//
//int DMH::getContainerStartTick(lua_State* state)
//{
//
//}
//
//int DMH::getContainerLength(lua_State* state)
//{
//
//}
//
//int DMH::setContainerPattern(lua_State* state)
//{
//
//}
//
//int DMH::getContainerPattern(lua_State* state)
//{
//
//}
//
////容器属性
//
//int DMH::addPattern(lua_State* state)
//{
//
//}
//
//int DMH::removePattern(lua_State* state)
//{
//
//}
//
//int DMH::countPattern(lua_State* state)
//{
//
//}
//
////样式数量
//
//int DMH::setPatternName(lua_State* state)
//{
//
//}
//
//int DMH::getPatternName(lua_State* state)
//{
//
//}
//
//int DMH::setPatternFile(lua_State* state)
//{
//
//}
//
//int DMH::getPatternFile(lua_State* state)
//{
//
//}
//
//int DMH::clearPatternFile(lua_State* state)
//{
//
//}
//
//int DMH::setPatternDeviation(lua_State* state)
//{
//
//}
//
//int DMH::getPatternDeviation(lua_State* state)
//{
//
//}
//
//int DMH::setPatternLibrary(lua_State* state)
//{
//
//}
//
//int DMH::getPatternLibrary(lua_State* state)
//{
//
//}
//
//int DMH::setPatternDictionary(lua_State* state)
//{
//
//}
//
//int DMH::getPatternDictionary(lua_State* state)
//{
//
//}
//
//int DMH::setPatternTimbreA(lua_State* state)
//{
//
//}
//
//int DMH::getPatternTimbreA(lua_State* state)
//{
//
//}
//
//int DMH::setPatternTimbreB(lua_State* state)
//{
//
//}
//
//int DMH::getPatternTimbreB(lua_State* state)
//{
//
//}
//
////样式属性
//
//int DMH::addNote(lua_State* state)
//{
//
//}
//
//int DMH::removeNote(lua_State* state)
//{
//
//}
//
//int DMH::countNote(lua_State* state)
//{
//
//}
//
////音符数量
//
//int DMH::setNotePlace(lua_State* state)
//{
//
//}
//
//int DMH::getNoteStartBeat(lua_State* state)
//{
//
//}
//
//int DMH::getNoteStartTick(lua_State* state)
//{
//
//}
//
//int DMH::getNoteLength(lua_State* state)
//{
//
//}
//
//int DMH::getNotePitch(lua_State* state)
//{
//
//}
//
//int DMH::setNoteName(lua_State* state)
//{
//
//}
//
//int DMH::getNoteName(lua_State* state)
//{
//
//}
//
//int DMH::setNoteTimbre(lua_State* state)
//{
//
//}
//
//int DMH::getNoteTimbre(lua_State* state)
//{
//
//}
//
//int DMH::setNotePhonemes(lua_State* state)
//{
//
//}
//
//int DMH::getNotePhonemes(lua_State* state)
//{
//
//}
//
//int DMH::setNoteFlags(lua_State* state)
//{
//
//}
//
//int DMH::getNoteFlags(lua_State* state)
//{
//
//}
//
//int DMH::getNoteConsonant(lua_State* state)
//{
//
//}
//
////音符属性
//
//int DMH::countNoteParam(lua_State* state)
//{
//
//}
//
////音符参数数量
//
//int DMH::getNoteParamName(lua_State* state)
//{
//
//}
//
//int DMH::getNoteParamMax(lua_State* state)
//{
//
//}
//
//int DMH::getNoteParamMin(lua_State* state)
//{
//
//}
//
//int DMH::getNoteParamDefault(lua_State* state)
//{
//
//}
//
//int DMH::setNoteParamValue(lua_State* state)
//{
//
//}
//
//int DMH::getNoteParamValue(lua_State* state)
//{
//
//}
//
////音符参数属性
//
//int DMH::countParam(lua_State* state)
//{
//
//}
//
////参数数量
//
//int DMH::getParamName(lua_State* state)
//{
//
//}
//
//int DMH::getParamMax(lua_State* state)
//{
//
//}
//
//int DMH::getParamMin(lua_State* state)
//{
//
//}
//
//int DMH::getParamDefault(lua_State* state)
//{
//
//}
//
//int DMH::setParamColor(lua_State* state)
//{
//
//}
//
//int DMH::getParamColor(lua_State* state)
//{
//
//}
//
////参数属性
//
//int DMH::addParamPattern(lua_State* state)
//{
//
//}
//
//int DMH::removeParamPattern(lua_State* state)
//{
//
//}
//
//int DMH::countParamPattern(lua_State* state)
//{
//
//}
//
////参数样式数量
//
//int DMH::setParamPatternPlace(lua_State* state)
//{
//
//}
//
//int DMH::getParamPatternStartBeat(lua_State* state)
//{
//
//}
//
//int DMH::getParamPatternStartTick(lua_State* state)
//{
//
//}
//
//int DMH::getParamPatternLength(lua_State* state)
//{
//
//}
//
////参数样式属性
//
//int DMH::addPoint(lua_State* state)
//{
//
//}
//
//int DMH::addPointUnsafe(lua_State* state)
//{
//
//}
//
//int DMH::removePoint(lua_State* state)
//{
//
//}
//
//int DMH::countPoint(lua_State* state)
//{
//
//}
//
////点数量
//
//int DMH::setPointPlace(lua_State* state)
//{
//
//}
//
//int DMH::setPointPlaceUnsafe(lua_State* state)
//{
//
//}
//
//int DMH::getPointX(lua_State* state)
//{
//
//}
//
//int DMH::getPointY(lua_State* state)
//{
//
//}
//
////点属性
//
//void DMH::loadPreScript(lua_State* state, std::string name)
//{
//	std::string scriptPath = DMH::scriptPath("pre" + name);
//	if (luaL_loadfile(state, scriptPath.c_str()) != 0) {
//		lua_error(state);
//		return;
//	}
//	lua_getglobal(state, std::string("pre" + name).c_str());
//}
//
//void DMH::loadPostScript(lua_State* state, std::string name)
//{
//	std::string scriptPath = DMH::scriptPath("post" + name);
//	if (luaL_loadfile(state, scriptPath.c_str()) != 0) {
//		lua_error(state);
//		return;
//	}
//	lua_getglobal(state, std::string("post" + name).c_str());
//}
//
//void DMH::callPreScript(lua_State* state, int nargs)
//{
//	if (lua_pcall(state, nargs, 0, NULL) != 0) {
//		lua_error(state);
//		return;
//	}
//}
//
//void DMH::callPostScript(lua_State* state, int nargs)
//{
//	if (lua_pcall(state, nargs, 0, NULL) != 0) {
//		lua_error(state);
//		return;
//	}
//}
//
//std::string DMH::scriptPath(std::string name)
//{
//	return QString(QCoreApplication::applicationDirPath() + "/scripts/utils/" + QString::fromStdString(name) + ".lua").toStdString();
//}
