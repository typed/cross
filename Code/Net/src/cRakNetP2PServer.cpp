#include "cRakNetP2PServer.h"
#include "RakNetConfig.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakSleep.h"
#include "raknet/Kbhit.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/BitStream.h"
#include "raknet/RakSleep.h"
#include "raknet/UDPProxyServer.h"
#include "raknet/UDPProxyCoordinator.h"
#include "raknet/NatPunchthroughServer.h"
#include "raknet/NatTypeDetectionServer.h"
#include "raknet/SocketLayer.h"
#include "raknet/Getche.h"
#include "raknet/Gets.h"
#include "raknet/CloudClient.h"
#include "CloudServerHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace RakNet;

namespace cm
{

enum FeatureSupport
{
	SUPPORTED,
	UNSUPPORTED,
	QUERY
};

enum FeatureList
{
	NAT_TYPE_DETECTION_SERVER,
	NAT_PUNCHTHROUGH_SERVER,
	UDP_PROXY_COORDINATOR,
	UDP_PROXY_SERVER,
	CLOUD_SERVER,
	FEATURE_LIST_COUNT,
};

//´úÀí
std::vector<SystemAddress> g_aProxyCoordinator;

#define NatTypeDetectionServerFramework_Supported QUERY
#define NatPunchthroughServerFramework_Supported QUERY
#define UDPProxyCoordinatorFramework_Supported QUERY
#define UDPProxyServerFramework_Supported QUERY
#define CloudServerFramework_Supported QUERY

struct SampleFramework
{
    virtual ~SampleFramework() {}
	virtual const char * QueryName(void) = 0;
	virtual const char * QueryRequirements(void) = 0;
	virtual const char * QueryFunction(void) = 0;
	virtual void Init(RakNet::RakPeerInterface *rakPeer) = 0;
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet) = 0;
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer) = 0;
	FeatureSupport isSupported;
};

