#include "DataModel.h"

DataModel DataModel::_model;

DataModel& DataModel::getModel()
{
	return DataModel::_model;
}

DataModel::DataModel()
{
	this->modelMutex.lock();
	project = new(std::nothrow) org::infinity::idm::Project;
	if (project == nullptr) {
		exit(-1);
	}

	project->set_time(0);
	project->set_beat(4);
	project->set_tempo(120.00);
	project->set_srate(48000);
	project->set_bit(16);
	project->set_editor("Infinity Studio 0");
	this->modelMutex.unlock();
}

DataModel::~DataModel()
{
	delete project;
	project = nullptr;
}

void DataModel::setProjectTime()
{
	this->modelMutex.lock();
	uint32_t minTime = 0;
	for (auto& i : this->project->tracks()) {
		if (i.containers_size() > 0) {
			auto& p = i.containers().Get(i.containers_size() - 1);
			std::pair<uint32_t, uint32_t> EP = DataModel::Utils::getEP(p.startbeat(), p.starttick(), p.length());
			minTime = std::max(minTime, EP.second == 0 ? EP.first : EP.first + 1);
		}//判断最后pattern
	}
	if (minTime == this->project->time()) {
		this->modelMutex.unlock();
		return;
	}
	this->project->set_time(minTime);//time永远与最后元素齐平

	this->modelMutex.unlock();

	this->viewFunc();
}

uint32_t DataModel::getProjectTime()
{
	this->modelMutex.lock();
	auto&& time = this->project->time();
	this->modelMutex.unlock();
	return time;
}

void DataModel::setProjectBeat(uint32_t beat)
{
	if (beat > 0) {
		this->modelMutex.lock();
		if (beat == this->project->beat()) {
			this->modelMutex.unlock();
			return;
		}
		this->project->set_beat(beat);
		this->modelMutex.unlock();

		this->viewFunc();
	}
}

uint32_t DataModel::getProjectBeat()
{
	this->modelMutex.lock();
	auto&& beat = this->project->beat();
	this->modelMutex.unlock();
	return beat;
}

void DataModel::setProjectTempo(double tempo)
{
	if (tempo > 0) {
		this->modelMutex.lock();
		if (tempo == this->project->tempo()) {
			this->modelMutex.unlock();
			return;
		}
		this->project->set_tempo(tempo);

		std::list<std::string> namelist;
		for (auto& p : this->project->patterns()) {
			namelist.push_back(p.name());
		}

		this->modelMutex.unlock();

		for (auto& s : namelist) {
			this->renderFunc(s);
		}
		this->viewFunc();
	}
}

double DataModel::getProjectTempo()
{
	this->modelMutex.lock();
	auto&& tempo = this->project->tempo();
	this->modelMutex.unlock();
	return tempo;
}

void DataModel::setProjectSRate(uint64_t sRate)
{
	if (sRate > 0) {
		this->modelMutex.lock();
		if (sRate == this->project->srate()) {
			this->modelMutex.unlock();
			return;
		}
		this->project->set_srate(sRate);

		std::list<std::string> namelist;
		for (auto& p : this->project->patterns()) {
			namelist.push_back(p.name());
		}

		this->modelMutex.unlock();

		for (auto& s : namelist) {
			this->renderFunc(s);
		}
		this->viewFunc();
	}
}

uint64_t DataModel::getProjectSRate()
{
	this->modelMutex.lock();
	auto&& sRate = this->project->srate();
	this->modelMutex.unlock();
	return sRate;
}

void DataModel::setProjectBit(uint32_t bit)
{
	if (bit > 0) {
		this->modelMutex.lock();
		if (bit == this->project->bit()) {
			this->modelMutex.unlock();
			return;
		}
		this->project->set_bit(bit);

		std::list<std::string> namelist;
		for (auto& p : this->project->patterns()) {
			namelist.push_back(p.name());
		}

		this->modelMutex.unlock();

		for (auto& s : namelist) {
			this->renderFunc(s);
		}
		this->viewFunc();
	}
}

uint32_t DataModel::getProjectBit()
{
	this->modelMutex.lock();
	auto&& bit = this->project->bit();
	this->modelMutex.unlock();
	return bit;
}

void DataModel::setProjectQuantize(uint32_t quantize)
{
	std::set<uint32_t> avaliableQ = { 480,240,120,60,30,160,80,40,20,1 };//允许的量化
	if (avaliableQ.count(quantize) > 0) {
		this->modelMutex.lock();
		if (quantize == this->project->quantize()) {
			this->modelMutex.unlock();
			return;
		}
		this->project->set_quantize(quantize);

		this->modelMutex.unlock();
		this->viewFunc();
	}
}

uint32_t DataModel::getProjectQuantize()
{
	this->modelMutex.lock();
	auto&& quantize = this->project->quantize();
	this->modelMutex.unlock();
	return quantize;
}

void DataModel::setProjectEditor(std::string editor)
{
	if (!editor.empty()) {
		this->modelMutex.lock();
		this->project->set_editor(editor);
		this->modelMutex.unlock();
	}
}

std::string DataModel::getProjectEditor()
{
	this->modelMutex.lock();
	auto&& editor = this->project->editor();
	this->modelMutex.unlock();
	return editor;
}

void DataModel::addProjectAuthor(std::string author)
{
	if (!author.empty()) {
		this->modelMutex.lock();
		if (this->project->authors_size() > 0) {
			if (this->project->authors(this->project->authors_size() - 1) == author) {
				this->modelMutex.unlock();
				return;
			}
		}
		this->project->add_authors(author);
		this->modelMutex.unlock();
	}
}

std::list<std::string> DataModel::getProjectAuthors()
{
	this->modelMutex.lock();
	std::list<std::string> result(this->project->authors().begin(), this->project->authors().end());
	this->modelMutex.unlock();
	return result;
}

void DataModel::addTrack(std::string name, std::string color)
{
	if (name.empty()) {
		return;
	}
	if (color.empty()) {
		return;
	}
	this->modelMutex.lock();

	bool haveSolo = false;
	for (auto& t : this->project->tracks()) {
		if (t.solo()) {
			haveSolo = true;
			break;
		}
	}//判断solo是否存在

	org::infinity::idm::Track* track = this->project->add_tracks();
	track->set_name(name);
	track->set_color(color);
	track->set_solo(false);
	track->set_mute(!haveSolo);

	this->modelMutex.unlock();

	this->viewFunc();
}

