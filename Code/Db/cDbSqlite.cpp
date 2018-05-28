
#include "cDbSqlite.h"
#include "../../inc/CrossString.h"
#include "../../inc/iLog.h"
#include <fstream>

using namespace std;

namespace cm
{

cDbSqliteRecordSet::cDbSqliteRecordSet(cDbSqlite* pDb, sqlite3_stmt* p)
{
	m_pDb = pDb;
	m_pStm = p;
	m_bReleaseStm = ( p == nullptr );
}

cDbSqliteRecordSet::~cDbSqliteRecordSet()
{
	if (m_bReleaseStm && m_pStm)
		sqlite3_finalize(m_pStm);
}

bool cDbSqliteRecordSet::Step()
{
	m_pDb->m_sLastError.clear();
	int sqr = sqlite3_step(m_pStm);
	if (sqr == SQLITE_ERROR) {
		m_pDb->m_sLastError = sqlite3_errmsg((sqlite3*)m_pDb->m_pDb);
		return false;
	}
	b8 bNext = false;
	if (sqr == SQLITE_ROW)
		bNext = true;
	return bNext;
}

cpstr cDbSqliteRecordSet::GetFieldString(i32 idx)
{
	return (cpstr)sqlite3_column_text(m_pStm, idx);
}

i32 cDbSqliteRecordSet::GetFieldInt32(i32 idx)
{
	return (i32)sqlite3_column_int(m_pStm, idx);
}

f32 cDbSqliteRecordSet::GetFieldFloat32(i32 idx)
{
	return (f32)sqlite3_column_double(m_pStm, idx);
}

i64 cDbSqliteRecordSet::GetFieldInt64(i32 idx)
{
	return (i64)sqlite3_column_int64(m_pStm, idx);
}

f64 cDbSqliteRecordSet::GetFieldFloat64(i32 idx)
{
	return (f64)sqlite3_column_double(m_pStm, idx);
}

std::string cDbSqliteRecordSet::GetFieldBuffer(i32 idx)
{
	ui32 len = (ui32)sqlite3_column_bytes(m_pStm, idx);
	i8* buf = (i8*)sqlite3_column_blob(m_pStm, idx);
	return std::string(buf, len);
}

i32 cDbSqliteRecordSet::GetFieldType(i32 idx)
{
	return sqlite3_column_type(m_pStm, idx);
}

std::string cDbSqliteRecordSet::GetFieldName(i32 idx)
{
	return sqlite3_column_name(m_pStm, idx);
}

i32 cDbSqliteRecordSet::GetFieldCount()
{
	return sqlite3_column_count(m_pStm);
}

void cDbSqliteRecordSet::Release()
{
	delete this;
}

cDbSqliteBind::cDbSqliteBind(cDbSqlite* pDb)
{
	m_pDb = pDb;
	m_pStm = 0;
}

cDbSqliteBind::~cDbSqliteBind()
{
	if (m_pStm)
		sqlite3_finalize(m_pStm);
}

void cDbSqliteBind::Release()
{
	delete this;
}

bool cDbSqliteBind::Exce()
{
	m_pDb->m_sLastError.clear();
	int sqr = sqlite3_step(m_pStm);
	if (sqr == SQLITE_ERROR) {
		m_pDb->m_sLastError = sqlite3_errmsg((sqlite3*)m_pDb->m_pDb);
		return false;
	}
	return true;
}

iDbRecordSet* cDbSqliteBind::ExceResult()
{
	m_pDb->m_sLastError.clear();
	int sqr = sqlite3_step(m_pStm);
	if (sqr == SQLITE_ERROR) {
		m_pDb->m_sLastError = sqlite3_errmsg((sqlite3*)m_pDb->m_pDb);
		return nullptr;
	}
	if (sqr == SQLITE_DONE)
		return nullptr;
	return new cDbSqliteRecordSet(m_pDb, m_pStm);
}

bool cDbSqliteBind::BindString(std::string s, i32 i)
{
	m_mapBuffer[i] = s;
	std::string& sRef = m_mapBuffer[i];
	int br = sqlite3_bind_text(m_pStm, i, sRef.c_str(), sRef.length(), 0);
	return br == SQLITE_OK;
}

bool cDbSqliteBind::BindInt32(i32 v, i32 i)
{
	int br = sqlite3_bind_int(m_pStm, i, v);
	return br == SQLITE_OK;
}

bool cDbSqliteBind::BindFloat32(f32 v, i32 i)
{
	int br = sqlite3_bind_double(m_pStm, i, v);
	return br == SQLITE_OK;
}

bool cDbSqliteBind::BindInt64(i64 v, i32 i)
{
	int br = sqlite3_bind_int64(m_pStm, i, v);
	return br == SQLITE_OK;
}

bool cDbSqliteBind::BindFloat64(f64 v, i32 i)
{
	int br = sqlite3_bind_double(m_pStm, i, v);
	return br == SQLITE_OK;
}

bool cDbSqliteBind::BindBuffer(std::string buf, i32 i)
{
	m_mapBuffer[i] = buf;
	std::string& bufRef = m_mapBuffer[i];
	i8* pBuf = (i8*)bufRef.data();
	ui32 len = bufRef.size();
	int br = sqlite3_bind_blob(m_pStm, i, pBuf, len, 0);
	return br == SQLITE_OK;
}

cDbSqlite::cDbSqlite()
{
	m_pDb = 0;
	m_backupObj = nullptr;
	m_backupDb = nullptr;
}

cDbSqlite::~cDbSqlite()
{
	CloseDb();
}

bool cDbSqlite::OpenDb(std::string s)
{
	CloseDb();
	sqlite3* pDb = 0;
	i32 re = sqlite3_open(s.c_str(), &pDb);
	if (re != SQLITE_OK) {
		m_sLastError = CrossString::Format("Can't open database: %s.", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return false;
	}
	m_pDb = pDb;
	return true;
}

bool cDbSqlite::OpenDbInMemory()
{
	CloseDb();
	sqlite3* pDb = 0;
	i32 re = sqlite3_open(":memory:", &pDb);
	if (re != SQLITE_OK) {
		m_sLastError = CrossString::Format("Can't open database: %s.", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return false;
	}
	m_pDb = pDb;
	return true;
}

bool cDbSqlite::FromDB(std::string s)
{
	if (m_pDb == nullptr)
		return false;
	bool ok = false; 
	sqlite3* pFile;
	int rc = sqlite3_open(s.c_str(), &pFile);
	if (rc == SQLITE_OK) {
		sqlite3_backup* pBackup = sqlite3_backup_init((sqlite3*)m_pDb, "main", pFile, "main");  
		if (pBackup) {
			sqlite3_backup_step(pBackup, -1);
			sqlite3_backup_finish(pBackup);
			ok = true;
		}
		sqlite3_close(pFile);
	}
	return ok;
}

void cDbSqlite::Backup2(std::string sbak)
{
	if (!IsOpened())
		return;
	if (m_backupObj)
		return;
	sqlite3* db_source = (sqlite3*)m_pDb;
	sqlite3_open(sbak.c_str(), &m_backupDb);
	if (m_backupDb) {
		m_backupObj = sqlite3_backup_init(m_backupDb, "main", db_source, "main");
		if (m_backupObj == nullptr) {
			sqlite3_close(m_backupDb);
			m_backupDb = nullptr;
		} else {
			int maxpage = sqlite3_backup_pagecount(m_backupObj);
			LInfo << "need backup page " << maxpage << LEnd;
		}
	}
}

void cDbSqlite::Backup2Process(b8 bTrace, i32 pages)
{
	if (m_backupObj == nullptr || m_backupDb == nullptr)
		return;
	int rc = sqlite3_backup_step(m_backupObj, pages);
	if (bTrace) {
		int maxpage = sqlite3_backup_pagecount(m_backupObj);
		int curpage = sqlite3_backup_remaining(m_backupObj);
		LInfo << "backup process:" << (maxpage-curpage) << "/" << maxpage << LEnd;
	}
	if (rc == SQLITE_DONE) {
		sqlite3_backup_finish(m_backupObj);
		m_backupObj = nullptr;
		sqlite3_close(m_backupDb);
		m_backupDb = nullptr;
		LInfo << "Backup2 end." << LEnd;
	}
}

bool cDbSqlite::IsOpened()
{
	return m_pDb != 0;
}

void cDbSqlite::CloseDb()
{
	while(true) {
		Backup2Process(false,100);
		if (m_backupObj == nullptr)
			break;
	}
	if (m_pDb) {
		sqlite3_close((sqlite3*)m_pDb);
		m_pDb = 0;
	}
}

void cDbSqlite::Optimize(eOptimize o)
{
	if (!IsOpened()) {
		m_sLastError = "db is not opened.";
		return;
	}
	switch(o) {
	case O_Safe:
		sqlite3_exec((sqlite3*)m_pDb, "PRAGMA synchronous = FULL;", 0, 0, 0);break;
	case O_Normal:
		sqlite3_exec((sqlite3*)m_pDb, "PRAGMA synchronous = NORMAL;", 0, 0, 0);break;
	case O_Fast:
		sqlite3_exec((sqlite3*)m_pDb, "PRAGMA synchronous = OFF;", 0, 0, 0);break;
	}
}

bool cDbSqlite::ExceSql(std::string s)
{
	if (!IsOpened()) {
		m_sLastError = "db is not opened.";
		return false;
	}
	m_sLastError.clear();
	char* zErrMsg = 0;
	i32 rc = sqlite3_exec((sqlite3*)m_pDb, s.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		m_sLastError = CrossString::Format("SQL error: %s.", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;
}

iDbRecordSet* cDbSqlite::ExceSqlResult(std::string s)
{
	m_sLastError.clear();
	cDbSqliteRecordSet* pSet = new cDbSqliteRecordSet(this);
	i32 rc = sqlite3_prepare((sqlite3*)m_pDb, s.c_str(), s.length(), &pSet->m_pStm, 0);
	if (rc != SQLITE_OK) {
		pSet->Release();
		m_sLastError = CrossString::Format("SQL error: %s.", sqlite3_errstr(rc));
		return nullptr;
	}
	if (!pSet->Step()) {
		pSet->Release();
		return nullptr;
	}
	return pSet;
}

iDbBind* cDbSqlite::BindSql(std::string s)
{
	if (!IsOpened()) {
		m_sLastError = "db is not opened.";
		return 0;
	}
	m_sLastError.clear();
	cDbSqliteBind* pDsb = new cDbSqliteBind(this);
	i32 rc = sqlite3_prepare((sqlite3*)m_pDb, s.c_str(), s.length(), &pDsb->m_pStm, 0);
	if (rc != SQLITE_OK) {
		pDsb->Release();
		m_sLastError = "SQL error.";
		return 0;
	}
	return pDsb;
}

void cDbSqlite::Shell(i32 argc, i8** argv)
{
	sqlite_main(argc, argv);
}

void cDbSqlite::CreateDb(std::string s)
{
	OpenDb(s);
	iDbBind* pBind = BindSql("select count(*) from sqlite_master where type=? and name=?");
	if (pBind) {
		pBind->BindString("table", 1);
		pBind->BindString("tbl_kv", 2);
		iDbRecordSet* pSet = pBind->ExceResult();
		if (pSet) {
			i32 num = pSet->GetFieldInt32(0);
			if (num == 0)
				ExceSql("create table tbl_kv(key_bin BLOB primary key, value_bin BLOB)");
			pSet->Release();
		}
		pBind->Release();
	}
}

void cDbSqlite::Backup(std::string sfile)
{
	if (!IsOpened())
		return;
	sqlite3* db_source = (sqlite3*)m_pDb;
	sqlite3* db_des = nullptr;
	sqlite3_backup* bak = nullptr;
	sqlite3_open(sfile.c_str(), &db_des);
	if (db_des) {
		bak = sqlite3_backup_init(db_des, "main", db_source, "main");
		if (bak) {
			sqlite3_backup_step(bak, -1);
			sqlite3_backup_finish(bak);
		}
		sqlite3_close(db_des);
	}
}

bool cDbSqlite::Set(std::string vKey, std::string vValue)
{
	iDbBind* pBind = BindSql("replace into tbl_kv(key_bin, value_bin) values(?,?)");
	if (pBind) {
		pBind->BindBuffer(vKey, 1);
		pBind->BindBuffer(vValue, 2);
		b8 b = pBind->Exce();
		pBind->Release();
		return b;
	}
	return false;
}

bool cDbSqlite::Get(std::string vKey, std::string& vValue)
{
	iDbBind* pBind = BindSql("select value_bin from tbl_kv where key_bin=?");
	if (pBind) {
		pBind->BindBuffer(vKey, 1);
		iDbRecordSet* pSet = pBind->ExceResult();
		b8 bOk = false;
		if (pSet) {
			vValue = pSet->GetFieldBuffer(0);
			pSet->Release();
			bOk = true;
		}
		pBind->Release();
		return bOk;
	}
	return false;
}

bool cDbSqlite::Del(std::string vKey)
{
	iDbBind* pBind = BindSql("delete from tbl_kv where key_bin=?");
	if (pBind) {
		pBind->BindBuffer(vKey, 1);
		b8 b = pBind->Exce();
		pBind->Release();
		return b;
	}
	return true;
}

}