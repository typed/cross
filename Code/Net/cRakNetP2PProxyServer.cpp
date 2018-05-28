#include "cRakNetP2PProxyServer.h"
#include "RakNetConfig.h"
#include "Raknet/RakPeerInterface.h"
#include "Raknet/RakSleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Raknet/Kbhit.h"
#include "Raknet/MessageIdentifiers.h"
#include "Raknet/BitStream.h"
#include "Raknet/RakSleep.h"
#include "Raknet/SocketLayer.h"
#include "Raknet/Getche.h"
#include "Raknet/Gets.h"
#include "Raknet/CloudClient.h"

using namespace std;
using namespace RakNet;

namespace cross
{

cRakNetP2PProxyServer::cRakNetP2PProxyServer()
{
	m_server = UNASSIGNED_SYSTEM_ADDRESS;
	m_clientConnect = UNASSIGNED_SYSTEM_ADDRESS;
	m_serverguid = UNASSIGNED_RAKNET_GUID;
	m_time = 0;
	m_bTrace = false;
	m_pPeer = RakPeerInterface::GetInstance();
	m_pCloudPlugin = new CloudClient();
	m_pPeer->AttachPlugin(m_pCloudPlugin);
}

cRakNetP2PProxyServer::~cRakNetP2PProxyServer()
{
	m_pPeer->DetachPlugin(m_pCloudPlugin);
	RakPeerInterface::DestroyInstance(m_pPeer);
	SafeDelete(m_pCloudPlugin);
}

bool cRakNetP2PProxyServer::Init(cpstr sBind, cpstr sServer, cpstr sClientConnect)
{
	m_pPeer->SetMaximumIncomingConnections(c_ui16ServerMaxConnect);
	m_pPeer->SetTimeoutTime(c_tmsPeerTimeout, UNASSIGNED_SYSTEM_ADDRESS);
	vector< pair<string,ui16> > aServer;
	if (!StrToAddress(aServer, sServer))
		return false;
	m_server.FromStringExplicitPort(aServer[0].first.c_str(), aServer[0].second);
	vector< pair<string,ui16>> aClientConnect;
	if (!StrToAddress(aClientConnect, sClientConnect))
		return false;
	m_clientConnect.FromStringExplicitPort(aClientConnect[0].first.c_str(), aClientConnect[0].second);
	vector<SocketDescriptor> aSd;
	vector< pair<string,ui16>> aSad;
	if (!StrToAddress(aSad, sBind))
		return false;
	for (ui32 i = 0; i < (ui32)aSad.size(); i++)
		aSd.push_back(SocketDescriptor(aSad[i].second, aSad[i].first.c_str()));
	if (m_pPeer->Startup(c_ui16ServerMaxConnect, &aSd.front(), (ui32)aSd.size()) != RAKNET_STARTED)
		return false;
	return true;
}

void cRakNetP2PProxyServer::Release()
{
	m_pPeer->Shutdown(100);
	delete this;
}

void cRakNetP2PProxyServer::RunOnce()
{
	if (GetTime() - m_time >= c_tmConnectServerCheckTime) {
		ConnectionState cs = m_pPeer->GetConnectionState(m_server);
		switch(cs) {
		case IS_DISCONNECTED:
		case IS_NOT_CONNECTED:
			m_pPeer->Connect(m_server.ToString(false), m_server.GetPort(), 0, 0);
			break;
        default:
            break;
		}
		m_time = GetTime();
	}
	SWITCH_PACKAGE_BEGIN
	CASE_PACKAGE_BEGIN(ID_CONNECTION_REQUEST_ACCEPTED)
		m_serverguid = pPacket->guid;
		BitStream bs;
		bs.Write(m_pPeer->GetMyGUID());
		bs.Write(m_clientConnect);
		m_pCloudPlugin->Post(&GetCloudKeyProxyInfo(), bs.GetData(), bs.GetNumberOfBytesUsed(), m_serverguid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_ATTEMPT_FAILED)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NEW_INCOMING_CONNECTION)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_DISCONNECTION_NOTIFICATION)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_LOST)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYP2P_DATA_2_ACTION)
		BitStream bs(pPacket->data, pPacket->length, false);
		bs.IgnoreBytes(sizeof(MessageID));
		RakNetGUID guidFrom, guidTo;
		if (bs.Read(guidFrom) && bs.Read(guidTo))
			m_pPeer->Send((i8*)pPacket->data, pPacket->length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guidTo, false);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYP2P_DATA_2_SET)
		BitStream bs(pPacket->data, pPacket->length, false);
		bs.IgnoreBytes(sizeof(MessageID));
		RakNetGUID guidFrom, guidTo;
		if (bs.Read(guidFrom) && bs.Read(guidTo))
			m_pPeer->Send((i8*)pPacket->data, pPacket->length, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, guidTo, false);
	CASE_PACKAGE_END
	SWITCH_PACKAGE_END
}

}