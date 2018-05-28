#include "../inc/CrossFileOP.h"
#include "../inc/CrossString.h"
#include "../inc/iCrc.h"
#include <string>
#include <string.h>
#include <vector>
#include <fstream>

#if (defined(WIN32) || defined(_WIN32))
#	include <windows.h>
#	include <direct.h>
#	include <io.h>
#else
#	include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

using namespace std;

namespace cross
{

	bool CrossFileOP::Delete(const string& s) {
#if (defined(WIN32) || defined(_WIN32))
		string sFolder = s;
		string sPath = sFolder;
		if (sFolder.find(":") == string::npos) {
			i8 tmp[_MAX_PATH];
			_getcwd(tmp, _MAX_PATH);
			sPath = tmp;
			sPath += "\\";
			sPath += sFolder;
		}
		_finddata_t c_file;
		intptr_t hFile;
		string sFile, sFileFull, sFind;
		if (access(sPath.c_str(), 0) == -1)
			return false;
		struct stat statbuf;
		if (stat(sPath.c_str(), &statbuf))
			return false;
		bool bFolder = statbuf.st_mode & S_IFDIR ? true : false;
		if (bFolder)
			sFind = sPath + "\\*";
		else {
			SetFileReadOnly(false, sPath);
			return ::remove(sPath.c_str()) == 0;
		}
		if( (hFile = _findfirst( sFind.c_str(), &c_file )) != -1L ) {
			do {
				sFile = c_file.name;
				if (sFile == "." || sFile == "..")
					continue;
				sFileFull = sPath + "\\" + sFile;
				if ((_A_SUBDIR & c_file.attrib) != 0) {
					if (!Delete(sFileFull))
						return false;
					continue;
				}
				SetFileReadOnly(false, sFileFull);
				if (::remove(sFileFull.c_str()))
					return false;
			}
			while( _findnext( hFile, &c_file ) == 0 );
			_findclose( hFile );
			SetFileReadOnly(false, sPath);
			if (rmdir(sPath.c_str()))
				return false;
		}
		return true;
#else
		return false;
#endif
	}

	bool CrossFileOP::Copy(const string& sSrc, const string& sDes)
	{
		if (sSrc == sDes)
			return false;
		std::ifstream ifs(sSrc, std::ios::binary);
		std::ofstream ofs(sDes, std::ios::binary);
		if (!ifs.is_open() || !ofs.is_open())
			return false;
		const std::streamsize MAXSIZE = 4096;
		i8 buffer[MAXSIZE];
		std::streamsize n = ifs.read(buffer, MAXSIZE).gcount();
		while (n > 0) {
			ofs.write(buffer, n);
			n = ifs.read(buffer, MAXSIZE).gcount();
		}
		return true;
	}

	bool CrossFileOP::CopyAndDir(const string& sSrc, const string& sDes)
	{
		ui32 count = GetPathLayoutCount(sDes);
		if (count == 0)
			return false;
		if (count == 1)
			return CrossFileOP::Copy(sSrc, sDes);
		string sPath = GetPathSub(sDes, 0, count - 1);
		CreateDirPath(sPath);
		return CrossFileOP::Copy(sSrc, sDes);
	}

	bool CrossFileOP::Move(const string& sOld, const string& sNew)
	{
		if (!CrossFileOP::Copy(sOld, sNew))
			return false;
		return CrossFileOP::Delete(sOld);
	}

	bool CrossFileOP::SetFileReadOnly(bool bReadOnly, const string& sFile)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		return -1 != _chmod( sFile.c_str(), ( bReadOnly ? _S_IREAD : _S_IWRITE ) );
#else
        return -1;
#endif
	}

	bool CrossFileOP::IsExist(const string& sFile)
	{
		std::ifstream ifs;
		ifs.open(sFile, std::ios::in|std::ios::binary);
		if (ifs.is_open()) {
			ifs.close();
			return true;
		}
		return false;
	}

	bool CrossFileOP::IsSameFile(const string& sz1, const string& sz2)
	{
		return GetFileCrc(sz1) == GetFileCrc(sz2);
	}

	ui32 CrossFileOP::GetFileCrc(const string& sFile)
	{
		ui32 crc = 0;
		if (FileCrc32(sFile.c_str(), crc))
			return crc;
		return -1;
	}

	ui32 CrossFileOP::GetFilePathCrc(const string& file_name)
	{
		ui32 crc = 0;
		if (FilePathCrc32(file_name.c_str(), crc))
			return crc;
		return -1;
	}

	ui32 CrossFileOP::GetFileLength(const string& sFile)
	{
		struct stat buf;
		if (stat(sFile.c_str(), &buf) != -1)
			return (ui32)buf.st_size;
		return 0;
	}

	bool CrossFileOP::AppSetCurDir(cpstr sPath)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//i8 szFileName[_MAX_PATH];
		//if (sPath == 0) {
		//	::GetModuleFileNameA(NULL, szFileName, _MAX_PATH);
		//	(*strrchr(szFileName,'\\')) = 0;
		//} else
		//	strcpy(szFileName, sPath);
		//return ::SetCurrentDirectoryA(szFileName) ? true : false;
		return false;
