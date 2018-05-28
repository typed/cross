#ifndef craknetp2pserver_h
#define craknetp2pserver_h

#include "iNet.h"
#include "Common/iReference.h"
#include "Raknet/RakNetTypes.h"
#include "Raknet/RakPeerInterface.h"

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