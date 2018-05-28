#include "cRakNetCSServerTCP.h"
#include "Common/iCompress.h"
#include "Common/CrossString.h"

using namespace std;
using namespace cross;
using namespace RakNet;

namespace cross
{

ui32 cRakNetCSServerTCP::s_id = 1;

cRakNetCSServerTCP::cRakNetCSServerTCP()
{
	m_bTrace = false;
	m_bCompress = true;
	m_pCallback = 0;
	m_pTcp = 0;
}

cRakNetCSServerTCP::~cRakNetCSServerTCP()
{
}

bool cRakNetCSServerTCP::Init(cpstr sBind, ui16 nMaxConnect, ui32 nTimeout)
{
	m_pTcp = TCPInterface::GetInstance();
	ui16 maxconnect = (nMaxConnect ? nMaxConnect : c_ui16ServerMaxConnect);
	vector< pair<string, ui16> > aSad;
	if (!StrToAddress(aSad, sBind))
		return false;
	return m_pTcp->Start(aSad[0].second, maxconnect);
}

void cRakNetCSServerTCP::Release()
{
	m_packs.clear();
	m_mapLinkID_To_SystemAddress.clear();
	m_mapSystemAddress_To_LinkID.clear();
	if (m_pTcp) {
		m_pTcp->Stop();
		TCPInterface::DestroyInstance(m_pTcp);
		m_pTcp = 0;
	}
	delete this;
}

bool cRakNetCSServerTCP::SendAll(cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0)
		return false;
	for (mapLinkID_To_SystemAddress::iterator it = m_mapLinkID_To_SystemAddress.begin(); it != m_mapLinkID_To_SystemAddress.end(); it++)
		Send(it->first, buf, len);
	return true;
}

bool cRakNetCSServerTCP::Disconnect(ui32 id)
{
	m_pTcp->CloseConnection(ToSA(id));
	return true;
}

bool cRakNetCSServerTCP::Send(ui32 id, cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0 || len >= 1048576)
		return false;
	stTCPPack pk;
	pk.header.lenData = len;
	pk.data.append((cpstr)&pk.header, sizeof(pk.header));
	pk.data.append((cpstr)buf, len);
	m_pTcp->Send(pk.data.c_str(), pk.data.size(), ToSA(id), false);
	return true;
}

bool cRakNetCSServerTCP::GetLinkIpPort(ui32 id, pstr ip, ui16& port)
{
	RakNet::SystemAddress sa = ToSA(id);
	if (sa == UNASSIGNED_SYSTEM_ADDRESS)
		return false;
	CrossString::StrCpy(ip, sa.ToString(false));
	port = sa.GetPort();
	return true;
}

void cRakNetCSServerTCP::RunOnce()
{
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasNewIncomingConnection();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			ui32 linkid = s_id++;
			m_mapSystemAddress_To_LinkID[sa] = linkid;
			m_mapLinkID_To_SystemAddress[linkid] = sa;
			NETTRACE << "HasNewIncomingConnection " << sa.ToString(true) << LEnd;
			if (m_pCallback)
				m_pCallback->OnConnect(linkid, m_userdata);
		} else
			break;
	}
	for (;;) {
		RakNet::SystemAddress sa = m_pTcp->HasLostConnection();
		if (sa != UNASSIGNED_SYSTEM_ADDRESS) {
			ui32 linkid = ToLinkID(sa);
			m_mapLinkID_To_SystemAddress.erase(linkid);
			m_mapSystemAddress_To_LinkID.erase(sa);
			m_packs.erase(linkid);
			NETTRACE << "HasLostConnection " << sa.ToString(true) << LEnd;
			if (m_pCallback)
				m_pCallback->OnDisconnect(linkid, m_userdata);
		} else
			break;
	}
	ui32 hsz = sizeof(stTCPHeader);
	for (Packet* packet = m_pTcp->Receive(); packet; m_pTcp->DeallocatePacket(packet), packet = m_pTcp->Receive()) {
		NETTRACE << packet->systemAddress.ToString(true) << " " << packet->length << LEnd;
		ui32 linkid = ToLinkID(packet->systemAddress);
		stTCPPack& pk = m_packs[linkid];
		i8* pData = (i8*)packet->data;
		ui32 nLen = packet->length;
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
					m_pCallback->OnRecv(linkid, pk.data.c_str(), pk.data.size(), m_userdata);
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
}

ui32 cRakNetCSServerTCP::ToLinkID(const RakNet::SystemAddress& sa)
{
	mapSystemAddress_To_LinkID::iterator it = m_mapSystemAddress_To_LinkID.find(sa);
	if (it == m_mapSystemAddress_To_LinkID.end())
		return 0;
	return it->second;
}

const RakNet::SystemAddress& cRakNetCSServerTCP::ToSA(ui32 linkid)
{
	mapLinkID_To_SystemAddress::iterator it = m_mapLinkID_To_SystemAddress.find(linkid);
	if (it == m_mapLinkID_To_SystemAddress.end())
		return UNASSIGNED_SYSTEM_ADDRESS;
	return it->second;
}

}