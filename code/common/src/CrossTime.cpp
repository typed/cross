#include "../inc/CrossTime.h"

//包含文件版本区分

#include <stdio.h>
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
#	include <windows.h>
#	include <time.h>
#elif defined(ANDROID)
#	include <time.h>
#else
#	include <sys/time.h>
#endif

using namespace std;

namespace cm
{
	//时钟
	TypeDate CrossTime::GetDate() {
		return (TypeDate)time(0);
	}

#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
	class cPerformanceClock
	{
	public:
		cPerformanceClock() {
			QueryPerformanceFrequency(&m_initlv);
			ui64 init = m_initlv.QuadPart * 1000 / m_initlv.QuadPart;
			LARGE_INTEGER lv;
			QueryPerformanceCounter( &lv );
			m_init = (lv.QuadPart * 1000 / m_initlv.QuadPart) - init;
		}
		TypeTime GetTime() {
			LARGE_INTEGER lv;
			QueryPerformanceCounter( &lv );
			return (lv.QuadPart * 1000 / m_initlv.QuadPart) - m_init;
		}
	private:
		LARGE_INTEGER		m_initlv;
		ui64				m_init;
	};
	cPerformanceClock& GetPerformanceClock() {
		static cPerformanceClock s_pc;
		return s_pc;
	}
	//CPU时间
	TypeTime CrossTime::GetCPUTime() {
		return GetPerformanceClock().GetTime();
	}
#else
	//CPU时间
	TypeTime CrossTime::GetCPUTime() {
		timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_usec / 1000 + tv.tv_sec * 1000;
	}
#endif

	TypeDate CrossTime::MkTime(i32 year, i32 month, i32 day, i32 hour, i32 minute, i32 second) {
		tm t;
		t.tm_year = year;
		t.tm_mon = month;
		t.tm_mday = day;
		t.tm_hour = hour;
		t.tm_min = minute;
		t.tm_sec = second;
		t.tm_year -= 1900;
		t.tm_mon--;
		t.tm_isdst = 0;
		return (TypeDate)mktime(&t);
	}

	bool CrossTime::LocalTime(TypeDate tm1, i32& year, i32& month, i32& day, i32& wday, i32& hour, i32& minute, i32& second) {
		time_t tmt = tm1;
		tm* ptm = localtime(&tmt);
		if (ptm) {
			year = 1900 + ptm->tm_year;
			month = 1 + ptm->tm_mon;
			day = ptm->tm_mday;
			wday = ptm->tm_wday;
			hour = ptm->tm_hour;
			minute = ptm->tm_min;
			second = ptm->tm_sec;
			return true;
		}
		else
			return false;
	}

	void CrossTime::FormatStringToTime(const string& stm, i32& year, i32& month, i32& day, i32& hour, i32& minute, i32& second) {
		sscanf(stm.c_str(), "%04d_%02d_%02d-%02d.%02d.%02d", &year, &month, &day, &hour, &minute, &second);
	}

	TypeDate CrossTime::VariantTimeToSystemTime(f64 d) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		SYSTEMTIME timeDest;
		::VariantTimeToSystemTime(d, &timeDest);
		tm tm1;
		tm1.tm_year = timeDest.wYear - 1900;
		tm1.tm_mon = timeDest.wMonth - 1;
		tm1.tm_wday = timeDest.wDayOfWeek;
		tm1.tm_mday = timeDest.wDay;
		tm1.tm_hour = timeDest.wHour;
		tm1.tm_min = timeDest.wMinute;
		tm1.tm_sec = timeDest.wSecond;
		tm1.tm_isdst = 0;
		return (TypeDate)mktime(&tm1);
#else
		return 0;
#endif
	}
	f64 CrossTime::SystemTimeToVariantTime(TypeDate tm) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		SYSTEMTIME timeDest;
		i32 year, month, day, wday, hour, minute, second;
		if (LocalTime(tm, year, month, day, wday, hour, minute, second)) {
			timeDest.wYear = (WORD) year;
			timeDest.wMonth = (WORD) month;
			timeDest.wDayOfWeek = (WORD) wday;
			timeDest.wDay = (WORD) day;
			timeDest.wHour = (WORD) hour;
			timeDest.wMinute = (WORD) minute;
			timeDest.wSecond = (WORD) second;
			timeDest.wMilliseconds = 0;
			f64 d = 0;
			::SystemTimeToVariantTime(&timeDest, &d);
			return d;
		}
		else
			return 0;
#else
		return 0;
#endif
	}

}
