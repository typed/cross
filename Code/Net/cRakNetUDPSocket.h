#ifndef craknetudpsocket_h
#define craknetudpsocket_h

#include "iNet.h"
#include "Raknet/RakNetTypes.h"
#include "Raknet/RakPeerInterface.h"
#include "Raknet/BitStream.h"

#include "RakNetConfig.h"

namespace cross
{

class cRakNetUDPSocket :
	public iUDPSocket
{
public:
	cRakNetUDPSocket();
	~cRakNetUDPSocket();
	bool Init(cpstr sBind);
	void Release();
	void RunOnce();
	void Send(const std::string& data, cpstr sAddress);
	void SetCallback(funcUDPCallback p) {m_funcUDPCallback = p;}
private:
	void _Release();
	funcUDPCallback					m_funcUDPCallback;
	RakNet::RakPeerInterface*		m_pUdp;
	b8								m_bTrace;
};

}

#endif