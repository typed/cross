#ifndef craknetp2pserver_h
#define craknetp2pserver_h

#include "../../inc/iNet.h"
#include "../../inc/iReference.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakPeerInterface.h"

namespace cross
{

struct SampleFramework;

class cRakNetP2PServer :
	public iP2PServer
{
public:
	cRakNetP2PServer();
	~cRakNetP2PServer();
	bool Init(cpstr sBind, cpstr sProxy, cpstr sClientConnect);
	void Release();
	void RunOnce();
private:
	SampleFramework**					m_ppSampleFramework;
	RakNet::RakPeerInterface*			m_pPeer;
};

}

#endif