#ifndef _cross_db_cdbsqlite_h
#define _cross_db_cdbsqlite_h

extern "C"
{
#include "Sqlite/sqlite3.h"
int sqlite_main(int argc, char **argv);
}
#include "iDb.h"
#include <map>
#include <vector>

namespace cross
{

class cDbSqlite;

class cDbSqliteRecordSet :
	public iDbRecordSet
{
public:
	cDbSqliteRecordSet(cDbSqlite*, sqlite3_stmt* p = nullptr);
	~cDbSqliteRecordSet();
	void Release();
	bool Step();
	cpstr GetFieldString(i32);
	i32 GetFieldInt32(i32);
	f32 GetFieldFloat32(i32);
	i64 GetFieldInt64(i32);
	f64 GetFieldFloat64(i32);
	std::string GetFieldBuffer(i32);
	i32 GetFieldType(i32);
	std::string GetFieldName(i32);
	i32 GetFieldCount();
	sqlite3_stmt*				m_pStm;
	cDbSqlite*					m_pDb;
	b8							m_bReleaseStm;
};

class cDbSqliteBind :
	public iDbBind
{
public:
	cDbSqliteBind(cDbSqlite*);
	~cDbSqliteBind();
	void Release();
	bool Exce();
	iDbRecordSet* ExceResult();
	bool BindString(std::string, i32);
	bool BindInt32(i32, i32);
	bool BindFloat32(f32, i32);
	bool BindInt64(i64, i32);
	bool BindFloat64(f64, i32);
	bool BindBuffer(std::string, i32);
	sqlite3_stmt*				m_pStm;
	std::map<i32, std::string>	m_mapBuffer;
	cDbSqlite*					m_pDb;
};

class cDbSqlite :
	public iDb
{
public:
	cDbSqlite();
	~cDbSqlite();
	void Release() {delete this;}
	cpstr GetLastError() const {return m_sLastError.c_str();}
	bool OpenDb(std::string);
	bool OpenDbInMemory();
	bool IsOpened();
	void CloseDb();
	bool ExceSql(std::string);
	iDbRecordSet* ExceSqlResult(std::string);
	iDbBind* BindSql(std::string);
	void Optimize(eOptimize);
	void Shell(i32 argc, i8** argv);
	void CreateDb(std::string);
	void Backup(std::string);
	bool FromDB(std::string);
	void Backup2(std::string);
	void Backup2Process(b8 bTrace, i32 pages);

	bool Set(std::string vKey, std::string vValue);
	bool Get(std::string vKey, std::string& vValue);
	bool Del(std::string vKey);

	void*			m_pDb;
	std::string		m_sLastError;
	sqlite3_backup*	m_backupObj;
	sqlite3*		m_backupDb;
};

}

#endif