//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef _cross_common_crossfileop_h
#define _cross_common_crossfileop_h

#include "Common.h"
#include "CrossString.h"
#include <string>
#include <vector>


namespace cross
{

struct stFileFindParam
{
	stFileFindParam() : bTree(false), bUseFullPath(true), bFloder(false), bIgnoreHidden(true), sFindText("*.*") {}
	stFileFindParam(
		cpstr sfolder, 
		cpstr sfindtext,
		bool btree = false,
		bool busefu = false,
		bool bfloder = false,
		bool IgnoreHid = true) : 
		bTree(btree),
		bUseFullPath(busefu),
		bFloder(bfloder),
		bIgnoreHidden(IgnoreHid),
        sFolder(sfolder),
        sFindText(sfindtext){}
	bool bTree;					//是否查子目录
	bool bUseFullPath;			//使用绝对路径
	bool bFloder;				//是否包含目录
	bool bIgnoreHidden;			//是否忽略隐藏目录或文件
    std::string sFolder;		//查找的目录
    std::string sFindText;		//查找文件
};

struct stFileEntry
{
	std::string sName;
	std::string sFullName;		//当使用绝对路径时是绝对路径，否则是相对路径
	ui32 ulSize;
	TypeDate time_create;
	TypeDate time_write;
	TypeDate time_access;
	bool isDirectory;
	stFileEntry() : ulSize(0), 
		time_create(0), time_write(0), time_access(0), isDirectory(false) {}
	//bool operator < (const stFileEntry& other) const {
	//	if ( isDirectory ^ other.isDirectory )
	//		return isDirectory;
	//	return CrossString::EqualIgnore(sName, other.sName);
	//}
};

typedef std::vector<stFileEntry> aFileEntry;

//文件处理
class CrossFileOP
{
public:
//file
	//Delete File and Folder
	static bool Delete(const std::string& s);
	static bool Copy(const std::string& sSrc, const std::string& sDes);
	static bool CopyAndDir(const std::string& sSrc, const std::string& sDes);
	static bool Move(const std::string& sOld, const std::string& sNew);
	static bool SetFileReadOnly(bool bReadOnly, const std::string& sFile);
	static bool IsExist(const std::string& sFile);
	static bool IsSameFile(const std::string& sz1, const std::string& sz2);

//directory
	static bool AppSetCurDir(cpstr sPath = 0);
	static std::string AppGetCurDir();
	static bool CreateDir(const std::string& sz);
	static bool CreateDirPath(const std::string& sz);
	static bool CreateFilePath(const std::string& sz);
	static bool CreateDirPathIgnoreFile(const std::string& sz);
	static bool FileBuildPath(const std::string& sz);
	static bool IsAbsolutePath(const std::string& sz);
	static bool IsRelativePath(const std::string& sz);
	static std::string AbsoluteToRelativePath(const std::string& s);
	static std::string RelativeToAbsolutePath(const std::string& s);

	static std::string GetPostfixName(const std::string& sz);
	static std::string GetPrefixName(const std::string& sz);
	static std::string ChangePostfixName(const std::string& filename, const std::string& postfixname);
	static std::string ChangePathSub(const std::string& filename, ui32 offset, const std::string& newname);
	static ui32 GetPathLayoutCount(const std::string& sz);
	static std::string GetPathSub(const std::string& sz, ui32 offset, ui32 count = 9999);
	static std::string ReplaceAPath(const std::string& sPath, ui32 id, const std::string& s);
	static std::string GetFilePath(const std::string& sz);
	static std::string GetFileName(const std::string& sz);
	static std::string GetFileLastName(const std::string& sz);

	static bool Rename(const std::string& oldname, const std::string& newname);
	static bool IsReadonly(const std::string& szFileName);
	static bool RemoveReadonly(const std::string& szFileName);

	static bool IsNoShare(const std::string& szName);

	static bool ClearAllAttributes(const std::string& szFileName);

	//for example
	//GetModuleFullName return servergame_r.exe
	//GetModuleName return servergame_r
	static std::string GetModuleFullName();
	static std::string GetModuleName();
	static std::string GetModulePath();

	static std::string GetErrorString(i32 error);

	static std::string GetFileVersionStr(const std::string& sExeFile);

	//crc
	static ui32 GetFileCrc(const std::string& sFile);
	static ui32 GetFilePathCrc(const std::string& file_name);			//add by cwh 6.2

	//length
	static ui32 GetFileLength(const std::string& sFile);

	//比较文件修改时间
	static bool IsTimeLate(cpstr sz1, cpstr sz2);
	static bool SetFileWriteTime(cpstr sFile, TypeDate tm);
	//获取文件修改时间
	static TypeDate GetFileWriteTime(cpstr sFile);
	//filefind

	static void FindFile(const stFileFindParam& param, aFileEntry& lst);
	static void FindDir(const stFileFindParam& param, aFileEntry& lst);

};

}
#endif
