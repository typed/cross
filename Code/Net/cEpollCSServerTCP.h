#ifndef cepollcsservertcp_h
#define cepollcsservertcp_h

#ifdef BUILD_EPOLL

#include "RakNetConfig.h"
#include "../../inc/iNet.h"
#include "../../inc/cThread.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <map>
#include <vector>

namespace cross
{
	
#define BUFF_SIZE 1048576
#define LISTENQ 20

typedef std::map<i32,ui32> mapSk2Link;
typedef std::map<ui32,i32> mapLink2Sk;
struct stEpEvent {
	i32 type;
	ui32 lnk;
	std::string buf;
};
typedef std::vector<stEpEvent> aEpEvent;
typedef std::map<i32,std::string> mapSend;
typedef std::map<ui32,std::string> mapAddress;

class cEpollCSServerTCP :
	public iCSServer
{
public:
	cEpollCSServerTCP();
	~cEpollCSServerTCP();
	bool Init(cpstr sBind, ui16 nMaxConnect = 0, ui32 nTimeout = 0);
	void Release();
	bool Send(ui32, cpvd, ui32);
	bool SendAll(cpvd, ui32);
	bool Disconnect(ui32);
	void DisconnectAll() {}
	i32 GetPing(ui32) {return -1;}
	bool GetLinkIpPort(ui32, pstr, ui16&);
	void SetCallback(iCSServerCallback* p) {m_pCallback = p;}
	void RunOnce();
	void SetUserData(i32 ud) {m_userdata = ud;}
	i32 GetUserData() const {return m_userdata;}
private:
	void EpollRun();
	ui32 Sk2Link(i32);
	i32 Link2Sk(ui32);
	i32								m_maxConnect;
	i32								m_listenfd;
	i32								m_epfd;
	epoll_event*					m_events;
	i32 							m_userdata;
	iCSServerCallback* 				m_pCallback;
	i8*								m_data;
	b8								m_bTrace;
	aEpEvent						m_aEpEvent;
	mapSend							m_mapSend;
	mapSk2Link						m_mapSk2Link;
	mapLink2Sk						m_mapLink2Sk;
	mapAddress						m_mapAddress;
	ui32							m_baselinkid;
	ui32							m_linknum;
	std::map<ui32,stTCPPack>		m_packs;
};

}

#endif

#endif