void DataModel::removeTrack(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (this->project->tracks_size() == 1) {
			this->modelMutex.unlock();
			return;
		}

		bool solo = this->project->tracks(trackIndex).solo();

		auto it = this->project->mutable_tracks()->begin() + trackIndex;
		this->project->mutable_tracks()->erase(it);

		if (solo) {
			bool haveSolo = false;
			for (auto& t : this->project->tracks()) {
				if (t.solo()) {
					haveSolo = true;
					break;
				}
			}//判断是否还存在solo
			if (!haveSolo) {
				for (int i = 0; i < this->project->tracks_size(); i++) {
					if (this->project->tracks(i).mute()) {
						this->project->mutable_tracks(i)->set_mute(false);
					}
				}
			}//无solo，所有轨取消mute
		}

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

int DataModel::countTrack()
{
	this->modelMutex.lock();
	auto&& tracks_size = this->project->tracks_size();
	this->modelMutex.unlock();
	return tracks_size;
}

void DataModel::setTrackName(int trackIndex, std::string name)
{
	if (name.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		this->project->mutable_tracks(trackIndex)->set_name(name);
		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getTrackName(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& name = this->project->tracks(trackIndex).name();
		this->modelMutex.unlock();
		return name;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setTrackColor(int trackIndex, std::string color)
{
	if (color.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {

		std::string currentColor = this->project->tracks(trackIndex).color();

		this->project->mutable_tracks(trackIndex)->set_color(color);

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getTrackColor(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& color = this->project->tracks(trackIndex).color();
		this->modelMutex.unlock();
		return color;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setTrackMute(int trackIndex, bool mute)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (mute == this->project->tracks(trackIndex).mute()) {
			this->modelMutex.unlock();
			return;
		}//未作改动
		if (this->project->tracks(trackIndex).solo()) {
			this->modelMutex.unlock();
			return;
		}//当前轨solo，mute不可改动，必为false
		if (!mute) {
			for (auto& t : this->project->tracks()) {
				if (t.solo()) {
					this->modelMutex.unlock();
					return;
				}
			}
		}//存在其他轨solo，当前mute不可改动，必为true
		this->project->mutable_tracks(trackIndex)->set_mute(mute);

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

bool DataModel::getTrackMute(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& mute = this->project->tracks(trackIndex).mute();
		this->modelMutex.unlock();
		return mute;
	}
	this->modelMutex.unlock();
	return false;
}

void DataModel::setTrackSolo(int trackIndex, bool solo)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (solo == this->project->tracks(trackIndex).solo()) {
			this->modelMutex.unlock();
			return;
		}//未作改动
		this->project->mutable_tracks(trackIndex)->set_solo(solo);
		if (solo) {
			this->project->mutable_tracks(trackIndex)->set_mute(false);//关闭当前轨mute
			for (int i = 0; i < this->project->tracks_size(); i++) {
				if (!this->project->tracks(i).solo()) {
					this->project->mutable_tracks(i)->set_mute(true);
				}
			}//将所有非solo轨设为mute
		}
		else {
			bool haveSolo = false;
			for (auto& t : this->project->tracks()) {
				if (t.solo()) {
					haveSolo = true;
					break;
				}
			}//判断是否还存在solo
			if (haveSolo) {
				this->project->mutable_tracks(trackIndex)->set_mute(true);
			}//有solo，当前轨设为mute
			else {
				for (int i = 0; i < this->project->tracks_size(); i++) {
					if (this->project->tracks(i).mute()) {
						this->project->mutable_tracks(i)->set_mute(false);
					}
				}
			}//无solo，所有轨取消mute
		}

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

bool DataModel::getTrackSolo(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& solo = this->project->tracks(trackIndex).solo();
		this->modelMutex.unlock();
		return solo;
	}
	this->modelMutex.unlock();
	return false;
}

void DataModel::setTrackGain(int trackIndex, double gain)
{
	if (gain < -20.0 || gain > 20.0) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (gain == this->project->tracks(trackIndex).gain()) {
			this->modelMutex.unlock();
			return;
		}//未作改动

		this->project->mutable_tracks(trackIndex)->set_gain(gain);

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

double DataModel::getTrackGain(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& gain = this->project->tracks(trackIndex).gain();
		this->modelMutex.unlock();
		return gain;
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setTrackPan(int trackIndex, double pan)
{
	if (pan < -64.0 || pan > 64.0) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (pan == this->project->tracks(trackIndex).pan()) {
			this->modelMutex.unlock();
			return;
		}//未作改动

		this->project->mutable_tracks(trackIndex)->set_pan(pan);

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

double DataModel::getTrackPan(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& pan = this->project->tracks(trackIndex).pan();
		this->modelMutex.unlock();
		return pan;
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setTrackMix(int trackIndex, double mix)
{
	if (mix < -90.0 || mix > 6.0) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (mix == this->project->tracks(trackIndex).mix()) {
			this->modelMutex.unlock();
			return;
		}//未作改动

		this->project->mutable_tracks(trackIndex)->set_mix(mix);

		this->modelMutex.unlock();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

double DataModel::getTrackMix(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& mix = this->project->tracks(trackIndex).mix();
		this->modelMutex.unlock();
		return mix;
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::addContainer(int trackIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, std::string pattern)
{
	if (pattern.empty()) {
		return;
	}
	if (length == 0) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (startTick >= 480) {
			startBeat += startTick / 480;
			startTick %= 480;
		}
		{
			bool havePattern = false;
			for (auto& p : this->project->patterns()) {
				if (p.name() == pattern) {
					havePattern = true;
					break;
				}
			}
			if (!havePattern) {
				this->modelMutex.unlock();
				return;
			}
		}//判断pattern存在
		uint64_t ST = DataModel::Utils::getTick(startBeat, startTick);
		std::pair<uint32_t, uint32_t> EP = DataModel::Utils::getEP(startBeat, startTick, length);
		uint64_t ET = DataModel::Utils::getTick(EP.first, EP.second);

		for (auto& c : this->project->tracks(trackIndex).containers()) {
			uint64_t STC = DataModel::Utils::getTick(c.startbeat(), c.starttick());
			std::pair<uint32_t, uint32_t> EPC = DataModel::Utils::getEP(c.startbeat(), c.starttick(), c.length());
			uint64_t ETC = DataModel::Utils::getTick(EPC.first, EPC.second);
			if (ET > STC && ETC > ST) {
				this->modelMutex.unlock();
				return;
			}
		}//判断重叠

		org::infinity::idm::Container* container = nullptr;
		if (this->project->tracks(trackIndex).containers_size() > 0) {
			if (DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(0).startbeat(), this->project->tracks(trackIndex).containers(0).starttick())) {
				this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾建立空element
				for (int i = this->project->tracks(trackIndex).containers_size() - 2; i >= 0; i--) {
					this->project->mutable_tracks(trackIndex)->mutable_containers(i + 1)->CopyFrom(this->project->tracks(trackIndex).containers(i));
				}//element集体后移
				container = this->project->mutable_tracks(trackIndex)->mutable_containers(0);//腾出头element
				container->Clear();
			}
			else if (DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(this->project->tracks(trackIndex).containers_size() - 1).startbeat(), this->project->tracks(trackIndex).containers(this->project->tracks(trackIndex).containers_size() - 1).starttick())) {
				container = this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾
			}
			else {
				this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾建立空element
				for (int i = this->project->tracks(trackIndex).containers_size() - 2; i > 0; i--) {
					this->project->mutable_tracks(trackIndex)->mutable_containers(i + 1)->CopyFrom(this->project->tracks(trackIndex).containers(i));//element后移
					if (
						DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(i - 1).startbeat(), this->project->tracks(trackIndex).containers(i - 1).starttick()) &&
						DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(i + 1).startbeat(), this->project->tracks(trackIndex).containers(i + 1).starttick())
						) {//如果当前element符合要求
						container = this->project->mutable_tracks(trackIndex)->mutable_containers(i);//移动element空出位置即是插入位置
						container->Clear();
					}
				}
			}
		}
		else {
			container = this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//唯一一个
		}

		container->set_startbeat(startBeat);
		container->set_starttick(startTick);
		container->set_length(length);
		container->set_pattern(pattern);

		this->modelMutex.unlock();

		this->setProjectTime();
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

void DataModel::removeContainer(int trackIndex, int containerIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			auto it = this->project->mutable_tracks(trackIndex)->mutable_containers()->begin() + containerIndex;
			this->project->mutable_tracks(trackIndex)->mutable_containers()->erase(it);

			this->modelMutex.unlock();

			this->setProjectTime();
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

int DataModel::countContainer(int trackIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		auto&& containers_size = this->project->tracks(trackIndex).containers_size();
		this->modelMutex.unlock();
		return containers_size;
	}
	this->modelMutex.unlock();
	return -1;
}

void DataModel::setContainerPlace(int trackIndex, int containerIndex, uint32_t startBeat, uint32_t startTick, uint64_t length)
{
	if (length == 0) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			if (startTick >= 480) {
				startBeat += startTick / 480;
				startTick %= 480;
			}
			if (
				startBeat == this->project->tracks(trackIndex).containers(containerIndex).startbeat() &&
				startTick == this->project->tracks(trackIndex).containers(containerIndex).starttick() &&
				length == this->project->tracks(trackIndex).containers(containerIndex).length()
				) {
				this->modelMutex.unlock();
				return;
			}

			uint64_t ST = DataModel::Utils::getTick(startBeat, startTick);
			std::pair<uint32_t, uint32_t> EP = DataModel::Utils::getEP(startBeat, startTick, length);
			uint64_t ET = DataModel::Utils::getTick(EP.first, EP.second);

			for (int i = 0; i < this->project->tracks(trackIndex).containers_size(); i++) {
				if (i == containerIndex) {
					continue;
				}//非当前容器
				auto&& cont = this->project->tracks(trackIndex).containers(i);
				uint64_t STC = DataModel::Utils::getTick(cont.startbeat(), cont.starttick());
				std::pair<uint32_t, uint32_t> EPC = DataModel::Utils::getEP(cont.startbeat(), cont.starttick(), cont.length());
				uint64_t ETC = DataModel::Utils::getTick(EPC.first, EPC.second);
				if (ET > STC && ETC > ST) {
					this->modelMutex.unlock();
					return;
				}
			}//判断是否存在重叠容器

			org::infinity::idm::Container cTemp(this->project->tracks(trackIndex).containers(containerIndex));
			auto it = this->project->mutable_tracks(trackIndex)->mutable_containers()->begin() + containerIndex;
			this->project->mutable_tracks(trackIndex)->mutable_containers()->erase(it);//抽取当前容器

			org::infinity::idm::Container* container = nullptr;
			if (this->project->tracks(trackIndex).containers_size() > 0) {
				if (DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(0).startbeat(), this->project->tracks(trackIndex).containers(0).starttick())) {
					this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾建立空element
					for (int i = this->project->tracks(trackIndex).containers_size() - 2; i >= 0; i--) {
						this->project->mutable_tracks(trackIndex)->mutable_containers(i + 1)->CopyFrom(this->project->tracks(trackIndex).containers(i));
					}//element集体后移
					container = this->project->mutable_tracks(trackIndex)->mutable_containers(0);//腾出头element
					container->Clear();
				}
				else if (DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(this->project->tracks(trackIndex).containers_size() - 1).startbeat(), this->project->tracks(trackIndex).containers(this->project->tracks(trackIndex).containers_size() - 1).starttick())) {
					container = this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾
				}
				else {
					this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//末尾建立空element
					for (int i = this->project->tracks(trackIndex).containers_size() - 2; i > 0; i--) {
						this->project->mutable_tracks(trackIndex)->mutable_containers(i + 1)->CopyFrom(this->project->tracks(trackIndex).containers(i));//element后移
						if (
							DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(i - 1).startbeat(), this->project->tracks(trackIndex).containers(i - 1).starttick()) &&
							DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->tracks(trackIndex).containers(i + 1).startbeat(), this->project->tracks(trackIndex).containers(i + 1).starttick())
							) {//如果当前element符合要求
							container = this->project->mutable_tracks(trackIndex)->mutable_containers(i);//移动element空出位置即是插入位置
							container->Clear();
						}
					}
				}
			}
			else {
				container = this->project->mutable_tracks(trackIndex)->mutable_containers()->Add();//唯一一个
			}

			cTemp.set_startbeat(startBeat);
			cTemp.set_starttick(startTick);
			cTemp.set_length(length);
			container->CopyFrom(cTemp);

			this->modelMutex.unlock();

			this->setProjectTime();
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

uint32_t DataModel::getContainerStartBeat(int trackIndex, int containerIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			auto&& startbeat = this->project->tracks(trackIndex).containers(containerIndex).startbeat();
			this->modelMutex.unlock();
			return startbeat;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint32_t DataModel::getContainerStartTick(int trackIndex, int containerIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			auto&& starttick = this->project->tracks(trackIndex).containers(containerIndex).starttick();
			this->modelMutex.unlock();
			return starttick;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint64_t DataModel::getContainerLength(int trackIndex, int containerIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			auto&& length = this->project->tracks(trackIndex).containers(containerIndex).length();
			this->modelMutex.unlock();
			return length;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setContainerPattern(int trackIndex, int containerIndex, std::string pattern)
{
	if (pattern.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			if (this->project->tracks(trackIndex).containers(containerIndex).pattern() == pattern) {
				this->modelMutex.unlock();
				return;
			}
			{
				bool havePattern = false;
				for (auto& p : this->project->patterns()) {
					if (p.name() == pattern) {
						havePattern = true;
						break;
					}
				}
				if (!havePattern) {
					this->modelMutex.unlock();
					return;
				}
			}//判断pattern存在
			this->project->mutable_tracks(trackIndex)->mutable_containers(containerIndex)->set_pattern(pattern);

			this->modelMutex.unlock();

			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::string DataModel::getContainerPattern(int trackIndex, int containerIndex)
{
	this->modelMutex.lock();
	if (trackIndex >= 0 && trackIndex < this->project->tracks_size()) {
		if (containerIndex >= 0 && containerIndex < this->project->tracks(trackIndex).containers_size()) {
			auto&& pattern = this->project->tracks(trackIndex).containers(containerIndex).pattern();
			this->modelMutex.unlock();
			return pattern;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::addPattern(std::string name)
{
	if (name.empty()) {
		return;
	}

	this->modelMutex.lock();
	for (auto& p : this->project->patterns()) {
		if (p.name() == name) {
			this->modelMutex.unlock();
			return;
		}
	}//判断名称存在
	org::infinity::idm::Pattern* pattern = this->project->add_patterns();
	pattern->set_name(name);
	pattern->set_type(org::infinity::idm::Pattern::Type::Pattern_Type_EMPTY);

	this->modelMutex.unlock();

	this->viewFunc();
}

void DataModel::removePattern(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		std::string name = this->project->patterns(patternIndex).name();
		auto it = this->project->mutable_patterns()->begin() + patternIndex;
		this->project->mutable_patterns()->erase(it);

		for (int i = 0; i < this->project->tracks_size(); i++) {
			org::infinity::idm::Track* track = this->project->mutable_tracks(i);
			for (int j = 0; j < track->containers_size(); ) {
				org::infinity::idm::Container* container = track->mutable_containers(j);
				if (container->pattern() == name) {
					auto it = track->mutable_containers()->begin() + j;
					track->mutable_containers()->erase(it);
					continue;
				}
				j++;
			}
		}//查找并移除关联容器

		this->modelMutex.unlock();

		this->renderFunc(name);
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

int DataModel::countPattern()
{
	this->modelMutex.lock();
	auto&& patterns_size = this->project->patterns_size();
	this->modelMutex.unlock();
	return patterns_size;
}

void DataModel::setPatternName(int patternIndex, std::string name)
{
	if (name.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		std::string currentName = this->project->patterns(patternIndex).name();
		if (currentName == name) {
			this->modelMutex.unlock();
			return;
		}
		for (auto& p : this->project->patterns()) {
			if (p.name() == name) {
				this->modelMutex.unlock();
				return;
			}
		}//判断名称存在

		this->project->mutable_patterns(patternIndex)->set_name(name);

		for (int i = 0; i < this->project->tracks_size(); i++) {
			org::infinity::idm::Track* track = this->project->mutable_tracks(i);
			for (int j = 0; j < track->containers_size(); j++) {
				org::infinity::idm::Container* container = track->mutable_containers(j);
				if (container->pattern() == currentName) {
					container->set_pattern(name);
				}
			}
		}//查找并更改关联容器内容

		this->modelMutex.unlock();

		this->renderFunc(name);
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternName(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& name = this->project->patterns(patternIndex).name();
		this->modelMutex.unlock();
		return name;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setPatternFile(int patternIndex, std::string file, uint64_t deviation)
{
	if (file.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_MIDI) {
			this->modelMutex.unlock();
			return;
		}//不接受对MIDI样式的关联文件修改
		if (
			this->project->patterns(patternIndex).file() == file &&
			this->project->patterns(patternIndex).deviation() == deviation
			) {
			this->modelMutex.unlock();
			return;
		}//未作修改

		this->project->mutable_patterns(patternIndex)->set_type(org::infinity::idm::Pattern::Type::Pattern_Type_WAVE);
		this->project->mutable_patterns(patternIndex)->set_file(file);
		this->project->mutable_patterns(patternIndex)->set_deviation(deviation);
		this->project->mutable_patterns(patternIndex)->clear_library();
		this->project->mutable_patterns(patternIndex)->clear_dictionary();
		this->project->mutable_patterns(patternIndex)->clear_timbrea();
		this->project->mutable_patterns(patternIndex)->clear_timbreb();
		
		for (int i = 0; i < this->project->patterns(patternIndex).params_size(); ) {
			org::infinity::idm::TrackParam* param = this->project->mutable_patterns(patternIndex)->mutable_params(i);
			if (param->patterns_size() == 0) {
				auto it = this->project->mutable_patterns(patternIndex)->mutable_params()->begin() + i;
				this->project->mutable_patterns(patternIndex)->mutable_params()->erase(it);
				continue;
			}
			i++;
		}//清理全部未改动参数

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternFile(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& file = this->project->patterns(patternIndex).file();
		this->modelMutex.unlock();
		return file;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::clearPatternFile(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() != org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}

		this->project->mutable_patterns(patternIndex)->set_type(org::infinity::idm::Pattern::Type::Pattern_Type_EMPTY);
		this->project->mutable_patterns(patternIndex)->clear_file();
		this->project->mutable_patterns(patternIndex)->set_deviation(0);

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

void DataModel::setPatternDeviation(int patternIndex, uint64_t deviation)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() != org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}
		if (this->project->patterns(patternIndex).deviation() == deviation) {
			this->modelMutex.unlock();
			return;
		}

		this->project->mutable_patterns(patternIndex)->set_deviation(deviation);

		this->modelMutex.unlock();

		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

uint64_t DataModel::getPatternDeviation(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& deviation = this->project->patterns(patternIndex).deviation();
		this->modelMutex.unlock();
		return deviation;
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setPatternLibrary(int patternIndex, std::string library)
{
	if (library.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的声库修改
		std::string libraryCurrent = this->project->patterns(patternIndex).library();
		if (library == libraryCurrent) {
			this->modelMutex.unlock();
			return;
		}
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getLibraryAvailable()) {
				if (i == library) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断声库是否存在
		std::string dictionary = Package::getPackage().getLibraryDictionaryDefault(library);
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getDictionaryAvailable()) {
				if (i == dictionary) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断字典是否存在
		std::string engine = Package::getPackage().getEngineName(library);
		std::string engineCurrent = Package::getPackage().getEngineName(libraryCurrent);
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getEngineAvailable()) {
				if (i == engine) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断引擎是否存在

		std::string timbre = Package::getPackage().getLibraryTimbreDefault(library);

		this->project->mutable_patterns(patternIndex)->set_library(library);
		this->project->mutable_patterns(patternIndex)->set_dictionary(dictionary);
		this->project->mutable_patterns(patternIndex)->set_timbrea(timbre);
		this->project->mutable_patterns(patternIndex)->set_timbreb(timbre);

		if (engine != engineCurrent) {
			auto epl = Package::getPackage().getEngineParam(engine);//获取新引擎参数表
			auto ckF = [epl](std::string name)->bool {
				bool flag = false;
				for (auto& i : epl) {
					if (i.name == name) {
						flag = true;
						break;
					}
				}
				return flag;
			};//检查参数是否存在于引擎的列表中
			std::set<std::string> paramL;//已存在的参数
			for (auto it = this->project->mutable_patterns(patternIndex)->mutable_params()->begin(); it != this->project->mutable_patterns(patternIndex)->mutable_params()->end(); ) {
				if ((*it).patterns_size() == 0) {
					if (!ckF((*it).name())) {
						it = this->project->mutable_patterns(patternIndex)->mutable_params()->erase(it);
						continue;
					}
				}
				paramL.insert((*it).name());
				it++;
			}//后侧迭代器在移除元素时会失效
			for (auto& p : epl) {
				if (paramL.count(p.name) == 0) {
					org::infinity::idm::TrackParam* param = this->project->mutable_patterns(patternIndex)->add_params();
					param->set_name(p.name);
					param->set_vmax(p.vMax);
					param->set_vmin(p.vMin);
					param->set_vdefault(p.vDefault);

					param->set_color("0fabff");
				}
			}//添加参数
		}//调整轨道参数

		if (engine != engineCurrent) {
			auto epl = Package::getPackage().getEngineNoteParam(engine);//获取新引擎参数表
			auto ckF = [epl](std::string name)->bool {
				bool flag = false;
				for (auto& i : epl) {
					if (i.name == name) {
						flag = true;
						break;
					}
				}
				return flag;
			};//检查参数是否存在于引擎的列表中
			for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);

				std::set<std::string> paramL;//已存在的参数
				for (auto it = note->mutable_params()->begin(); it != note->mutable_params()->end(); ) {
					if (!ckF((*it).name())) {
						it = note->mutable_params()->erase(it);
						continue;
					}
					paramL.insert((*it).name());
					it++;
				}//后侧迭代器在移除元素时会失效
				for (auto& p : epl) {
					if (paramL.count(p.name) == 0) {
						org::infinity::idm::NoteParam* param = note->add_params();
						param->set_name(p.name);
						param->set_vmax(p.vMax);
						param->set_vmin(p.vMin);
						param->set_vdefault(p.vDefault);
						param->set_value(p.vDefault);
					}
				}//添加参数
			}
		}//调整音符参数

		if (Package::getPackage().getEngineSplit(engine)) {
			for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}
		}//调用引擎分词器
		else {
			for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}
		}//使用字典分词

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternLibrary(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& library = this->project->patterns(patternIndex).library();
		this->modelMutex.unlock();
		return library;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setPatternDictionary(int patternIndex, std::string dictionary)
{
	if (dictionary.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的字典修改
		if (dictionary == this->project->patterns(patternIndex).dictionary()) {
			this->modelMutex.unlock();
			return;
		}
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getDictionaryAvailable()) {
				if (i == dictionary) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断字典存在
		std::string engine = Package::getPackage().getEngineName(this->project->patterns(patternIndex).library());
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getEngineAvailable()) {
				if (i == engine) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断引擎存在

		this->project->mutable_patterns(patternIndex)->set_dictionary(dictionary);

		if (Package::getPackage().getEngineSplit(engine)) {
			for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}
		}//调用引擎分词器
		else {
			for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}
		}//使用字典分词

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternDictionary(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& dictionary = this->project->patterns(patternIndex).dictionary();
		this->modelMutex.unlock();
		return dictionary;
	}
	this->modelMutex.unlock();
	return std::string();
}


void DataModel::setPatternTimbreA(int patternIndex, std::string timbre)
{
	if (timbre.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音色修改
		std::string timbreNow = this->project->patterns(patternIndex).timbrea();
		if (timbre == timbreNow) {
			this->modelMutex.unlock();
			return;
		}
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getLibraryTimbre(this->project->patterns(patternIndex).library())) {
				if (i == timbre) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断音色存在
		this->project->mutable_patterns(patternIndex)->set_timbrea(timbre);

		for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
			if (this->project->patterns(patternIndex).notes(i).timbre() != timbreNow) {
				continue;
			}//修改过音色的音符不受影响
			this->project->mutable_patterns(patternIndex)->mutable_notes(i)->set_timbre(timbre);
		}//音色A决定音符主音色

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternTimbreA(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& timbrea = this->project->patterns(patternIndex).timbrea();
		this->modelMutex.unlock();
		return timbrea;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setPatternTimbreB(int patternIndex, std::string timbre)
{
	if (timbre.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音色修改
		if (timbre == this->project->patterns(patternIndex).timbreb()) {
			this->modelMutex.unlock();
			return;
		}
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getLibraryTimbre(this->project->patterns(patternIndex).library())) {
				if (i == timbre) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断音色存在
		this->project->mutable_patterns(patternIndex)->set_timbreb(timbre);

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

std::string DataModel::getPatternTimbreB(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& timbreb = this->project->patterns(patternIndex).timbreb();
		this->modelMutex.unlock();
		return timbreb;
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::addNote(int patternIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, uint32_t pitch, std::string name)
{
	if (length == 0) {
		return;
	}
	if (name.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (startTick >= 480) {
			startBeat += startTick / 480;
			startTick %= 480;
		}
		if (this->project->patterns(patternIndex).library().empty()) {
			std::string library = Package::getPackage().getLibraryDefault();
			std::string dictionary = Package::getPackage().getLibraryDictionaryDefault(library);
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getDictionaryAvailable()) {
					if (i == dictionary) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断字典是否存在
			std::string engine = Package::getPackage().getEngineName(library);
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getEngineAvailable()) {
					if (i == engine) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断引擎是否存在

			std::string timbre = Package::getPackage().getLibraryTimbreDefault(library);

			this->project->mutable_patterns(patternIndex)->set_library(library);
			this->project->mutable_patterns(patternIndex)->set_dictionary(dictionary);
			this->project->mutable_patterns(patternIndex)->set_timbrea(timbre);
			this->project->mutable_patterns(patternIndex)->set_timbreb(timbre);

			{
				auto epl = Package::getPackage().getEngineParam(engine);//获取新引擎参数表
				auto ckF = [epl](std::string name)->bool {
					bool flag = false;
					for (auto& i : epl) {
						if (i.name == name) {
							flag = true;
							break;
						}
					}
					return flag;
				};//检查参数是否存在于引擎的列表中
				std::set<std::string> paramL;//已存在的参数
				for (auto it = this->project->mutable_patterns(patternIndex)->mutable_params()->begin(); it != this->project->mutable_patterns(patternIndex)->mutable_params()->end(); ) {
					if ((*it).patterns_size() == 0) {
						if (!ckF((*it).name())) {
							it = this->project->mutable_patterns(patternIndex)->mutable_params()->erase(it);
							continue;
						}
					}
					paramL.insert((*it).name());
					it++;
				}//后侧迭代器在移除元素时会失效
				for (auto& p : epl) {
					if (paramL.count(p.name) == 0) {
						org::infinity::idm::TrackParam* param = this->project->mutable_patterns(patternIndex)->add_params();
						param->set_name(p.name);
						param->set_vmax(p.vMax);
						param->set_vmin(p.vMin);
						param->set_vdefault(p.vDefault);

						param->set_color("0fabff");
					}
				}//添加参数
			}//调整轨道参数

			{
				auto epl = Package::getPackage().getEngineNoteParam(engine);//获取新引擎参数表
				auto ckF = [epl](std::string name)->bool {
					bool flag = false;
					for (auto& i : epl) {
						if (i.name == name) {
							flag = true;
							break;
						}
					}
					return flag;
				};//检查参数是否存在于引擎的列表中
				for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
					org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);

					std::set<std::string> paramL;//已存在的参数
					for (auto it = note->mutable_params()->begin(); it != note->mutable_params()->end(); ) {
						if (!ckF((*it).name())) {
							it = note->mutable_params()->erase(it);
							continue;
						}
						paramL.insert((*it).name());
						it++;
					}//后侧迭代器在移除元素时会失效
					for (auto& p : epl) {
						if (paramL.count(p.name) == 0) {
							org::infinity::idm::NoteParam* param = note->add_params();
							param->set_name(p.name);
							param->set_vmax(p.vMax);
							param->set_vmin(p.vMin);
							param->set_vdefault(p.vDefault);
							param->set_value(p.vDefault);
						}
					}//添加参数
				}
			}//调整音符参数

			if (Package::getPackage().getEngineSplit(engine)) {
				for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
					org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
					std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, note->name());
					note->clear_phonemes();
					for (auto& p : phonemeM.first) {
						org::infinity::idm::utils::Pair* pair = note->add_phonemes();
						pair->set_key(p.first);
						pair->set_value(p.second);
					}
					note->set_consonant(phonemeM.second);
				}
			}//调用引擎分词器
			else {
				for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
					org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
					std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, note->name());
					note->clear_phonemes();
					for (auto& p : phonemeM.first) {
						org::infinity::idm::utils::Pair* pair = note->add_phonemes();
						pair->set_key(p.first);
						pair->set_value(p.second);
					}
					note->set_consonant(phonemeM.second);
				}
			}//使用字典分词
		}//轨道未设置声库
		std::string dictionary = Package::getPackage().getLibraryDictionaryDefault(this->project->patterns(patternIndex).library());
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getDictionaryAvailable()) {
				if (i == dictionary) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断字典存在
		std::string engine = Package::getPackage().getEngineName(this->project->patterns(patternIndex).library());
		{
			bool flag = false;
			for (auto& i : Package::getPackage().getEngineAvailable()) {
				if (i == engine) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->modelMutex.unlock();
				return;
			}
		}//判断引擎存在

		org::infinity::idm::Note* note = nullptr;
		if (this->project->patterns(patternIndex).notes_size() > 0) {
			if (DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(0).startbeat(), this->project->patterns(patternIndex).notes(0).starttick())) {
				this->project->mutable_patterns(patternIndex)->mutable_notes()->Add();//末尾建立空element
				for (int i = this->project->patterns(patternIndex).notes_size() - 2; i >= 0 ; i--) {
					this->project->mutable_patterns(patternIndex)->mutable_notes(i + 1)->CopyFrom(this->project->patterns(patternIndex).notes(i));
				}//element集体后移
				note = this->project->mutable_patterns(patternIndex)->mutable_notes(0);//腾出头element
				note->Clear();
			}
			else if (DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(this->project->patterns(patternIndex).notes_size() - 1).startbeat(), this->project->patterns(patternIndex).notes(this->project->patterns(patternIndex).notes_size() - 1).starttick())) {
				note = this->project->mutable_patterns(patternIndex)->mutable_notes()->Add();//末尾
			}
			else {
				this->project->mutable_patterns(patternIndex)->mutable_notes()->Add();//末尾建立空element
				for (int i = this->project->patterns(patternIndex).notes_size() - 2; i > 0; i--) {
					this->project->mutable_patterns(patternIndex)->mutable_notes(i + 1)->CopyFrom(this->project->patterns(patternIndex).notes(i));//element后移
					if (
						DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i - 1).startbeat(), this->project->patterns(patternIndex).notes(i - 1).starttick()) &&
						DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i + 1).startbeat(), this->project->patterns(patternIndex).notes(i + 1).starttick())
						) {//如果当前element符合要求
						note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);//移动element空出位置即是插入位置
						note->Clear();
					}
				}
			}
		}
		else {
			note = this->project->mutable_patterns(patternIndex)->mutable_notes()->Add();//唯一一个
		}

		note->set_startbeat(startBeat);
		note->set_starttick(startTick);
		note->set_length(length);
		note->set_pitch(pitch);

		note->set_name(name);
		note->set_timbre(this->project->patterns(patternIndex).timbrea());

		if (Package::getPackage().getEngineSplit(engine)) {
			std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, name);
			for (auto& p : phonemeM.first) {
				org::infinity::idm::utils::Pair* pair = note->add_phonemes();
				pair->set_key(p.first);
				pair->set_value(p.second);
			}
			note->set_consonant(phonemeM.second);
		}//调用引擎分词器
		else {
			std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, name);
			for (auto& p : phonemeM.first) {
				org::infinity::idm::utils::Pair* pair = note->add_phonemes();
				pair->set_key(p.first);
				pair->set_value(p.second);
			}
			note->set_consonant(phonemeM.second);
		}//使用字典分词

		auto epl = Package::getPackage().getEngineNoteParam(engine);//获取引擎参数表
		for (auto& p : epl) {
			org::infinity::idm::NoteParam* param = note->add_params();
			param->set_name(p.name);
			param->set_vmax(p.vMax);
			param->set_vmin(p.vMin);
			param->set_vdefault(p.vDefault);
			param->set_value(p.vDefault);
		}//添加参数

		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_EMPTY) {
			this->project->mutable_patterns(patternIndex)->set_type(org::infinity::idm::Pattern::Type::Pattern_Type_MIDI);
		}//设置样式类型为MIDI

		this->modelMutex.unlock();

		this->renderFunc(this->project->patterns(patternIndex).name());
		this->viewFunc();
		return;
	}
	this->modelMutex.unlock();
}

void DataModel::removeNote(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto it = this->project->mutable_patterns(patternIndex)->mutable_notes()->begin() + noteIndex;
			this->project->mutable_patterns(patternIndex)->mutable_notes()->erase(it);

			if (this->project->patterns(patternIndex).notes_size() == 0) {
				this->project->mutable_patterns(patternIndex)->set_type(org::infinity::idm::Pattern::Type::Pattern_Type_EMPTY);
			}//如果无音符则样式类型为EMPTY

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

int DataModel::countNote(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& notes_size = this->project->patterns(patternIndex).notes_size();
		this->modelMutex.unlock();
		return notes_size;
	}
	this->modelMutex.unlock();
	return -1;
}

void DataModel::setNotePlace(int patternIndex, int noteIndex, uint32_t startBeat, uint32_t startTick, uint64_t length, uint32_t pitch)
{
	if (length == 0) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (startTick >= 480) {
				startBeat += startTick / 480;
				startTick %= 480;
			}
			uint32_t currentStartBeat = this->project->patterns(patternIndex).notes(noteIndex).startbeat();
			uint32_t currentStartTick = this->project->patterns(patternIndex).notes(noteIndex).starttick();
			uint64_t currentLength = this->project->patterns(patternIndex).notes(noteIndex).length();
			if (
				currentStartBeat == startBeat &&
				currentStartTick == startTick &&
				currentLength == length
				) {
				this->modelMutex.unlock();
				return;
			}//未作修改

			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_startbeat(startBeat);
			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_starttick(startTick);
			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_length(length);
			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_pitch(pitch);

			if (noteIndex > 0) {
				for (int i = noteIndex; i > 0; i--)
				{
					if (DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i).startbeat(), this->project->patterns(patternIndex).notes(i).starttick()) < DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i - 1).startbeat(), this->project->patterns(patternIndex).notes(i - 1).starttick())) {
						this->project->mutable_patterns(patternIndex)->mutable_notes()->SwapElements(i, i - 1);
					}
					else {
						break;
					}
				}
			}
			if (noteIndex < this->project->patterns(patternIndex).notes_size() - 1) {
				for (int i = noteIndex; i < this->project->patterns(patternIndex).notes_size(); i++)
				{
					if (DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i).startbeat(), this->project->patterns(patternIndex).notes(i).starttick()) > DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(i + 1).startbeat(), this->project->patterns(patternIndex).notes(i + 1).starttick())) {
						this->project->mutable_patterns(patternIndex)->mutable_notes()->SwapElements(i, i + 1);
					}
					else {
						break;
					}
				}
			}//排序

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

uint32_t DataModel::getNoteStartBeat(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& startbeat = this->project->patterns(patternIndex).notes(noteIndex).startbeat();
			this->modelMutex.unlock();
			return startbeat;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint32_t DataModel::getNoteStartTick(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& starttick = this->project->patterns(patternIndex).notes(noteIndex).starttick();
			this->modelMutex.unlock();
			return starttick;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint64_t DataModel::getNoteLength(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& length = this->project->patterns(patternIndex).notes(noteIndex).length();
			this->modelMutex.unlock();
			return length;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint32_t DataModel::getNotePitch(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& pitch = this->project->patterns(patternIndex).notes(noteIndex).pitch();
			this->modelMutex.unlock();
			return pitch;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setNoteName(int patternIndex, int noteIndex, std::string name)
{
	if (name.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (name == this->project->patterns(patternIndex).notes(noteIndex).name()) {
				this->modelMutex.unlock();
				return;
			}

			std::string dictionary = this->project->patterns(patternIndex).dictionary();
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getDictionaryAvailable()) {
					if (i == dictionary) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断字典存在
			std::string engine = Package::getPackage().getEngineName(this->project->patterns(patternIndex).library());
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getEngineAvailable()) {
					if (i == engine) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断引擎存在

			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_name(name);

			if (Package::getPackage().getEngineSplit(engine)) {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}//调用引擎分词器
			else {
				org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex);
				std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, note->name());
				note->clear_phonemes();
				for (auto& p : phonemeM.first) {
					org::infinity::idm::utils::Pair* pair = note->add_phonemes();
					pair->set_key(p.first);
					pair->set_value(p.second);
				}
				note->set_consonant(phonemeM.second);
			}//使用字典分词

			uint32_t startBeat = this->project->patterns(patternIndex).notes(noteIndex).startbeat();
			uint32_t startTick = this->project->patterns(patternIndex).notes(noteIndex).starttick();
			uint64_t length = this->project->patterns(patternIndex).notes(noteIndex).length();

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::string DataModel::getNoteName(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& name = this->project->patterns(patternIndex).notes(noteIndex).name();
			this->modelMutex.unlock();
			return name;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setNoteTimbre(int patternIndex, int noteIndex, std::string timbre)
{
	if (timbre.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (timbre == this->project->patterns(patternIndex).notes(noteIndex).timbre()) {
				this->modelMutex.unlock();
				return;
			}
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getLibraryTimbre(this->project->patterns(patternIndex).library())) {
					if (i == timbre) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断音色存在

			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_timbre(timbre);

			uint32_t startBeat = this->project->patterns(patternIndex).notes(noteIndex).startbeat();
			uint32_t startTick = this->project->patterns(patternIndex).notes(noteIndex).starttick();
			uint64_t length = this->project->patterns(patternIndex).notes(noteIndex).length();

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::string DataModel::getNoteTimbre(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& timbre = this->project->patterns(patternIndex).notes(noteIndex).timbre();
			this->modelMutex.unlock();
			return timbre;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::setNotePhonemes(int patternIndex, int noteIndex, std::vector<int64_t>& phonemes)
{
	if (phonemes.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			std::vector<std::pair<std::string, int64_t>> currentPhonemes;
			for (auto& p : this->project->patterns(patternIndex).notes(noteIndex).phonemes()) {
				currentPhonemes.push_back(std::make_pair(p.key(), p.value()));
			}//获取当前状态

			bool consonant = this->project->patterns(patternIndex).notes(noteIndex).consonant();

			if (phonemes.size() != currentPhonemes.size()) {
				this->modelMutex.unlock();
				return;
			}//数量不匹配

			if (consonant) {
				if (phonemes.at(0) > 0) {
					this->modelMutex.unlock();
					return;
				}
			}
			else {
				if (phonemes.at(0) == 0) {
					this->modelMutex.unlock();
					return;
				}
			}//检查头辅音

			{
				int64_t minTemp = phonemes.at(0);
				for (int i = 1; i < phonemes.size(); i++) {
					if (phonemes.at(i) < minTemp || phonemes.at(i) < 0) {
						this->modelMutex.unlock();
						return;
					}
					minTemp = phonemes.at(i);
				}
			}//递增检查

			{
				int64_t endMax = this->project->patterns(patternIndex).notes(noteIndex).length();
				if (noteIndex < this->project->patterns(patternIndex).notes_size() - 1) {
					uint64_t endTick = DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(noteIndex + 1).startbeat(), this->project->patterns(patternIndex).notes(noteIndex + 1).starttick()) - this->project->patterns(patternIndex).notes(noteIndex + 1).phonemes(0).value();
					int64_t endLength = endTick - DataModel::Utils::getTick(this->project->patterns(patternIndex).notes(noteIndex).startbeat(), this->project->patterns(patternIndex).notes(noteIndex).starttick());
					endMax = std::min(endMax, endLength);
				}
				if (phonemes.at(phonemes.size() - 1) > endMax) {
					this->modelMutex.unlock();
					return;
				}
			}//检查尾音出界

			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->mutable_phonemes()->Clear();
			for (int i = 0; i < phonemes.size(); i++) {
				org::infinity::idm::utils::Pair* pair = this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->mutable_phonemes()->Add();
				pair->set_key(currentPhonemes.at(i).first);
				pair->set_value(phonemes.at(i));
			}//更改

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::vector<std::pair<std::string, int64_t>> DataModel::getNotePhonemes(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			std::vector<std::pair<std::string, int64_t>> phonemes;
			for (auto& p : this->project->patterns(patternIndex).notes(noteIndex).phonemes()) {
				phonemes.push_back(std::make_pair(p.key(), p.value()));
			}
			this->modelMutex.unlock();
			return phonemes;
		}
	}
	this->modelMutex.unlock();
	return std::vector<std::pair<std::string, int64_t>>();
}

void DataModel::setNoteFlags(int patternIndex, int noteIndex, std::string flags)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的音符修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (flags == this->project->patterns(patternIndex).notes(noteIndex).flags()) {
				this->modelMutex.unlock();
				return;
			}
			this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->set_flags(flags);

			uint32_t startBeat = this->project->patterns(patternIndex).notes(noteIndex).startbeat();
			uint32_t startTick = this->project->patterns(patternIndex).notes(noteIndex).starttick();
			uint64_t length = this->project->patterns(patternIndex).notes(noteIndex).length();

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::string DataModel::getNoteFlags(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& flags = this->project->patterns(patternIndex).notes(noteIndex).flags();
			this->modelMutex.unlock();
			return flags;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

bool DataModel::getNoteConsonant(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& consonant = this->project->patterns(patternIndex).notes(noteIndex).consonant();
			this->modelMutex.unlock();
			return consonant;
		}
	}
	this->modelMutex.unlock();
	return false;
}

int DataModel::countNoteParam(int patternIndex, int noteIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			auto&& params_size = this->project->patterns(patternIndex).notes(noteIndex).params_size();
			this->modelMutex.unlock();
			return params_size;
		}
	}
	this->modelMutex.unlock();
	return -1;
}

std::string DataModel::getNoteParamName(int patternIndex, int noteIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				auto&& name = this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).name();
				this->modelMutex.unlock();
				return name;
			}
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

double DataModel::getNoteParamMax(int patternIndex, int noteIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				auto&& vmax = this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).vmax();
				this->modelMutex.unlock();
				return vmax;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

double DataModel::getNoteParamMin(int patternIndex, int noteIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				auto&& vmin = this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).vmin();
				this->modelMutex.unlock();
				return vmin;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

double DataModel::getNoteParamDefault(int patternIndex, int noteIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				auto&& vdefault = this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).vdefault();
				this->modelMutex.unlock();
				return vdefault;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setNoteParamValue(int patternIndex, int noteIndex, int paramIndex, double value)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				if (value == this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).value()) {
					this->modelMutex.unlock();
					return;
				}
				if (
					value< this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).vmin() ||
					value>this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).vmax()
					) {
					this->modelMutex.unlock();
					return;
				}
				this->project->mutable_patterns(patternIndex)->mutable_notes(noteIndex)->mutable_params(paramIndex)->set_value(value);

				this->modelMutex.unlock();

				this->renderFunc(this->project->patterns(patternIndex).name());
				this->viewFunc();
				return;
			}
		}
	}
	this->modelMutex.unlock();
}

