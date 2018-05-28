#include "cRakNetP2PClient2.h"
#include "raknet/GetTime.h"
#include "../../inc/iCrc.h"

using namespace std;
using namespace RakNet;

namespace cross
{

cRakNetP2PClientStream2::cRakNetP2PClientStream2()
{}

cRakNetP2PClientStream2::cRakNetP2PClientStream2(ui8* p, ui32 len):m_bs(p, len, false)
{}

void cRakNetP2PClientStream2::Write(cpvd buf, ui32 len)
{
	m_bs.Write((i8*)buf, len);
}

bool cRakNetP2PClientStream2::Read(pvd buf, ui32 len)
{
	return m_bs.Read((i8*)buf, len);
}

ui32 cRakNetP2PClientStream2::GetCrc()
{
	if (m_bs.GetNumberOfBytesUsed() == 0)
		return 0;
	return BufferCrc32Easy(m_bs.GetData(), m_bs.GetNumberOfBytesUsed());
}

cRakNetP2PClient2::cRakNetP2PClient2()
{
	m_pCallback = 0;
	m_pCloudPlugin = new CloudClient();
	m_pNatPunchthroughPlugin = new NatPunchthroughClient();
	m_pPeer = RakPeerInterface::GetInstance();
	m_pPeer->AttachPlugin(m_pCloudPlugin);
	m_pPeer->AttachPlugin(m_pNatPunchthroughPlugin);
	m_id = 0;
	m_server = UNASSIGNED_SYSTEM_ADDRESS;
	m_proxyserverguid = UNASSIGNED_RAKNET_GUID;
	m_bTrace = false;
	m_uiCrc = 0;
	m_bSendGet = false;
	m_time = 0;
	m_tmCheckRoom = 0;
	//if (m_bTrace)
	//	LInfo << m_pPeer->GetMyGUID().ToString() << LEnd;
}

cRakNetP2PClient2::~cRakNetP2PClient2()
{
	m_pPeer->DetachPlugin(m_pNatPunchthroughPlugin);
	m_pPeer->DetachPlugin(m_pCloudPlugin);
	RakPeerInterface::DestroyInstance(m_pPeer);
	SafeDelete(m_pNatPunchthroughPlugin);
	SafeDelete(m_pCloudPlugin);
}

bool cRakNetP2PClient2::Init()
{
	m_pPeer->SetMaximumIncomingConnections(c_ui16ClientMaxConnect);
	m_pPeer->SetTimeoutTime(c_tmsPeerTimeout, UNASSIGNED_SYSTEM_ADDRESS);
	SocketDescriptor sd(0,0);
	if (RAKNET_STARTED == m_pPeer->Startup(c_ui16ClientMaxConnect+1, &sd, 1)) {
		//OpenUPNP(m_pPeer);
		return true;
	}
	return false;
}

void cRakNetP2PClient2::Release()
{
	Logout();
	m_pPeer->Shutdown(100);
	delete this;
}

void cRakNetP2PClient2::Login(ui32 id, cpstr ip, ui16 port)
{
	Logout();
	m_id = id;
	m_server = RaknetAddressMapping(ip, port);
	SendConnect(m_server);
}

void cRakNetP2PClient2::Logout()
{
	LeaveRoom();
	m_mapPlayerGUID.clear();
	m_mapGUIDToID.clear();
	m_mmRoom.clear();
	m_mapProxy.clear();
	m_id = 0;
	if (m_server != UNASSIGNED_SYSTEM_ADDRESS) {
		m_pPeer->CloseConnection(m_server, true);
		m_server = UNASSIGNED_SYSTEM_ADDRESS;
	}
}

void cRakNetP2PClient2::Connect(cpstr ip, ui16 port)
{
	SendConnect(RaknetAddressMapping(ip, port));
}

void cRakNetP2PClient2::Disconnect()
{
}

bool cRakNetP2PClient2::IsLoginOk()
{
	return m_server != UNASSIGNED_SYSTEM_ADDRESS;
}

void cRakNetP2PClient2::RunOnce()
{
	if (m_pPeer == 0)
		return;
	CheckServer();
	CheckProxy();
	for (itAMsg it = m_aMsg.begin(); it != m_aMsg.end(); it++) {
		const stMyMsg& mm = *it;
		switch(mm.type)
		{
		case MMT_Connect:
			m_pPeer->Connect(mm.sa.ToString(false), mm.sa.GetPort(), 0, 0); break;
		case MMT_OpenNAT:
			OpenNAT(mm.guid); break;
		default:
			break;
		}
	}
	m_aMsg.clear();
	SWITCH_PACKAGE_BEGIN
	CASE_PACKAGE_BEGIN(ID_IP_RECENTLY_CONNECTED)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_INCOMPATIBLE_PROTOCOL_VERSION)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_REQUEST_ACCEPTED)
		if (pPacket->systemAddress == m_server) {
			m_serverguid = pPacket->guid;
			UploadInfo();
			GetClients();
		}
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_ATTEMPT_FAILED)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_ALREADY_CONNECTED)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NO_FREE_INCOMING_CONNECTIONS)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NEW_INCOMING_CONNECTION)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_ADVERTISE_SYSTEM)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_DISCONNECTION_NOTIFICATION)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_LOST)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CLOUD_GET_RESPONSE)
		m_mapPlayerGUID.clear();
		m_mapGUIDToID.clear();
		m_mmRoom.clear();
		m_mapProxy.clear();
		CloudQueryResult cloudQueryResult;
		m_pCloudPlugin->OnGetReponse(&cloudQueryResult, pPacket);
		for (ui32 rowIndex = 0; rowIndex < cloudQueryResult.rowsReturned.Size(); rowIndex++) {
			CloudQueryRow* row = cloudQueryResult.rowsReturned[rowIndex];
			if (CloudKeyComp(row->key, GetCloudKeyClientInfo()) == 0) {
				BitStream bsIn(row->data, row->length, false);
				ui32 id;
				if (bsIn.Read(id)) {
					stPlayer& ply = m_mapPlayerGUID[id];
					ply.guid = row->clientGUID;
					m_mapGUIDToID[row->clientGUID] = id;
					ply.sa = row->clientSystemAddress;
					ply.sRoom.Clear();
					bsIn.Read(ply.sRoom);
					if (!ply.sRoom.IsEmpty()) {
						m_mmRoom.insert(mmRoom::value_type(ply.sRoom, id));
						if (ply.sRoom == m_sRoomName)
							m_bSendGet = true;
					}
				}
			}
			else if (CloudKeyComp(row->key, GetCloudKeyProxyInfo()) == 0) {
				BitStream bsIn(row->data, row->length, false);
				RakNetGUID guid;
				SystemAddress sa;
				if (bsIn.Read(guid) && bsIn.Read(sa)) {
					stProxy pro;
					pro.m_sa = RaknetAddressMapping(sa);
					pro.m_ping = 10000;
					m_mapProxy[guid] = pro;
				}
			}
		}
		DisplayPlayers();
		DisplayRooms();
		DisplayProxys();
		m_pCloudPlugin->DeallocateWithDefaultAllocator(&cloudQueryResult);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CLOUD_SUBSCRIPTION_NOTIFICATION)
		bool wasUpdated;
		RakNet::CloudQueryRow cloudQueryRow;
		m_pCloudPlugin->OnSubscriptionNotification(&wasUpdated, &cloudQueryRow, pPacket);
		if (CloudKeyComp(cloudQueryRow.key, GetCloudKeyClientInfo()) == 0) {
			BitStream bsIn(cloudQueryRow.data, cloudQueryRow.length, false);
			ui32 id;
			if (bsIn.Read(id)) {
				itMapPlayerGUID it = m_mapPlayerGUID.find(id);
				if (it != m_mapPlayerGUID.end()) {
					stPlayer& plyOld = it->second;
					ui32 n = (ui32)m_mmRoom.count(plyOld.sRoom);
					if (n) {
						itMmRoom it1 = m_mmRoom.find(plyOld.sRoom);
						for (ui32 i = 0; i < n; i++, it1++) {
							if (it1->second == id) {
								m_mmRoom.erase(it1);
								break;
							}
						}
					}
				}
				if (wasUpdated) {
					stPlayer& ply = m_mapPlayerGUID[id];
					ply.guid = cloudQueryRow.clientGUID;
					m_mapGUIDToID[cloudQueryRow.clientGUID] = id;
					ply.sa = cloudQueryRow.clientSystemAddress;
					ply.sRoom.Clear();
					bsIn.Read(ply.sRoom);
					if (!ply.sRoom.IsEmpty()) {
						m_mmRoom.insert(mmRoom::value_type(ply.sRoom, id));
						if (ply.sRoom == m_sRoomName)
							m_bSendGet = true;
					}
				}
				else {
					m_mapPlayerGUID.erase(id);
					m_mapGUIDToID.erase(cloudQueryRow.clientGUID);
				}
				DisplayPlayers();
				DisplayRooms();
			}
		}
		else if (CloudKeyComp(cloudQueryRow.key, GetCloudKeyProxyInfo()) == 0) {
			BitStream bsIn(cloudQueryRow.data, cloudQueryRow.length, false);
			RakNetGUID guid;
			SystemAddress sa;
			if (bsIn.Read(guid) && bsIn.Read(sa)) {
				if (wasUpdated) {
					stProxy pro;
					pro.m_sa = RaknetAddressMapping(sa);
					pro.m_ping = 10000;
					m_mapProxy[guid] = pro;
				}
				else
					m_mapProxy.erase(guid);
			}
			DisplayProxys();
		}
		m_pCloudPlugin->DeallocateWithDefaultAllocator(&cloudQueryRow);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NAT_TARGET_NOT_CONNECTED)
		OnNatPunchthroughError(pPacket->guid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NAT_TARGET_UNRESPONSIVE)
		OnNatPunchthroughError(pPacket->guid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NAT_CONNECTION_TO_TARGET_LOST)
		OnNatPunchthroughError(pPacket->guid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NAT_PUNCHTHROUGH_FAILED)
		OnNatPunchthroughError(pPacket->guid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NAT_PUNCHTHROUGH_SUCCEEDED)
		if (IsSameRoom(pPacket->guid))
			SendConnect(pPacket->systemAddress);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYP2P_DATA_2_ACTION)
		BitStream bs(pPacket->data, pPacket->length, false);
		bs.IgnoreBytes(sizeof(MessageID));
		RakNetGUID guidFrom, guidTo;
		if (bs.Read(guidFrom) && bs.Read(guidTo)) {
			ui32 id;
			if (GUIDToID(id, guidFrom)) {
				i8* p = (i8*)bs.GetData();
				p += BITS_TO_BYTES(bs.GetReadOffset());
				ui32 len = bs.GetNumberOfBytesUsed() - BITS_TO_BYTES(bs.GetReadOffset());
				m_pCallback->OnRecvAction(id, (cpbyte)p, len);
			}
		}
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYP2P_DATA_2_SET)
		cRakNetP2PClientStream2 stream(pPacket->data, pPacket->length);
		stream.m_bs.IgnoreBytes(sizeof(MessageID));
		RakNetGUID guidFrom, guidTo;
		if (stream.m_bs.Read(guidFrom) && stream.m_bs.Read(guidTo)) {
			ui32 id;
			if (GUIDToID(id, guidFrom))
				m_pCallback->OnSet(id, &stream);
		}
	CASE_PACKAGE_END
	SWITCH_PACKAGE_END
	CheckRoomOpenNAT();
	CheckSendRoom();
}

