
#include "../inc/iCompress.h"
#include "quicklz.h"
#include "../inc/cThread.h"

namespace cross
{

MUTEX& GetMutex()
{
	static MUTEX s_mutex;
	return s_mutex;
}

cpstr GetCompressErrString(eCompress e)
{
	switch(e)
	{
	case C_OK:
		return "Ok";
	case C_BufferError:
		return "BufferError";
	case C_TypeError:
		return "TypeError";
	}
	return "Unknow";
}

ui32 GetCompressSized(cpvd pSrc, ui32 iSrc, eCompressType t)
{
	switch(t)
	{
	case CT_Zlib:
		{
			return iSrc + 400;
		}
	case CT_Qlz:
		{
			//size_t t = qlz_size_compressed((cpstr)pSrc);
			// allocate "uncompressed size" + 400 for the destination buffer
			return iSrc + 400;
		}
	}
	return 0;
}

ui32 GetDecompressSized(cpvd pSrc, ui32 iSrc, eCompressType t)
{
	switch(t)
	{
	case CT_Zlib:
		{
			return iSrc + 400;
		}
	case CT_Qlz:
		{
			return (ui32)qlz_size_decompressed((cpstr)pSrc);
		}
	}
	return 0;
}

eCompress Compress(cpvd pSrc, ui32 iSrc, pvd pDes, ui32& iDes, eCompressType t)
{
	LOCK(GetMutex());
	switch(t)
	{
	case CT_Zlib:
		{
			//i32 err = compress((Bytef*)pDes, (uLongf*)&iDes, (const Bytef*)pSrc, iSrc);
			//if (err != 0)
				return C_BufferError;
			//return C_OK;
		}
	case CT_Qlz:
		{
			static qlz_state_compress qs;
			size_t sz = qlz_compress(pSrc, (char*)pDes, iSrc, &qs);
			if (sz == 0)
				return C_BufferError;
			iDes = (ui32)sz;
			return C_OK;
		}
	}
	return C_TypeError;
}

eCompress Decompress(cpvd pSrc, ui32 iSrc, pvd pDes, ui32& iDes, eCompressType t)
{
	LOCK(GetMutex());
	switch(t)
	{
	case CT_Zlib:
		{
			//i32 err = uncompress((Bytef*)pDes, (uLongf*)&iDes, (const Bytef*)pSrc, iSrc);
			//if (err != 0)
				return C_BufferError;
			//return C_OK;
		}
	case CT_Qlz:
		{
			qlz_state_decompress qs;
			size_t sz = qlz_decompress((cpstr)pSrc, pDes, &qs);
			if (sz == 0)
				return C_BufferError;
			iDes = (ui32)sz;
			return C_OK;
		}
	}
	return C_TypeError;
}

}