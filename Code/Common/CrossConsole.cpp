#include "../inc/CrossConsole.h"
#include "../inc/CrossString.h"
#include "../inc/CrossMemory.h"
#include "../inc/cThread.h"
#include "../inc/iReference.h"

//包含文件版本区分

#include <stdio.h>
#if (defined(WIN32) || defined(_WIN32))
#	include <windows.h>
#	include <iomanip>
#	include <fstream>
#	include <conio.h>
#else
#	include <stdlib.h>
#	include <stdarg.h>
#endif
#include <iostream>

namespace cm
{

	std::string CrossConsole::GetString() {
		std::string buf;
		std::getline(std::cin, buf);
		return buf;
	}
	class cThreadGetString
	{
	public:
		cThreadGetString() {
			m_pThread = new cThread([this](cThread* p){
				while(!p->IsNeedEnd()) {
					std::string s = CrossConsole::GetString();
					m_mutex.Lock();
					m_sCmd = s;
					m_mutex.Unlock();
					SLEEP(10);
				}
			});
		}
		~cThreadGetString() {
			End();
		}
		void End() {
			if (m_pThread) {
				m_pThread->End(false);
				delete m_pThread;
				m_pThread = nullptr;
			}
		}
		bool GetString(std::string& s) {
			LOCK(m_mutex);
			if (m_sCmd.empty())
				return false;
			s = m_sCmd;
			m_sCmd.clear();
			return true;
		}
		cThread*			m_pThread;
		std::string			m_sCmd;
		MUTEX				m_mutex;
	};
	cThreadGetString& GetThreadGetString() {
		static cThreadGetString s_obj;
		return s_obj;
	}
	bool CrossConsole::GetString(std::string& s) {
		return GetThreadGetString().GetString(s);
	}

	void CrossConsole::Pause() {
		system("pause");
	}

#if (defined(WIN32) || defined(_WIN32))
	typedef ui16 word;
	typedef const word cword;
	typedef ui32 dword;
	namespace JadedHoboConsole
	{
		class con_dev
		{
		private:
			HANDLE						hCon;
			ui32						cCharsWritten;
			CONSOLE_SCREEN_BUFFER_INFO	csbi;
			ui32						dwConSize;
		public:
			static cword bgMask;
			static cword fgMask;
			static cword fgBlack;
			static cword fgLoRed;
			static cword fgLoGreen;
			static cword fgLoBlue;
			static cword fgLoCyan;
			static cword fgLoMagenta;
			static cword fgLoYellow;
			static cword fgLoWhite;
			static cword fgGray;
			static cword fgHiWhite;
			static cword fgHiBlue;
			static cword fgHiGreen;
			static cword fgHiRed;
			static cword fgHiCyan;
			static cword fgHiMagenta;
			static cword fgHiYellow;
			static cword bgBlack;
			static cword bgLoRed;
			static cword bgLoGreen;
			static cword bgLoBlue;
			static cword bgLoCyan;
			static cword bgLoMagenta;
			static cword bgLoYellow;
			static cword bgLoWhite;
			static cword bgGray;
			static cword bgHiWhite;
			static cword bgHiBlue;
			static cword bgHiGreen;
			static cword bgHiRed;
			static cword bgHiCyan;
			static cword bgHiMagenta;
			static cword bgHiYellow;
			con_dev();
			~con_dev();
			void get_handle();
		private:
			void GetInfo();
		public:
			void Clear();
			void SetColor( word wRGBI, word Mask );
			static con_dev& instance();
		};

		cword con_dev::bgMask		( BACKGROUND_BLUE |
										BACKGROUND_GREEN |
										BACKGROUND_RED |
										BACKGROUND_INTENSITY );
		cword con_dev::fgMask		( FOREGROUND_BLUE |
										FOREGROUND_GREEN |
										FOREGROUND_RED |
										FOREGROUND_INTENSITY );

