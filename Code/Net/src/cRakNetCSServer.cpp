#include "cRakNetCSServer.h"
#include "../../inc/iCompress.h"
#include "../../inc/CrossString.h"
#include "../../inc/cThread.h"

using namespace std;
using namespace cm;
using namespace RakNet;

namespace cm
{

stLink::stLink() : m_id(0), m_guid(UNASSIGNED_RAKNET_GUID), m_address(UNASSIGNED_SYSTEM_ADDRESS)
{
}

ui32 cRakNetCSServer::s_id = 1;

cRakNetCSServer::cRakNetCSServer()
{
	m_bTrace = false;
	m_bCompress = true;
	m_pCallback = 0;
	m_pPeer = 0;
	m_bBroadcast = true;
}

cRakNetCSServer::~cRakNetCSServer()
{
}

bool cRakNetCSServer::Init(cpstr sBind, ui16 nMaxConnect, ui32 nTimeout)
{
	m_pPeer = RakPeerInterface::GetInstance();
	ui16 maxconnect = (nMaxConnect ? nMaxConnect : c_ui16ServerMaxConnect);
	m_pPeer->SetMaximumIncomingConnections(maxconnect);
	m_pPeer->SetTimeoutTime(nTimeout ? nTimeout : c_tmsPeerTimeout, UNASSIGNED_SYSTEM_ADDRESS);
	vector<SocketDescriptor> aSd;
	vector< pair<string, ui16> > aSad;
	if (!StrToAddress(aSad, sBind))
		return false;
	//for (ui32 i = 0; i < (ui32)aSad.size(); i++) {
	//	SocketDescriptor socketDescriptors6;
	//	socketDescriptors6.port = aSad[i].second;
	//	socketDescriptors6.socketFamily = AF_INET6;
	//	aSd.push_back(socketDescriptors6);
	//	SocketDescriptor socketDescriptors;
	//	socketDescriptors.port = aSad[i].second;
	//	aSd.push_back(socketDescriptors);
	//}
	//if (m_pPeer->Startup(maxconnect, &aSd.front(), (ui32)aSd.size() * 2) == RakNet::RAKNET_STARTED)
	//	return true;
	//aSad.clear();
	//for (ui32 i = 0; i < (ui32)aSad.size(); i++) {
	//	SocketDescriptor socketDescriptors;
	//	socketDescriptors.port = aSad[i].second;
	//	aSd.push_back(socketDescriptors);
	//}
	//if (RAKNET_STARTED == m_pPeer->Startup(maxconnect, &aSd.front(), (ui32)aSd.size()))
	//	return true;
	SocketDescriptor sd6[2];
	sd6[0].port = aSad[0].second;
	sd6[0].socketFamily = AF_INET;
	sd6[1].port = aSad[0].second;
	sd6[1].socketFamily = AF_INET6;
	if (RAKNET_STARTED == m_pPeer->Startup(maxconnect, sd6, 2))
		return true;
	if (RAKNET_STARTED == m_pPeer->Startup(maxconnect, sd6, 1))
		return true;
	return false;
}

void cRakNetCSServer::Release()
{
	SendPackage();
	DisconnectAll();
	RemoveLinkAll();
	if (m_pPeer) {
		m_pPeer->Shutdown(100);
		RakPeerInterface::DestroyInstance(m_pPeer);
		m_pPeer = 0;
	}
	delete this;
}

bool cRakNetCSServer::SendAll(cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0)
		return false;
	for (itMapServerLinkID it = m_mapServerLinkID.begin(); it != m_mapServerLinkID.end(); it++)
		Send(it->first, buf, len);
	return true;
}

void cRakNetCSServer::DisconnectAll()
{
	if (m_pPeer) {
		ui16 n = 0;
		m_pPeer->GetConnectionList(0, &n);
		if (n) {
			SystemAddress* asa = new SystemAddress[n];
			m_pPeer->GetConnectionList(asa, &n);
			ui32 sum = n;
			for (ui32 i = 0; i < sum; i++)
				m_pPeer->CloseConnection(asa[i], true);
			delete[] asa;
		}
	}
	for (itMapPackage it = m_mapPackage.begin(); it != m_mapPackage.end(); it++)
		delete it->second;
	m_mapPackage.clear();
}

bool cRakNetCSServer::Disconnect(ui32 id)
{
	stLink* pLink = FindLink(id);
	if (pLink == 0)
		return false;
	m_pPeer->CloseConnection(pLink->m_guid, true);
	itMapPackage it = m_mapPackage.find(pLink->m_guid);
	if (it == m_mapPackage.end())
		return true;
	delete it->second;
	m_mapPackage.erase(it);
	return true;
}