void cRakNetP2PClient2::SendInRoom(cpvd buf, ui32 len, bool bReliable, i8 msgid)
{
	if (!IsInRoom())
		return;
	if (buf == 0 || len == 0)
		return;
	ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
	if (n <= 1)
		return;
	vector<RakNetGUID> aGuid;
	itMmRoom it = m_mmRoom.find(m_sRoomName);
	for (ui32 i = 0; i < n; i++, it++) {
		RakNetGUID g;
		if (IDToGUID(g, it->second)) {
			if (m_pPeer->GetMyGUID() != g)
				aGuid.push_back(g);
		}
	}
	if (aGuid.empty())
		return;
	for (ui32 i = 0; i < aGuid.size(); i++) {
		BitStream bs;
		bs.Write((i8)msgid);
		bs.Write(m_pPeer->GetMyGUID());
		bs.Write(aGuid[i]);
		bs.Write((i8*)buf, len);
		ConnectionState cs = m_pPeer->GetConnectionState(aGuid[i]);
		if (cs == IS_CONNECTED)
			m_pPeer->Send(&bs, HIGH_PRIORITY, bReliable ? RELIABLE_ORDERED : RELIABLE_SEQUENCED, 0, aGuid[i], false);
		else {
			cs = m_pPeer->GetConnectionState(m_proxyserverguid);
			if (cs == IS_CONNECTED)
				m_pPeer->Send(&bs, HIGH_PRIORITY, bReliable ? RELIABLE_ORDERED : RELIABLE_SEQUENCED, 0, m_proxyserverguid, false);
			else {
				FindBaseProxy();
				if (m_proxyserverguid != UNASSIGNED_RAKNET_GUID)
					m_pPeer->Send(&bs, HIGH_PRIORITY, bReliable ? RELIABLE_ORDERED : RELIABLE_SEQUENCED, 0, m_proxyserverguid, false);
			}
		}
	}
}

