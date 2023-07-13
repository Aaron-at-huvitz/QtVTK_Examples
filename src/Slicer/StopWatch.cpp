#include "StopWatch.h"
#include <iostream>
using namespace std;

StopWatchElement::StopWatchElement(const string& name) : m_name(name)
{
}

StopWatchElement::~StopWatchElement()
{
	m_times.clear();
}

void StopWatchElement::Start()
{
	m_times.clear();
	m_times.push_back(chrono::system_clock::now());
}

pair<double, double> StopWatchElement::Touch()
{
	m_times.push_back(chrono::system_clock::now());
	m_fromBegin = m_times[m_times.size() - 1] - m_times[0];
	m_fromLast = m_times[m_times.size() - 1] - m_times[m_times.size() - 2];
	double fromBegin = double(std::chrono::duration_cast<std::chrono::nanoseconds>(m_fromBegin).count()) / 1000000000.0;
	double fromLast = double(std::chrono::duration_cast<std::chrono::nanoseconds>(m_fromLast).count()) / 1000000000.0;
	return make_pair(fromBegin, fromLast);
}

pair<double, double> StopWatchElement::Stop()
{
	m_times.push_back(chrono::system_clock::now());
	m_fromBegin = m_times[m_times.size() - 1] - m_times[0];
	m_fromLast = m_times[m_times.size() - 1] - m_times[m_times.size() - 2];
	double fromBegin = double(std::chrono::duration_cast<std::chrono::nanoseconds>(m_fromBegin).count()) / 1000000000.0;
	double fromLast = double(std::chrono::duration_cast<std::chrono::nanoseconds>(m_fromLast).count()) / 1000000000.0;
	cout << m_name << " : " << fromBegin << " from beginning" << endl;
	cout << m_name << " : " << fromLast << " from last" << endl;
	return make_pair(fromBegin, fromLast);
}

void StopWatchElement::Reset()
{
	m_times.clear();
}

StopWatch StopWatch::s_instance;
map<string, StopWatchElement*> StopWatch::s_stopWatches;
mutex StopWatch::s_stopWatchesLock;

StopWatch::StopWatch()
{
}

StopWatch::~StopWatch()
{
	for (auto kvp : s_stopWatches) {
		if (kvp.second != nullptr) {
			//kvp.second->Stop();
			delete kvp.second;
			kvp.second = nullptr;
		}
	}
	s_stopWatches.clear();
}

StopWatchElement* StopWatch::GetStopWatch(const string& key)
{
	lock_guard<mutex> lock(s_stopWatchesLock);

	if (s_stopWatches.count(key) == 0)
	{
		return nullptr;
	}
	else
	{
		return s_stopWatches[key];
	}
}

StopWatchElement* StopWatch::SetStopWatch(const string& key, StopWatchElement* pStopWatch)
{
	lock_guard<mutex> lock(s_stopWatchesLock);

	if (s_stopWatches.count(key) != 0)
	{
		delete s_stopWatches[key];
	}

	s_stopWatches[key] = pStopWatch;
	return pStopWatch;
}

void StopWatch::Start(const string& key)
{
	auto pStopWatch = GetStopWatch(key);

	if (pStopWatch == nullptr) {
		pStopWatch = SetStopWatch(key, new StopWatchElement(key));
	}

	pStopWatch->Start();
}

pair<double, double> StopWatch::Touch(const string& key)
{
	auto pStopWatch = GetStopWatch(key);

	if (pStopWatch == nullptr) {
		pStopWatch = SetStopWatch(key, new StopWatchElement(key));
		pStopWatch->Start();
	}

	return pStopWatch->Touch();
}

pair<double, double> StopWatch::Stop(const string& key)
{
	auto pStopWatch = GetStopWatch(key);

	if (pStopWatch == nullptr) {
		pStopWatch = SetStopWatch(key, new StopWatchElement(key));
		pStopWatch->Start();
	}

	return pStopWatch->Stop();
}

void StopWatch::Reset(const string& key)
{
	auto pStopWatch = GetStopWatch(key);

	if (pStopWatch == nullptr) {
		pStopWatch = SetStopWatch(key, new StopWatchElement(key));
		pStopWatch->Start();
	}

	pStopWatch->Reset();
}
