#ifndef istream_h
#define istream_h

#include "CommonDef.h"
#include "iLog.h"
#include "CrossMemory.h"
#include "CrossString.h"
#include "cGuid.h"
#include <string>
#include <fstream>

//不是线程安全的

namespace cm
{

static cpstr g_sStreamError = "iStream";

inline iLog& StreamErrorLog()
{
	return LogError(g_sStreamError);
}

static const ui32 c_uiDefaultBlock = 1024;

class cOStream
{
public:

	cOStream(ui32 uiBlk = c_uiDefaultBlock) :
		m_pData(0), m_nDataSize(0), m_nDataOffset(0),
		m_nDataSizeTemp(0), m_uiBlock(uiBlk),
		m_bOk(true) {}
	~cOStream() {
		delete[] m_pData;
	}
	cOStream(const cOStream& out) : m_pData(0), m_nDataSize(0), m_nDataOffset(0),
		m_nDataSizeTemp(0), m_uiBlock(c_uiDefaultBlock), m_bOk(true) {
		*this = out;
	}
	cOStream& operator = (const cOStream& out) {
		Clear();
		if (out.GetDataSize() == 0 || out.GetData() == 0)
			return *this;
		return Write(out.GetData(), out.GetDataSize());
	}

	cpstr GetData() const {
		return m_pData;
	}
	ui32 GetDataSize() const {
		return m_nDataSize;
	}

	void Clear() {
		m_nDataSize = 0;
		m_nDataOffset = 0;
		m_bOk = true;
	}

	//输出
	cOStream& operator << (const bool& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const i8& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const i16& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const i32& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const i64& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const ui8& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const ui16& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const ui32& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const ui64& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const f32& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const f64& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const l32& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}
	cOStream& operator << (const ul32& n) {
		WriteData(&n, sizeof(n));
		return *this;
	}

	cOStream& operator << (const std::string& s) {
		ui32 nstr = (ui32)s.size();
		WriteData(&nstr, sizeof(nstr));
		if (nstr)
			WriteData(s.data(), nstr);
		return *this;
	}
	cOStream& operator << (const cGuid& guid) {
		return *this << guid[0] << guid[1] << guid[2] << guid[3];
	}

	bool IsOk() const {return m_bOk;}

	cOStream& Write(cpvd buf, ui32 bufSize) {
		WriteData(buf, bufSize);
		return *this;
	}

	cOStream& Write(const cOStream& os) {
		WriteData(os.GetData(), os.GetDataSize());
		return *this;
	}

	bool WriteBool(b8 n) {
		return WriteData(&n, sizeof(n));
	}
	bool WriteInt32(i32 n) {
		return WriteData(&n, sizeof(n));
	}
	bool WriteFloat32(f32 n) {
		return WriteData(&n, sizeof(n));
	}
	bool WriteString(const std::string& s) {
		ui32 nstr = (ui32)s.size();
		if (WriteData(&nstr, sizeof(nstr))) {
			if (nstr)
				return WriteData(s.data(), nstr);
			else
				return true;
		}
		return false;
	}
	bool WriteInFile(const std::string& sfile) {
		std::ifstream ifs;
		ifs.open(sfile, std::ios_base::in | std::ios_base::binary);
		if (ifs.is_open()) {
			std::ifstream::pos_type old = ifs.tellg();
			ifs.seekg(0, std::ios_base::end);
			ui32 len = (ui32)ifs.tellg();
			ifs.seekg(old, std::ios_base::beg);
			std::string buf;
			buf.resize(len);
			ifs.read((i8*)buf.data(), len);
			WriteString(sfile);
			WriteString(buf);
			return true;
		}
		WriteString("");
		return false;
	}
	std::string GetBuffer() {
		if (m_pData && m_nDataSize)
			return std::string(m_pData, m_nDataSize);
		else
			return std::string();
	}

protected:

	bool WriteData(cpvd buf, ui32 bufSize)
	{
		try
		{
			ui32 ofs = m_nDataSize;
			if (AddDataSize(bufSize)) {
				i8* pDes = m_pData + ofs;
				cmemcpy(pDes, buf, bufSize);
				return true;
			}
			else {
				m_bOk = false;
				return false;
			}
		}
		catch(...)
		{
			StreamErrorLog() << "Error!" << LogEnter;
			//throw exception_steam_writeoverflow(bufSize,__FUNCTION__);
			m_bOk = false;
			return false;
		}
	}

	bool AddDataSize(ui32 szData)
	{
		if (szData == 0) {
			//Assert(0);
			return false;
		}
		ui32 oldSize = m_nDataSize;
		m_nDataSize += szData;
		if (m_nDataSizeTemp >= m_nDataSize)
			return true;
		if (szData % m_uiBlock == 0)
			m_nDataSizeTemp += ( szData / m_uiBlock ) * m_uiBlock;
		else
			m_nDataSizeTemp += ( szData / m_uiBlock + 1 ) * m_uiBlock;
		i8* p = new i8[m_nDataSizeTemp];
		if (m_pData) {
			CrossMemory::MemCpy(p, m_pData, oldSize);
			delete[] m_pData;
		}
		m_pData = p;
		return true;
	}

	i8*		m_pData;

	ui32	m_nDataSize;
	ui32	m_nDataOffset;
	ui32	m_nDataSizeTemp;