		cword con_dev::fgBlack		( 0 );
		cword con_dev::fgLoRed		( FOREGROUND_RED   );
		cword con_dev::fgLoGreen	( FOREGROUND_GREEN );
		cword con_dev::fgLoBlue		( FOREGROUND_BLUE  );
		cword con_dev::fgLoCyan		( fgLoGreen   | fgLoBlue );
		cword con_dev::fgLoMagenta	( fgLoRed     | fgLoBlue );
		cword con_dev::fgLoYellow	( fgLoRed     | fgLoGreen );
		cword con_dev::fgLoWhite	( fgLoRed     | fgLoGreen | fgLoBlue );
		cword con_dev::fgGray		( fgBlack     | FOREGROUND_INTENSITY );
		cword con_dev::fgHiWhite	( fgLoWhite   | FOREGROUND_INTENSITY );
		cword con_dev::fgHiBlue		( fgLoBlue    | FOREGROUND_INTENSITY );
		cword con_dev::fgHiGreen	( fgLoGreen   | FOREGROUND_INTENSITY );
		cword con_dev::fgHiRed		( fgLoRed     | FOREGROUND_INTENSITY );
		cword con_dev::fgHiCyan		( fgLoCyan    | FOREGROUND_INTENSITY );
		cword con_dev::fgHiMagenta	( fgLoMagenta | FOREGROUND_INTENSITY );
		cword con_dev::fgHiYellow	( fgLoYellow  | FOREGROUND_INTENSITY );
		cword con_dev::bgBlack		( 0 );
		cword con_dev::bgLoRed		( BACKGROUND_RED );
		cword con_dev::bgLoGreen	( BACKGROUND_GREEN );
		cword con_dev::bgLoBlue		( BACKGROUND_BLUE );
		cword con_dev::bgLoCyan		( bgLoGreen   | bgLoBlue );
		cword con_dev::bgLoMagenta	( bgLoRed     | bgLoBlue );
		cword con_dev::bgLoYellow	( bgLoRed     | bgLoGreen );
		cword con_dev::bgLoWhite	( bgLoRed     | bgLoGreen | bgLoBlue );
		cword con_dev::bgGray		( bgBlack     | BACKGROUND_INTENSITY );
		cword con_dev::bgHiWhite	( bgLoWhite   | BACKGROUND_INTENSITY );
		cword con_dev::bgHiBlue		( bgLoBlue    | BACKGROUND_INTENSITY );
		cword con_dev::bgHiGreen	( bgLoGreen   | BACKGROUND_INTENSITY );
		cword con_dev::bgHiRed		( bgLoRed     | BACKGROUND_INTENSITY );
		cword con_dev::bgHiCyan		( bgLoCyan    | BACKGROUND_INTENSITY );
		cword con_dev::bgHiMagenta	( bgLoMagenta | BACKGROUND_INTENSITY );
		cword con_dev::bgHiYellow	( bgLoYellow  | BACKGROUND_INTENSITY );

		con_dev::con_dev()
		{
			czeromem(&csbi, sizeof(csbi));
			hCon = NULL;
		}
		con_dev::~con_dev()
		{
		}
		void con_dev::get_handle()
		{
			if (hCon == NULL)
				hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		void con_dev::GetInfo()
		{
			get_handle();
			GetConsoleScreenBufferInfo(hCon, &csbi);
			dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		}
		void con_dev::Clear()
		{
			get_handle();
			COORD coordScreen = {0, 0};
			GetInfo();
			FillConsoleOutputCharacter( hCon,
										TEXT(' '),
										dwConSize,
										coordScreen,
										(LPDWORD)&cCharsWritten );
			GetInfo();
			FillConsoleOutputAttribute(	hCon,
										csbi.wAttributes,
										dwConSize,
										coordScreen,
										(LPDWORD)&cCharsWritten );
			SetConsoleCursorPosition(hCon, coordScreen);
		}
		void con_dev::SetColor( word wRGBI, word Mask )
		{
			get_handle();
			GetInfo();
			csbi.wAttributes &= Mask;
			csbi.wAttributes |= wRGBI;
			BOOL b = SetConsoleTextAttribute( (HANDLE)hCon, csbi.wAttributes );
			//if (!b){
			//	int error = GetLastError();
			//	printf("SetColor Error : %d\n", error);
			//	if (error == 6)
			//		get_handle();
			//}
		}
		con_dev& con_dev::instance()
		{
			static con_dev c;
			return c;
		}

	}

