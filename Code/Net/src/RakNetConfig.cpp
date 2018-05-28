#include "RakNetConfig.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/SocketLayer.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/Gets.h"
#include "raknet/RakSleep.h"
#include "raknet/Itoa.h"
#include "cNetMapping.h"
#include "../../inc/CrossString.h"

using namespace std;
using namespace RakNet;

namespace cm
{

CloudKey& GetCloudKeyClientInfo() {
	static CloudKey s_ck("ClientInfo", 0);
	return s_ck;
}

RakNet::CloudKey& GetCloudKeyProxyInfo() {
	static CloudKey s_ck("ProxyInfo", 0);
	return s_ck;
}

//bool OpenUPNP(RakPeerInterface* rakPeer)
//{
//	bool sampleResult = false;
//	struct UPNPDev * devlist = 0;
//	devlist = upnpDiscover(2000, 0, 0, 0, 0, 0);
//	if (devlist)
//	{
//		printf("List of UPNP devices found on the network :\n");
//		struct UPNPDev * device;
//		for(device = devlist; device; device = device->pNext)
//		{
//			printf(" desc: %s\n st: %s\n\n",
//				device->descURL, device->st);
//		}
//
//		char lanaddr[64];	/* my ip address on the LAN */
//		struct UPNPUrls urls;
//		struct IGDdatas data;
//		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1)
//		{
//			// 4/16/2012 Why was I doing this? Just to read my external port? That shouldn't be necessary
//			/*
//			SystemAddress serverAddress=SelectAmongConnectedSystems(rakPeer, "NatTypeDetectionServer");
//			if (serverAddress==RakNet::UNASSIGNED_SYSTEM_ADDRESS)
//			{
//				serverAddress=ConnectBlocking(rakPeer, "NatTypeDetectionServer", DEFAULT_SERVER_ADDRESS, DEFAULT_SERVER_PORT);
//				if (serverAddress==RakNet::UNASSIGNED_SYSTEM_ADDRESS)
//				{
//					printf("Failed to connect to a server.\n");
//					sampleResult=FAILED;
//					return;
//				}
//			}
//
//
//			char iport[32];
//			Itoa(sockets[0]->boundAddress.GetPort(),iport,10);
//			char eport[32];
//			Itoa(rakPeer->GetExternalID(serverAddress).GetPort(),eport,10);
//			*/
//
//			// Use same external and internal ports
//			DataStructures::List<RakNetSmartPtr<RakNetSocket> > sockets;
//			rakPeer->GetSockets(sockets);
//			char iport[32];
//			Itoa(sockets[0]->boundAddress.GetPort(),iport,10);
//			char eport[32];
//			strcpy(eport, iport);
//
//
//			// Version 1.5
//// 				int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
//// 					 eport, iport, lanaddr, 0, "UDP", 0);
//
//			// Version miniupnpc-1.6.20120410
//			int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
//				 					    eport, iport, lanaddr, 0, "UDP", 0, "0");
//
//			if(r!=UPNPCOMMAND_SUCCESS)
//				printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
//				eport, iport, lanaddr, r, strupnperror(r));
//
//			char intPort[6];
//			char intClient[16];
//
//			// Version 1.5
//// 				r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
//// 					data.first.servicetype,
//// 					eport, "UDP",
//// 					intClient, intPort);
//
//			// Version miniupnpc-1.6.20120410
//			char desc[128];
//			char enabled[128];
//			char leaseDuration[128];
//			r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
//				data.first.servicetype,
//				eport, "UDP",
//				intClient, intPort,
//				desc, enabled, leaseDuration);
//
//			if(r!=UPNPCOMMAND_SUCCESS)
//			{
//				printf("GetSpecificPortMappingEntry() failed with code %d (%s)\n",
//				r, strupnperror(r));
//				sampleResult = false;
//			}
//			else
//				sampleResult = true;
//		}
//		else
//			sampleResult = false;
//	}
//	else
//		sampleResult = false;
//	return sampleResult;
//}

SystemAddress ConnectBlocking(RakNet::RakPeerInterface *rakPeer, const char *hostName, const SystemAddress& sa)
{
	SystemAddress address;
	if (sa == UNASSIGNED_SYSTEM_ADDRESS)
	{
		char ipAddr[64];
		printf("Enter IP of system %s is running on: ", hostName);
		Gets(ipAddr,sizeof(ipAddr));
		if (ipAddr[0]==0)
		{
			printf("Failed. Not connected to %s.\n", hostName);
			return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		}
		char port[64];
		printf("Enter port of system %s is running on: ", hostName);
		Gets(port, sizeof(port));
		if (port[0]==0)
		{
			printf("Failed. Not connected to %s.\n", hostName);
			return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		}
		address.FromStringExplicitPort(ipAddr, CrossString::_10ToInt(port));
	}
	else
		address = sa;
	if (rakPeer->Connect(address.ToString(false), address.GetPort(), 0, 0)!=RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		printf("Failed connect call for %s.\n", hostName);
		return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	}
	printf("Connecting...\n");
	RakNet::Packet *packet;
	while (1)
	{
		for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
		{
			if (packet->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
			{
				return packet->systemAddress;
			}
			else
			{
				//LInfo << packet->data[0] << LEnd;
				return RakNet::UNASSIGNED_SYSTEM_ADDRESS;
			}
			RakSleep(100);
		}
	}
}

bool ConnectTrue(RakNet::RakPeerInterface* rakPeer, const RakNet::SystemAddress& sa)
{
	rakPeer->Connect(sa.ToString(false), sa.GetPort(), 0, 0);
	//while(1) {
	//	ConnectionAttemptResult car = rakPeer->Connect(sa.ToString(false), sa.GetPort(), 0, 0);
	//	if (car == CONNECTION_ATTEMPT_STARTED)
	//		return true;
	//	RakSleep(1000);
	//}
	return true;
}

void ConnectMustTrue(RakNet::RakPeerInterface* rakPeer, const RakNet::SystemAddress& sa)
{
	while(1) {
		if (sa == ConnectBlocking(rakPeer, "", sa))
			return;
		RakSleep(1000);
	}
}

cpstr RaknetDomainToIp(cpstr sDomain)
{
	static i8 ip[65];
	RakNetSocket2::DomainNameToIP(sDomain, ip);
	return ip;
}

SystemAddress RaknetAddressMapping(cpstr sIP, ui16 port)
{
	string sIPMapped;
	ui16 portMapped;
	cNetMapping::GetInstance().AddressMapping(sIP, port, sIPMapped, portMapped);
	return SystemAddress(sIPMapped.c_str(), portMapped);
}

SystemAddress RaknetAddressMapping(SystemAddress sa)
{
	return RaknetAddressMapping(sa.ToString(false), sa.GetPort());
}

}