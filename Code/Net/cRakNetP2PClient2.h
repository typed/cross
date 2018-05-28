#ifndef craknetp2pclient2_h
#define craknetp2pclient2_h

#include "../../inc/iNet.h"
#include "../../inc/iReference.h"
#include "RakNetConfig.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
#include "raknet/NatTypeDetectionClient.h"
#include "raknet/CloudClient.h"
#include "raknet/NatPunchthroughClient.h"
#include <map>
#include <set>
#include <vector>

namespace cross
{

struct stProxy
{
	RakNet::SystemAddress	m_sa;
	i32						m_ping;
};
typedef std::map<RakNet::RakNetGUID, stProxy> mapProxy;
typedef mapProxy::iterator itMapProxy;

class cRakNetP2PClientStream2 :
	public iP2PClientStream
{
public:
	cRakNetP2PClientStream2();
	cRakNetP2PClientStream2(ui8* p, ui32 len);
	void Write(cpvd buf, ui32 len);
	bool Read(pvd buf, ui32 len);
	ui32 GetCrc();
	RakNet::BitStream	m_bs;
};

class cRakNetP2PClient2 :
	public iP2PClient
{
public:
	cRakNetP2PClient2();
	~cRakNetP2PClient2();
	bool Init();
	void Release();
	void Login(ui32 id, cpstr ip, ui16 port);
	void Logout();
	void Connect(cpstr ip, ui16 port);
	void Disconnect();
	bool IsLoginOk();
	void RunOnce();
	void DisconnectAllPlayer();

	void SetCallback(iP2PClientCallback* p) {m_pCallback = p;}
	iP2PClientCallback* GetCallback() {return m_pCallback;}

	void EnterRoom(cpstr sName);
	void LeaveRoom();
	void TraceRoom();
	bool IsInRoom() const;
	bool IsHost() const;
	ui32 GetHostID() const;
	void Action(cpvd buf, ui32 len);
	i32 GetPing(ui32 id);
	cpstr GetRoomName();

private:

	const RakNet::RakNetGUID* GetPlayerGUID(ui32) const;
	bool IsConnected(ui32);
	bool IsServerConnected();
	void UploadInfo();
	void GetClients();
	void DisplayPlayers();
	void DisplayRooms();
	void DisplayProxys();
	void CheckServer();
	void CheckRoomOpenNAT();
	void CheckProxy();
	void CheckSendRoom();
	bool GUIDToID(ui32& id, const RakNet::RakNetGUID& guid) const;
	bool IDToGUID(RakNet::RakNetGUID& guid, ui32 id) const;
	bool IsSameRoom(const RakNet::RakNetGUID& guid);
	void SendOpenNAT(const RakNet::RakNetGUID& guid);
	void OpenNAT(const RakNet::RakNetGUID& guid);
	void SendConnect(const RakNet::SystemAddress& sa);
	void SendInRoom(cpvd buf, ui32 len, bool bReliable, i8 msgid);
	void OnNatPunchthroughError(const RakNet::RakNetGUID& guid);
	void FindBaseProxy();

	RakNet::RakPeerInterface*			m_pPeer;
	RakNet::CloudClient*				m_pCloudPlugin;
	RakNet::NatPunchthroughClient*		m_pNatPunchthroughPlugin;

	iP2PClientCallback*					m_pCallback;

	mapPlayerGUID						m_mapPlayerGUID;
	mapGUIDToID							m_mapGUIDToID;
	mmRoom								m_mmRoom;
	RakNet::Time						m_tmCheckRoom;
	mapProxy							m_mapProxy;
	RakNet::Time						m_tmCheckProxy;
	ui32								m_id;
	RakNet::SystemAddress				m_server;
	RakNet::RakNetGUID					m_serverguid;
	RakNet::RakNetGUID					m_proxyserverguid;
	aMsg								m_aMsg;
	mapNatPunchthroughTime				m_mapNPT;
	RakNet::Time						m_time;
	bool								m_bTrace;
	RakNet::RakString					m_sRoomName;
	ui32								m_uiCrc;
	bool								m_bSendGet;
};

}

#endif