#ifndef icrc_h
#define icrc_h

#include "CommonDef.h"

namespace cross
{

COMMON_API bool StringCrc32(cpstr szString, ui32& ulCrc32);
COMMON_API bool StringIgnoCrc32(cpstr szString, ui32 ulBufSize, ui32& ulCrc32);
COMMON_API bool BufferCrc32(cpvd pBuffer, ui32 ulBufSize, ui32& ulCrc32);
COMMON_API bool FileCrc32(cpstr szFilename, ui32& ulCrc32);
COMMON_API bool FilePathCrc32(cpstr szFilePath, ui32& ulCrc32);

inline ui32 FilePathCrc32Easy(cpstr szFilePath) {
	ui32 crc = 0;
	FilePathCrc32(szFilePath, crc);
	return crc;
}

inline ui32 StringIgnoCrc32Easy(cpstr szString, ui32 ulSize) {
	ui32 crc = 0;
	StringIgnoCrc32(szString, ulSize, crc);
	return crc;
}

inline ui32 StringCrc32Easy(cpstr szString) {
	ui32 crc = 0;
	StringCrc32(szString, crc);
	return crc;
}

inline ui32 BufferCrc32Easy(cpvd pBuffer, ui32 ulBufSize) {
	ui32 crc = 0;
	BufferCrc32(pBuffer, ulBufSize, crc);
	return crc;
}

}
#endif
