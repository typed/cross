//程序名称：Manager.h
//功能描述：管理器
//开发人：	龚晓剑
//开发时间：2011-12-7
//版本：1.0

//修改者：	龚晓剑
//修改时间：2014-1-8
//修改说明：
//版本：2.0

#ifndef manager_h
#define manager_h

#include "CommonDef.h"
#include "iCrc.h"
#include "iReference.h"
#include "cGuid.h"
#include <vector>
#include <map>

namespace cross
{

class cTManager;

class cTItem :
	public iReference
{
public:
	inline cTItem(ui32, cTManager*);
	virtual inline ~cTItem() {}
	inline ui32 GetCrc() const {return m_crc;}
	inline void OnDestroy();
	inline cTManager* GetMgr() {return m_pMgr;}
private:
	ui32			m_crc;
	cTManager*		m_pMgr;
};
typedef cRefPtr<cTItem> cTItemPtr;
typedef std::map<ui32, cTItem*> mapTItem;

class cTManager
{
public:
	inline cTItem* CreateTItem();
	inline cTItem* GetTItem(cpstr);
protected:
	virtual cTItem* NewTItem(ui32, cpstr, cTManager*) = 0;
private:
	inline bool RegTItem(cTItem* p);
	inline bool UnregTItem(cTItem* p);
	mapTItem m_mapTItem;
	friend class cTItem;
};

inline cTItem::cTItem(ui32 crc, cTManager* pMgr) : m_crc(crc), m_pMgr(pMgr)
{
}

inline void cTItem::OnDestroy()
{
	GetMgr()->UnregTItem(this);
	delete this;
}

inline cTItem* cTManager::CreateTItem()
{
	cGuid guid;
	return GetTItem(guid.GetString().c_str());
}

inline cTItem* cTManager::GetTItem(cpstr name)
{
	ui32 crc = FilePathCrc32Easy(name);
	mapTItem::iterator it = m_mapTItem.find(crc);
	if (it != m_mapTItem.end())
		return it->second;
	cTItem* p = NewTItem(crc, name, this);
	if (p && RegTItem(p))
		return p;
	DEL(p);
	return NULL;
}

inline bool cTManager::RegTItem(cTItem* p)
{
	mapTItem::iterator it = m_mapTItem.find(p->GetCrc());
	if (it != m_mapTItem.end())
		return false;
	m_mapTItem[p->GetCrc()] = p;
	return true;
}

inline bool cTManager::UnregTItem(cTItem* p)
{
	mapTItem::iterator it = m_mapTItem.find(p->GetCrc());
	if (it == m_mapTItem.end())
		return false;
	m_mapTItem.erase(it);
	return true;
}

}
#endif
