#ifndef craknetcsservertcp_h
#define craknetcsservertcp_h

#include "../../inc/iNet.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/BitStream.h"
#include "raknet/TCPInterface.h"

#include "RakNetConfig.h"
#include <map>

namespace cross
{

typedef std::map<ui32, RakNet::SystemAddress> mapLinkID_To_SystemAddress;
typedef std::map<RakNet::SystemAddress, ui32> mapSystemAddress_To_LinkID;

class cRakNetCSServerTCP :
	public iCSServer
{
public:
	cRakNetCSServerTCP();
	~cRakNetCSServerTCP();
	bool Init(cpstr sBind, ui16 nMaxConnect, ui32 nTimeout);
	void Release();
	bool Send(ui32, cpvd, ui32);
	bool SendAll(cpvd, ui32);
	bool Disconnect(ui32);
	void DisconnectAll() {}
	i32 GetPing(ui32) {return -1;}
	bool GetLinkIpPort(ui32, pstr, ui16&);
	void SetCallback(iCSServerCallback* p) {m_pCallback = p;}
	void RunOnce();
	void SetUserData(i32 ud) {m_userdata = ud;}
	i32 GetUserData() const {return m_userdata;}
	ui32 ToLinkID(const RakNet::SystemAddress&);
	const RakNet::SystemAddress& ToSA(ui32 linkid);
	void CloseBroadcast() {}
	iCSServerCallback*					m_pCallback;
	RakNet::TCPInterface*				m_pTcp;
	i32									m_userdata;
	b8									m_bTrace;
	b8									m_bCompress;
	mapLinkID_To_SystemAddress			m_mapLinkID_To_SystemAddress;
	mapSystemAddress_To_LinkID			m_mapSystemAddress_To_LinkID;
	std::map<ui32,stTCPPack>			m_packs;
	static ui32							s_id;
};

}


#endif