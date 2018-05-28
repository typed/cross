#include "cRakNetCSClient.h"
#include "../../inc/iCompress.h"

using namespace std;
using namespace cross;
using namespace RakNet;

namespace cross
{

cRakNetCSClient::cRakNetCSClient()
{
	m_bTrace = false;
	m_bCompress = true;
	m_pCallback = 0;
	m_pPeer = 0;
	m_time = 0;
	m_autoreconnecttime = 0;
	m_server = UNASSIGNED_SYSTEM_ADDRESS;
	m_bCheckTime = false;
	m_bAutoReconnect = false;
	m_port = 0;
	m_bConnecting = false;
}

cRakNetCSClient::~cRakNetCSClient()
{
}

bool cRakNetCSClient::Init(ui32 nTimeout, b8 bCheckTime)
{
	m_bCheckTime = bCheckTime;
	m_pPeer = RakPeerInterface::GetInstance();
	//m_pPeer->SetMaximumIncomingConnections(0);
	m_pPeer->SetTimeoutTime(nTimeout ? nTimeout : c_tmsPeerTimeout, UNASSIGNED_SYSTEM_ADDRESS);
	SocketDescriptor sd;
#ifdef _WIN32
#else
	sd.socketFamily = AF_UNSPEC;
#endif
	return RAKNET_STARTED == m_pPeer->Startup(1, &sd, 1);
}

void cRakNetCSClient::Release()
{
	SendPackage(0);
	Disconnect();
	if (m_pPeer) {
		m_pPeer->Shutdown(100);
		RakPeerInterface::DestroyInstance(m_pPeer);
		m_pPeer = 0;
	}
	delete this;
}

bool cRakNetCSClient::Connect(cpstr ip, ui16 port)
{
	Disconnect();
	m_port = port;
	m_ip = ip;
	return CONNECTION_ATTEMPT_STARTED == m_pPeer->Connect(m_ip.c_str(), m_port, 0, 0);
}

bool cRakNetCSClient::IsConnected()
{
	return IS_CONNECTED == m_pPeer->GetConnectionState(m_server);
}

void cRakNetCSClient::AutoConnect()
{
	m_bAutoReconnect = true;
}

void cRakNetCSClient::NotAutoConnect()
{
	m_bAutoReconnect = false;
}

bool cRakNetCSClient::Disconnect()
{
	m_ip = "";
	m_port = 0;
	m_aSend.clear();
	m_package.Reset();
	if (m_pPeer && m_server != UNASSIGNED_SYSTEM_ADDRESS) {
		m_pPeer->CloseConnection(m_server, true);
		return true;
	}
	return false;
}

void cRakNetCSClient::SendPackage(ui32 waittime)
{
	ui32 len = m_package.GetNumberOfBytesUsed();
	if (len) {
		i8* pSrcBuf = (i8*)m_package.GetData() + sizeof(MessageID);
		ui32 srclen = len - sizeof(MessageID);
		ui32 compressedlen = GetCompressSized(pSrcBuf, srclen, c_eCompressType);
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
			m_pPeer->Send(pCompressedS, sendsize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_server, false);
		}
		//else
		//	LogError("net") << "cRakNetCSClient::SendPackage Compress Error " << srclen << LEnd;
		delete[] pCompressedS;
		m_package.Reset();
	}
	//if (waittime) {
	//	ui32 num = waittime / 100;
	//	for (ui32 i = 0; i < num; i++) {
	//		Packet* pPacket = m_pPeer->Receive();
	//		if (pPacket) {
	//			m_pPeer->DeallocatePacket(pPacket);
	//			pPacket = 0;
	//		}
	//		CrossThread::Sleep(100);
	//	}
	//}
}

void cRakNetCSClient::RecvPackage(i8* pDataS, ui32 lenSum)
{
	i8* pData = pDataS;
	ui32 offset = 0;
	while(offset < lenSum) {
		ui32 len;
		memcpy(&len, pData, sizeof(len));
		offset += sizeof(len);
		pData += sizeof(len);
		m_pCallback->OnRecv(pData, len, m_userdata);
		offset += len;
		pData += len;
	}
}

