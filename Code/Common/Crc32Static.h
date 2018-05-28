#ifndef _CRC32STATIC_H_
#define _CRC32STATIC_H_

#include "../inc/CommonDef.h"

#define NO_ERROR 0
#define ERROR_CRC 2
#define MAX_BUFFER_SIZE	4096

class CCrc32Static
{
public:
	static ui32 StringCrc32(cpstr sString, ui32& dwCrc32);
	static ui32 StringIgnoCrc32(cpstr sString, ui32 ulSize, ui32& dwCrc32);
	static ui32 BufferCrc32(cpvd pBuffer, ui32 size, ui32& dwCrc32);
	static ui32 FileCrc32(cpstr sFilename, ui32& dwCrc32);
	static ui32 FilePathCrc32(cpstr sFilePath, ui32 &dwCrc32);
protected:
	static inline void CalcCrc32(byte c, ui32& dwCrc32);
	static ui32 s_arrdwCrc32Table[256];
};

#endif

