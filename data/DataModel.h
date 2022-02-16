#pragma once

#include <algorithm>
#include <string>
#include <list>
#include <functional>
#include <set>
#include <mutex>

#include "Package.h"

#include "Pb/project.pb.h"

class DataModel final
{
	DataModel();
	~DataModel();

	static DataModel _model;
	
public:
	static DataModel& getModel();
	//单例接口

	void setProjectTime();
	uint32_t getProjectTime();
	void setProjectBeat(uint32_t beat);
	uint32_t getProjectBeat();
	void setProjectTempo(double tempo);
	double getProjectTempo();
	void setProjectSRate(uint64_t sRate);
	uint64_t getProjectSRate();
	void setProjectBit(uint32_t bit);
	uint32_t getProjectBit();
	void setProjectQuantize(uint32_t quantize);
	uint32_t getProjectQuantize();
	void setProjectEditor(std::string editor);
	std::string getProjectEditor();
	void addProjectAuthor(std::string author);
	std::list<std::string> getProjectAuthors();
	//项目属性

	void addTrack(std::string name, std::string color);
	void removeTrack(int trackIndex);
	int countTrack();
	//轨道数量

	void setTrackName(int trackIndex, std::string name);
	std::string getTrackName(int trackIndex);
	void setTrackColor(int trackIndex, std::string color);
	std::string getTrackColor(int trackIndex);
	void setTrackMute(int trackIndex, bool mute);
	bool getTrackMute(int trackIndex);
	void setTrackSolo(int trackIndex, bool solo);
	bool getTrackSolo(int trackIndex);
	void setTrackGain(int trackIndex, double gain);
	double getTrackGain(int trackIndex);
	void setTrackPan(int trackIndex, double pan);
	double getTrackPan(int trackIndex);
	void setTrackMix(int trackIndex, double mix);
	double getTrackMix(int trackIndex);
	//轨道属性

	void addContainer(int trackIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, std::string pattern);
	void removeContainer(int trackIndex, int containerIndex);
	int countContainer(int trackIndex);
	//容器数量

	void setContainerPlace(int trackIndex, int containerIndex, uint32_t startBeat, uint32_t startTick, uint64_t length);
	uint32_t getContainerStartBeat(int trackIndex, int containerIndex);
	uint32_t getContainerStartTick(int trackIndex, int containerIndex);
	uint64_t getContainerLength(int trackIndex, int containerIndex);
	void setContainerPattern(int trackIndex, int containerIndex, std::string pattern);
	std::string getContainerPattern(int trackIndex, int containerIndex);
	//容器属性

	void addPattern(std::string name);
	void removePattern(int patternIndex);
	int countPattern();
	//样式数量

	void setPatternName(int patternIndex, std::string name);
	std::string getPatternName(int patternIndex);
	void setPatternFile(int patternIndex, std::string file, uint64_t deviation);
	std::string getPatternFile(int patternIndex);
	void clearPatternFile(int patternIndex);
	void setPatternDeviation(int patternIndex, uint64_t deviation);
	uint64_t getPatternDeviation(int patternIndex);
	void setPatternLibrary(int patternIndex, std::string library);
	std::string getPatternLibrary(int patternIndex);
	void setPatternDictionary(int patternIndex, std::string dictionary);
	std::string getPatternDictionary(int patternIndex);
	void setPatternTimbreA(int patternIndex, std::string timbre);
	std::string getPatternTimbreA(int patternIndex);
	void setPatternTimbreB(int patternIndex, std::string timbre);
	std::string getPatternTimbreB(int patternIndex);
	//样式属性
	
	void addNote(int patternIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, uint32_t pitch, std::string name);
	void removeNote(int patternIndex, int noteIndex);
	int countNote(int patternIndex);
	//音符数量