#else
		return true;
#endif
	}

	string CrossFileOP::AppGetCurDir()
	{
#if (defined(WIN32) || defined(_WIN32))
		char szFileName[_MAX_PATH];
		::GetCurrentDirectoryA(_MAX_PATH, szFileName);
		return szFileName;
#else
		return "";
#endif
	}

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

	bool CrossFileOP::CreateDir(const string& sz)
	{
		int res;
#if (defined(WIN32) || defined(_WIN32))
		res = _mkdir(sz.c_str());
#else
		res = mkdir(sz.c_str(), 0744);
#endif
		if (res < 0 && errno != EEXIST && errno != EACCES)
			return false;
		return true;
	}

	bool CrossFileOP::CreateDirPath(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		if (count == 0)
			return false;
		for (ui32 i = 0; i < count; i++) {
			string s = GetPathSub(sz, 0, i+1);
			CreateDir(s);
		}
		return true;
	}

	bool CrossFileOP::CreateFilePath(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		if (count == 0)
			return false;
		for (ui32 i = 0; i < count; i++) {
			string s = GetPathSub(sz, 0, i+1);
			if (i == count - 1) {
				if (!CrossFileOP::IsExist(s))
					std::ofstream f(s.c_str());
			} else
				CreateDir(s);
		}
		return true;
	}

	bool CrossFileOP::CreateDirPathIgnoreFile(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		for (ui32 i = 0; i < count - 1; i++) {
			string s = GetPathSub(sz, 0, i+1);
			//if (s.find(".") == -1)
			CreateDir(s);
		}
		return true;
	}

	bool CrossFileOP::FileBuildPath(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		if (count <= 1)
			return false;
		count--;
		for (ui32 i = 0; i < count; i++) {
			string s = GetPathSub(sz, 0, i+1);
			CreateDir(s);
		}
		return true;
	}

	bool CrossFileOP::IsAbsolutePath(const string& sz)
	{
		return sz.find(":") != string::npos;
	}

	bool CrossFileOP::IsRelativePath(const string& sz)
	{
		return !IsAbsolutePath(sz);
	}

	string CrossFileOP::AbsoluteToRelativePath(const string& s)
	{
		string sCur = CrossFileOP::AppGetCurDir();
		if (sCur == s)
			return "";
		string ss = CrossString::ReplaceAll(s, "/", "\\");
		string ss2 = CrossString::ToLower(ss);
		string ss1 = CrossString::ReplaceAll(CrossString::ToLower(sCur), "/", "\\");
		if (ss2.find(ss1) == string::npos)
			return "";
		else
			return ss.substr(ss1.length()+1);
	}

	string CrossFileOP::RelativeToAbsolutePath(const string& s)
	{
		return CrossFileOP::AppGetCurDir() + "\\" + s;
	}

	string CrossFileOP::GetPostfixName(const string& sz)
	{
		int off = (ui32)sz.find_last_of(".");
		if (off == -1)
			return "";
		else
			return sz.substr(off+1);
	}

	string CrossFileOP::GetPrefixName(const string& sz)
	{
        int off = (ui32)sz.find_last_of(".");
		if (off == -1)
			return sz;
		else
			return sz.substr(0,off);
	}

	string CrossFileOP::ChangePostfixName(const string& filename, const string& postfixname)
	{
		string newlf = postfixname;
		string fn = filename;
		string lf = GetPostfixName(filename);
		string ff = GetPrefixName(filename);
		if (lf == newlf)
			return filename;
		else
			return ff + (newlf==""?"":(string)"."+newlf);
	}

	string CrossFileOP::ChangePathSub(const string& filename, ui32 offset, const string& newname)
	{
		string s;
		//暂时未实现
		return s;
	}

	ui32 CrossFileOP::GetPathLayoutCount(const string& sz)
	{
		string ss = CrossString::ReplaceAll(sz, "\\", "/");
		vector<string> aStr;
		if (!CrossString::Split(ss, "/", aStr))
			return 0;
		return (ui32)aStr.size();
	}
	string CrossFileOP::GetPathSub(const string& sz, ui32 offset, ui32 count)
	{
		ui32 num = GetPathLayoutCount(sz);
		if (num == 0 || count == 0 || offset >= count)
			return "";
		if (offset + count > num)
			count = num - offset;
		string ss = CrossString::ReplaceAll(sz, "\\", "/");
		vector<string> aStr;
		if (!CrossString::Split(ss, "/", aStr))
			return "";
		string ssr;
		for (ui32 i = offset; i < offset + count; i++) {
			ssr += aStr[i];
			if ( i < offset + count - 1 )
#if (defined(WIN32) || defined(_WIN32))
				ssr += "\\";
#else
				ssr += "/";
#endif
		}
		return ssr;
	}
	string CrossFileOP::ReplaceAPath(const string& sPath, ui32 id, const string& s)
	{
		ui32 num = GetPathLayoutCount(sPath);
		if (num == 0)
			return "";
		string ss = CrossString::ReplaceAll(sPath, "\\", "/");
		vector<string> aStr;
		if (!CrossString::Split(ss, "/", aStr))
			return "";
		if (id >= aStr.size())
			return "";
		aStr[id] = s;
		string ssr;
		for (ui32 i = 0; i < num; i++) {
			ssr += aStr[i];
			if ( i + 1 < num )
#if (defined(WIN32) || defined(_WIN32))
				ssr += "\\";
#else
				ssr += "/";
#endif
		}
		return ssr;
	}
	string CrossFileOP::GetFilePath(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		if (count <= 1)
			return "";
		else
			return GetPathSub(sz, 0, count - 1);
	}
	string CrossFileOP::GetFileName(const string& sz)
	{
		ui32 count = GetPathLayoutCount(sz);
		if (count <= 1)
			return sz;
		else
			return GetPathSub(sz, count - 1);
	}

	string CrossFileOP::GetFileLastName(const string& sz)
	{
		ui32 pos0 = 0;
		string str = sz;
		pos0 = (ui32)str.find_first_of(".");
		str = str.substr(0, pos0);
		ui32 count = GetPathLayoutCount(str);
		if (count <= 1)
			return str;
		else
			return GetPathSub(str, count - 1);
	}

	bool CrossFileOP::Rename(const string& oldname, const string& newname)
	{
		return 0 == rename(oldname.c_str(), newname.c_str());
	}

	//<?>清除只读, 隐藏?还有独占呢?
	bool CrossFileOP::IsReadonly(const string& szFileName)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//if (CrossFileOP::IsExist(szFileName))
		//	return GetFileAttributes(szFileName.c_str()) & FILE_ATTRIBUTE_READONLY;
		return false;
