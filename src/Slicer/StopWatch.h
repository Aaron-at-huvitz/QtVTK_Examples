#pragma once

#include <chrono>
using namespace std::chrono;

#include <map>
#include <string>
#include <vector>
using namespace std;

#include <mutex>

class StopWatchElement
{
public:
	StopWatchElement(const string& name);
	~StopWatchElement();

	void Start();
	pair<double, double> Touch();
	pair<double, double> Stop();
	void Reset();

private:
	string m_name;
	vector<system_clock::time_point> m_times;
	duration<double> m_fromBegin;
	duration<double> m_fromLast;
};

class StopWatch
{
private:
	static StopWatch s_instance;
	static map<string, StopWatchElement*> s_stopWatches;
	static mutex s_stopWatchesLock;

	StopWatch();
	~StopWatch();

	static StopWatchElement* GetStopWatch(const string& key);
	static StopWatchElement* SetStopWatch(const string& key, StopWatchElement* pStopWatch);

public:
	static void Start(const string& key);
	static pair<double, double> Touch(const string& key);
	static pair<double, double> Stop(const string& key);
	static void Reset(const string& key);
};