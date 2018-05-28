#include "cGuid.h"
#include "Math/Random.h"
#include "iCrc.h"
#include "CrossString.h"

namespace cross
{

cGuid::cGuid()
{
	data[0] = RandomAUnsignedInt();
	data[1] = RandomAUnsignedInt();
	data[2] = RandomAUnsignedInt();
	data[3] = RandomAUnsignedInt();
}

cGuid::cGuid(ui32 data1, ui32 data2, ui32 data3, ui32 data4)
{
	data[0] = data1;
	data[1] = data2;
	data[2] = data3;
	data[3] = data4;
}

cGuid::cGuid(const cGuid& guid)
{
	*this = guid;
}

cGuid& cGuid::operator = (const cGuid& guid)
{
	data[0] = guid.data[0];
	data[1] = guid.data[1];
	data[2] = guid.data[2];
	data[3] = guid.data[3];
	return *this;
}

bool cGuid::operator == (const cGuid& guid) const
{
	return data[0] == guid.data[0] &&
		data[1] == guid.data[1] &&
		data[2] == guid.data[2] &&
		data[3] == guid.data[3];
}

bool cGuid::operator == (cpstr sguid) const
{
	return GetString() == sguid;
}

unsigned int cGuid::ToUint() const
{
	ui32 crc = RandomAUnsignedInt();
	BufferCrc32((cpbyte)data, sizeof(data), crc);
	return crc;
}

cGuid::operator ui32() const
{
	return ToUint();
}

bool cGuid::operator < (const cGuid& guid) const
{
	return data[0] < guid.data[0];
}

std::string cGuid::GetString() const
{
	std::string s;
	for (int i = 0; i < 4; i++)
		s += CrossString::UIntTo016(data[i]);
	return s;
}

void cGuid::SetString(std::string s)
{
	for (int i = 0; i < 4; i++)
		data[i] = CrossString::_16ToUInt(s.substr(i * 8, 8));
}

void cGuid::Set0()
{
	memset(data, 0, sizeof(data));
}

bool cGuid::Is0()
{
	cGuid guid(0,0,0,0);
	return operator == (guid);
}

bool operator == (unsigned int v, const cGuid& guid)
{
	return v == guid.ToUint();
}

bool operator != (unsigned int v, const cGuid& guid)
{
	return v != guid.ToUint();
}

#include <string>
#include <string.h>
#include <stdio.h>
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
#	include <objbase.h>
#elif (defined(__linux__) && !defined(ANDROID))
#include <uuid/uuid.h>
#endif
std::string NewGUID()
{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
	GUID guid;
	CoCreateGuid(&guid);
	char buf[256] = {0};
	sprintf(
		buf,
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return std::string(buf);
#elif (defined(__linux__) && !defined(ANDROID))
	uuid_t guid;
	uuid_generate(guid);
	char buf[256] = {0};
	sprintf(
	buf,
	"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
	guid[0],guid[1],guid[2],guid[3],guid[4],guid[5],guid[6],guid[7],\
	guid[8],guid[9],guid[10],guid[11],guid[12],guid[13],guid[14],guid[15]);
	
	return std::string(buf);
#else
	ui32 n1 = RandomAUnsignedInt();
	ui32 n2 = RandomAUnsignedInt();
	ui32 n3 = RandomAUnsignedInt();
	ui32 n4 = RandomAUnsignedInt();
	char buf[64] = {0};
	sprintf(
	buf,
	"%08X-%04X-%04X-%04X-%04X%08X",
	n1, (n2>>16), (n2&0xffff),
	(n3>>16), (n3&0xffff),
	n4);
	return std::string(buf);
	//cGuid guid;
	//return guid.GetString();
#endif
}

}
