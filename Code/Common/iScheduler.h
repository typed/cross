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
	TypeTime curTime;
	TypeTime startTime;
	TypeTime sumTime;
};

class iTaskObj
{
public:
	iTaskObj();
	virtual ~iTaskObj();
	virtual eTaskEnd OnTask(const stTaskParam& param) = 0;
	virtual void OnTaskEnd(const stTaskParam& param, eTaskEndEvent event) {}
protected:
	bool AddTaskNow(i32 id, TypeTime durationTime) {
		return AddTaskNow(id, durationTime, 0);
	}
	bool AddTaskNow(i32 id, TypeTime durationTime, TypeTime stepTime) {
		return AddTask(id, GetTimeCur(), durationTime, stepTime);
	}
	bool AddTask(i32 id, TypeTime startTime, TypeTime durationTime) {
		return AddTask(id, GetTimeCur(), durationTime, 0);
	}
	bool AddTask(i32 id, TypeTime startTime, TypeTime durationTime, TypeTime stepTime);
	bool DelTask(i32 id);
	bool DelTask();
	bool HaveTask(i32 id);
	TypeTime GetTimeCur() const;
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