	ui32	m_uiBlock;
	b8		m_bOk;

};

static const ui32 g_ui1MBlock = 1024*1024;
class cOStream1M : public cOStream
{
public:
	cOStream1M() : cOStream(g_ui1MBlock) {}
};

static const ui32 g_ui10MBlock = 10*1024*1024;
class cOStream10M : public cOStream
{
public:
	cOStream10M() : cOStream(g_ui10MBlock) {}
};

class cIStream
{
public:

	cIStream() {
		_Init();
	}
	cIStream(cOStream& out) {
		_Init();
		SetData(out.GetData(), out.GetDataSize(), true);
	}
	cIStream(const std::string& bin) {
		_Init();
		SetData(bin.data(), bin.size(), true);
	}
	cIStream(cpvd buf, ui32 len, b8 bCopy = false) {
		_Init();
		SetData(buf, len, bCopy);
	}
	cIStream(const cIStream& in) {
		_Init();
		*this = in;
	}
	~cIStream() {
		if (m_bSelf)
			delete[] m_pData;
	}
	cIStream& operator = (const cIStream& in) {
		if (this == &in)
			return *this;
		SetData(in.GetData(), in.GetDataSize(), in.m_bSelf);
		return *this;
	}

	void SetData(std::string bin) {
		SetData(bin.data(), bin.size(), true);
	}

	void SetData(cpvd buf, ui32 len, b8 bCopy = false) {
		if (buf && len) {
			Clear();
			if (bCopy) {
				m_pData = new i8[len];
				m_nDataSize = len;
				cmemcpy(m_pData, buf, len);
				m_nDataOffset = 0;
				m_bOk = true;
				m_bSelf = true;
			} else {
				m_pData = (i8*)buf;
				m_nDataSize = len;
				m_nDataOffset = 0;
				m_bOk = true;
				m_bSelf = false;
			}
		}
	}

	cpstr GetData() const {
		return m_pData;
	}
	ui32 GetDataOffset() const {
		return m_nDataOffset;
	}
	ui32 GetDataSize() const {
		return m_nDataSize;
	}

	void SeekBegin() {
		m_nDataOffset = 0;
		m_bOk = true;
	}
	void SeekOffset(ui32 pos) {
		m_nDataOffset = pos;
		m_bOk = true;
	}
	void Clear() {
		SeekBegin();
		if (m_bSelf)
			delete[] m_pData;
		m_pData = 0;
		m_nDataSize = 0;
		m_bOk = true;
		m_bSelf = true;
	}

	//输入
	cIStream& operator >> (bool& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}

	cIStream& operator >> (i8& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (i16& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (i32& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (i64& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (ui8& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (ui16& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (ui32& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (ui64& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}

	cIStream& operator >> (f32& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (f64& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (l32& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}
	cIStream& operator >> (ul32& n) {
		ReadData(&n, sizeof(n));
		return *this;
	}

	cIStream& operator >> (std::string& s) {
		ui32 nstr = 0;
		ReadData(&nstr, sizeof(nstr));
		if (nstr <= 0) {
			s.clear();
			//Assert(0);
			return *this;
		}
		s.resize(nstr);
		ReadData((pvd)s.data(), nstr);
		return *this;
	}
	cIStream& operator >> (cGuid& guid) {
		*this >> guid[0] >> guid[1] >> guid[2] >> guid[3];
		return *this;
	}

	bool IsOk() const {
		return m_bOk;
	}

	cIStream& Read(pvd buf, ui32 bufSize) {
		ReadData(buf, bufSize);
		return *this;
	}

	b8 ReadBool() {
		b8 n = false;
		ReadData(&n, sizeof(n));
		return n;
	}
	i32 ReadInt32() {
		i32 n = 0;
		ReadData(&n, sizeof(n));
		return n;
	}
	f32 ReadFloat32() {
		f32 n = 0.f;
		ReadData(&n, sizeof(n));
		return n;
	}
	std::string ReadString() {
		std::string s;
		ui32 nstr = 0;
		ReadData(&nstr, sizeof(nstr));
		if (nstr <= 0)
			return s;
		s.resize(nstr);
		ReadData((pvd)s.data(), nstr);
		return s;
	}
	std::string ReadToFile() {
		std::string sfile = ReadString();
		if (sfile.empty())
			return sfile;
		std::ofstream ofs;
		ofs.open(sfile.c_str(), std::ios_base::out | std::ios_base::binary);
		if (ofs.is_open()) {
			std::string buf = ReadString();
			if (buf.size())
				ofs.write(buf.data(), buf.size());
			return sfile;
		}
		return sfile;
	}

private:
	
	void _Init() {
		m_pData = 0;
		m_nDataSize = 0;
		m_nDataOffset = 0;
		m_bOk = true;
		m_bSelf = true;
	}

	bool ReadData(pvd buf, ui32 bufSize)
	{
		if (m_nDataOffset + bufSize > m_nDataSize) {
			m_bOk = false;
			return false;
		}
		try
		{
			cmemcpy(buf, m_pData + m_nDataOffset, bufSize);
			m_nDataOffset += bufSize;
			return true;
		}
		catch(...)
		{
			//throw exception_steam("stream memory exception");
			StreamErrorLog() << "stream memory exception" << LogEnter;
			m_bOk = false;
			return false;
		}
	}

	i8*			m_pData;
	ui32		m_nDataSize;
	ui32		m_nDataOffset;
	b8			m_bOk;
	b8			m_bSelf;
};

}
#endif