	namespace con = JadedHoboConsole;

	void CrossConsole::SetColor(eColor fgCol) {
		std::cout.flush();
		switch(fgCol) {
		case CC_White:
			con::con_dev::instance().SetColor( con::con_dev::fgLoWhite, con::con_dev::bgMask ); break;
		case CC_Red:
			con::con_dev::instance().SetColor( con::con_dev::fgHiRed, con::con_dev::bgMask ); break;
		case CC_Yellow:
			con::con_dev::instance().SetColor( con::con_dev::fgHiYellow, con::con_dev::bgMask ); break;
		case CC_Green:
			con::con_dev::instance().SetColor( con::con_dev::fgHiGreen, con::con_dev::bgMask ); break;
		case CC_Blue:
			con::con_dev::instance().SetColor( con::con_dev::fgHiBlue, con::con_dev::bgMask ); break;
		case CC_Cyan:
			con::con_dev::instance().SetColor( con::con_dev::fgHiCyan, con::con_dev::bgMask ); break;
		default:
			con::con_dev::instance().SetColor( con::con_dev::fgLoWhite, con::con_dev::bgMask ); break;
		}
	}
#else
	void CrossConsole::SetColor(eColor fgCol) {
		return;
	}
#endif

	static bool g_bShowConsole = false;
	bool CrossConsole::IsShow() {
		return g_bShowConsole;
	}
	void CrossConsole::Show() {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		AllocConsole();
		freopen("conin$","r",stdin);
		freopen("conout$","w",stdout);
		freopen("conout$","w",stderr);
		//	std::ios::sync_with_stdio();
		static std::ofstream conout("con");
		if (conout.is_open())
			std::cout.rdbuf(conout.rdbuf());
		COORD coord;
		coord.Y = 6400;
		coord.X = 80;
		SetConsoleScreenBufferSize(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
		g_bShowConsole = true;
#else
#endif
	}
	void CrossConsole::Hide() {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		GetThreadGetString().End();
		if (g_bShowConsole) {
			FreeConsole();
			g_bShowConsole = false;
		}
#else
#endif
	}
	void CrossConsole::Write(i8 ch)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//tell the console to quit...
		HANDLE hInput = GetStdHandle (STD_INPUT_HANDLE);
		INPUT_RECORD    ir[2];
		ir[0].EventType = KEY_EVENT;
		ir[0].Event.KeyEvent.bKeyDown = T32;
		ir[0].Event.KeyEvent.dwControlKeyState = 0;
		ir[0].Event.KeyEvent.uChar.AsciiChar = ch;
		ir[0].Event.KeyEvent.wRepeatCount = 1;
		ir[0].Event.KeyEvent.wVirtualKeyCode = 0;
		ir[0].Event.KeyEvent.wVirtualScanCode = 0;
		ir[1].EventType = KEY_EVENT;
		ir[1].Event.KeyEvent.bKeyDown = F32;
		ir[1].Event.KeyEvent.dwControlKeyState = 0;
		ir[1].Event.KeyEvent.uChar.AsciiChar = ch;
		ir[1].Event.KeyEvent.wRepeatCount = 1;
		ir[1].Event.KeyEvent.wVirtualKeyCode = 0;
		ir[1].Event.KeyEvent.wVirtualScanCode = 0;
		DWORD dwTmp;
		WriteConsoleInput (hInput, ir, 2, &dwTmp);
#else
#endif
	}

}
