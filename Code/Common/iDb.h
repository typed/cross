#ifndef cross_common_idb_h
#define cross_common_idb_h

//非线程安全

#include "Common.h"
#include "CrossMemory.h"
#include <string>

namespace cross
{

class iDbRecordSet
{
public:
	virtual void Release() = 0;
	virtual bool Step() = 0;
	virtual cpstr GetFieldString(i32) = 0;
	virtual i32 GetFieldInt32(i32) = 0;
	virtual f32 GetFieldFloat32(i32) = 0;
	virtual i64 GetFieldInt64(i32) = 0;
	virtual f64 GetFieldFloat64(i32) = 0;
	virtual std::string GetFieldBuffer(i32) = 0;
	virtual i32 GetFieldType(i32) = 0;
	virtual std::string GetFieldName(i32) = 0;
	virtual i32 GetFieldCount() = 0;
};

class iDbBind
{
public:
	virtual void Release() = 0;
	virtual bool Exce() = 0;
	virtual iDbRecordSet* ExceResult() = 0;
	virtual bool BindString(std::string, i32) = 0;
	virtual bool BindInt32(i32, i32) = 0;
	virtual bool BindFloat32(f32, i32) = 0;
	virtual bool BindInt64(i64, i32) = 0;
	virtual bool BindFloat64(f64, i32) = 0;
	virtual bool BindBuffer(std::string, i32) = 0;
};

enum eOptimize
{
	O_Safe,
	O_Normal,
	O_Fast
};

enum eDbType
{
	DT_Sqlite,
	DT_MySql,
	DT_SqlServer,
	DT_LevelDb,
};

class iDb
{
public:

	static iDb* Create(eDbType);

	virtual void Release() = 0;
	virtual cpstr GetLastError() const = 0;

	virtual bool OpenDb(std::string) {return false;}
	virtual bool OpenDbInMemory() {return false;}
	virtual bool IsOpened() {return false;}
	virtual void CloseDb() {}

	//only sqldb
	virtual bool ExceSql(std::string) {return false;}
	virtual iDbRecordSet* ExceSqlResult(std::string) {return nullptr;}
	virtual iDbBind* BindSql(std::string) {return 0;}
	virtual void Optimize(eOptimize) {}

	//only sqlite
	virtual void Shell(i32 argc, i8** argv) {}
	virtual void CreateDb(std::string) {}
	virtual void Backup(std::string) {}
	virtual bool FromDB(std::string) {return false;}
	virtual void Backup2(std::string) {}
	virtual void Backup2Process(b8 bTrace, i32 pages) {}

	//kv数据
	virtual bool Set(std::string vKey, std::string vValue) {return false;}
	virtual bool SetSync(std::string vKey, std::string vValue) {return Set(vKey, vValue);}
	virtual bool Get(std::string vKey, std::string& vValue) {return false;}
	virtual bool Del(std::string vKey) {return false;}
};

}

#endif