	void setNotePlace(int patternIndex, int noteIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, uint32_t pitch);
	uint32_t getNoteStartBeat(int patternIndex, int noteIndex);
	uint32_t getNoteStartTick(int patternIndex, int noteIndex);
	uint64_t getNoteLength(int patternIndex, int noteIndex);
	uint32_t getNotePitch(int patternIndex, int noteIndex);
	void setNoteName(int patternIndex, int noteIndex, std::string name);
	std::string getNoteName(int patternIndex, int noteIndex);
	void setNoteTimbre(int patternIndex, int noteIndex, std::string timbre);
	std::string getNoteTimbre(int patternIndex, int noteIndex);
	void setNotePhonemes(int patternIndex, int noteIndex, std::vector<int64_t>& phonemes);
	std::vector<std::pair<std::string, int64_t>> getNotePhonemes(int patternIndex, int noteIndex);
	void setNoteFlags(int patternIndex, int noteIndex, std::string flags);
	std::string getNoteFlags(int patternIndex, int noteIndex);
	bool getNoteConsonant(int patternIndex, int noteIndex);
	//音符属性
	
	int countNoteParam(int patternIndex, int noteIndex);
	//音符参数数量

	std::string getNoteParamName(int patternIndex, int noteIndex, int paramIndex);
	double getNoteParamMax(int patternIndex, int noteIndex, int paramIndex);
	double getNoteParamMin(int patternIndex, int noteIndex, int paramIndex);
	double getNoteParamDefault(int patternIndex, int noteIndex, int paramIndex);
	void setNoteParamValue(int patternIndex, int noteIndex, int paramIndex, double value);
	double getNoteParamValue(int patternIndex, int noteIndex, int paramIndex);
	//音符参数属性

	int countParam(int patternIndex);
	//参数数量

	std::string getParamName(int patternIndex, int paramIndex);
	double getParamMax(int patternIndex, int paramIndex);
	double getParamMin(int patternIndex, int paramIndex);
	double getParamDefault(int patternIndex, int paramIndex);
	void setParamColor(int patternIndex, int paramIndex, std::string color);
	std::string getParamColor(int patternIndex, int paramIndex);
	//参数属性

	void addParamPattern(int patternIndex, int paramIndex, uint32_t startBeat, uint32_t startTick, uint64_t length);
	void removeParamPattern(int patternIndex, int paramIndex, int ppatternIndex);
	int countParamPattern(int patternIndex, int paramIndex);
	//参数样式数量

	void setParamPatternPlace(int patternIndex, int paramIndex, int ppatternIndex, uint32_t startBeat, uint32_t startTick, uint64_t length);
	uint32_t getParamPatternStartBeat(int patternIndex, int paramIndex, int ppatternIndex);
	uint32_t getParamPatternStartTick(int patternIndex, int paramIndex, int ppatternIndex);
	uint64_t getParamPatternLength(int patternIndex, int paramIndex, int ppatternIndex);
	//参数样式属性

	void addPoint(int patternIndex, int paramIndex, int ppatternIndex, double x, double y);
	void addPointUnsafe(int patternIndex, int paramIndex, int ppatternIndex, double x, double y);
	void removePoint(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex);
	int countPoint(int patternIndex, int paramIndex, int ppatternIndex);
	//点数量

	void setPointPlace(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex, double x, double y);
	void setPointPlaceUnsafe(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex, double x, double y);
	double getPointX(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex);
	double getPointY(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex);
	//点属性
private:
	org::infinity::idm::Project* project = nullptr;
	std::mutex modelMutex;

	class Utils {
	public:
		static std::pair<uint32_t,uint32_t> getEP(uint32_t startBeat, uint32_t startTick, uint64_t length);
		static uint64_t getTick(uint32_t startBeat, uint32_t startTick);
		static std::pair<uint32_t, uint32_t> getBeat(uint64_t tick);
	};

	//std::function<void(int, uint32_t, uint32_t)> renderFunc;//渲染
	std::function<void(std::string)> renderFunc;//渲染
	std::function<void(void)> viewFunc;//刷新界面

	std::function<std::pair<std::map<std::string, int64_t>, bool>(std::string, std::string, std::string)> eSplitFunc;//调用引擎分词

};
