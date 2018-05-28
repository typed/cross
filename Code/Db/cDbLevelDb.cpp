#include "cDbLevelDb.h"

#if USE_LEVELDB

#include "leveldb.h"

using namespace std;

static const i32 c_iwrite_buffer_size = 32 * 1024 * 1024;

namespace cross
{

cDbLevelDb::cDbLevelDb()
{
	m_pLevelDb = 0;
}

cDbLevelDb::~cDbLevelDb()
{
}

bool cDbLevelDb::OpenDb(std::string s)
{
	leveldb::DB* db = NULL;
	leveldb::Options options;
	options.create_if_missing = true;
	options.write_buffer_size = c_iwrite_buffer_size;
	leveldb::Status status = leveldb::DB::Open(options, s, &db);
	if (!status.ok()) {
		m_sLastError = "db can not open.";
		return false;
	}
	m_pLevelDb = db;
	return true;
}

bool cDbLevelDb::IsOpened()
{
	return m_pLevelDb != 0;
}

void cDbLevelDb::CloseDb()
{
	if (m_pLevelDb) {
		delete (leveldb::DB*)m_pLevelDb;
		m_pLevelDb = 0;
	}
}

bool cDbLevelDb::Set(std::string vKey, std::string vValue)
{
	if (!IsOpened()) {
		m_sLastError = "db can not open.";
		return false;
	}
	if (vKey.empty()) {
		m_sLastError = "vKey.empty().";
		return false;
	}
	leveldb::DB* db = (leveldb::DB*)m_pLevelDb;
	leveldb::WriteOptions wo;
	wo.sync = false;
	return db->Put(wo, leveldb::Slice(vKey.data(), vKey.size()), leveldb::Slice(vValue.data(), vValue.size())).ok();
}

bool cDbLevelDb::SetSync(std::string vKey, std::string vValue)
{
	if (!IsOpened()) {
		m_sLastError = "db can not open.";
		return false;
	}
	if (vKey.empty()) {
		m_sLastError = "vKey.empty().";
		return false;
	}
	leveldb::DB* db = (leveldb::DB*)m_pLevelDb;
	leveldb::WriteOptions wo;
	wo.sync = true;
	return db->Put(wo, leveldb::Slice(vKey.data(), vKey.size()), leveldb::Slice(vValue.data(), vValue.size())).ok();
}

bool cDbLevelDb::Get(std::string vKey, std::string& vValue)
{
	if (!IsOpened()) {
		m_sLastError = "db can not open.";
		return false;
	}
	if (vKey.empty()) {
		m_sLastError = "vKey.empty().";
		return false;
	}
	leveldb::DB* db = (leveldb::DB*)m_pLevelDb;
	leveldb::ReadOptions ro;
	return db->Get(ro, leveldb::Slice(vKey.data(), vKey.size()), &vValue).ok();
}

bool cDbLevelDb::Del(std::string vKey)
{
	if (!IsOpened()) {
		m_sLastError = "db can not open.";
		return false;
	}
	if (vKey.empty()) {
		m_sLastError = "vKey.empty().";
		return false;
	}
	leveldb::DB* db = (leveldb::DB*)m_pLevelDb;
	leveldb::WriteOptions wo;
	wo.sync = false;
	return db->Delete(wo, leveldb::Slice(vKey.data(), vKey.size())).ok();
}

}

#endif