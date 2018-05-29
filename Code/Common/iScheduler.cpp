#include "iScheduler.h"
#include "iLog.h"
#include "cClock.h"
#include <vector>
#include <map>
#include <set>

using namespace std;

namespace cross
{

struct stATask
{
	ui64		to;
	i32			id;
	tTime	sortTime;
	tTime	startTime;
	tTime	endTime;
	tTime	stepTime;
	i32			bDel;
	stATask() : to(0), id(0), sortTime(0), startTime(0), endTime(0), stepTime(0), bDel(0) {}
	bool operator == (const stATask& other) const {
		return sortTime == other.sortTime;
	}
	bool operator < (const stATask& other) const {
		return sortTime < other.sortTime;
	}
};

typedef multiset<stATask> aTask;
typedef aTask::iterator itTask;

typedef map<i32, itTask > hmTask1;
typedef hmTask1::iterator itHmTask1;

typedef map<ui64, hmTask1 > hmTask2;
typedef hmTask2::iterator itHmTask2;

typedef vector<itTask> aDelTask;

typedef map<ui64, iTaskObj*> hmTaskObj;

hmTaskObj& GetHmTaskObj()
{
	static hmTaskObj s_hm;
	return s_hm;
}

iTaskObj* FindTaskObj(ui64 id)
{
	hmTaskObj::iterator it = GetHmTaskObj().find(id);
	if (it != GetHmTaskObj().end())
		return it->second;
	return 0;
}

class cScheduler : 
	public iScheduler
{
public:

	//<?> 处理stepTime有问题，应该采用startTime，重新加入列表排序
	void RunOnce()
	{
		tTime tmTimeCur = GetClock()->GetTimeCur();
		aDelTask deltask;

		for (itTask i = m_aTask.begin(); i != m_aTask.end(); i++) {
			stATask& task = (stATask&)(*i);
			if (task.bDel) {
				deltask.push_back(i);
				continue;
			}
			if (task.sortTime > tmTimeCur)
				break;
			//endTime为0是永久任务
			m_param.id = task.id;
			m_param.curTime = tmTimeCur;
			m_param.startTime = task.startTime;
			if (task.endTime)
				m_param.sumTime = task.endTime - task.startTime;
			else
				m_param.sumTime = 0;
			iTaskObj* pTo = FindTaskObj(task.to);
			if (pTo == 0) {
				deltask.push_back(i);
				continue;
			}
			eTaskEnd te = pTo->OnTask(m_param);
			if (task.bDel) {
				deltask.push_back(i);
				continue;
			}
			if (task.stepTime) {
				if (te == TE_Continue) {
					if (task.endTime == 0 || task.sortTime + task.stepTime < task.endTime) {
						stATask taskTemp = task;
						taskTemp.sortTime += task.stepTime;
						m_hmTask[taskTemp.to][taskTemp.id] = m_aTask.insert(taskTemp);
					}
					else {
						RemoveHash(task.to, task.id);
						iTaskObj* pTo = FindTaskObj(task.to);
						if (pTo == 0) {
							deltask.push_back(i);
							continue;
						}
						pTo->OnTaskEnd(m_param, EV_TimeOut);
					}
				}
				else {
					RemoveHash(task.to, task.id);
					iTaskObj* pTo = FindTaskObj(task.to);
					if (pTo == 0) {
						deltask.push_back(i);
						continue;
					}
					pTo->OnTaskEnd(m_param, EV_Exit);
				}
				task.bDel = 1;
			}
			else {
				if (task.endTime && task.endTime < tmTimeCur) {
					RemoveHash(task.to, task.id);
					iTaskObj* pTo = FindTaskObj(task.to);
					if (pTo == 0) {
						deltask.push_back(i);
						continue;
					}
					pTo->OnTaskEnd(m_param, EV_TimeOut);
					task.bDel = 1;
				}
				else {
					if (te == TE_Exit) {
						RemoveHash(task.to, task.id);
						iTaskObj* pTo = FindTaskObj(task.to);
						if (pTo == 0) {
							deltask.push_back(i);
							continue;
						}
						pTo->OnTaskEnd(m_param, EV_Exit);
						task.bDel = 1;
					}
				}
			}
		}
		for (ui32 i = 0; i < deltask.size(); i++)
			m_aTask.erase(deltask[i]);
		deltask.clear();
	}

	tTime GetTimeCur()
	{
		return GetClock()->GetTimeCur();
	}

	tTime GetTimeEslasped()
	{
		return GetClock()->GetTimeEslasped();
	}

	bool AddTask(ui64 to, i32 id, tTime startTime, tTime durationTime, tTime stepTime)
	{
		if (HaveTask(to, id))
			return false;
		stATask at;
		at.to = to;
		at.id = id;
		at.sortTime = startTime;
		at.startTime = startTime;
		if (durationTime)
			at.endTime = startTime + durationTime;
		else
			at.endTime = 0;
		at.stepTime = stepTime;
		m_hmTask[to][id] = m_aTask.insert(at);
		return true;
	}

	bool DelTask(ui64 to, i32 id)
	{
		itHmTask2 i = m_hmTask.find(to);
		if (i == m_hmTask.end())
			return false;
		hmTask1& t = i->second;
		itHmTask1 j = t.find(id);
		if (j == t.end())
			return false;
		itTask k1 = j->second;
		stATask& k = (stATask&)(*k1);
		m_param.id = k.id;
		m_param.curTime = GetTimeCur();
		m_param.startTime = k.startTime;
		if (k.endTime)
			m_param.sumTime = k.endTime - k.startTime;
		else
			m_param.sumTime = 0;
		iTaskObj* pTo = FindTaskObj(k.to);
		if (pTo)
			pTo->OnTaskEnd(m_param, EV_Delete);
		k.bDel = 1;
		t.erase(j);
		if (t.empty())
			m_hmTask.erase(i);
		return true;
	}

	bool DelTask(ui64 to)
	{
		itHmTask2 i = m_hmTask.find(to);
		if (i == m_hmTask.end())
			return false;
		hmTask1& t = i->second;
		for (itHmTask1 j = t.begin(); j != t.end(); j++) {
			stATask& k = (stATask&)(*(j->second));
			m_param.id = k.id;
			m_param.curTime = GetTimeCur();
			m_param.startTime = k.startTime;
			if (k.endTime)
				m_param.sumTime = k.endTime - k.startTime;
			else
				m_param.sumTime = 0;
			iTaskObj* pTo = FindTaskObj(k.to);
			if (pTo)
				pTo->OnTaskEnd(m_param, EV_Delete);
			k.bDel = 1;
		}
		t.clear();
		m_hmTask.erase(i);
		return true;
	}

	bool HaveTask(ui64 to, i32 id)
	{
		itHmTask2 i = m_hmTask.find(to);
		if (i == m_hmTask.end())
			return false;
		hmTask1& t = i->second;
		itHmTask1 j = t.find(id);
		return j != t.end();
	}

	bool RemoveHash(ui64 to, i32 id)
	{
		itHmTask2 i = m_hmTask.find(to);
		if (i == m_hmTask.end())
			return false;
		hmTask1& t = i->second;
		itHmTask1 j = t.find(id);
		if (j == t.end())
			return false;
		t.erase(j);
		if (t.empty())
			m_hmTask.erase(i);
		return true;
	}

	bool AddTaskTime(ui64 to, i32 id, tTime addTime, tTime durationTime)
	{
		itHmTask2 i = m_hmTask.find(to);
		if (i == m_hmTask.end())
			return false;
		hmTask1& t = i->second;
		itHmTask1 j = t.find(id);
		if (j == t.end())
			return false;
		stATask& k = (stATask&)(*(j->second));
		k.startTime = GetTimeCur() + addTime;
		k.sortTime = k.startTime;
		k.endTime = k.startTime + durationTime;
		return true;
	}

private:
	aTask		m_aTask;
	hmTask2		m_hmTask;
	stTaskParam m_param;
};

cScheduler* GetScheduler2()
{
	static cScheduler t;
	return &t;
}

iScheduler* GetScheduler()
{
	return GetScheduler2();
}

iTaskObj::iTaskObj()
{
	static ui64 s_id = 1;
	m_id = s_id++;
	GetHmTaskObj()[m_id] = this;
}

iTaskObj::~iTaskObj()
{
	GetScheduler2()->DelTask(m_id);
	GetHmTaskObj().erase(m_id);
}

bool iTaskObj::AddTask(i32 id, tTime startTime, tTime durationTime, tTime stepTime)
{
	return GetScheduler2()->AddTask(m_id, id, startTime, durationTime, stepTime);
}

bool iTaskObj::DelTask(i32 id)
{
	return GetScheduler2()->DelTask(m_id, id);
}

bool iTaskObj::DelTask()
{
	return GetScheduler2()->DelTask(m_id);
}

bool iTaskObj::HaveTask(i32 id)
{
	return GetScheduler2()->HaveTask(m_id, id);
}

tTime iTaskObj::GetTimeCur() const
{
	return GetScheduler2()->GetTimeCur();
}

}
