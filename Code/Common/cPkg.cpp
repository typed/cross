#include "cPkg.h"

using namespace std;

namespace cross
{

cPkgNormal::cPkgNormal():
m_version(0xF1C3A2C8)
{
}

cPkgNormal::~cPkgNormal()
{
	Close();
}

bool cPkgNormal::Open(std::string pkg, bool bDiffuse)
{
	m_pkgname = pkg;
	i8 dat[c_header_size] = {0};
	m_bDiffuse = bDiffuse;
	if (m_bDiffuse) {
		if (m_pCallback) {
			std::string des;
			if (!m_pCallback->ReadDiffuse(pkg, dat, c_header_size))
				return false;
			memcpy(dat, des.data(), des.size());
		} else {
			std::fstream _file;
			_file.open(pkg, std::ios::in|std::ios::binary);
			if (!_file.is_open())
				return false;
			_file.read(dat, c_header_size);
		}
	} else {
		if (m_pCallback) {
			if (!m_pCallback->Open(pkg))
				return false;
			if (c_header_size != m_pCallback->Read(dat, c_header_size))
				return false;
		} else {
			std::fstream _file;
			_file.open(pkg, std::ios::in|std::ios::binary);
			if (!_file.is_open())
				return false;
			_file.read(dat, c_header_size);
		}
	}
	cIStream is;
	is.SetData(dat, c_header_size);
	ui32 version = 0;
	is >> version;
	if (version != m_version)
		return false;
	is >> m_mapFileInfo >> m_version_info;
	return true;
}

void cPkgNormal::Close()
{
	if (m_bDiffuse) {
	} else {
		if (m_pCallback) {
			m_pCallback->Close();
		}
	}
}

bool cPkgNormal::HasFile(std::string sFile)
{
	return _GetFileInfo(sFile) != nullptr;
}

bool cPkgNormal::GetFileData(std::string sFile, std::string& dat)
{
	return GetFileDataFromCrc(FilePathCrc32Easy(sFile.c_str()), dat);
}

bool cPkgNormal::GetFileDataFromCrc(ui32 crc, std::string& dat)
{
	mapFileInfo::iterator it = m_mapFileInfo.find(crc);
	if (it == m_mapFileInfo.end())
		return false;
	stFileInfo* pFile = &it->second;
	string src;
	src.resize(pFile->len);
	dat.resize(pFile->lenFile);
	if (m_bDiffuse) {
		if (m_pCallback) {
			if (!m_pCallback->ReadDiffuse(m_pkgdir+CrossString::Format("%u", crc), (i8*)src.data(), src.size()))
				return false;
		} else {
			std::fstream _file;
			_file.open(m_pkgdir+CrossString::Format("%u", crc), std::ios::in|std::ios::binary);
			if (!_file.is_open())
				return false;
			if (src.size() != _file.read((i8*)src.data(), src.size()).gcount())
				return false;
		}
	} else {
		if (m_pCallback) {
			m_pCallback->Seek(pFile->offset);
			if (src.size() != m_pCallback->Read((i8*)src.data(), src.size()))
				return false;
		} else {
			std::fstream _file;
			_file.open(m_pkgname, std::ios::in|std::ios::binary);
			if (!_file.is_open())
				return false;
			_file.seekg(pFile->offset, std::ios::beg);
			if (src.size() != _file.read((i8*)src.data(), src.size()).gcount())
				return false;
		}
	}
	return Decompress(src, dat) == C_OK;
}

bool cPkgNormal::GetFileDataFromCrc2(std::vector<stPkgRead2>& crcs)
{
	//only http
	for (std::vector<stPkgRead2>::iterator it = crcs.begin(); it != crcs.end(); it++) {
		mapFileInfo::iterator it1 = m_mapFileInfo.find(it->crc);
		if (it1 == m_mapFileInfo.end())
			return false;
		stFileInfo* pFile = &it1->second;
		stPkgRead2& pr = *it;
		pr.offset = pFile->offset;
		pr.len = pFile->len;
		pr.lenfile = pFile->lenFile;
		pr.datacrc = pFile->dataCrc;
	}
	return m_pCallback->Read2(crcs);
}

stFileInfo* cPkgNormal::_GetFileInfo(std::string sFile)
{
	ui32 crc = FilePathCrc32Easy(sFile.c_str());
	mapFileInfo::iterator it = m_mapFileInfo.find(crc);
	if (it == m_mapFileInfo.end())
		return nullptr;
	return &it->second;
}

bool cPkgNormal::Pack(std::string dir, std::string sfile, const std::vector<std::string>& igno, std::string ver)
{
	ofstream ofs;
	ofs.open(sfile, std::ios::out|std::ios::binary);
	if (!ofs.is_open())
		return false;
	i8 dat[c_header_size] = {0};
	ofs.write(dat, c_header_size);
	ui32 sum = c_header_size;
	mapFileInfo mfi;
	aFileEntry lst;
	CrossFileOP::FindFile(stFileFindParam(dir.c_str(), "*.*", true, false), lst);
	for (aFileEntry::iterator it = lst.begin(); it != lst.end(); it++) {
		string sFile = it->sFullName;
		bool bFoundIgno = false;
		for (std::vector<std::string>::const_iterator it1 = igno.begin(); it1 != igno.end(); it1++) {
			if (sFile.find(*it1) == 0) {
				bFoundIgno = true;
				break;
			}
		}
		if (bFoundIgno)
			continue;
		string sdir = dir;
		sFile = sFile.substr(sdir.length()+1);
		ui32 crc = FilePathCrc32Easy(sFile.c_str());
		stFileInfo fi;
		ifstream ifs;
		ifs.open(it->sFullName, std::ios::in|std::ios::binary);
		if (ifs.is_open()) {
			ifstream::pos_type old = ifs.tellg();
			ifs.seekg(0, std::ios_base::end);
			ui32 len = (ui32)ifs.tellg();
			ifs.seekg(old, std::ios_base::beg);
			if (len > 0) {
				string buf;
				buf.resize(len);
				ifs.read((i8*)buf.data(), len);
				string bufDes;
				Compress(buf, bufDes);
				fi.offset = sum;
				fi.dataCrc = BufferCrc32Easy(buf.data(), len);
				fi.lenFile = len;
				fi.len = bufDes.size();
				sum += bufDes.size();
				mfi[crc] = fi;
				ofs.write(bufDes.data(), bufDes.size());
				LOk << it->sFullName << " ok." << LEnd;
			}
		}
	}
	m_version_info = ver;
	cOStream1M os;
	os << m_version << mfi << ver;
	ofs.seekp(0, ios::beg);
	ofs.write(os.GetData(), os.GetDataSize());
	return true;
}

void cPkgNormal::GetFileCrc(mapFileCrc& m)
{
	for (mapFileInfo::iterator it = m_mapFileInfo.begin(); it != m_mapFileInfo.end(); it++) {
		stFileCrc& fc = m[it->first];
		fc.dataCrc = it->second.dataCrc;
		fc.dataLen = it->second.len;
	}
}

bool cPkgNormal::Diffuse(std::string dir, std::string pkgname)
{
	CrossFileOP::Delete(dir);
	CrossFileOP::CreateDir(dir);
	for (mapFileInfo::iterator it = m_mapFileInfo.begin(); it != m_mapFileInfo.end(); it++) {
		std::string s = dir + "/" + CrossString::Format("%u", it->first);
		stFileInfo* pFile = &it->second;
		string dat;
		dat.resize(pFile->len);
		std::fstream _file;
		_file.open(m_pkgname, std::ios::in|std::ios::binary);
		if (!_file.is_open()) {
			LError << s << " !_file.is_open() ToDiffuse error." << LEnd;
			return false;
		}
		_file.seekg(pFile->offset, std::ios::beg);
		if (dat.size() != _file.read((i8*)dat.data(), dat.size()).gcount()) {
			LError << s << "_file.read ToDiffuse error." << LEnd;
			return false;
		}
		std::ofstream ofs(s, std::ios::binary);
		if (!ofs.is_open()) {
			LError << s << " !ofs.is_open() ToDiffuse error." << LEnd;
			return false;
		}
		ofs.write((i8*)dat.data(), dat.size());
	}
	std::string s = dir + "/" + pkgname;
	std::ofstream ofs(s, std::ios::binary);
	if (!ofs.is_open()) {
		LError << s << " ToDiffuse error." << LEnd;
		return false;
	}
	i8 dat[c_header_size] = {0};
	cOStream1M os;
	os << m_version << m_mapFileInfo;
	memcpy(dat, os.GetData(), os.GetDataSize());
	ofs.write(dat, c_header_size);
	return true;
}

cPkgSqlite::cPkgSqlite()
{
	m_pDb = nullptr;
}

cPkgSqlite::~cPkgSqlite()
{
	Close();
}

bool cPkgSqlite::Open(std::string pkg, bool bReadOnly)
{
	iDbBind* bind = nullptr;
	iDbRecordSet* rs = nullptr;
	i32 num = 0;
	if (m_pDb)
		return false;
	m_pDb = iDb::Create(DT_Sqlite);
	if (!m_pDb->OpenDb(pkg.c_str()))
		goto redo;
	//m_pDb->Optimize(O_Fast);
	rs = m_pDb->ExceSqlResult("select count(*) from sqlite_master where type='table' and name='tbl_files'");
	if (rs) {
		num = rs->GetFieldInt32(0);
		if (num == 0) {
			if (!m_pDb->ExceSql("create table tbl_files(crc INTEGER primary key, lenfile INTEGER, len INTEGER, datacrc INTEGER, data BLOB)"))
				goto redo;
		}
		rs->Release();
	} else
		goto redo;
	rs = m_pDb->ExceSqlResult("select count(*) from sqlite_master where type='table' and name='tbl_tmp_files'");
	if (rs) {
		num = rs->GetFieldInt32(0);
		if (num == 0) {
			if (!m_pDb->ExceSql("create table tbl_tmp_files(crc INTEGER primary key, lenfile INTEGER, len INTEGER, datacrc INTEGER, data BLOB)"))
				goto redo;
		}
		rs->Release();
	} else
		goto redo;
	rs = m_pDb->ExceSqlResult("select count(*) from sqlite_master where type='table' and name='tbl_version'");
	if (rs) {
		num = rs->GetFieldInt32(0);
		if (num == 0) {
			if (!m_pDb->ExceSql("create table tbl_version(ver TEXT)"))
				goto redo;
		}
		rs->Release();
	} else
		goto redo;
	return true;
redo:
	Close();
	std::ofstream ofs;
	ofs.open(pkg);
	if (ofs.is_open())
		return Open(pkg, bReadOnly);
	else
		return false;
}

void cPkgSqlite::Close()
{
	SafeRelease(m_pDb);
}

bool cPkgSqlite::HasFile(std::string sFile)
{
	if (m_pDb == nullptr)
		return false;
	iDbBind* bind = m_pDb->BindSql("select crc from tbl_files where crc=?");
	if (!bind)
		return false;
	ui32 crc = FilePathCrc32Easy(sFile.c_str());
	bind->BindInt64(crc, 1);
	bool has = false;
	iDbRecordSet* rs = bind->ExceResult();
	if (rs) {
		rs->Release();
		has = true;
	}
	bind->Release();
	return has;
}

bool cPkgSqlite::GetFileData(std::string sFile, std::string& dat)
{
	return GetFileDataFromCrc(FilePathCrc32Easy(sFile.c_str()), dat);
}

bool cPkgSqlite::GetFileDataFromCrc(ui32 crc, std::string& dat)
{
	if (m_pDb == nullptr)
		return false;
	iDbBind* bind = m_pDb->BindSql("select lenfile,data from tbl_files where crc=?");
	if (!bind)
		return false;
	bool ok = false;
	bind->BindInt64(crc, 1);
	iDbRecordSet* rs = bind->ExceResult();
	if (rs) {
		i32 lenFile = rs->GetFieldInt32(0);
		std::string src = rs->GetFieldBuffer(1);
		dat.resize(lenFile);
		ok = (C_OK == Decompress(src, dat));
		rs->Release();
	}
	bind->Release();
	return ok;
}

bool cPkgSqlite::Pack(std::string dir, std::string sfile, const std::vector<std::string>& igno, std::string ver)
{
	Open(sfile, false);
	m_pDb->Optimize(O_Fast);
	aFileEntry lst;
	CrossFileOP::FindFile(stFileFindParam(dir.c_str(), "*.*", true, false), lst);
	for (aFileEntry::iterator it = lst.begin(); it != lst.end(); it++) {
		string sFile = it->sFullName;
		bool bFoundIgno = false;
		for (std::vector<std::string>::const_iterator it1 = igno.begin(); it1 != igno.end(); it1++) {
			if (sFile.find(*it1) == 0) {
				bFoundIgno = true;
				break;
			}
		}
		if (bFoundIgno)
			continue;
		ifstream ifs;
		ifs.open(it->sFullName, std::ios::in|std::ios::binary);
		if (ifs.is_open()) {
			ifstream::pos_type old = ifs.tellg();
			ifs.seekg(0, std::ios_base::end);
			ui32 len = (ui32)ifs.tellg();
			ifs.seekg(old, std::ios_base::beg);
			if (len > 0) {
				string buf;
				buf.resize(len);
				ifs.read((i8*)buf.data(), len);
				string sFile1 = sFile.substr(dir.length()+1);
				if (UpdateFile(sFile1.c_str(), buf.data(), len))
					LOk << sFile << " ok." << LEnd;
				else
					LError << sFile << " error." << LEnd;
			}
		}
	}
	Close();
	return true;
}

bool cPkgSqlite::UpdateFile(std::string sFile, cpvd dat, ui32 len)
{
	return UpdateFileFromCrc(FilePathCrc32Easy(sFile.c_str()), dat, len);
}

bool cPkgSqlite::UpdateFileFromCrc(ui32 crc, cpvd dat, ui32 len)
{
	if (m_pDb == nullptr)
		return false;
	std::string data((i8*)dat, len);
	ui32 datacrc = BufferCrc32Easy(dat, len);
	std::string data_comp;
	if (C_OK != Compress(data, data_comp))
		return false;
	iDbBind* bind = m_pDb->BindSql("replace into tbl_files(crc,lenfile,len,datacrc,data) values(?,?,?,?,?)");
	if (bind == nullptr)
		return false;
	bind->BindInt64(crc, 1);
	bind->BindInt32(len, 2);
	bind->BindInt32(data_comp.size(), 3);
	bind->BindInt64(datacrc, 4);
	bind->BindBuffer(data_comp, 5);
	bool bOk = bind->Exce();
	bind->Release();
	return bOk;
}

bool cPkgSqlite::UpdateFileFromCrc2(const std::vector<stPkgRead2>& crcs)
{
	if (m_pDb == nullptr)
		return false;
	if (!m_pDb->ExceSql("BEGIN TRANSACTION"))
		return false;
	for (std::vector<stPkgRead2>::const_iterator cit = crcs.begin(); cit != crcs.end(); cit++) {
		const stPkgRead2& pr = *cit;
		iDbBind* bind = m_pDb->BindSql("replace into tbl_files(crc,lenfile,len,datacrc,data) values(?,?,?,?,?)");
		if (bind == nullptr)
			return false;
		bind->BindInt64(pr.crc, 1);
		bind->BindInt32(pr.lenfile, 2);
		bind->BindInt32(pr.data.size(), 3);
		bind->BindInt64(pr.datacrc, 4);
		bind->BindBuffer(pr.data, 5);
		bool bOk = bind->Exce();
		bind->Release();
		if (!bOk)
			return false;
	}
	return m_pDb->ExceSql("COMMIT TRANSACTION");
}

void cPkgSqlite::BeginTmpFile()
{
	if (m_pDb == nullptr)
		return;
	m_pDb->ExceSql("delete from tbl_tmp_files");
}
bool cPkgSqlite::UpdateTmpFileFromCrc(const std::vector<stPkgRead2>& crcs)
{
	if (m_pDb == nullptr)
		return false;
	if (!m_pDb->ExceSql("BEGIN TRANSACTION"))
		return false;
	for (std::vector<stPkgRead2>::const_iterator cit = crcs.begin(); cit != crcs.end(); cit++) {
		const stPkgRead2& pr = *cit;
		iDbBind* bind = m_pDb->BindSql("replace into tbl_tmp_files(crc,lenfile,len,datacrc,data) values(?,?,?,?,?)");
		if (bind == nullptr)
			return false;
		bind->BindInt64(pr.crc, 1);
		bind->BindInt32(pr.lenfile, 2);
		bind->BindInt32(pr.data.size(), 3);
		bind->BindInt64(pr.datacrc, 4);
		bind->BindBuffer(pr.data, 5);
		bool bOk = bind->Exce();
		bind->Release();
		if (!bOk)
			return false;
	}
	return m_pDb->ExceSql("COMMIT TRANSACTION");
}
void cPkgSqlite::EndTmpFile()
{
	if (m_pDb == nullptr)
		return;
	m_pDb->ExceSql("replace into tbl_files select * from tbl_tmp_files");
	m_pDb->ExceSql("delete from tbl_tmp_files");
}

void cPkgSqlite::RemoveFile(std::string sFile)
{
	if (m_pDb == nullptr)
		return;
	ui32 crc = FilePathCrc32Easy(sFile.c_str());
	iDbBind* bind = m_pDb->BindSql("delete from tbl_files where crc=?");
	if (bind) {
		bind->BindInt64(crc, 1);
		bind->Exce();
		bind->Release();
	}
}

void cPkgSqlite::GetFileCrc(mapFileCrc& m)
{
	if (m_pDb == nullptr)
		return;
	iDbRecordSet* rs = m_pDb->ExceSqlResult("select crc,len,datacrc from tbl_files");
	if (rs == nullptr)
		return;
	while(true) {
		stFileCrc fc;
		ui32 crc = (ui32)rs->GetFieldInt64(0);
		fc.dataLen = (ui32)rs->GetFieldInt32(1);
		fc.dataCrc = (ui32)rs->GetFieldInt64(2);
		m[crc] = fc;
		if (!rs->Step())
			break;
	}
	rs->Release();
}

void cPkgSqlite::SetVersion(std::string s)
{
	m_pDb->ExceSql("delete from tbl_version");
	m_pDb->ExceSql("insert into tbl_version(ver) values('"+s+"')");
}

std::string cPkgSqlite::GetVersion()
{
	iDbRecordSet* rs = m_pDb->ExceSqlResult("select * from tbl_version");
	if (rs) {
		std::string v = rs->GetFieldString(0);
		rs->Release();
		return v;
	}
	return "";
}

void cPkgSqlite::ClearData()
{
	m_pDb->ExceSql("delete from tbl_files");
}

iPkg* iPkg::Create(ePkgType pt)
{
	switch (pt) {
	case cm::PT_Normal:
		return new cPkgNormal();
	case cm::PT_Sqlite:
		return new cPkgSqlite();
	default:
		return nullptr;
	}
}

}
