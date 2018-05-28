#include "cRakNetUDPSocket.h"
#include "../../inc/CrossString.h"

using namespace std;
using namespace cm;
using namespace RakNet;

namespace cm
{

cRakNetUDPSocket::cRakNetUDPSocket()
{
	m_pUdp = nullptr;
	m_bTrace = true;
	m_funcUDPCallback = nullptr;
}

cRakNetUDPSocket::~cRakNetUDPSocket()
{
	_Release();
}

bool cRakNetUDPSocket::Init(cpstr sBind)
{
	m_pUdp = RakPeerInterface::GetInstance();
	if (sBind) {
		vector<SocketDescriptor> aSd;
		vector< pair<string,ui16> > aSad;
		if (!StrToAddress(aSad, sBind))
			return false;
		for (ui32 i = 0; i < (ui32)aSad.size(); i++)
			aSd.push_back(SocketDescriptor(aSad[i].second, aSad[i].first.c_str()));
		m_pUdp->SetMaximumIncomingConnections(1);
		return RAKNET_STARTED == m_pUdp->Startup(1, &aSd.front(), (ui32)aSd.size());
	} else {
		SocketDescriptor sd(0,0);
		m_pUdp->SetMaximumIncomingConnections(1);
		return RAKNET_STARTED == m_pUdp->Startup(1, &sd, 1);
	}
}

void cRakNetUDPSocket::_Release()
{
	if (m_pUdp) {
		m_pUdp->Shutdown(100);
		RakPeerInterface::DestroyInstance(m_pUdp);
		m_pUdp = nullptr;
	}
}

void cRakNetUDPSocket::Release()
{
	_Release();
	delete this;
}

void cRakNetUDPSocket::RunOnce()
{
	for (Packet* packet = m_pUdp->Receive(); packet; m_pUdp->DeallocatePacket(packet), packet = m_pUdp->Receive()) {
		if (packet->data[0] == ID_ADVERTISE_SYSTEM && packet->length > 1) {
			std::string data((cpstr)packet->data+1, packet->length-1);
			NETTRACE << packet->systemAddress.ToString(true) << " " << data << LEnd;
			if (m_funcUDPCallback) {
				std::string ret = m_funcUDPCallback(data);
				if (!ret.empty())
					m_pUdp->AdvertiseSystem(packet->systemAddress.ToString(false),
						packet->systemAddress.GetPort(), ret.data(), ret.size());
			}
		}
	}
}

void cRakNetUDPSocket::Send(const std::string& data, cpstr sAddress)
{
	string s = sAddress;
	int of = s.find(":");
	string host = s.substr(0, of);
	ui16 port = (ui16)CrossString::_10ToInt(s.substr(of+1));
	m_pUdp->AdvertiseSystem(host.c_str(), port, data.data(), data.size());
}

}