double DataModel::getNoteParamValue(int patternIndex, int noteIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (noteIndex >= 0 && noteIndex < this->project->patterns(patternIndex).notes_size()) {
			if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).notes(noteIndex).params_size()) {
				auto&& value = this->project->patterns(patternIndex).notes(noteIndex).params(paramIndex).value();
				this->modelMutex.unlock();
				return value;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

int DataModel::countParam(int patternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		auto&& params_size = this->project->patterns(patternIndex).params_size();
		this->modelMutex.unlock();
		return params_size;
	}
	this->modelMutex.unlock();
	return -1;
}

std::string DataModel::getParamName(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& name = this->project->patterns(patternIndex).params(paramIndex).name();
			this->modelMutex.unlock();
			return name;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

double DataModel::getParamMax(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& vmax = this->project->patterns(patternIndex).params(paramIndex).vmax();
			this->modelMutex.unlock();
			return vmax;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

double DataModel::getParamMin(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& vmin = this->project->patterns(patternIndex).params(paramIndex).vmin();
			this->modelMutex.unlock();
			return vmin;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

double DataModel::getParamDefault(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& vdefault = this->project->patterns(patternIndex).params(paramIndex).vdefault();
			this->modelMutex.unlock();
			return vdefault;
		}
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::setParamColor(int patternIndex, int paramIndex, std::string color)
{
	if (color.empty()) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (color == this->project->patterns(patternIndex).params(paramIndex).color()) {
				this->modelMutex.unlock();
				return;
			}

			this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->set_color(color);

			this->modelMutex.unlock();

			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

std::string DataModel::getParamColor(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& color = this->project->patterns(patternIndex).params(paramIndex).color();
			this->modelMutex.unlock();
			return color;
		}
	}
	this->modelMutex.unlock();
	return std::string();
}

void DataModel::addParamPattern(int patternIndex, int paramIndex, uint32_t startBeat, uint32_t startTick, uint64_t length)
{
	if (length == 0) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (startTick >= 480) {
				startBeat += startTick / 480;
				startTick %= 480;
			}

			uint64_t ST = DataModel::Utils::getTick(startBeat, startTick);
			std::pair<uint32_t, uint32_t> EP = DataModel::Utils::getEP(startBeat, startTick, length);
			uint64_t ET = DataModel::Utils::getTick(EP.first, EP.second);

			for (auto& c : this->project->patterns(patternIndex).params(paramIndex).patterns()) {
				uint64_t STC = DataModel::Utils::getTick(c.startbeat(), c.starttick());
				std::pair<uint32_t, uint32_t> EPC = DataModel::Utils::getEP(c.startbeat(), c.starttick(), c.length());
				uint64_t ETC = DataModel::Utils::getTick(EPC.first, EPC.second);
				if (ET > STC && ETC > ST) {
					this->modelMutex.unlock();
					return;
				}
			}//判断重叠

			if (this->project->patterns(patternIndex).library().empty()) {
				std::string library = Package::getPackage().getLibraryDefault();
				std::string dictionary = Package::getPackage().getLibraryDictionaryDefault(library);
				{
					bool flag = false;
					for (auto& i : Package::getPackage().getDictionaryAvailable()) {
						if (i == dictionary) {
							flag = true;
							break;
						}
					}
					if (!flag) {
						this->modelMutex.unlock();
						return;
					}
				}//判断字典是否存在
				std::string engine = Package::getPackage().getEngineName(library);
				{
					bool flag = false;
					for (auto& i : Package::getPackage().getEngineAvailable()) {
						if (i == engine) {
							flag = true;
							break;
						}
					}
					if (!flag) {
						this->modelMutex.unlock();
						return;
					}
				}//判断引擎是否存在

				std::string timbre = Package::getPackage().getLibraryTimbreDefault(library);

				this->project->mutable_patterns(patternIndex)->set_library(library);
				this->project->mutable_patterns(patternIndex)->set_dictionary(dictionary);
				this->project->mutable_patterns(patternIndex)->set_timbrea(timbre);
				this->project->mutable_patterns(patternIndex)->set_timbreb(timbre);

				{
					auto epl = Package::getPackage().getEngineParam(engine);//获取新引擎参数表
					auto ckF = [epl](std::string name)->bool {
						bool flag = false;
						for (auto& i : epl) {
							if (i.name == name) {
								flag = true;
								break;
							}
						}
						return flag;
					};//检查参数是否存在于引擎的列表中
					std::set<std::string> paramL;//已存在的参数
					for (auto it = this->project->mutable_patterns(patternIndex)->mutable_params()->begin(); it != this->project->mutable_patterns(patternIndex)->mutable_params()->end(); ) {
						if ((*it).patterns_size() == 0) {
							if (!ckF((*it).name())) {
								it = this->project->mutable_patterns(patternIndex)->mutable_params()->erase(it);
								continue;
							}
						}
						paramL.insert((*it).name());
						it++;
					}//后侧迭代器在移除元素时会失效
					for (auto& p : epl) {
						if (paramL.count(p.name) == 0) {
							org::infinity::idm::TrackParam* param = this->project->mutable_patterns(patternIndex)->add_params();
							param->set_name(p.name);
							param->set_vmax(p.vMax);
							param->set_vmin(p.vMin);
							param->set_vdefault(p.vDefault);

							param->set_color("0fabff");
						}
					}//添加参数
				}//调整轨道参数

				{
					auto epl = Package::getPackage().getEngineNoteParam(engine);//获取新引擎参数表
					auto ckF = [epl](std::string name)->bool {
						bool flag = false;
						for (auto& i : epl) {
							if (i.name == name) {
								flag = true;
								break;
							}
						}
						return flag;
					};//检查参数是否存在于引擎的列表中
					for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
						org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);

						std::set<std::string> paramL;//已存在的参数
						for (auto it = note->mutable_params()->begin(); it != note->mutable_params()->end(); ) {
							if (!ckF((*it).name())) {
								it = note->mutable_params()->erase(it);
								continue;
							}
							paramL.insert((*it).name());
							it++;
						}//后侧迭代器在移除元素时会失效
						for (auto& p : epl) {
							if (paramL.count(p.name) == 0) {
								org::infinity::idm::NoteParam* param = note->add_params();
								param->set_name(p.name);
								param->set_vmax(p.vMax);
								param->set_vmin(p.vMin);
								param->set_vdefault(p.vDefault);
								param->set_value(p.vDefault);
							}
						}//添加参数
					}
				}//调整音符参数

				if (Package::getPackage().getEngineSplit(engine)) {
					for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
						org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
						std::pair<std::map<std::string, int64_t>, bool> phonemeM = this->eSplitFunc(engine, dictionary, note->name());
						note->clear_phonemes();
						for (auto& p : phonemeM.first) {
							org::infinity::idm::utils::Pair* pair = note->add_phonemes();
							pair->set_key(p.first);
							pair->set_value(p.second);
						}
						note->set_consonant(phonemeM.second);
					}
				}//调用引擎分词器
				else {
					for (int i = 0; i < this->project->patterns(patternIndex).notes_size(); i++) {
						org::infinity::idm::Note* note = this->project->mutable_patterns(patternIndex)->mutable_notes(i);
						std::pair<std::map<std::string, int64_t>, bool> phonemeM = Package::getPackage().getDictionaryPhoneme(dictionary, note->name());
						note->clear_phonemes();
						for (auto& p : phonemeM.first) {
							org::infinity::idm::utils::Pair* pair = note->add_phonemes();
							pair->set_key(p.first);
							pair->set_value(p.second);
						}
						note->set_consonant(phonemeM.second);
					}
				}//使用字典分词
			}//轨道未设置声库

			std::string engine = Package::getPackage().getEngineName(this->project->patterns(patternIndex).library());
			{
				bool flag = false;
				for (auto& i : Package::getPackage().getEngineAvailable()) {
					if (i == engine) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->modelMutex.unlock();
					return;
				}
			}//判断引擎存在

			{
				auto epl = Package::getPackage().getEngineParam(engine);//获取新引擎参数表
				auto ckF = [epl](std::string name)->bool {
					bool flag = false;
					for (auto& i : epl) {
						if (i.name == name) {
							flag = true;
							break;
						}
					}
					return flag;
				};//检查参数是否存在于引擎的列表中
				if (!ckF(this->project->patterns(patternIndex).params(paramIndex).name())) {
					this->modelMutex.unlock();
					return;
				}
			}//检查参数存在

			org::infinity::idm::ParamPattern* pattern = nullptr;
			double dvalue = (this->project->patterns(patternIndex).params(paramIndex).vdefault() - this->project->patterns(patternIndex).params(paramIndex).vmin()) / (this->project->patterns(patternIndex).params(paramIndex).vmax() - this->project->patterns(patternIndex).params(paramIndex).vmin());
			if (this->project->patterns(patternIndex).params(paramIndex).patterns_size() > 0) {
				if (DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(0).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(0).starttick())) {
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾建立空element
					for (int i = this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 2; i >= 0; i--) {
						this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i + 1)->CopyFrom(this->project->patterns(patternIndex).params(paramIndex).patterns(i));
					}//element集体后移
					pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(0);//腾出头element
					pattern->Clear();
				}
				else if (DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).starttick())) {
					pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾
					dvalue = this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).points(this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).points_size() - 1).y();
				}
				else {
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾建立空element
					for (int i = this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 2; i > 0; i--) {
						this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i + 1)->CopyFrom(this->project->patterns(patternIndex).params(paramIndex).patterns(i));//element后移
						if (
							DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).starttick()) &&
							DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(i + 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(i + 1).starttick())
							) {//如果当前element符合要求
							pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i);//移动element空出位置即是插入位置
							pattern->Clear();
							dvalue = this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).points(this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).points_size() - 1).y();
						}
					}
				}
			}
			else {
				pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//唯一一个
			}

			pattern->set_startbeat(startBeat);
			pattern->set_starttick(startTick);
			pattern->set_length(length);

			org::infinity::idm::Point* pointS = pattern->add_points();
			org::infinity::idm::Point* pointE = pattern->add_points();
			pointS->set_x(0);
			pointS->set_y(dvalue);
			pointS->set_x(1);
			pointS->set_y(dvalue);

			this->modelMutex.unlock();

			this->renderFunc(this->project->patterns(patternIndex).name());
			this->viewFunc();
			return;
		}
	}
	this->modelMutex.unlock();
}

