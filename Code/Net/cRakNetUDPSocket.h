#ifndef craknetudpsocket_h
#define craknetudpsocket_h

#include "../../inc/iNet.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/BitStream.h"

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