#else
		return false;
#endif
	}
	bool CrossFileOP::RemoveReadonly(const string& szFileName)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//return SetFileAttributes(szFileName.c_str(),FILE_ATTRIBUTE_NORMAL) ? true : false;
		return false;
#else
		return false;
#endif
	}

	bool CrossFileOP::IsNoShare(const string& szFileName)
	{
		if (!CrossFileOP::IsExist(szFileName))
			return false;
		//pvd p = CrossFile::Open(szFileName.c_str(), CrossFile::FP_Read, CrossFile::FP_Share_Null);
		//if (p) {
		//	CrossFile::Close(p);
		//	return false;
		//}
		return true;
	}

	bool CrossFileOP::ClearAllAttributes(const string& szFileName)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//return SetFileAttributes(szFileName.c_str(), FILE_ATTRIBUTE_NORMAL)?true:false;
		return false;
#else
		return false;
#endif
	}

	string CrossFileOP::GetModuleName()
	{
		return GetPrefixName(GetModuleFullName());
	}

	string CrossFileOP::GetModulePath()
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//char szFileName[_MAX_PATH];
		//::GetModuleFileName(NULL, szFileName, _MAX_PATH);
		//return GetFilePath(szFileName);
		return "";
#else
		return "";
#endif
	}

	string CrossFileOP::GetModuleFullName()
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//static string s_sModuleName;
		//if (s_sModuleName.empty()) {
		//	char szModuleName[MAX_PATH*2];
		//	ZeroMemory(szModuleName, sizeof(szModuleName));
		//	if (GetModuleFileName(0, szModuleName, _countof(szModuleName)-2) <= 0)
		//		strcpy(szModuleName, ("Unknown"));
		//	s_sModuleName = CrossFileOP::GetFileName(szModuleName);//???这个函数不是取得模块的全限定名称吗？怎么还需这一句？Ask by lkm,2013.3.26
		//}
		//return s_sModuleName;
		return "";
#else
		return "";
#endif
	}

	string CrossFileOP::GetErrorString(i32 error)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//static string s;
		//LPVOID lpMsgBuf;
		//DWORD sz = FormatMessage(
		//	FORMAT_MESSAGE_ALLOCATE_BUFFER |
		//	FORMAT_MESSAGE_FROM_SYSTEM |
		//	FORMAT_MESSAGE_IGNORE_INSERTS,
		//	NULL,
		//	error,
		//	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		//	(LPTSTR) &lpMsgBuf,
		//	0,
		//	NULL );
		//if (!sz)
		//	return "";
		//s = CrossString::Format("(%d)%s",error,(LPCTSTR)lpMsgBuf);
		//s = CrossString::Trim(s);
		//LocalFree( lpMsgBuf );
		//return s;
		return "";
