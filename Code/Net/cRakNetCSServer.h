#ifndef craknetcsserver_h
#define craknetcsserver_h

#include "../../inc/iNet.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/BitStream.h"

#include "RakNetConfig.h"
#include <map>

namespace cross
{

struct stLink
{
	ui32					m_id;
	RakNet::RakNetGUID		m_guid;
	RakNet::SystemAddress	m_address;
	stLink();
};

typedef std::map<RakNet::RakNetGUID, stLink*> mapServerLink;
typedef mapServerLink::iterator itMapServerLink;
typedef std::map<ui32, stLink*> mapServerLinkID;
typedef mapServerLinkID::iterator itMapServerLinkID;
typedef std::map<RakNet::RakNetGUID, RakNet::BitStream*> mapPackage;
typedef mapPackage::iterator itMapPackage;
typedef std::map<RakNet::RakNetGUID, RakNet::Time> mapTime;
typedef mapTime::iterator itMapTime;

class cRakNetCSServer :
	public iCSServer
{
public:
	cRakNetCSServer();
	~cRakNetCSServer();
	bool Init(cpstr sBind, ui16 nMaxConnect, ui32 nTimeout);
	void Release();
	bool Send(ui32, cpvd, ui32);
	bool SendAll(cpvd, ui32);
	bool Disconnect(ui32);
	void DisconnectAll();
	i32 GetPing(ui32);
	bool GetLinkIpPort(ui32, pstr, ui16&);
	void SetCallback(iCSServerCallback* p) {m_pCallback = p;}
	void RunOnce();
	void SetUserData(i32 ud) {m_userdata = ud;}
	i32 GetUserData() const {return m_userdata;}
	void RemoveLink(const RakNet::RakNetGUID& guid, bool bDisconnect = false);
	void RemoveLinkAll(bool bDisconnect = false);
	void SendPackage();
	void RecvPackage(ui32 id, i8* pData, ui32 len);
	void BroadcastClient();
	void CloseBroadcast() {m_bBroadcast = false;}
	stLink* FindLink(ui32 id);
	iCSServerCallback*					m_pCallback;
	RakNet::RakPeerInterface*			m_pPeer;
	mapServerLink						m_mapServerLink;
	mapServerLinkID						m_mapServerLinkID;
	mapPackage							m_mapPackage;
	mapTime								m_mapTime;
	RakNet::Time						m_broadcasttime;
	i32									m_userdata;
	bool								m_bTrace;
	bool								m_bCompress;
	bool								m_bBroadcast;
	static ui32							s_id;
};

}


#endif