void DataModel::removeParamPattern(int patternIndex, int paramIndex, int ppatternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				std::string engine = Package::getPackage().getEngineName(this->project->patterns(patternIndex).library());
				{
					bool flag = false;
					for (auto& i : Package::getPackage().getEngineAvailable()) {
						if (i == engine) {
							flag = true;
							break;
						}
					}
					if (!flag) {
						this->modelMutex.unlock();
						return;
					}
				}//判断引擎存在

				{
					auto it = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->begin() + ppatternIndex;
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->erase(it);
				}

				{
					auto epl = Package::getPackage().getEngineParam(engine);//获取新引擎参数表
					auto ckF = [epl](std::string name)->bool {
						bool flag = false;
						for (auto& i : epl) {
							if (i.name == name) {
								flag = true;
								break;
							}
						}
						return flag;
					};//检查参数是否存在于引擎的列表中
					if (!ckF(this->project->patterns(patternIndex).params(paramIndex).name())) {
						if (this->project->patterns(patternIndex).params(paramIndex).patterns_size() == 0) {
							auto it = this->project->mutable_patterns(patternIndex)->mutable_params()->begin() + paramIndex;
							this->project->mutable_patterns(patternIndex)->mutable_params()->erase(it);
						}
					}
				}//如果参数不存在，可以删除参数

				this->modelMutex.unlock();

				this->renderFunc(this->project->patterns(patternIndex).name());
				this->viewFunc();
				return;
			}
		}
	}
	this->modelMutex.unlock();
}

