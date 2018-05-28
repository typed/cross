#ifndef raknetconfig_h
#define raknetconfig_h

#include "../../inc/CommonDef.h"
#include "raknet/CloudCommon.h"
#include "raknet/RakNetTypes.h"
#include "raknet/GetTime.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/MessageIdentifiers.h"

#include "../../inc/iLog.h"
#include "../../inc/iCompress.h"
#include <vector>
#include <map>
#include <set>

namespace cm
{

static RakNet::SystemAddress c_saNatTypeServer("8.17.250.34", 60481);
static cpstr c_sPassword = "x!1!i@2@a#3#z$4$o%5%o";
RakNet::CloudKey& GetCloudKeyClientInfo();
RakNet::CloudKey& GetCloudKeyProxyInfo();
static const ui32 c_ui32RequestForwardWaitTime = 60*1000;
static const RakNet::Time c_tmConnectServerCheckTime = 2000;
static const RakNet::Time c_tmNatPunchthroughTime = 10000;
static const RakNet::Time c_tmNatPunchthroughTime2 = 5000;
static const ui32 c_ui32NatPunchthroughMaxErrorTimes = 3;
static const ui32 c_ui32MaximumForwardingRequests = 10;
static const ui32 c_ui32RouterMaxErrorTimes = 3;
static const RakNet::Time c_tmRouterTime = 10000;
static const RakNet::Time c_tmForwardTime = 10000;
static const RakNet::TimeMS c_tmsPeerTimeout = 30000;
static const RakNet::Time c_tmCheckConnectRoom = 1000;
static const RakNet::Time c_tmCheckRoomOpenNAT = 5000;
static const RakNet::Time c_tmCheckConnectProxy = 1000;
static const RakNet::Time c_tmCheckTime = 5000;
static const RakNet::Time c_tmCheckAutoConnect = 1000;
static const RakNet::Time c_tmCheckTimeBlur = 300;
static const RakNet::Time c_tmBroadcast = 60*1000;
static const ui16 c_ui16ServerMaxConnect = 30000;
static const ui16 c_ui16ClientMaxConnect = 10;
static const ui64 c_ui64TCPGUID = 0x50c31b7313c39e0a;
static const eCompressType c_eCompressType = CT_Qlz;
//bool OpenUPNP(RakNet::RakPeerInterface* rakPeer);
RakNet::SystemAddress ConnectBlocking(RakNet::RakPeerInterface* rakPeer, const char* hostName = "", const RakNet::SystemAddress& sa = RakNet::UNASSIGNED_SYSTEM_ADDRESS);
bool ConnectTrue(RakNet::RakPeerInterface* rakPeer, const RakNet::SystemAddress& sa);
void ConnectMustTrue(RakNet::RakPeerInterface* rakPeer, const RakNet::SystemAddress& sa);
cpstr RaknetDomainToIp(cpstr sDomain);
RakNet::SystemAddress RaknetAddressMapping(cpstr sIP, ui16 port);
RakNet::SystemAddress RaknetAddressMapping(RakNet::SystemAddress sa);

#define NETTRACE if (m_bTrace) LInfo
#define SWITCH_PACKAGE_BEGIN for (Packet* pPacket = m_pPeer->Receive(); pPacket; m_pPeer->DeallocatePacket(pPacket), pPacket = m_pPeer->Receive()) { switch(pPacket->data[0]) {
#define SWITCH_PACKAGE_END }}
#define CASE_PACKAGE_BEGIN(id) case id : {NETTRACE << #id << " " << pPacket->systemAddress.ToString() << " " << pPacket->guid.ToString() << LEnd;
#define CASE_PACKAGE_END break; }

struct stTCPHeader {
	ui64 guid;
	ui32 lenData;
	ui32 lenSrc;
	stTCPHeader():
		guid(c_ui64TCPGUID),
		lenData(0),
		lenSrc(0)
	{}
};
struct stTCPPack {
	stTCPHeader header;
	std::string data;
	void clear() {
		header.lenData = 0;
		header.lenSrc = 0;
		data.clear();
	}
};

enum
{
	ID_MYP2P_DATA = ID_USER_PACKET_ENUM,
	ID_MYCS_DATA,
	ID_MYCS_DATA_COMPRESS,
	ID_MYCS_DATA_SRC,
	ID_MYP2P_DATA_2_ACTION,
	ID_MYP2P_DATA_2_SET,
	ID_MYSC_DATA_TMP,
};

struct stPlayer
{
	RakNet::RakNetGUID		guid;
	RakNet::SystemAddress	sa;
	RakNet::RakString		sRoom;	//房间名,如果为"",表示没有在房间里
	i32						nPing;
	stPlayer():nPing(-1){}
};
typedef std::map<ui32, stPlayer> mapPlayerGUID;
typedef mapPlayerGUID::iterator itMapPlayerGUID;
typedef mapPlayerGUID::const_iterator citMapPlayerGUID;
typedef std::map<RakNet::RakNetGUID, ui32> mapGUIDToID;
typedef mapGUIDToID::iterator itMapGUIDToID;
typedef mapGUIDToID::const_iterator citMapGUIDToID;
typedef std::multimap<RakNet::RakString, ui32> mmRoom;
typedef mmRoom::iterator itMmRoom;
typedef mmRoom::const_iterator citMmRoom;

struct stSendItem
{
	ui32				m_id;
	RakNet::RakNetGUID	m_guid;
	char*				m_pData;
	ui32				m_dataSize;
};
typedef std::vector<stSendItem> aSendItem;
typedef aSendItem::iterator itSendItem;

typedef std::map<RakNet::RakNetGUID, RakNet::Time> mapNatPunchthroughTime;
typedef mapNatPunchthroughTime::iterator itMapNatPunchthroughTime;
typedef std::map<RakNet::RakNetGUID, ui32> mapNatPunchthroughErrorTimes;
typedef mapNatPunchthroughErrorTimes::iterator itMapNatPunchthroughErrorTimes;

enum eMyMsgType
{
	MMT_Connect,
	MMT_Forward,
	MMT_Router,
	MMT_OpenNAT,
};
struct stMyMsg
{
	eMyMsgType				type;
	RakNet::SystemAddress	sa;
	RakNet::RakNetGUID		guid;
};
typedef std::vector<stMyMsg> aMsg;
typedef aMsg::iterator itAMsg;

}

#endif