bool cRakNetCSServer::Send(ui32 id, cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0)
		return false;
	stLink* pLink = FindLink(id);
	if (pLink == 0)
		return false;
	ConnectionState cs = m_pPeer->GetConnectionState(pLink->m_guid);
	if (cs != IS_CONNECTED)
		return false;
	if (m_bCompress) {
		BitStream* pBs;
		itMapPackage it = m_mapPackage.find(pLink->m_guid);
		if (it == m_mapPackage.end()) {
			pBs = new BitStream();
			pBs->Write((i8)ID_MYCS_DATA_COMPRESS);
			m_mapPackage.insert(mapPackage::value_type(pLink->m_guid, pBs));
		}
		else
			pBs = it->second;
		pBs->Write((i8*)&len, sizeof(len));
		pBs->Write((cpstr)buf, len);
		//if (len == 1) {
		//	LogError("net") << "cRakNetCSServer::Send size = 1" << LEnd;
		//}
	} else {
		BitStream bs;
		bs.Write((i8)ID_MYCS_DATA_SRC);
		bs.Write((cpstr)&len, sizeof(len));
		bs.Write((cpstr)buf, len);
		m_pPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pLink->m_guid, false);
		//if (len == 1) {
		//	LogError("net") << "cRakNetCSServer::Send size = 1" << LEnd;
		//}
	}
	return true;
}

i32 cRakNetCSServer::GetPing(ui32 id)
{
	stLink* pLink = FindLink(id);
	if (pLink == 0)
		return -1;
	return m_pPeer->GetLastPing(pLink->m_guid);
}

bool cRakNetCSServer::GetLinkIpPort(ui32 id, pstr ip, ui16& port)
{
	stLink* pLink = FindLink(id);
	if (pLink == 0)
		return false;
	CrossString::StrCpy(ip, pLink->m_address.ToString(false));
	port = pLink->m_address.GetPort();
	return true;
}

void cRakNetCSServer::SendPackage()
{
	for (itMapPackage it = m_mapPackage.begin(); it != m_mapPackage.end(); it++) {
		BitStream* pBs = it->second;
		BitStream& bs = *pBs;
		i8* pSrcBuf = (i8*)bs.GetData() + sizeof(MessageID);
		ui32 srclen = bs.GetNumberOfBytesUsed() - sizeof(MessageID);
		ui32 compressedlen = GetCompressSized(pSrcBuf, srclen, c_eCompressType);
		if (compressedlen) {
			ui32 headsize = sizeof(MessageID) + sizeof(srclen);
			ui32 compressedBufferLen = compressedlen + headsize;
			i8* pCompressedS = new i8[compressedBufferLen];
			memset(pCompressedS, 0, compressedBufferLen);
			i8* pCompressed = pCompressedS;
			MessageID idmsg = ID_MYCS_DATA_COMPRESS;
			memcpy(pCompressed, &idmsg, sizeof(idmsg));
			pCompressed += sizeof(idmsg);
			memcpy(pCompressed, &srclen, sizeof(srclen));
			pCompressed += sizeof(srclen);
			eCompress e = Compress(pSrcBuf, srclen, pCompressed, compressedlen, c_eCompressType);
			if (e == C_OK) {
				ui32 sendsize = headsize + compressedlen;
				m_pPeer->Send(pCompressedS, sendsize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it->first, false);
			}
			//else
			//	LogError("net") << "cRakNetCSServer::SendPackage Compress Error " << srclen << LEnd;
			delete[] pCompressedS;
		}
		delete pBs;
	}
	m_mapPackage.clear();
}

void cRakNetCSServer::RecvPackage(ui32 id, i8* pData, ui32 lenSum)
{
	ui32 offset = 0;
	while(offset < lenSum) {
		ui32 len;
		memcpy(&len, pData, sizeof(len));
		offset += sizeof(len);
		pData += sizeof(len);
		m_pCallback->OnRecv(id, pData, len, m_userdata);
		offset += len;
		pData += len;
	}
}

