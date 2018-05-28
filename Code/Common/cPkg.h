#ifndef cPkg_h_
#define cPkg_h_

#include "../inc/iPkg.h"
#include "../inc/iCrc.h"
#include "../inc/iStream_DS.h"
#include "../inc/iDb.h"
#include "../inc/iReference.h"
#include "../inc/CrossFileOP.h"
#include "../inc/iCompress.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>

namespace cm
{

static const unsigned int c_header_size = 128*1024;

struct stFileInfo {
	ui32 offset;
	ui32 len;
	ui32 lenFile;
	ui32 dataCrc;
	stFileInfo():offset(0),len(0),lenFile(0),dataCrc(0){}
};
inline cOStream& operator << (cOStream& os, const stFileInfo& fi) {
	return os << fi.offset << fi.len << fi.lenFile << fi.dataCrc;
}
inline cIStream& operator >> (cIStream& is, stFileInfo& fi) {
	return is >> fi.offset >> fi.len >> fi.lenFile >> fi.dataCrc;
}

typedef std::map<ui32, stFileInfo> mapFileInfo;

class cPkgNormal :
	public iPkg
{
public:
	cPkgNormal();
	~cPkgNormal();
	bool Open(std::string pkg, bool bDiffuse);
	std::string GetVersion() {return m_version_info;}
	void Close();
	bool HasFile(std::string sFile);
	bool GetFileData(std::string sFile, std::string& dat);
	bool GetFileDataFromCrc(ui32 crc, std::string& dat);
	bool GetFileDataFromCrc2(std::vector<stPkgRead2>& crcs);
	bool Pack(std::string dir, std::string sfile, const std::vector<std::string>& igno, std::string ver);
	bool Diffuse(std::string dir, std::string pkgname);
	void GetFileCrc(mapFileCrc& m);
	void SetDir(std::string dir) {m_pkgdir = dir;}
private:
	stFileInfo* _GetFileInfo(std::string sFile);
	ui32				m_version;
	mapFileInfo			m_mapFileInfo;
	b8					m_bDiffuse;
	std::string			m_pkgname;
	std::string			m_pkgdir;
	std::string			m_version_info;
};

class cPkgSqlite :
	public iPkg
{
public:
	cPkgSqlite();
	~cPkgSqlite();
	bool Open(std::string pkg, bool bReadOnly);
	void Close();
	void SetVersion(std::string s);
	std::string GetVersion();
	bool HasFile(std::string sFile);
	bool GetFileData(std::string sFile, std::string& dat);
	bool GetFileDataFromCrc(ui32 crc, std::string& dat);
	bool Pack(std::string dir, std::string sfile, const std::vector<std::string>& igno, std::string ver);
	bool UpdateFile(std::string sFile, cpvd dat, ui32 len);
	bool UpdateFileFromCrc(ui32 crc, cpvd dat, ui32 len);
	bool UpdateFileFromCrc2(const std::vector<stPkgRead2>& crcs);
	void BeginTmpFile();
	bool UpdateTmpFileFromCrc(const std::vector<stPkgRead2>& crcs);
	void EndTmpFile();
	void RemoveFile(std::string sFile);
	void GetFileCrc(mapFileCrc& m);
	void ClearData();
private:
	iDb*				m_pDb;
};

}

#endif  