const RakNetGUID* cRakNetP2PClient2::GetPlayerGUID(ui32 id) const
{
	citMapPlayerGUID it = m_mapPlayerGUID.find(id);
	if (it == m_mapPlayerGUID.end())
		return 0;
	return &it->second.guid;
}

void cRakNetP2PClient2::DisconnectAllPlayer()
{
	ui16 n = 0;
	m_pPeer->GetConnectionList(0, &n);
	if (n) {
		SystemAddress* asa = new SystemAddress[n];
		m_pPeer->GetConnectionList(asa, &n);
		ui32 sum = n;
		for (ui32 i = 0; i < sum; i++) {
			RakNetGUID guid = m_pPeer->GetGuidFromSystemAddress(asa[i]);
			if (guid != m_serverguid && m_mapProxy.find(guid) == m_mapProxy.end())
				m_pPeer->CloseConnection(asa[i], true);
		}
		delete[] asa;
	}
	m_mapNPT.clear();
}

void cRakNetP2PClient2::EnterRoom(cpstr sName)
{
	FindBaseProxy();
	LeaveRoom();
	m_sRoomName = sName;
	UploadInfo();
}

void cRakNetP2PClient2::LeaveRoom()
{
	DisconnectAllPlayer();
	m_sRoomName.Clear();
	UploadInfo();
}