void cRakNetCSClient::RunOnce()
{
	if (m_pPeer == 0)
		return;
	SendPackage(0);
	SendTime();
	AutoReconnect();
	SWITCH_PACKAGE_BEGIN
	CASE_PACKAGE_BEGIN(ID_TIMESTAMP)
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_ATTEMPT_FAILED)
		m_bConnecting = false;
		m_pCallback->OnConnect(CT_Error, m_userdata);
		//m_server = UNASSIGNED_SYSTEM_ADDRESS;
		m_aSend.clear();
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_REQUEST_ACCEPTED)
		m_bConnecting = false;
		m_package.Reset();
		m_server = pPacket->systemAddress;
		m_pCallback->OnConnect(CT_OK, m_userdata);
		for (vector< vector<i8> >::iterator it = m_aSend.begin(); it != m_aSend.end(); it++)
			_Send(&it->front(), (ui32)it->size());
		m_aSend.clear();
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_DISCONNECTION_NOTIFICATION)
		NETTRACE << "DT_ServerInitiative" << LEnd;
		m_pCallback->OnDisconnect(DT_ServerInitiative, m_userdata);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_CONNECTION_LOST)
		m_pCallback->OnDisconnect(DT_Exception, m_userdata);
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYCS_DATA_COMPRESS)
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
					RecvPackage(pRecv, lenDecompressed);
				//else
				//	LogError("net") << "cRakNetCSClient Decompress Error " << len << " to " << lenDecompressed << LEnd;
				delete[] pRecv;
			}
		}
		//else
		//	LogError("net") << "cRakNetCSClient ID_MYCS_DATA_COMPRESS size = " << pPacket->length << LEnd;
	CASE_PACKAGE_END
	CASE_PACKAGE_BEGIN(ID_MYCS_DATA_SRC)
		BitStream bs(pPacket->data, pPacket->length, false);
		bs.IgnoreBytes(sizeof(MessageID));
		ui32 len = 0;
		bs.Read((i8*)&len, sizeof(len));
		if (len) {
			i8* p = new i8[len];
			if (bs.Read(p, len))
				m_pCallback->OnRecv(p, len, m_userdata);
			delete[] p;
		}
	CASE_PACKAGE_END
	SWITCH_PACKAGE_END
}

bool cRakNetCSClient::Send(cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0)
		return false;
	if (m_server == UNASSIGNED_SYSTEM_ADDRESS)
		return false;
	ConnectionState cs = m_pPeer->GetConnectionState(m_server);
	if (cs == IS_CONNECTING || cs == IS_PENDING) {
		vector<i8> data(len);
		memcpy(&data.front(), buf, len);
		m_aSend.push_back(data);
		return true;
	}
	if (cs != IS_CONNECTED)
		return false;
	return _Send(buf, len);
}

bool cRakNetCSClient::_Send(cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0)
		return false;
	if (m_bCompress) {
		if (m_package.GetNumberOfBytesUsed() == 0)
			m_package.Write((i8)ID_MYCS_DATA_COMPRESS);
		m_package.Write((i8*)&len, sizeof(len));
		m_package.Write((cpstr)buf, len);
		//if (len == 1) {
			//LogError("net") << "cRakNetCSClient::_Send size = 1" << LEnd;
		//}
	} else {
		BitStream bs;
		bs.Write((i8)ID_MYCS_DATA_SRC);
		bs.Write((cpstr)&len, sizeof(len));
		bs.Write((cpstr)buf, len);
		m_pPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_server, false);
		//if (len == 1) {
		//	LogError("net") << "cRakNetCSClient::_Send size = 1" << LEnd;
		//}
	}
	return true;
}

i32 cRakNetCSClient::GetPing()
{
	if (m_server == UNASSIGNED_SYSTEM_ADDRESS)
		return -1;
	return m_pPeer->GetLastPing(m_server);
}

void cRakNetCSClient::SendTime()
{
	//if (m_bCheckTime) {
	//	if (GetTime() - m_time >= c_tmCheckTime) {
	//		m_time = GetTime();
	//		ConnectionState cs = m_pPeer->GetConnectionState(m_server);
	//		if (cs != IS_CONNECTED)
	//			return;
	//		BitStream bs;
	//		bs.Write((MessageID)ID_TIMESTAMP);
	//		bs.Write(m_time);
	//		m_pPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_server, false);
	//	}
	//}
}

void cRakNetCSClient::AutoReconnect()
{
	if (m_bAutoReconnect && m_ip != "") {
		if (GetTime() - m_autoreconnecttime >= c_tmCheckAutoConnect) {
			m_autoreconnecttime = GetTime();
			if (m_server == UNASSIGNED_SYSTEM_ADDRESS && !m_bConnecting) {
				m_bConnecting = (CONNECTION_ATTEMPT_STARTED == m_pPeer->Connect(m_ip.c_str(), m_port, 0, 0));
			} else {
				ConnectionState cs = m_pPeer->GetConnectionState(m_server);
				switch (cs) {
				case RakNet::IS_DISCONNECTED:
				case RakNet::IS_NOT_CONNECTED:
					{
						m_pPeer->Connect(m_ip.c_str(), m_port, 0, 0);
						break;
					}
				default:
					break;
				}
			}
		}
	}
}

}