int DataModel::countParamPattern(int patternIndex, int paramIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			auto&& patterns_size = this->project->patterns(patternIndex).params(paramIndex).patterns_size();
			this->modelMutex.unlock();
			return patterns_size;
		}
	}
	this->modelMutex.unlock();
	return -1;
}

void DataModel::setParamPatternPlace(int patternIndex, int paramIndex, int ppatternIndex, uint32_t startBeat, uint32_t startTick, uint64_t length)
{
	if (length == 0) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				if (startTick >= 480) {
					startBeat += startTick / 480;
					startTick %= 480;
				}

				uint64_t ST = DataModel::Utils::getTick(startBeat, startTick);
				std::pair<uint32_t, uint32_t> EP = DataModel::Utils::getEP(startBeat, startTick, length);
				uint64_t ET = DataModel::Utils::getTick(EP.first, EP.second);

				for (auto& c : this->project->patterns(patternIndex).params(paramIndex).patterns()) {
					uint64_t STC = DataModel::Utils::getTick(c.startbeat(), c.starttick());
					std::pair<uint32_t, uint32_t> EPC = DataModel::Utils::getEP(c.startbeat(), c.starttick(), c.length());
					uint64_t ETC = DataModel::Utils::getTick(EPC.first, EPC.second);
					if (ET > STC && ETC > ST) {
						this->modelMutex.unlock();
						return;
					}
				}//判断重叠

				org::infinity::idm::ParamPattern pTemp(this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex));
				auto it = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->begin() + ppatternIndex;
				this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->erase(it);//抽取当前样式

				org::infinity::idm::ParamPattern* pattern = nullptr;
				if (this->project->patterns(patternIndex).params(paramIndex).patterns_size() > 0) {
					if (DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(0).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(0).starttick())) {
						this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾建立空element
						for (int i = this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 2; i >= 0; i--) {
							this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i + 1)->CopyFrom(this->project->patterns(patternIndex).params(paramIndex).patterns(i));
						}//element集体后移
						pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(0);//腾出头element
						pattern->Clear();
					}
					else if (DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 1).starttick())) {
						pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾
					}
					else {
						this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//末尾建立空element
						for (int i = this->project->patterns(patternIndex).params(paramIndex).patterns_size() - 2; i > 0; i--) {
							this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i + 1)->CopyFrom(this->project->patterns(patternIndex).params(paramIndex).patterns(i));//element后移
							if (
								DataModel::Utils::getTick(startBeat, startTick) >= DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(i - 1).starttick()) &&
								DataModel::Utils::getTick(startBeat, startTick) < DataModel::Utils::getTick(this->project->patterns(patternIndex).params(paramIndex).patterns(i + 1).startbeat(), this->project->patterns(patternIndex).params(paramIndex).patterns(i + 1).starttick())
								) {//如果当前element符合要求
								pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(i);//移动element空出位置即是插入位置
								pattern->Clear();
							}
						}
					}
				}
				else {
					pattern = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns()->Add();//唯一一个
				}

				pTemp.set_startbeat(startBeat);
				pTemp.set_starttick(startTick);
				pTemp.set_length(length);
				pattern->CopyFrom(pTemp);

				this->modelMutex.unlock();

				this->renderFunc(this->project->patterns(patternIndex).name());
				this->viewFunc();
				return;
			}
		}
	}
	this->modelMutex.unlock();
}

