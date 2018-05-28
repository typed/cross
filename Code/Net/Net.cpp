#include "iNet.h"
#include "cRakNetP2PServer.h"
#include "cRakNetCSServer.h"
#include "cRakNetCSClient.h"
#include "cRakNetCSServerTCP.h"
#include "cRakNetCSClientTCP.h"
#include "cEpollCSServerTCP.h"
#include "cRakNetP2PProxyServer.h"
#include "cRakNetP2PClient2.h"
#include "cRakNetTCPSocket.h"
#include "cRakNetUDPSocket.h"
#include "cNetMapping.h"
#include "Common/CrossString.h"
#include "Common/cThread.h"
#include "Raknet/HTTPConnection.h"

using namespace std;
using namespace RakNet;

namespace cross
{

iP2PServer* iP2PServer::CreateNormalInstance()
{
	return new cRakNetP2PServer();
}

iP2PServer* iP2PServer::CreateProxyInstance()
{
	return new cRakNetP2PProxyServer();
}

iP2PClient* iP2PClient::CreateInstance()
{
	return new cRakNetP2PClient2();
}

iCSServer* iCSServer::CreateInstance(b8 tcp)
{
	if (tcp) {
#ifdef BUILD_EPOLL
		return new cEpollCSServerTCP();
#else
		return new cRakNetCSServerTCP();
#endif
	}
	else
		return new cRakNetCSServer();
}

iCSClient* iCSClient::CreateInstance(b8 tcp)
{
	if (tcp)
		return new cRakNetCSClientTCP();
	else
		return new cRakNetCSClient();
}

iTCPSocket* iTCPSocket::CreateInstance()
{
	return new cRakNetTCPSocket();
}

iUDPSocket* iUDPSocket::CreateInstance()
{
	return new cRakNetUDPSocket();
}

void HTTPPostRTVoid(std::string host, i32 port, std::string dir, std::string data)
{
	TCPInterface* tcp = nullptr;
	HTTPConnection* http = nullptr;
	tcp = TCPInterface::GetInstance();
	if (tcp->Start(0, 64)) {
		http = HTTPConnection::GetInstance();
		http->Init(tcp, host.c_str(), port);
		http->Post(dir.c_str(), data.c_str());
		while (1) {
			Packet* packet = tcp->Receive();
			if (packet) {
				http->ProcessTCPPacket(packet);
				tcp->DeallocatePacket(packet);
			}
			http->Update();
			if (!http->IsBusy()) {
				//RakString ret = http->Read();
				break;
			}
			// Prevent 100% cpu usage
			SLEEP(30);
		}
	}
	if (http) {
		HTTPConnection::DestroyInstance(http);
		http = nullptr;
	}
	if (tcp) {
		tcp->Stop();
		TCPInterface::DestroyInstance(tcp);
		tcp = nullptr;
	}
}
std::string DomainToIP(cpstr sdomain)
{
	RakNet::SystemAddress sa;
	if (sa.FromStringExplicitPort(sdomain, 8080)) {
		i8 str1[128];
		sa.ToString(false, str1);
		return str1;
	}
	return "";
}
void UseNetMapped(bool b)
{
	cNetMapping::GetInstance().Use(b);
}
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
#	include <windows.h>
#	include <httpext.h>   
#	include <Nb30.h>
#	pragma comment(lib, "netapi32.lib")
#endif
cpstr GetLocalMac()
{
	static std::string s_mac;
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} ASTAT, *PASTAT;
	ASTAT Adapter;
	typedef struct _LANA_ENUM
	{
		UCHAR length;
		UCHAR lana[MAX_LANA];
	}LANA_ENUM;
	LANA_ENUM lana_enum;
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;     
	ncb.ncb_length = sizeof(LANA_ENUM);     
	uRetCode = Netbios(&ncb);     
	if (uRetCode == NRC_GOODRET) {
		for (int lana = 0; lana < lana_enum.length; lana++) {
			ncb.ncb_command = NCBRESET;
			ncb.ncb_lana_num = lana_enum.lana[lana];
			uRetCode = Netbios(&ncb);
			if (uRetCode == NRC_GOODRET)
				break;
		}
		if (uRetCode == NRC_GOODRET) {
			memset(&ncb, 0, sizeof(ncb)); 
			ncb.ncb_command = NCBASTAT; 
			ncb.ncb_lana_num = lana_enum.lana[0];
			strcpy((char*)ncb.ncb_callname, "*"); 
			ncb.ncb_buffer = (unsigned char *)&Adapter;
			ncb.ncb_length = sizeof(Adapter);
			uRetCode = Netbios(&ncb); 
			if (uRetCode == NRC_GOODRET) {
				s_mac = CrossString::Format("%02X-%02X-%02X-%02X-%02X-%02X",     
					Adapter.adapt.adapter_address[0],     
					Adapter.adapt.adapter_address[1],     
					Adapter.adapt.adapter_address[2],     
					Adapter.adapt.adapter_address[3],     
					Adapter.adapt.adapter_address[4],     
						Adapter.adapt.adapter_address[5] 
				);
			}
		}
	}
#endif
	return s_mac.c_str();
}

bool StrToAddress(addrs& as, string s)
{
	as.clear();
	vector<string> aSet;
	if (!CrossString::Split(s, ",", aSet))
		return false;
	for (vector<string>::iterator it = aSet.begin(); it != aSet.end(); it++) {
		string ss = *it;
		i32 of = ss.rfind(":");
		if (of != -1) {
			std::string ip = ss.substr(0, of);
			std::string port = ss.substr(of+1);
			as.push_back(make_pair(ip, (ui16)CrossString::_10ToInt(port)));
		}
	}
	return !as.empty();
}

ui64 NetNewGUID()
{
	return RakNet::RakPeerInterface::Get64BitUniqueRandomNumber();
}

}