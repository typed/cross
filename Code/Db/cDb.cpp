#include "iDb.h"
#include "cDbSqlite.h"
#include "cDbLevelDb.h"

namespace cross
{

iDb* iDb::Create(eDbType dt)
{
	switch(dt)
	{
	case DT_Sqlite:
		return new cDbSqlite();
#if USE_LEVELDB
	case DT_LevelDb:
		return new cDbLevelDb();
#endif
	default:
		return 0;
	}
}

}