uint32_t DataModel::getParamPatternStartBeat(int patternIndex, int paramIndex, int ppatternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				auto&& startbeat = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).startbeat();
				this->modelMutex.unlock();
				return startbeat;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint32_t DataModel::getParamPatternStartTick(int patternIndex, int paramIndex, int ppatternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				auto&& starttick = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).starttick();
				this->modelMutex.unlock();
				return starttick;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

uint64_t DataModel::getParamPatternLength(int patternIndex, int paramIndex, int ppatternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				auto&& length = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).length();
				this->modelMutex.unlock();
				return length;
			}
		}
	}
	this->modelMutex.unlock();
	return 0;
}

void DataModel::addPoint(int patternIndex, int paramIndex, int ppatternIndex, double x, double y)
{
	if (y < 0 || y>1) {
		return;
	}
	this->addPointUnsafe(patternIndex, paramIndex, ppatternIndex, x, y);
}

void DataModel::addPointUnsafe(int patternIndex, int paramIndex, int ppatternIndex, double x, double y)
{
	if (x <= 0 || x >= 1) {
		return;
	}
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				org::infinity::idm::Point* point = nullptr;
				this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points()->Add();//末尾建立空element
				for (int i = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size() - 2; i > 0; i--) {
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points(i + 1)->CopyFrom(this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(i));//element后移
					if (
						x >= this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(i - 1).x() &&
						x < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(i + 1).x()
						) {//如果当前element符合要求
						point = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points(i);//移动element空出位置即是插入位置
						point->Clear();
					}
				}//确认插入点

				point->set_x(x);
				point->set_y(y);

				this->modelMutex.unlock();

				this->renderFunc(this->project->patterns(patternIndex).name());
				this->viewFunc();
				return;
			}
		}
	}
	this->modelMutex.unlock();
}