#else
		return "";
#endif
	}

	string CrossFileOP::GetFileVersionStr(const string& sExeFile)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//DWORD dwLen = GetFileVersionInfoSize(sExeFile.c_str(), 0);
		//if (dwLen == 0)
		//	return "";
		//vector<BYTE> buf(dwLen);
		//if (!GetFileVersionInfo(sExeFile.c_str(), 0, dwLen, &buf.front()))
		//	return "";
		//LPVOID lpBuffer = NULL;
		//ui32 uLen = 0;
		//if (!VerQueryValue(&buf.front(), (CHAR*)"StringFileInfo\\080404b0\\FileVersion", &lpBuffer, &uLen))
		//	return "";
		//return (char*)lpBuffer;
		return "";
#else
		return "";
#endif
	}

	bool CrossFileOP::IsTimeLate(cpstr sz1, cpstr sz2)
	{
		return GetFileWriteTime(sz1) < GetFileWriteTime(sz2);
	}

	bool CrossFileOP::SetFileWriteTime(cpstr sFile, TypeDate tm)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		return false;
#else
		return false;
#endif
	}

	TypeDate CrossFileOP::GetFileWriteTime(cpstr sFile)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		if (!IsExist(sFile))
			return 0;
		struct stat statbuf;
		if (stat(sFile, &statbuf))
			return 0;
		return (TypeDate)statbuf.st_mtime;
#else
		return 0;
#endif
	}

	void CrossFileOP::FindFile(const stFileFindParam& param, aFileEntry& lstFile)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		stFileFindParam param1 = param;
		param1.sFindText = CrossString::ToLower(param1.sFindText);
		string sPath = param1.sFolder;
		if (param1.bUseFullPath && param1.sFolder.find(":") == -1) {
			char tmp[256];
			_getcwd(tmp, 256);
			sPath = tmp;
			if (!param1.sFolder.empty()) {
				sPath += "\\";
				sPath += param1.sFolder;
			}
		}
		_finddata_t c_file;
		intptr_t hFile;
		stFileEntry entry;
		string sFile, sFileFull, sFind;
		if (sPath.empty())
			sFind = param1.sFindText;
		else
			sFind = sPath + "\\" + param1.sFindText;
		if( (hFile = _findfirst( sFind.c_str(), &c_file )) != -1L ) {
			do {
				sFile = c_file.name;
				if (sFile == "." || sFile == "..")
					continue;
				if (sPath.empty())
					sFileFull = sFile;
				else
					sFileFull = sPath + "\\" + sFile;
				if (param1.bIgnoreHidden && ( _A_HIDDEN & c_file.attrib ) )
					continue;
				entry.isDirectory = (_A_SUBDIR & c_file.attrib) != 0;
				if (entry.isDirectory) {
					param1.sFolder = sFileFull;
					if (param1.bTree)
						FindFile(param1, lstFile);
					if (!param1.bFloder)
						continue;
				}
				entry.sName = sFile;
				entry.sFullName = sFileFull;
				entry.ulSize = c_file.size;
				entry.time_create = (TypeDate)c_file.time_create;
				entry.time_access = (TypeDate)c_file.time_access;
				entry.time_write = (TypeDate)c_file.time_write;
				lstFile.push_back(entry);
			}
			while( _findnext( hFile, &c_file ) == 0 );
			_findclose( hFile );
		}
#else
#endif
	}

	void CrossFileOP::FindDir(const stFileFindParam& param, aFileEntry& lst)
	{
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		stFileFindParam param1 = param;
		string sPath = param1.sFolder;
		if (param1.sFolder.find(":") == -1) {
			char tmp[256];
			_getcwd(tmp, 256);
			sPath = tmp;
			sPath += "\\";
			sPath += param1.sFolder;
		}
		_finddata_t c_file;
		intptr_t hFile;
		stFileEntry entry;
		string sFile, sFileFull, sFind;
		sFind = sPath + "\\*";
		if( (hFile = _findfirst( sFind.c_str(), &c_file )) != -1L ) {
			do {
				sFile = c_file.name;
				if (sFile == "." || sFile == "..")
					continue;
				sFileFull = sPath + "\\" + sFile;
				entry.sName = sFile;
				entry.sFullName = sFileFull;
				entry.isDirectory = (_A_SUBDIR & c_file.attrib) != 0;
				if (entry.isDirectory)
					lst.push_back(entry);
			}
			while( _findnext( hFile, &c_file ) == 0 );
			_findclose( hFile );
		}
#else
#endif
	}

}