cpstr cRakNetP2PClient2::GetRoomName()
{
	return m_sRoomName.C_String();
}

void cRakNetP2PClient2::TraceRoom()
{
}

bool cRakNetP2PClient2::IsInRoom() const
{
	return !m_sRoomName.IsEmpty();
}

bool cRakNetP2PClient2::IsHost() const
{
	if (!IsInRoom())
		return false;
	ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
	if (n == 0)
		return false;
	return m_id == GetHostID();
}

ui32 cRakNetP2PClient2::GetHostID() const
{
	if (!IsInRoom())
		return m_id;
	ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
	if (n == 0)
		return m_id;
	map<RakNetGUID, ui32> mapSoot;
	citMmRoom it = m_mmRoom.find(m_sRoomName);
	for (ui32 i = 0; i < n; i++, it++) {
		RakNetGUID guid;
		if (IDToGUID(guid, it->second))
			mapSoot[guid] = it->second;
	}
	if (mapSoot.empty())
		return m_id;
	return mapSoot.begin()->second;
}

void cRakNetP2PClient2::Action(cpvd buf, ui32 len)
{
	if (!IsInRoom())
		return;
	if (buf == 0 || len == 0)
		return;
	SendInRoom(buf, len, true, (i8)ID_MYP2P_DATA_2_ACTION);
}