void DataModel::removePoint(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				if (pointIndex > 0 && pointIndex < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size() - 1) {

					auto it = this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points()->begin() + pointIndex;
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points()->erase(it);

					this->modelMutex.unlock();

					this->renderFunc(this->project->patterns(patternIndex).name());
					this->viewFunc();
					return;
				}
			}
		}
	}
	this->modelMutex.unlock();
}

int DataModel::countPoint(int patternIndex, int paramIndex, int ppatternIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				auto&& points_size = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size();
				this->modelMutex.unlock();
				return points_size;
			}
		}
	}
	this->modelMutex.unlock();
	return -1;
}

void DataModel::setPointPlace(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex, double x, double y)
{
	if (y < 0 || y>1) {
		return;
	}
	this->setPointPlaceUnsafe(patternIndex, paramIndex, ppatternIndex, pointIndex, x, y);
}

void DataModel::setPointPlaceUnsafe(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex, double x, double y)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (this->project->patterns(patternIndex).type() == org::infinity::idm::Pattern::Type::Pattern_Type_WAVE) {
			this->modelMutex.unlock();
			return;
		}//不接受对WAVE样式的参数修改
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				if (pointIndex >= 0 && pointIndex < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size()) {

					if (pointIndex == 0) {
						x = 0;
					}
					if (pointIndex == this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size() - 1) {
						x = 1;
					}//首尾控制点横坐标锁定

					if (pointIndex > 0) {
						if (x < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(pointIndex - 1).x()) {
							this->modelMutex.unlock();
							return;
						}
					}
					if (pointIndex < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size() - 1) {
						if (x > this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(pointIndex + 1).x()) {
							this->modelMutex.unlock();
							return;
						}
					}//控制点作用域

					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points(pointIndex)->set_x(x);
					this->project->mutable_patterns(patternIndex)->mutable_params(paramIndex)->mutable_patterns(ppatternIndex)->mutable_points(pointIndex)->set_y(y);

					this->modelMutex.unlock();

					this->renderFunc(this->project->patterns(patternIndex).name());
					this->viewFunc();
					return;
				}
			}
		}
	}
	this->modelMutex.unlock();
}