void cRakNetCSServer::BroadcastClient()
{
	if (m_bBroadcast && m_pPeer && GetTime() - m_broadcasttime >= c_tmBroadcast) {
		m_broadcasttime = GetTime();
		BitStream bs;
		bs.Write((i8)ID_MYSC_DATA_TMP);
		static int s_n = 1;
		bs.Write((cpstr)&(++s_n), sizeof(s_n));
		m_pPeer->Send(&bs, MEDIUM_PRIORITY, UNRELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
	}
}

void cRakNetCSServer::RunOnce()
{
	if (m_pPeer == 0)
		return;
	BroadcastClient();
	SendPackage();
	SWITCH_PACKAGE_BEGIN
	CASE_PACKAGE_BEGIN(ID_TIMESTAMP)
		//BitStream bs(pPacket->data, pPacket->length, false);
		//bs.IgnoreBytes(sizeof(MessageID));
		//Time clientTime;
		//bs.Read(clientTime);
		//Time serverTime = GetTime();
		//if (clientTime > serverTime) {
		//	Time t = clientTime - serverTime;
		//	if (t > c_tmCheckTimeBlur) {
		//		itMapServerLink it = m_mapServerLink.find(pPacket->guid);
		//		if (it != m_mapServerLink.end()) {
		//			m_pPeer->CloseConnection(pPacket->guid, true);
		//			//LogError("使用加速器") << m_pCallback->GetPlayerName(it->second->m_id) << " sumtime:" << t << "ms ipport:" << pPacket->systemAddress.ToString() << LEnd;
		//		}
		//		else {
		//			m_pPeer->CloseConnection(pPacket->guid, true);
		//			//LogError("使用加速器") << " sumtime:" << t << "ms ipport:" << pPacket->systemAddress.ToString() << LEnd;
		//		}
		//	}
		//}
		//LInfo << "clienttime:" << clientTime << " servertime:" << serverTime <<
			//" sub:" << (i32)(serverTime - clientTime) << " ping:" << m_pPeer->GetLastPing(pPacket->guid) << LEnd;
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYCS_DATA_COMPRESS)
		itMapServerLink it = m_mapServerLink.find(pPacket->guid);
		if (it != m_mapServerLink.end()) {
			const stLink* pLink = it->second;
			ui32 srclen;
			ui32 headsize = sizeof(MessageID) + sizeof(srclen);
			if (pPacket->length > headsize) {
				i8* pData = (i8*)pPacket->data;
				pData += sizeof(MessageID);
				memcpy(&srclen, pData, sizeof(srclen));
				pData += sizeof(srclen);
				ui32 len = pPacket->length - headsize;
				ui32 lenDecompressed = GetDecompressSized(pData, srclen, c_eCompressType);
				if (lenDecompressed) {
					i8* pRecv = new i8[lenDecompressed];
					memset(pRecv, 0, lenDecompressed);
					eCompress e = Decompress(pData, len, pRecv, lenDecompressed, c_eCompressType);
					if (e == C_OK)
						RecvPackage(pLink->m_id, pRecv, lenDecompressed);
					//else
					//	LogError("net") << "cRakNetCSServer Decompress Error " << len << " to " << lenDecompressed << LEnd;
					delete[] pRecv;
				}
			}
			//else
			//	LogError("net") << "cRakNetCSServer ID_MYCS_DATA_COMPRESS size = " << pPacket->length << LEnd;
		}
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYCS_DATA_SRC)
		itMapServerLink it = m_mapServerLink.find(pPacket->guid);
		if (it != m_mapServerLink.end()) {
			const stLink* pLink = it->second;
			BitStream bs(pPacket->data, pPacket->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			ui32 len = 0;
			bs.Read((i8*)&len, sizeof(len));
			if (len) {
				i8* p = new i8[len];
				if (bs.Read(p, len))
					m_pCallback->OnRecv(pLink->m_id, p, len, m_userdata);
				delete[] p;
			}
		}
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_NEW_INCOMING_CONNECTION)
		RemoveLink(pPacket->guid);
		stLink* pNew = new stLink();
		pNew->m_id = s_id++;
		pNew->m_guid = pPacket->guid;
		pNew->m_address = pPacket->systemAddress;
		m_mapServerLink[pPacket->guid] = pNew;
		m_mapServerLinkID[pNew->m_id] = pNew;
		m_pCallback->OnConnect(pNew->m_id, m_userdata);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_DISCONNECTION_NOTIFICATION)
		RemoveLink(pPacket->guid);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_LOST)
		RemoveLink(pPacket->guid);
	CASE_PACKAGE_END
	SWITCH_PACKAGE_END
}

stLink* cRakNetCSServer::FindLink(ui32 id)
{
	itMapServerLinkID it = m_mapServerLinkID.find(id);
	if (it == m_mapServerLinkID.end())
		return 0;
	return it->second;
}

void cRakNetCSServer::RemoveLink(const RakNetGUID& guid, bool bDisconnect)
{
	itMapServerLink it = m_mapServerLink.find(guid);
	if (it != m_mapServerLink.end()) {
		stLink* p = it->second;
		m_pCallback->OnDisconnect(p->m_id, m_userdata);
		m_mapServerLinkID.erase(p->m_id);
		delete p;
		m_mapServerLink.erase(it);
		if (bDisconnect)
			m_pPeer->CloseConnection(guid, true);
	}
}

void cRakNetCSServer::RemoveLinkAll(bool bDisconnect)
{
	for (itMapServerLink it = m_mapServerLink.begin(); it != m_mapServerLink.end(); it++) {
		stLink* p = it->second;
		m_pCallback->OnDisconnect(p->m_id, m_userdata);
		if (bDisconnect)
			m_pPeer->CloseConnection(p->m_guid, true);
		delete p;
	}
	m_mapServerLink.clear();
	m_mapServerLinkID.clear();
}

}