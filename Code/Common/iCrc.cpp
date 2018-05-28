#include "../inc/iCrc.h"
#include "Crc32Static.h"

namespace cross
{

bool StringCrc32(cpstr szString, ui32& ulCrc32)
{
	return NO_ERROR == CCrc32Static::StringCrc32(szString, ulCrc32);
}

bool StringIgnoCrc32(cpstr szFilePath, ui32 ulSize, ui32& ulCrc32)
{
	return NO_ERROR == CCrc32Static::StringIgnoCrc32(szFilePath, ulSize, ulCrc32);
}

bool BufferCrc32(cpvd pBuffer, ui32 ulBufSize, ui32& ulCrc32)
{
	return NO_ERROR == CCrc32Static::BufferCrc32(pBuffer, ulBufSize, ulCrc32);
}

bool FileCrc32(cpstr szFilename, ui32& ulCrc32)
{
	return NO_ERROR == CCrc32Static::FileCrc32(szFilename, ulCrc32);
}

bool FilePathCrc32(cpstr szFilePath, ui32& ulCrc32)
{
	return NO_ERROR == CCrc32Static::FilePathCrc32(szFilePath, ulCrc32);
}

}