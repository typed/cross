#ifndef cclock_h
#define cclock_h

#include "CommonDef.h"
#include "cGuid.h"
#include <string>
//#include <string.h>

namespace cross
{

class cClock;
COMMON_API cClock* GetClock();
COMMON_API TypeTime GetCurTime();

//时钟
class COMMON_API cClock
{
public:

	~cClock();

	bool IsRunning() const { return m_running; }
	void Run();
	void Stop();

	void SetSmoothTime(bool b) {m_bSmoothTime = b;}
	void SetTimePreframe(f64 d) {m_dPreframe = d;}

	//时间运行
	void UpdateOnce();

	//运行经过的总时间，可以停止和重置
	TypeTime GetTimeCur() const { return m_tmVirtalTimeCount; }
	TypeTime GetTimeEslasped() const { return m_tmVirtalTimeEslasped; }

	//时钟运行速度
	bool SetTimeSpeed(f64 d) {
		if (d <= 0.f)
			return false;
		m_dSpeed = d;
		return true;
	}
	f64 GetTimeSpeed() const { return m_dSpeed; }

private:

	cClock();

	TypeTime GetCurTimeReal();

	friend COMMON_API cClock* GetClock();
	friend COMMON_API TypeTime GetCurTime();

	bool		m_running;

	TypeTime	m_tmRealTimeInit;
	TypeTime	m_tmRealTimeCur;
	TypeTime	m_tmRealTimeLast;
	TypeTime	m_tmRealTimeEslasped;
	TypeTime	m_tmRealTimeCount;

	TypeTime	m_tmVirtalTimeEslasped;
	TypeTime	m_tmVirtalTimeCount;

	f64			m_dSpeed;

	bool		m_bSmoothTime;
	f64			m_dPreframe;
	f64			m_dSmoothSum;
	f64			m_dSmoothPreframe;

};

class COMMON_API cDate
{
public:

	cDate();
	cDate(TypeDate n);
	cDate(f64 n);
	cDate(const cDate& date);

	cDate& operator = (const cDate& date);
	bool operator == (const cDate& date) const;
	bool operator != (const cDate& date) const;

	bool operator > (const cDate& date) const;
	bool operator < (const cDate& date) const;
	bool operator >= (const cDate& date) const;
	bool operator <= (const cDate& date) const;

	void SetTime(TypeDate tm);
	TypeDate GetTime() const {return m_lTime;}

	//get win32 DATE
	f64 GetDATE() const;
	void SetDATE(f64 d);

	void SetDateTime(const cDate& date, const cDate& time);
	void SetDateTime(i32 year, i32 month, i32 day, i32 hour, i32 minute, i32 second);
	void SetDateTimeString(cpstr);
	void SetTime(i32 hour, i32 minute, i32 second);
	void SetTimeString(cpstr);

	//中文显示
	std::string GetTimeStringC();

	std::string GetTimeString();
	//显示格式：xx:xx:xx(时:分:秒)
	std::string GetTimeStringEx(bool b24 = false);
	//显示格式：xx:xx(时:分)
	std::string GetHourMinuteString(bool b24 = false);
	std::string GetDateString();
	std::string GetDateTimeString();
	i32 GetDateI64() const {return m_year * 10000 + m_month * 100 + m_mday;}

	i32	GetYear() const {return m_year;}
	i32	GetMonth() const {return m_month;}
	i32	GetDay() const {return m_mday;}
	i32	GetHour() const {return m_hour;}
	i32	GetMinute() const {return m_minute;}
	i32	GetSecond() const {return m_second;}
	i32	GetWeek() const {return m_wday;}

	i32	GetDateInt() const {return GetYear()*10000 + GetMonth()*100 + GetDay();}

	void GotoTomorrow0();

	bool IsSameDate(const cDate& dt);
	bool IsSameMonth(const cDate& dt);

	//获得当前消耗的所有分钟数 add by yfw
	int GetAllMinuteTheDay() const {return m_hour*60 + m_minute;}

	static cDate GetCurDate();
	static TypeDate GetCurDateTime();

protected:
	void MakeTime();
	TypeDate	m_lTime;
	i32			m_year;
	i32			m_month;
	i32			m_mday;
	i32			m_wday;
	i32			m_hour;
	i32			m_minute;
	i32			m_second;
};

COMMON_API TypeTime GetCheckNowTime();
COMMON_API TypeTime GetPerformanceTime();
COMMON_API ui64 GetCurLongTime();	
COMMON_API ui32 BeginCheckTime();
COMMON_API TypeTime EndCheckTime(ui32 id);
COMMON_API void BeginCheckTime(cpstr name);
COMMON_API TypeTime EndCheckTime(cpstr name);
COMMON_API void PrintEndCheckTime(cpstr name, TypeTime lessNotDisplay = 0);
COMMON_API void MsgBoxEndCheckTime(cpstr name, TypeTime lessNotDisplay = 0);
#define BeginCheckTimeCurFunc cm::BeginCheckTime(__FUNCTION__);
#define EndCheckTimeCurFunc cm::EndCheckTime(__FUNCTION__);
#define PrintEndCheckTimeCurFunc cm::PrintEndCheckTime(__FUNCTION__);
#define MsgBoxEndCheckTimeCurFunc cm::MsgBoxEndCheckTime(__FUNCTION__);

#define DateTime cm::cDate::GetCurDateTime()

inline std::string GetCurTimeString() {
	return cDate::GetCurDate().GetTimeString();
}

inline std::string GetCurDateString() {
	return cDate::GetCurDate().GetDateString();
}

inline std::string GetCurDateTimeString() {
	return cDate::GetCurDate().GetDateTimeString();
}

inline f64 GetCurDateDbl() {
	return cDate::GetCurDate().GetDATE();
}

}
#endif
