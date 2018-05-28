#ifndef craknetcsclienttcp_h
#define craknetcsclienttcp_h

#include "../../inc/iNet.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/BitStream.h"
#include "raknet/TCPInterface.h"

#include "RakNetConfig.h"

namespace cross
{

class cRakNetCSClientTCP :
	public iCSClient
{
public:
	cRakNetCSClientTCP();
	~cRakNetCSClientTCP();
	bool Init(ui32 nTimeout, b8 bCheckTime);
	void Release();
	bool Connect(cpstr ip, ui16 port);
	bool IsConnected();
	void AutoConnect();
	void NotAutoConnect();
	bool Disconnect();
	void RunOnce();
	void SetUserData(i32 ud) {m_userdata = ud;}
	i32 GetUserData() const {return m_userdata;}
	bool Send(cpvd, ui32);
	void SetCallback(iCSClientCallback* p) {m_pCallback = p;}
	i32 GetPing() {return 0;}
	void SendPackage(ui32 waittime) {}
	void RecvPackage(i8* pData, ui32 len) {}
private:
	void AutoReconnect();
	iCSClientCallback*					m_pCallback;
	RakNet::TCPInterface*				m_pTcp;
	RakNet::SystemAddress				m_server;
	std::string							m_ip;
	ui16								m_port;
	RakNet::BitStream					m_package;
	RakNet::Time						m_time;
	RakNet::Time						m_autoreconnecttime;
	b8									m_bTrace;
	b8									m_bCompress;
	b8									m_bAutoReconnect;
	std::vector< std::vector<i8> >		m_aSend;
	i32									m_userdata;
	stTCPPack							m_pack;
};

}

#endif