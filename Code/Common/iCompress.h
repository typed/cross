#ifndef icompress_h
#define icompress_h

#include "CommonDef.h"
#include <vector>
#include <string>

namespace cross
{

enum eCompressType
{
	CT_Zlib,
	CT_Qlz
};

enum eCompress
{
	C_OK,
	C_BufferError,
	C_TypeError,
};

COMMON_API cpstr GetCompressErrString(eCompress);

//得到压缩后目标buffer的最大size
COMMON_API ui32 GetCompressSized(cpvd pSrc, ui32 iSrc, eCompressType t = CT_Qlz);

//得到解压后目标buffer的最大size
COMMON_API ui32 GetDecompressSized(cpvd pSrc, ui32 iSrc, eCompressType t = CT_Qlz);

COMMON_API eCompress Compress(cpvd pSrc, ui32 iSrc, pvd pDes, ui32& iDes, eCompressType t = CT_Qlz);

COMMON_API eCompress Decompress(cpvd pSrc, ui32 iSrc, pvd pDes, ui32& iDes, eCompressType t = CT_Qlz);

inline eCompress Compress(const std::vector<i8>& src, std::vector<i8>& des, eCompressType t = CT_Qlz)
{
	ui32 lenCom = GetCompressSized(&src.front(), (ui32)src.size());
	des.resize(lenCom);
	eCompress e = Compress(&src.front(), (ui32)src.size(), &des.front(), lenCom, t);
	if (e != C_OK)
		des.clear();
	else
		des.resize(lenCom);
	return e;
}

inline eCompress Decompress(const std::vector<i8>& src, std::vector<i8>& des, eCompressType t = CT_Qlz)
{
	ui32 lenDecom = GetDecompressSized(&src.front(), (ui32)src.size());
	des.resize(lenDecom);
	eCompress e = Decompress(&src.front(), (ui32)src.size(), &des.front(), lenDecom, t);
	if (e != C_OK)
		des.clear();
	else
		des.resize(lenDecom);
	return e;
}

inline eCompress Compress(const std::string& src, std::string& des, eCompressType t = CT_Qlz)
{
	ui32 lenCom = GetCompressSized(src.data(), (ui32)src.size());
	des.resize(lenCom);
	eCompress e = Compress(src.data(), (ui32)src.size(), (i8*)des.data(), lenCom, t);
	if (e != C_OK)
		des.clear();
	else
		des.resize(lenCom);
	return e;
}

inline eCompress Decompress(const std::string& src, std::string& des, eCompressType t = CT_Qlz)
{
	ui32 lenDecom = des.size();
	eCompress e = Decompress(src.data(), (ui32)src.size(), (i8*)des.data(), lenDecom, t);
	if (e != C_OK)
		des.clear();
	else
		des.resize(lenDecom);
	return e;
}

}
#endif