i32 cRakNetP2PClient2::GetPing(ui32 id)
{
	if (id == m_id)
		return 0;
	itMapPlayerGUID it = m_mapPlayerGUID.find(id);
	if (it == m_mapPlayerGUID.end())
		return -1;
	if (IsConnected(id))
		return it->second.nPing;
	it->second.nPing = -1;
	return m_pPeer->GetLastPing(m_proxyserverguid);
}

bool cRakNetP2PClient2::IsConnected(ui32 id)
{
	const RakNetGUID* p = GetPlayerGUID(id);
	if (p == 0)
		return false;
	return m_pPeer->GetConnectionState(*p) == IS_CONNECTED;
}

bool cRakNetP2PClient2::IsServerConnected()
{
	return m_pPeer->GetConnectionState(m_server) == IS_CONNECTED;
}

void cRakNetP2PClient2::UploadInfo()
{
	BitStream bs;
	bs.Write(m_id);
	bs.Write(m_sRoomName);
	m_pCloudPlugin->Post(&GetCloudKeyClientInfo(), bs.GetData(), bs.GetNumberOfBytesUsed(), m_serverguid);
}

void cRakNetP2PClient2::GetClients()
{
	CloudQuery cloudQuery;
	cloudQuery.keys.Push(GetCloudKeyClientInfo(), _FILE_AND_LINE_);
	cloudQuery.keys.Push(GetCloudKeyProxyInfo(), _FILE_AND_LINE_);
	cloudQuery.subscribeToResults = true;
	m_pCloudPlugin->Get(&cloudQuery, m_serverguid);
}

void cRakNetP2PClient2::SendConnect(const RakNet::SystemAddress& sa)
{
	stMyMsg msg;
	msg.type = MMT_Connect;
	msg.sa = sa;
	m_aMsg.push_back(msg);
}

void cRakNetP2PClient2::SendOpenNAT(const RakNet::RakNetGUID& guid)
{
	stMyMsg msg;
	msg.type = MMT_OpenNAT;
	msg.guid = guid;
	m_aMsg.push_back(msg);
}

void cRakNetP2PClient2::OpenNAT(const RakNet::RakNetGUID& guid)
{
	itMapNatPunchthroughTime it = m_mapNPT.find(guid);
	if (it != m_mapNPT.end()) {
		if (GetTime() - it->second < c_tmNatPunchthroughTime2)
			return;
	}
	m_pNatPunchthroughPlugin->OpenNAT(guid, m_server);
	m_mapNPT[guid] = GetTime();
}

void cRakNetP2PClient2::DisplayPlayers()
{
	if (!m_bTrace)
		return;
	//for (itMapPlayerGUID it = m_mapPlayerGUID.begin(); it != m_mapPlayerGUID.end(); it++) {
	//	const stPlayer& ply = it->second;
		//LInfo << it->first << " " << ply.guid.ToString() << " " << ply.sa.ToString() << " " << ply.sRoom.C_String() << " " << ply.nPing << LEnd;
	//}
}

void cRakNetP2PClient2::DisplayRooms()
{
	if (!m_bTrace)
		return;
	//RakString sPre;
	//for (itMmRoom it = m_mmRoom.begin(); it != m_mmRoom.end(); it++) {
	//	if (sPre == it->first)
	//		LInfo << " " << it->second;
	//	else {
	//		if (it != m_mmRoom.begin())
	//			LInfo << LEnd;
	//		LInfo << it->first << " " << it->second;
	//	}
	//	sPre = it->first;
	//}
	//if (!m_mmRoom.empty())
	//	LInfo << LEnd;
}

void cRakNetP2PClient2::DisplayProxys()
{
	if (!m_bTrace)
		return;
	//for (itMapProxy it = m_mapProxy.begin(); it != m_mapProxy.end(); it++)
	//	LInfo << it->first.ToString() << " " << it->second.m_sa.ToString() <<LEnd;
}

