#include "cRakNetCSClientTCP.h"
#include "../../inc/iCompress.h"

using namespace std;
using namespace cm;
using namespace RakNet;

namespace cm
{

cRakNetCSClientTCP::cRakNetCSClientTCP()
{
	m_bTrace = false;
	m_bCompress = true;
	m_pCallback = 0;
	m_pTcp = 0;
	m_time = 0;
	m_autoreconnecttime = 0;
	m_server = UNASSIGNED_SYSTEM_ADDRESS;
	m_bAutoReconnect = false;
	m_port = 0;
}

cRakNetCSClientTCP::~cRakNetCSClientTCP()
{
}

bool cRakNetCSClientTCP::Init(ui32 nTimeout, b8 bCheckTime)
{
	m_pTcp = TCPInterface::GetInstance();
	return m_pTcp->Start(0, 0, 1);
}

void cRakNetCSClientTCP::Release()
{
	Disconnect();
	if (m_pTcp) {
		m_pTcp->Stop();
		TCPInterface::DestroyInstance(m_pTcp);
		m_pTcp = 0;
	}
	delete this;
}

bool cRakNetCSClientTCP::Connect(cpstr ip, ui16 port)
{
	m_ip = ip;
	m_port = port;
	m_pTcp->Connect(m_ip.c_str(), m_port, false);
	NETTRACE << "connect to " << m_ip << ":" << m_port << LEnd;
	return true;
}

bool cRakNetCSClientTCP::IsConnected()
{
	return m_server != UNASSIGNED_SYSTEM_ADDRESS;
}

void cRakNetCSClientTCP::AutoConnect()
{
	m_bAutoReconnect = true;
}

void cRakNetCSClientTCP::NotAutoConnect()
{
	m_bAutoReconnect = false;
}

bool cRakNetCSClientTCP::Disconnect()
{
	m_ip = "";
	m_port = 0;
	if (m_pTcp && m_server != UNASSIGNED_SYSTEM_ADDRESS) {
		m_pTcp->CloseConnection(m_server);
		m_server = UNASSIGNED_SYSTEM_ADDRESS;
		return true;
	}
	return false;
}

void cRakNetCSClientTCP::RunOnce()
{
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasLostConnection();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			if (sa == m_server) {
				m_server = UNASSIGNED_SYSTEM_ADDRESS;
				m_pack.clear();
			}
			NETTRACE << "HasLostConnection " << sa.ToString(true) << LEnd;
			if (m_pCallback)
				m_pCallback->OnDisconnect(DT_Exception, m_userdata);
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasCompletedConnectionAttempt();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			m_server = sa;
			m_pack.clear();
			NETTRACE << "HasCompletedConnectionAttempt " << sa.ToString(true) << LEnd;
			if (m_pCallback)
				m_pCallback->OnConnect(CT_OK, m_userdata);
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasFailedConnectionAttempt();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			m_server = UNASSIGNED_SYSTEM_ADDRESS;
			m_pack.clear();
			NETTRACE << "HasFailedConnectionAttempt " << sa.ToString(true) << LEnd;
			if (m_pCallback)
				m_pCallback->OnConnect(CT_Error, m_userdata);
		} else
			break;
	}
	ui32 hsz = sizeof(stTCPHeader);
	for (Packet* packet = m_pTcp->Receive(); packet; m_pTcp->DeallocatePacket(packet), packet = m_pTcp->Receive()) {
		NETTRACE << packet->systemAddress.ToString(true) << " " << packet->length << LEnd;
		i8* pData = (i8*)packet->data;
		ui32 nLen = packet->length;
		stTCPPack& pk = m_pack;
		while (nLen != 0) {
			if (nLen > hsz) {
				stTCPHeader tmp;
				memcpy(&tmp, pData, hsz);
				if (tmp.guid == c_ui64TCPGUID) {
					if (tmp.lenData >= 1048576)
						break;
					pk.clear();
					pk.header = tmp;
					if (nLen > pk.header.lenData) {
						pk.data.append(pData + hsz, pk.header.lenData);
						pData = pData + hsz + pk.header.lenData;
						nLen = nLen - (hsz + pk.header.lenData);
					} else {
						pk.data.append(pData + hsz, nLen - hsz);
						pData = 0;
						nLen = 0;
					}
					if (pk.data.size() < pk.header.lenData) {
						//需要组包
						break;
					}
				} else {
					ui32 nLast = pk.header.lenData - pk.data.size();
					if (nLen > nLast) {
						pk.data.append(pData, nLast);
						pData = pData + nLast;
						nLen = nLen - nLast;
					} else {
						pk.data.append(pData, nLen);
						pData = 0;
						nLen = 0;
					}
				}
			} else {
				ui32 nLast = pk.header.lenData - pk.data.size();
				if (nLen > nLast) {
					pk.data.append(pData, nLast);
					pData = pData + nLast;
					nLen = nLen - nLast;
				} else {
					pk.data.append(pData, nLen);
					pData = 0;
					nLen = 0;
				}
			}
			if (pk.data.size() == pk.header.lenData) {
				if (m_pCallback) {
					m_pCallback->OnRecv(pk.data.c_str(), pk.data.size(), m_userdata);
				}
				pk.clear();
			} else if (pk.data.size() > pk.header.lenData) {
				//error
				pk.clear();
				LError << "recv buffer error." << LEnd;
				//assert(0);
				break;
			}
		}
	}
	AutoReconnect();
}

bool cRakNetCSClientTCP::Send(cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0 || len >= 1048576)
		return false;
	if (m_server == UNASSIGNED_SYSTEM_ADDRESS)
		return false;
	stTCPPack pk;
	pk.header.lenData = len;
	pk.data.append((cpstr)&pk.header, sizeof(pk.header));
	pk.data.append((cpstr)buf, len);
	m_pTcp->Send(pk.data.c_str(), pk.data.size(), m_server, false);
	return true;
}

void cRakNetCSClientTCP::AutoReconnect()
{
	if (m_bAutoReconnect && !m_ip.empty() && m_server == UNASSIGNED_SYSTEM_ADDRESS) {
		if (GetTime() - m_autoreconnecttime >= c_tmCheckAutoConnect) {
			m_autoreconnecttime = GetTime();
			m_server = UNASSIGNED_SYSTEM_ADDRESS;
			m_pTcp->Connect(m_ip.c_str(), m_port);
		}
	}
}

}