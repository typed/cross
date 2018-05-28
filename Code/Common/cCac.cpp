#include "cCac.h"
#include "../inc/math/Random.h"

namespace cross
{

iCac* iCac::Create()
{
	return new cCac();
}

cCac::cCac()
{
	
}

cCac::~cCac()
{

}

bool cCac::LoadFromFile(const std::string& s)
{
	std::ifstream ifs;
	ifs.open(s, std::ios::in | std::ios::binary);
	if (!ifs.is_open())
		return false;
	std::ifstream::pos_type old = ifs.tellg();
	ifs.seekg(0, std::ios_base::end);
	ui32 len = (ui32)ifs.tellg();
	ifs.seekg(old, std::ios_base::beg);
	std::string buf;
	buf.resize(len);
	ifs.read((i8*)buf.data(), (ui32)buf.size());
	return Load(buf);
}

bool cCac::Load(const std::string& s)
{
	return Load(s.data(), (ui32)s.size());
}

bool cCac::Load(cpvd buf, ui32 len)
{
	cpi8 pBuf = (cpi8)buf;
	stCacHeader info;
	czeromem(&info, sizeof(info));
	cmemcpy(&info, pBuf, sizeof(info));
	if (info.name != 'CAC0')
		return false;
	pBuf += sizeof(info);
	std::string data;
	ui32 srclen = info.src_size;
	data.resize(srclen);
	i8* pDes = (i8*)data.data();
	if (C_OK != Decompress(pBuf, info.compress_size, pDes, srclen, CT_Qlz))
		return false;
	cIStream is(data);
	is >> m_mapCac;
	return true;
}

std::string cCac::Save()
{
	std::string des;
	cOStream1M os;
	os << m_mapCac;
	stCacHeader info;
	czeromem(&info, sizeof(info));
	info.src_size = os.GetDataSize();
	std::string data;
	ui32 deslen = GetCompressSized(os.GetData(), os.GetDataSize(), CT_Qlz);
	data.resize(deslen);
	i8* pDes = (i8*)data.data();
	if (C_OK != Compress(os.GetData(), os.GetDataSize(), pDes, deslen, CT_Qlz))
		return des;
	info.compress_size = deslen;
	info.name = 'CAC0';
	for (i32 i = 0; i < sizeof(info.ext); i++)
		info.ext[i] = (i8)(RandomAInt() % 0xff);
	des.resize(deslen + sizeof(info));
	i8* pOut = (i8*)des.data();
	cmemcpy(pOut, &info, sizeof(info));
	pOut += sizeof(info);
	cmemcpy(pOut, pDes, deslen);
	return des;
}

void cCac::Set(std::string vKey, std::string vValue)
{
	m_mapCac[vKey] = vValue;
}

bool cCac::Get(std::string vKey, std::string& vValue)
{
	mapCac::iterator it = m_mapCac.find(vKey);
	if (it == m_mapCac.end())
		return false;
	vValue = it->second;
	return true;
}

bool cCac::Del(std::string vKey)
{
	mapCac::iterator it = m_mapCac.find(vKey);
	if (it == m_mapCac.end())
		return false;
	m_mapCac.erase(it);
	return true;
}

void cCac::SetUI32(ui32 vKey, std::string vValue)
{
	std::string key;
	key.resize(sizeof(vKey));
	cmemcpy((i8*)key.data(), &vKey, sizeof(vKey));
	Set(key, vValue);
}

bool cCac::GetUI32(ui32 vKey, std::string& vValue)
{
	std::string key;
	key.resize(sizeof(vKey));
	cmemcpy((i8*)key.data(), &vKey, sizeof(vKey));
	return Get(key, vValue);
}

bool cCac::DelUI32(ui32 vKey)
{
	std::string key;
	key.resize(sizeof(vKey));
	cmemcpy((i8*)key.data(), &vKey, sizeof(vKey));
	return Del(key);
}

}