struct NatTypeDetectionServerFramework : public SampleFramework
{
	NatTypeDetectionServerFramework() {isSupported = NatTypeDetectionServerFramework_Supported; ntds = 0;}
	virtual const char * QueryName(void) {return "NatTypeDetectionServer";}
	virtual const char * QueryRequirements(void) {return "Requires 4 IP addresses";}
	virtual const char * QueryFunction(void) {return "Determines router type to filter by connectable systems.\nOne instance needed, multiple instances may exist to spread workload.";}
	virtual void Init(RakNet::RakPeerInterface *rakPeer)
	{
		if (isSupported==SUPPORTED)
		{
			ntds = new NatTypeDetectionServer;
			rakPeer->AttachPlugin(ntds);

			SystemAddress ipList[ MAXIMUM_NUMBER_OF_INTERNAL_IDS ];
			SocketLayer::GetMyIP( ipList );
			for (int i=0; i<4; i++)
			{
				if (ipList[i]==UNASSIGNED_SYSTEM_ADDRESS && i < MAXIMUM_NUMBER_OF_INTERNAL_IDS)
				{
					//LError << "Failed. Not enough IP addresses to bind to." << LEnd;
					rakPeer->DetachPlugin(ntds);
					delete ntds;
					ntds=0;
					isSupported=UNSUPPORTED;
					return;
				}
			}
			char ipListStr1[128],ipListStr2[128],ipListStr3[128];
			ipList[1].ToString(false,ipListStr1);
			ipList[2].ToString(false,ipListStr2);
			ipList[3].ToString(false,ipListStr3);
			//LInfo << "Starting " << QueryName() << " on " << ipListStr1 << ", " << ipListStr2 << ", " << ipListStr3 << "." << LEnd;
			ntds->Startup(ipListStr1, ipListStr2, ipListStr3);
		}
	}
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet)
	{
	}
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer)
	{
		if (ntds)
		{
			rakPeer->DetachPlugin(ntds);
			delete ntds;
		}
		ntds=0;
	}

	NatTypeDetectionServer *ntds;
};
struct NatPunchthroughServerFramework : public SampleFramework/*, public NatPunchthroughServerDebugInterface_Printf*/
{
	NatPunchthroughServerFramework() {isSupported=NatPunchthroughServerFramework_Supported; nps=0;}
	virtual const char * QueryName(void) {return "NatPunchthroughServerFramework";}
	virtual const char * QueryRequirements(void) {return "None";}
	virtual const char * QueryFunction(void) {return "Coordinates NATPunchthroughClient.";}
	virtual void Init(RakNet::RakPeerInterface *rakPeer)
	{
		if (isSupported==SUPPORTED)
		{
			nps = new NatPunchthroughServer;
			rakPeer->AttachPlugin(nps);
			//nps->SetDebugInterface(this);
		}
	}
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet)
	{
	}
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer)
	{
		if (nps)
		{
			rakPeer->DetachPlugin(nps);
			delete nps;
		}
		nps=0;
	}

	NatPunchthroughServer *nps;
};
struct UDPProxyCoordinatorFramework : public SampleFramework
{
	UDPProxyCoordinatorFramework() {isSupported=UDPProxyCoordinatorFramework_Supported;}
	virtual const char * QueryName(void) {return "UDPProxyCoordinator";}
	virtual const char * QueryRequirements(void) {return "Bandwidth to handle a few hundred bytes per game session.";}
	virtual const char * QueryFunction(void) {return "Coordinates UDPProxyClient to find available UDPProxyServer.\nExactly one instance required.";}
	virtual void Init(RakNet::RakPeerInterface *rakPeer)
	{
		if (isSupported==SUPPORTED)
		{
			udppc = new UDPProxyCoordinator;
			rakPeer->AttachPlugin(udppc);
			udppc->SetRemoteLoginPassword(c_sPassword);
		}
	}
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet)
	{
	}
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer)
	{
		if (udppc)
		{
			rakPeer->DetachPlugin(udppc);
			delete udppc;
			udppc=0;
		}
	}

	UDPProxyCoordinator *udppc;
};
SystemAddress SelectAmongConnectedSystems(RakNet::RakPeerInterface *rakPeer, const char *hostName)
{
	DataStructures::List<SystemAddress> addresses;
	DataStructures::List<RakNetGUID> guids;
	rakPeer->GetSystemList(addresses, guids);
	if (addresses.Size()==0)
	{
		return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	}
	if (addresses.Size()>1)
	{
		//LInfo << "Select IP address for " << hostName << "." << LEnd;
		char buff[64];
		for (unsigned int i=0; i < addresses.Size(); i++)
		{
			addresses[i].ToString(true, buff);
			//LInfo << i+1 << ". " << buff << LEnd;
		}
		Gets(buff,sizeof(buff));
		if (buff[0]==0)
		{
			return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		}
		unsigned int idx = atoi(buff);
		if (idx<=0 || idx > addresses.Size())
		{
			return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		}
		return addresses[idx-1];
	}
	else
		return addresses[0];
};
struct UDPProxyServerFramework : public SampleFramework, public UDPProxyServerResultHandler
{
	UDPProxyServerFramework() {isSupported=UDPProxyServerFramework_Supported;}
	virtual const char * QueryName(void) {return "UDPProxyServer";}
	virtual const char * QueryRequirements(void) {return "Bandwidth to handle forwarded game traffic.";}
	virtual const char * QueryFunction(void) {return "Allows game clients to forward network traffic transparently.\nOne or more instances required, can be added at runtime.";}
	virtual void Init(RakNet::RakPeerInterface *rakPeer)
	{
		if (isSupported==SUPPORTED)
		{
			//LInfo << "Logging into UDPProxyCoordinator..." << LEnd;

			for (ui32 i = 0; i < g_aProxyCoordinator.size(); i++) {
				SystemAddress coordinatorAddress = ConnectBlocking(rakPeer, "UDPProxyCoordinator", g_aProxyCoordinator[i]);
				if (coordinatorAddress == RakNet::UNASSIGNED_SYSTEM_ADDRESS)
				{
					//LError << "Failed to connect." << LEnd;
					isSupported = QUERY;
					return;
				}
				UDPProxyServer* p = new UDPProxyServer;
				//p->SetBindIP(g_aProxyCoordinator[i].ToString(false));
				p->SetResultHandler(this);
				rakPeer->AttachPlugin(p);
				if (p->LoginToCoordinator(c_sPassword, g_aProxyCoordinator[i])==false)
				{
					//LError << "LoginToCoordinator call failed." << LEnd;
					isSupported = QUERY;
					rakPeer->DetachPlugin(p);
					delete p;
					p = 0;
					return;
				}
				udpps.push_back(p);
			}
			
		}
	}
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet)
	{
	}
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer)
	{
		for (size_t i = 0; i < udpps.size(); i++)
		{
			rakPeer->DetachPlugin(udpps[i]);
			delete udpps[i];
			udpps[i] = 0;
		}
		udpps.clear();
	}

	virtual void OnLoginSuccess(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServerPlugin)
	{
		//LOk << QueryName() << "(" << proxyServerPlugin->GetBindIP().C_String() << ") logged into UDPProxyCoordinator." << LEnd;
	}
	virtual void OnAlreadyLoggedIn(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServerPlugin)
	{
		//LWarning << QueryName() << "(" << proxyServerPlugin->GetBindIP().C_String() << ") already logged into UDPProxyCoordinator." << LEnd;
	}
	virtual void OnNoPasswordSet(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServerPlugin)
	{
		//LWarning << QueryName() << "(" << proxyServerPlugin->GetBindIP().C_String() << ") failed login to UDPProxyCoordinator. No password set." << LEnd;
		isSupported=QUERY;
		//delete udpps;
		//udpps=0;
	}
	virtual void OnWrongPassword(RakNet::RakString usedPassword, RakNet::UDPProxyServer *proxyServerPlugin)
	{
		//LError << QueryName() << "(" << proxyServerPlugin->GetBindIP().C_String() << ") failed login to UDPProxyCoordinator. " << usedPassword.C_String() << " was the wrong password." << LEnd;
		isSupported=QUERY;
		//delete udpps;
		//udpps=0;
	}

	std::vector<UDPProxyServer*> udpps;
};
struct CloudServerFramework : public SampleFramework
{
	CloudServerFramework() {isSupported=CloudServerFramework_Supported; m_count = 0;}
	virtual const char * QueryName(void) {return "CloudServer";}
	virtual const char * QueryRequirements(void) {return "None.";}
	virtual const char * QueryFunction(void) {return "Single instance cloud server that maintains connection counts\nUseful as a directory server to find other client instances.";}
	virtual void Init(RakNet::RakPeerInterface *rakPeer)
	{
		if (isSupported==SUPPORTED)
		{
			cloudServer = new CloudServer;
			rakPeer->AttachPlugin(cloudServer);
			cloudClient = new CloudClient;
			rakPeer->AttachPlugin(cloudClient);
			cloudServerHelperFilter = new CloudServerHelperFilter;
			cloudServer->AddQueryFilter(cloudServerHelperFilter);
			cloudServerHelper.OnConnectionCountChange(rakPeer, cloudClient);
		}
	}
	virtual void ProcessPacket(RakNet::RakPeerInterface *rakPeer, Packet *packet)
	{
		if (isSupported!=SUPPORTED)
			return;

		switch (packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			//LOk << ++m_count << " Got connection to " << packet->systemAddress.ToString(true) << LEnd;
			cloudServerHelper.OnConnectionCountChange(rakPeer, cloudClient);
			break;
		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
			//LBlue << --m_count << " Lost connection to " << packet->systemAddress.ToString(true) << LEnd;
			cloudServerHelper.OnConnectionCountChange(rakPeer, cloudClient);
			break;
		}
	}
	virtual void Shutdown(RakNet::RakPeerInterface *rakPeer)
	{
		if (cloudServer)
		{
			rakPeer->DetachPlugin(cloudServer);
			delete cloudServer;
			cloudServer=0;
			rakPeer->DetachPlugin(cloudClient);
			delete cloudClient;
			cloudClient=0;
			delete cloudServerHelperFilter;
			cloudServerHelperFilter=0;
		}
	}

