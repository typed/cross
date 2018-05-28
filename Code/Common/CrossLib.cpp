#include "CrossLib.h"

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#if (defined(WIN32) || defined(_WIN32))
#	include <windows.h>
#	define DYNLIB_HANDLE hInstance
#	define DYNLIB_LOAD( a ) LoadLibraryA( a )
#	define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#	define DYNLIB_UNLOAD( a ) FreeLibrary( a )
#	define DYNLIB_ERROR( )  "Unknown Error"

#elif defined(__linux__)
#	define DYNLIB_HANDLE void*
#	define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#	define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#	define DYNLIB_UNLOAD( a ) dlclose( a )
#	define DYNLIB_ERROR( ) dlerror( )

#elif defined(__APPLE__)
#	define DYNLIB_HANDLE CFBundleRef
#	define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#	define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#	define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#	define DYNLIB_ERROR( ) mac_errorBundle()

#endif

namespace cross
{
#if (defined(WIN32) || defined(_WIN32))
	pvd CrossLib::LoadLib(cpstr sLib) {
		DYNLIB_HANDLE* p = new DYNLIB_HANDLE;
		*p = DYNLIB_LOAD(sLib);
		if (*p)
			return p;
		delete p;
		return 0;
	}
	pvd CrossLib::GetSym(pvd h, cpstr funcname) {
		DYNLIB_HANDLE* p = (DYNLIB_HANDLE*)h;
		return (pvd)DYNLIB_GETSYM(*p, funcname);
	}
	void CrossLib::UnloadLib(pvd h) {
		DYNLIB_HANDLE* p = (DYNLIB_HANDLE*)h;
		DYNLIB_UNLOAD(*p);
		delete p;
	}
#else
    pvd CrossLib::LoadLib(cpstr sLib) {
		return 0;
	}
	pvd CrossLib::GetSym(pvd h, cpstr funcname) {
		return 0;
	}
	void CrossLib::UnloadLib(pvd h) {
	}
#endif
}
