#include "cRakNetTCPSocket.h"

using namespace std;
using namespace cross;
using namespace RakNet;

namespace cross
{

cRakNetTCPSocket::cRakNetTCPSocket()
{
	m_pTcp = 0;
	m_server = UNASSIGNED_SYSTEM_ADDRESS;
	m_bConnected = false;
	m_bTrace = false;
	m_baseLinkID = 1;
	m_funcCallback = nullptr;
}

cRakNetTCPSocket::~cRakNetTCPSocket()
{
	_Release();
}

bool cRakNetTCPSocket::Init(ui16 nBindPort, ui16 nMaxIncomingConnect, ui16 nMaxConnect)
{
	m_pTcp = TCPInterface::GetInstance();
	return m_pTcp->Start(nBindPort, nMaxIncomingConnect, nMaxConnect);
}

void cRakNetTCPSocket::_Release()
{
	if (m_pTcp) {
		m_pTcp->Stop();
		TCPInterface::DestroyInstance(m_pTcp);
		m_pTcp = 0;
	}
}

void cRakNetTCPSocket::Release()
{
	_Release();
	delete this;
}

void cRakNetTCPSocket::Connect(cpstr ip, ui16 port)
{
	m_ip = ip;
	m_port = port;
	m_pTcp->Connect(ip, port, false);
	NETTRACE << "connect to " << m_server.ToString(true) << LEnd;
}

bool cRakNetTCPSocket::IsConnected(ui32 linkid)
{
	if (linkid == 0)
		return m_bConnected;
	else
		return ToSA(linkid) != UNASSIGNED_SYSTEM_ADDRESS;
}

void cRakNetTCPSocket::Disconnect(ui32 linkid)
{
	m_aDisconnectLinkID.push_back(linkid);
}

void cRakNetTCPSocket::RunOnce()
{
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasNewIncomingConnection();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			ui32 linkid = m_baseLinkID++;
			m_mapSystemAddress_To_LinkID[sa] = linkid;
			m_mapLinkID_To_SystemAddress[linkid] = sa;
			NETTRACE << "HasNewIncomingConnection " << sa.ToString(true) << LEnd;
			if (m_funcCallback) {
				stTCPSocketCallbackParam cp;
				cp.type = TCPC_OnConnect;
				cp.linkid = linkid;
				cp.bConnected = true;
				m_funcCallback(cp);
			}
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasLostConnection();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			ui32 linkid = ToLinkID(sa);
			m_mapLinkID_To_SystemAddress.erase(linkid);
			m_mapSystemAddress_To_LinkID.erase(sa);
			if (sa == m_server) {
				m_bConnected = false;
			}
			NETTRACE << "HasLostConnection " << sa.ToString(true) << LEnd;
			if (m_funcCallback) {
				stTCPSocketCallbackParam cp;
				cp.type = TCPC_OnDisconnect;
				cp.linkid = linkid;
				m_funcCallback(cp);
			}
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasCompletedConnectionAttempt();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			m_bConnected = true;
			m_server = sa;
			NETTRACE << "HasCompletedConnectionAttempt " << sa.ToString(true) << LEnd;
			if (m_funcCallback) {
				stTCPSocketCallbackParam cp;
				cp.type = TCPC_OnConnect;
				cp.bConnected = true;
				m_funcCallback(cp);
			}
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasFailedConnectionAttempt();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			m_bConnected = false;
			NETTRACE << "HasFailedConnectionAttempt " << sa.ToString(true) << LEnd;
			if (m_funcCallback) {
				stTCPSocketCallbackParam cp;
				cp.type = TCPC_OnConnect;
				cp.bConnected = false;
				m_funcCallback(cp);
			}
		} else
			break;
	}
	for (Packet* packet = m_pTcp->Receive(); packet; m_pTcp->DeallocatePacket(packet), packet = m_pTcp->Receive()) {
		NETTRACE << packet->systemAddress.ToString(true) << " " << (cpstr)packet->data << LEnd;
		if (m_funcCallback) {
			stTCPSocketCallbackParam cp;
			cp.type = TCPC_OnRecv;
			cp.linkid = ToLinkID(packet->systemAddress);
			if (packet->length && packet->data)
				cp.data = std::string((cpstr)packet->data, packet->length);
			m_funcCallback(cp);
		}
	}
	for (std::vector<ui32>::iterator it = m_aDisconnectLinkID.begin(); it != m_aDisconnectLinkID.end(); it++) {
		ui32 linkid = *it;
		if (linkid == 0) {
			if (m_bConnected) {
				m_pTcp->CloseConnection(m_server);
				m_bConnected = false;
			}
		}
		else
			m_pTcp->CloseConnection(ToSA(linkid));
	}
	m_aDisconnectLinkID.clear();
}

void cRakNetTCPSocket::Send(cpvd buf, ui32 len, ui32 linkid)
{
	if (linkid == 0)
		m_pTcp->Send((cpstr)buf, len, m_server, false);
	else
		m_pTcp->Send((cpstr)buf, len, ToSA(linkid), false);
}

ui32 cRakNetTCPSocket::ToLinkID(const RakNet::SystemAddress& sa)
{
	mapSystemAddress_To_LinkID::iterator it = m_mapSystemAddress_To_LinkID.find(sa);
	if (it == m_mapSystemAddress_To_LinkID.end())
		return 0;
	return it->second;
}

const RakNet::SystemAddress& cRakNetTCPSocket::ToSA(ui32 linkid)
{
	mapLinkID_To_SystemAddress::iterator it = m_mapLinkID_To_SystemAddress.find(linkid);
	if (it == m_mapLinkID_To_SystemAddress.end())
		return UNASSIGNED_SYSTEM_ADDRESS;
	return it->second;
}

}