	RakNet::CloudServer *cloudServer;
	RakNet::CloudClient *cloudClient;
	RakNet::CloudServerHelperFilter *cloudServerHelperFilter;
	RakNet::CloudServerHelper cloudServerHelper;
	i32 m_count;
};

cRakNetP2PServer::cRakNetP2PServer()
{
	m_pPeer = RakPeerInterface::GetInstance();
	m_ppSampleFramework = new SampleFramework*[FEATURE_LIST_COUNT];
	memset(m_ppSampleFramework, 0, sizeof(SampleFramework*)*FEATURE_LIST_COUNT);
	m_ppSampleFramework[NAT_TYPE_DETECTION_SERVER] = new NatTypeDetectionServerFramework;
	m_ppSampleFramework[NAT_PUNCHTHROUGH_SERVER] = new NatPunchthroughServerFramework;
	m_ppSampleFramework[UDP_PROXY_COORDINATOR] = new UDPProxyCoordinatorFramework;
	m_ppSampleFramework[UDP_PROXY_SERVER] = new UDPProxyServerFramework;
	m_ppSampleFramework[CLOUD_SERVER] = new CloudServerFramework;
}

cRakNetP2PServer::~cRakNetP2PServer()
{
	for (int i = 0; i < FEATURE_LIST_COUNT; i++)
		delete m_ppSampleFramework[i];
	delete[] m_ppSampleFramework;
	RakPeerInterface::DestroyInstance(m_pPeer);
}