double DataModel::getPointX(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				if (pointIndex >= 0 && pointIndex < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size()) {
					auto&& x = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(pointIndex).x();
					this->modelMutex.unlock();
					return x;
				}
			}
		}
	}
	this->modelMutex.unlock();
	return -1;
}

double DataModel::getPointY(int patternIndex, int paramIndex, int ppatternIndex, int pointIndex)
{
	this->modelMutex.lock();
	if (patternIndex >= 0 && patternIndex < this->project->patterns_size()) {
		if (paramIndex >= 0 && paramIndex < this->project->patterns(patternIndex).params_size()) {
			if (ppatternIndex >= 0 && ppatternIndex < this->project->patterns(patternIndex).params(paramIndex).patterns_size()) {
				if (pointIndex >= 0 && pointIndex < this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points_size()) {
					auto&& y = this->project->patterns(patternIndex).params(paramIndex).patterns(ppatternIndex).points(pointIndex).y();
					this->modelMutex.unlock();
					return y;
				}
			}
		}
	}
	this->modelMutex.unlock();
	return -1;
}

//Utils

std::pair<uint32_t, uint32_t> DataModel::Utils::getEP(uint32_t startBeat, uint32_t startTick, uint64_t length)
{
	length += startTick;
	startBeat += std::floor(length / 480.0);
	length %= 480;
	return std::make_pair(startBeat, static_cast<uint32_t>(length));
}

uint64_t DataModel::Utils::getTick(uint32_t startBeat, uint32_t startTick)
{
	return static_cast<uint64_t>(startBeat) * 480 + startTick;
}

std::pair<uint32_t, uint32_t> DataModel::Utils::getBeat(uint64_t tick)
{
	return std::make_pair((uint32_t)(tick / 480), (uint32_t)(tick % 480));
}
