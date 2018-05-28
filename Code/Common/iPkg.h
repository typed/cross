#ifndef cross_common_iPkg_h_
#define cross_common_iPkg_h_

#include "Common.h"
#include <string>
#include <map>
#include <vector>

namespace cross
{

struct stPkgRead2
{
	ui32 crc;
	ui32 offset;
	ui32 len;
	ui32 lenfile;
	ui32 datacrc;
	std::string data;
};

class iPkgIOCallback
{
public:
	virtual bool Open(std::string pkg) {return false;}
	virtual ui32 Read(pvd, ui32) {return 0;}
	virtual void Seek(ui32 pos) {}
	virtual void Close() {}
	virtual bool ReadDiffuse(std::string sfile, pvd, ui32) {return false;}
	virtual bool Read2(std::vector<stPkgRead2>& rl) {return false;}
};

enum ePkgType {
	PT_Normal,
	PT_Sqlite,
};

struct stFileCrc {
	ui32 dataCrc;
	ui32 dataLen;
};
typedef std::map<ui32, stFileCrc> mapFileCrc;

class iPkg
{
public:
	static iPkg* Create(ePkgType);
	iPkg():m_pCallback(nullptr) {}
	virtual ~iPkg() {}
	virtual void Release() {delete this;}
	virtual void SetCallback(iPkgIOCallback* callback) {m_pCallback = callback;}
	virtual void SetFFindCallback(iPkgIOCallback* callback) {m_pCallback = callback;}
	virtual bool Open(std::string pkg, bool bDiffuse) {return false;}
	virtual void Close() {}
	virtual bool HasFile(std::string sFile) {return false;}
	virtual bool GetFileData(std::string sFile, std::string& dat) {return false;}
	virtual bool GetFileDataFromCrc(ui32 crc, std::string& dat) {return false;}
	virtual bool Pack(std::string dir, std::string sfile, const std::vector<std::string>& igno, std::string ver) {return false;}
	virtual bool Diffuse(std::string dir, std::string pkgname) {return false;}
	virtual bool UpdateFile(std::string sFile, cpvd dat, ui32 len) {return false;}
	virtual bool UpdateFileFromCrc(ui32 crc, cpvd dat, ui32 len) {return false;}
	virtual bool GetFileDataFromCrc2(std::vector<stPkgRead2>& crcs) {return false;}
	virtual bool UpdateFileFromCrc2(const std::vector<stPkgRead2>& crcs) {return false;}
	virtual void BeginTmpFile() {}
	virtual bool UpdateTmpFileFromCrc(const std::vector<stPkgRead2>& crcs) {return false;}
	virtual void EndTmpFile() {}
	virtual void RemoveFile(std::string sFile) {}
	virtual void GetFileCrc(mapFileCrc& m) {}
	virtual void SetDir(std::string dir) {}
	virtual void SetVersion(std::string s) {}
	virtual std::string GetVersion() {return "";}
	virtual void ClearData() {}
protected:
	iPkgIOCallback*	m_pCallback;
};

}

#endif  