bool cRakNetP2PServer::Init(cpstr sBind, cpstr sProxy, cpstr sClientConnect)
{
	m_pPeer->SetMaximumIncomingConnections(c_ui16ServerMaxConnect);
	m_pPeer->SetTimeoutTime(c_tmsPeerTimeout, UNASSIGNED_SYSTEM_ADDRESS);
	if (sProxy == 0) {
		m_ppSampleFramework[NAT_TYPE_DETECTION_SERVER]->isSupported = UNSUPPORTED;
		m_ppSampleFramework[NAT_PUNCHTHROUGH_SERVER]->isSupported = SUPPORTED;
		m_ppSampleFramework[UDP_PROXY_COORDINATOR]->isSupported = SUPPORTED;
		m_ppSampleFramework[UDP_PROXY_SERVER]->isSupported = UNSUPPORTED;
		m_ppSampleFramework[CLOUD_SERVER]->isSupported = SUPPORTED;
		vector<SocketDescriptor> aSd;
		vector< pair<string,ui16> > aSad;
		if (!StrToAddress(aSad, sBind))
			return false;
		for (ui32 i = 0; i < (ui32)aSad.size(); i++)
			aSd.push_back(SocketDescriptor(aSad[i].second, aSad[i].first.c_str()));
		if (m_pPeer->Startup(c_ui16ServerMaxConnect, &aSd.front(), (ui32)aSd.size()) != RakNet::RAKNET_STARTED)
			return false;
	}
	else {
		m_ppSampleFramework[NAT_TYPE_DETECTION_SERVER]->isSupported = UNSUPPORTED;
		m_ppSampleFramework[NAT_PUNCHTHROUGH_SERVER]->isSupported = UNSUPPORTED;
		m_ppSampleFramework[UDP_PROXY_COORDINATOR]->isSupported = UNSUPPORTED;
		m_ppSampleFramework[UDP_PROXY_SERVER]->isSupported = SUPPORTED;
		m_ppSampleFramework[CLOUD_SERVER]->isSupported = UNSUPPORTED;
		vector< pair<string,ui16> > aSad;
		if (!StrToAddress(aSad, sProxy))
			return false;
		g_aProxyCoordinator.clear();
		for (vector< pair<string,ui16> >::iterator it = aSad.begin(); it != aSad.end(); it++)
			g_aProxyCoordinator.push_back(SystemAddress(it->first.c_str(), it->second));
		vector<SocketDescriptor> aSd;
		if (!StrToAddress(aSad, sBind))
			return false;
		for (ui32 i = 0; i < (ui32)aSad.size(); i++)
			aSd.push_back(SocketDescriptor(aSad[i].second, aSad[i].first.c_str()));
		if (m_pPeer->Startup(c_ui16ServerMaxConnect, &aSd.front(), (ui32)aSd.size()) != RakNet::RAKNET_STARTED)
			return false;
	}
	for (int i = 0; i < FEATURE_LIST_COUNT; i++)
		if (m_ppSampleFramework[i]->isSupported == SUPPORTED)
			m_ppSampleFramework[i]->Init(m_pPeer);
	return true;
}

void cRakNetP2PServer::Release()
{
	for (int i = 0; i < FEATURE_LIST_COUNT; i++)
		if (m_ppSampleFramework[i]->isSupported == SUPPORTED)
			m_ppSampleFramework[i]->Shutdown(m_pPeer);
	m_pPeer->Shutdown(100);
	delete this;
}

void cRakNetP2PServer::RunOnce()
{
	for (Packet* packet = m_pPeer->Receive(); packet; m_pPeer->DeallocatePacket(packet), packet = m_pPeer->Receive())
		for (int i = 0; i < FEATURE_LIST_COUNT; i++)
			if (m_ppSampleFramework[i]->isSupported == SUPPORTED)
				m_ppSampleFramework[i]->ProcessPacket(m_pPeer, packet);
}

}