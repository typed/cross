#ifndef cross_common_ischeduler_h
#define cross_common_ischeduler_h

#include "Common.h"

namespace cross
{

enum eTaskEnd
{
	TE_Continue,
	TE_Exit
};

enum eTaskEndEvent
{
	EV_TimeOut,
	EV_Exit,
	EV_Delete
};

class iScheduler;

struct stTaskParam
{
	i32 id;
	tTime curTime;
	tTime startTime;
	tTime sumTime;
};

class iTaskObj
{
public:
	iTaskObj();
	virtual ~iTaskObj();
	virtual eTaskEnd OnTask(const stTaskParam& param) = 0;
	virtual void OnTaskEnd(const stTaskParam& param, eTaskEndEvent event) {}
protected:
	bool AddTaskNow(i32 id, tTime durationTime) {
		return AddTaskNow(id, durationTime, 0);
	}
	bool AddTaskNow(i32 id, tTime durationTime, tTime stepTime) {
		return AddTask(id, GetTimeCur(), durationTime, stepTime);
	}
	bool AddTask(i32 id, tTime startTime, tTime durationTime) {
		return AddTask(id, GetTimeCur(), durationTime, 0);
	}
	bool AddTask(i32 id, tTime startTime, tTime durationTime, tTime stepTime);
	bool DelTask(i32 id);
	bool DelTask();
	bool HaveTask(i32 id);
	tTime GetTimeCur() const;
private:
	ui64 m_id;
};

class iScheduler
{
public:
	virtual ~iScheduler() {}
	virtual void RunOnce() = 0;
};

iScheduler* GetScheduler();

}
#endif
