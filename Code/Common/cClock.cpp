#include "cClock.h"
#include "iCrc.h"
#include "CrossTime.h"
#include "CrossString.h"
#include "iLog.h"
#include "cGuid.h"
#include <map>

using namespace std;

namespace cross
{

inline TypeTime _GetCurTime()
{
	return CrossTime::GetCPUTime();
}

TypeTime GetCurTime()
{
	return GetClock()->GetTimeCur();//GetCurTimeReal();
}

//时钟
//初始时间
cClock::cClock()
{
	m_tmRealTimeInit = _GetCurTime();
	m_tmRealTimeCur = 0;
	m_tmRealTimeLast = 0;
	m_tmRealTimeEslasped = 0;
	m_tmRealTimeCount = 0;
	m_tmVirtalTimeEslasped = 0;
	m_tmVirtalTimeCount = 0;
	m_dSpeed = 1.f;
	m_dPreframe = 1000.0/60.0;
	m_dSmoothSum = 0;
	m_dSmoothPreframe = 0;
	m_tmVirtalTimeCount = 0;
	m_running = true;
	m_bSmoothTime = false;
}

cClock::~cClock()
{
}

void cClock::Run()
{
	m_running = true;
}

void cClock::Stop()
{
	m_running = false;
}

TypeTime cClock::GetCurTimeReal()
{
	return (TypeTime) ( ( _GetCurTime() - m_tmRealTimeInit ) * m_dSpeed + 0.5 );
}

void cClock::UpdateOnce()
{
	//真实时间
	m_tmRealTimeCur = _GetCurTime();
	if (m_tmRealTimeLast == 0)
		m_tmRealTimeLast = m_tmRealTimeCur;
	if (m_tmRealTimeLast > m_tmRealTimeCur)
		m_tmRealTimeEslasped = 0;
	else {
		m_tmRealTimeEslasped = m_tmRealTimeCur - m_tmRealTimeLast;
		m_tmRealTimeLast = m_tmRealTimeCur;
	}
	m_tmRealTimeCount += m_tmRealTimeEslasped;
	//if (m_bSmoothTime) {
	//	//f64 tt = 0;
	//	if (m_tmRealTimeEslasped > m_dPreframe)
	//		//tt = m_tmRealTimeEslasped - m_dPreframe;
	//		m_dSmoothSum += m_tmRealTimeEslasped - m_dPreframe;
	//	m_dSmoothPreframe = m_dPreframe + ( m_dSmoothSum / 2.0 );
	//	m_dSmoothSum -= m_dSmoothPreframe;
	//	if (m_dSmoothSum < 0.0)
	//		m_dSmoothSum = 0.0;
	//	//虚拟逻辑时间
	//	m_tmVirtalTimeEslasped = m_dSmoothPreframe * m_dSpeed + 0.5;
	//}
	//else
		m_tmVirtalTimeEslasped = (TypeTime) ( m_tmRealTimeEslasped * m_dSpeed + 0.5 );
	if (m_running)
		m_tmVirtalTimeCount += m_tmVirtalTimeEslasped;
}

cClock* GetClock()
{
	static cClock s_clock;
	return &s_clock;
}

inline map<ui32, TypeTime>& GetCheckTimeHm()
{
	static map<ui32, TypeTime> s_hmCheckTime;
	return s_hmCheckTime;
}

TypeTime GetCheckNowTime()
{
	return _GetCurTime();
}

TypeTime GetPerformanceTime()
{
	return _GetCurTime();
}

ui32 BeginCheckTime()
{
	cross::cGuid guid;
	ui32 id = guid.ToUint();
	GetCheckTimeHm()[id] = _GetCurTime();
	return id;
}

TypeTime EndCheckTime(ui32 id)
{
	map<ui32, TypeTime>::iterator it = GetCheckTimeHm().find(id);
	if (it == GetCheckTimeHm().end())
		return 0;
	TypeTime tt = it->second;
	GetCheckTimeHm().erase(it);
	return _GetCurTime() - tt;
}

void BeginCheckTime(cpstr name)
{
	ui32 crc = 0;
	StringCrc32(name, crc);
	GetCheckTimeHm()[crc] = _GetCurTime();
}

TypeTime EndCheckTime(cpstr name)
{
	ui32 crc = 0;
	StringCrc32(name, crc);
	return EndCheckTime(crc);
}

void PrintEndCheckTime(cpstr name, TypeTime lessNotDisplay)
{
	TypeTime en = EndCheckTime(name);
	if (en >= lessNotDisplay)
		LInfo << name << " cost time: " << en << "ms." << LogEnter;
}

void MsgBoxEndCheckTime(cpstr name, TypeTime lessNotDisplay)
{
	TypeTime en = EndCheckTime(name);
	if (en >= lessNotDisplay)
		CrossApp::MsgBox(CrossString::Format("%s const time: %f ms.", name, en));
}

//=========================================================================================================================================//
//date
//=========================================================================================================================================//

cDate::cDate() : m_lTime(0), m_year(0), m_month(0), m_mday(0), m_wday(0), m_hour(0), m_minute(0), m_second(0)
{
}

cDate::cDate(TypeDate n) : m_lTime(n)
{
	MakeTime();
}

cDate::cDate(f64 n)
{
	SetDATE(n);
}

cDate::cDate(const cDate& date)
{
	m_lTime = date.m_lTime;
	MakeTime();
}
	
cDate& cDate::operator = (const cDate& date)
{
	m_lTime = date.m_lTime;
	MakeTime();
	return *this;
}

bool cDate::operator == (const cDate& date) const
{
	return m_lTime == date.m_lTime;
}

bool cDate::operator != (const cDate& date) const
{
	return !operator == (date);
}

bool cDate::operator > (const cDate& date) const
{
	return m_lTime > date.m_lTime;
}

bool cDate::operator < (const cDate& date) const
{
	return m_lTime < date.m_lTime;
}

bool cDate::operator >= (const cDate& date) const
{
	return m_lTime >= date.m_lTime;
}

bool cDate::operator <= (const cDate& date) const
{
	return m_lTime <= date.m_lTime;
}

void cDate::SetTime(TypeDate tm)
{
	m_lTime = tm;
	MakeTime();
}

void cDate::SetDATE(f64 d)
{
	m_lTime = CrossTime::VariantTimeToSystemTime(d);
	MakeTime();
}

f64 cDate::GetDATE() const
{
	return CrossTime::SystemTimeToVariantTime(m_lTime);
}

string cDate::GetTimeString()
{
	return CrossString::Format("%02d.%02d.%02d", m_hour, m_minute, m_second);
}

string cDate::GetTimeStringEx(bool b24)
{
	if (b24) {
		i32 nHour = (m_hour == 0) ? 24 : m_hour;
		return CrossString::Format("%02d:%02d:%02d", nHour, m_minute, m_second);
	}
	else
		return CrossString::Format("%02d:%02d:%02d", m_hour, m_minute, m_second);
}

string cDate::GetHourMinuteString(bool b24)
{
	//显示格式：xx:xx(时:分),不显示秒
	if (b24) {
		i32 nHour = (m_hour == 0) ? 24 : m_hour;
		return CrossString::Format("%02d:%02d", nHour, m_minute);
	}
	else
		return CrossString::Format("%02d:%02d", m_hour, m_minute);
}

string cDate::GetTimeStringC()
{
	return CrossString::Format("%02d.%02d.%02d", m_hour, m_minute, m_second);
}

string cDate::GetDateString()
{
	return CrossString::Format("%04d_%02d_%02d", m_year, m_month, m_mday);
}

string cDate::GetDateTimeString()
{
	return (string)GetDateString() + "-" + GetTimeString();
}

void cDate::SetDateTime(const cDate& date, const cDate& time)
{
	SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
}

void cDate::SetDateTime(i32 year, i32 month, i32 day, i32 hour, i32 minute, i32 second)
{
	m_lTime = CrossTime::MkTime(year, month, day, hour, minute, second);
	MakeTime();
}

void cDate::SetDateTimeString(cpstr s)
{
	i32 year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	CrossTime::FormatStringToTime(s, year, month, day, hour, minute, second);
	SetDateTime(year, month, day, hour, minute, second);
}

void cDate::SetTime(i32 hour, i32 minute, i32 second)
{
	cDate curdt = GetCurDate();
	SetDateTime(curdt.GetYear(), curdt.GetMonth(), curdt.GetDay(), hour, minute, second);
}

void cDate::SetTimeString(cpstr s)
{
	cDate curdt = GetCurDate();
	string s1 = curdt.GetDateString() + "-" + s;
	SetDateTimeString(s1.c_str());
}

void cDate::GotoTomorrow0()
{
	SetDateTime(m_year, m_month, m_mday + 1, 0, 0, 0);
}

bool cDate::IsSameDate(const cDate& dt)
{
	return GetYear() == dt.GetYear() && GetMonth() == dt.GetMonth() && GetDay() == dt.GetDay();
}

bool cDate::IsSameMonth(const cDate& dt)
{
	return GetYear() == dt.GetYear() && GetMonth() == dt.GetMonth();
}

void cDate::MakeTime()
{
	if (!CrossTime::LocalTime(m_lTime, m_year, m_month, m_mday, m_wday, m_hour, m_minute, m_second)) {
		m_lTime = 0;
		CrossTime::LocalTime(m_lTime, m_year, m_month, m_mday, m_wday, m_hour, m_minute, m_second);
	}
}

cDate cDate::GetCurDate()
{
	return cDate(GetCurDateTime());
}

TypeDate cDate::GetCurDateTime()
{
	return CrossTime::GetDate();
}

}