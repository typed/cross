#ifndef craknetp2pproxyserver_h
#define craknetp2pproxyserver_h

#include "iNet.h"
#include "Common/iReference.h"
#include "Raknet/RakNetTypes.h"
#include "Raknet/RakPeerInterface.h"
#include "Raknet/RakString.h"
#include "Raknet/CloudClient.h"

namespace cross
{

struct SampleFramework;

class cRakNetP2PProxyServer :
	public iP2PServer
{
public:
	cRakNetP2PProxyServer();
	~cRakNetP2PProxyServer();
	bool Init(cpstr sBind, cpstr sServer, cpstr sClientConnect);
	void Release();
	void RunOnce();
private:
	RakNet::RakPeerInterface*	m_pPeer;
	RakNet::SystemAddress		m_server;
	RakNet::SystemAddress		m_clientConnect;
	RakNet::Time				m_time;
	RakNet::RakNetGUID			m_serverguid;
	RakNet::CloudClient*		m_pCloudPlugin;
	bool						m_bTrace;
};

}

#endif