void cRakNetP2PClient2::CheckRoomOpenNAT()
{
	if (!IsInRoom())
		return;
	if (GetTime() - m_tmCheckRoom > c_tmCheckRoomOpenNAT) {
		ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
		if (n) {
			itMmRoom it = m_mmRoom.find(m_sRoomName);
			for (ui32 i = 0; i < n; i++, it++) {
				if (it->second != m_id) {
					RakNetGUID guid;
					if (!IDToGUID(guid, it->second))
						continue;
					if (IsConnected(it->second))
						m_mapPlayerGUID[it->second].nPing = m_pPeer->GetLastPing(guid);
					else {
						m_mapPlayerGUID[it->second].nPing = -1;
						SendOpenNAT(guid);
					}
				}
			}
		}
		m_tmCheckRoom = GetTime();
	}
}

void cRakNetP2PClient2::CheckSendRoom()
{
	if (!IsInRoom())
		return;
	ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
	if (n <= 1)
		return;
	cRakNetP2PClientStream2 stream;
	m_pCallback->OnGet(m_id, &stream);
	ui32 crc = stream.GetCrc();
	if (m_bSendGet || crc != m_uiCrc) {
		SendInRoom(stream.m_bs.GetData(), stream.m_bs.GetNumberOfBytesUsed(), false, (i8)ID_MYP2P_DATA_2_SET);
		m_uiCrc = crc;
		m_bSendGet = false;
	}
}

void cRakNetP2PClient2::CheckProxy()
{
	if (GetTime() - m_tmCheckProxy > c_tmCheckConnectProxy) {
		for (itMapProxy it = m_mapProxy.begin(); it != m_mapProxy.end(); it++) {
			stProxy& proxy = it->second;
			proxy.m_ping = 10000;
			ConnectionState cs = m_pPeer->GetConnectionState(it->first);
			switch(cs) {
			case IS_DISCONNECTED:
			case IS_NOT_CONNECTED:
				SendConnect(proxy.m_sa);
				break;
			case IS_CONNECTED:
				proxy.m_ping = m_pPeer->GetAveragePing(it->first);
				break;
            default:
                break;
			}
		}
		m_tmCheckProxy = GetTime();
	}
}

void cRakNetP2PClient2::FindBaseProxy()
{
	m_proxyserverguid = UNASSIGNED_RAKNET_GUID;
	multimap<i32, RakNetGUID> mBase;
	for (itMapProxy it = m_mapProxy.begin(); it != m_mapProxy.end(); it++)
		mBase.insert(multimap<i32, RakNetGUID>::value_type(it->second.m_ping, it->first));
	if (mBase.empty())
		return;
	m_proxyserverguid = mBase.begin()->second;
}

void cRakNetP2PClient2::CheckServer()
{
	if (GetTime() - m_time >= c_tmConnectServerCheckTime) {
		if (IsLoginOk()) {
			ConnectionState cs = m_pPeer->GetConnectionState(m_server);
			switch(cs) {
			case IS_DISCONNECTED:
			case IS_NOT_CONNECTED:
				SendConnect(m_server); break;
            default:
				break;
			}
		}
		m_time = GetTime();
	}
}

bool cRakNetP2PClient2::GUIDToID(ui32& id, const RakNet::RakNetGUID& guid) const
{
	citMapGUIDToID it = m_mapGUIDToID.find(guid);
	if (it == m_mapGUIDToID.end())
		return false;
	id = it->second;
	return true;
}

bool cRakNetP2PClient2::IDToGUID(RakNet::RakNetGUID& guid, ui32 id) const
{
	const RakNetGUID* p = GetPlayerGUID(id);
	if (p == 0)
		return false;
	guid = *p;
	return true;
}

bool cRakNetP2PClient2::IsSameRoom(const RakNet::RakNetGUID& guid)
{
	if (!IsInRoom())
		return false;
	ui32 id;
	if (!GUIDToID(id, guid))
		return false;
	ui32 n = (ui32)m_mmRoom.count(m_sRoomName);
	if (n) {
		itMmRoom it = m_mmRoom.find(m_sRoomName);
		for (ui32 i = 0; i < n; i++, it++)
			if (it->second == id)
				return true;
	}
	return false;
}

void cRakNetP2PClient2::OnNatPunchthroughError(const RakNet::RakNetGUID& guid)
{
}

}