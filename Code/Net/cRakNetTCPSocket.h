#ifndef craknettcpsocket_h
#define craknettcpsocket_h

#include "iNet.h"
#include "Raknet/RakNetTypes.h"
#include "Raknet/RakPeerInterface.h"
#include "Raknet/BitStream.h"
#include "Raknet/TCPInterface.h"

#include "RakNetConfig.h"

namespace cross
{

typedef std::map<ui32, RakNet::SystemAddress> mapLinkID_To_SystemAddress;
typedef std::map<RakNet::SystemAddress, ui32> mapSystemAddress_To_LinkID;

class cRakNetTCPSocket :
	public iTCPSocket
{
public:
	cRakNetTCPSocket();
	~cRakNetTCPSocket();
	bool Init(ui16 nBindPort, ui16 nMaxIncomingConnect, ui16 nMaxConnect);
	void Release();
	void Connect(cpstr ip, ui16 port);
	bool IsConnected(ui32 linkid);
	void Disconnect(ui32 linkid);
	void RunOnce();
	void Send(cpvd, ui32, ui32 linkid);
	void SetCallback(funcTCPCallback p) {m_funcCallback = p;}
private:
	ui32 ToLinkID(const RakNet::SystemAddress&);
	const RakNet::SystemAddress& ToSA(ui32 linkid);
	void _Release();
	funcTCPCallback					m_funcCallback;
	RakNet::TCPInterface*			m_pTcp;
	RakNet::SystemAddress			m_server;
	std::string						m_ip;
	i32								m_port;
	b8								m_bTrace;
	b8								m_bConnected;
	std::vector<ui32>				m_aDisconnectLinkID;
	ui32							m_baseLinkID;
	mapLinkID_To_SystemAddress		m_mapLinkID_To_SystemAddress;
	mapSystemAddress_To_LinkID		m_mapSystemAddress_To_LinkID;
};

}

#endif