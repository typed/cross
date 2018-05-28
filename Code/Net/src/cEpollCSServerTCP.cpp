#include "cEpollCSServerTCP.h"

#ifdef BUILD_EPOLL

#include "../../inc/iCompress.h"
#include "../../inc/CrossString.h"

using namespace std;
using namespace cm;

namespace cm
{

inline void setnonblocking(int sock) {
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts >= 0) {
		opts = opts | O_NONBLOCK;
		fcntl(sock, F_SETFL, opts);
	}
}

cEpollCSServerTCP::cEpollCSServerTCP()
{
	m_bTrace = false;
	m_epfd = 0;
	m_listenfd = 0;
	m_events = 0;
	m_baselinkid = 0;
	m_data = new i8[BUFF_SIZE];
	m_linknum = 0;
}

cEpollCSServerTCP::~cEpollCSServerTCP()
{
	m_packs.clear();
	if (m_data) {
		delete[] m_data;
		m_data = 0;
	}
	if (m_events) {
		delete[] m_events;
		m_events = 0;
	}
	if (m_listenfd) {
		close(m_listenfd);
		m_listenfd = 0;
	}
	if (m_epfd) {
		close(m_epfd);
		m_epfd = 0;
	}
}

bool cEpollCSServerTCP::Init(cpstr sBind, ui16 nMaxConnect, ui32 nTimeout)
{
	m_maxConnect = nMaxConnect;
	vector< pair<string, ui16> > aSad;
	if (!StrToAddress(aSad, sBind))
		return false;
	m_events = new epoll_event[1024];
	m_epfd = epoll_create(1024);
	m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(m_listenfd);
	epoll_event ev;
    ev.data.fd = m_listenfd;
    ev.events = EPOLLIN;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &ev);
	sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_aton(aSad[0].first.c_str(), &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(aSad[0].second);
    bind(m_listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(m_listenfd, LISTENQ);
	return true;
}
void cEpollCSServerTCP::Release()
{
	delete this;
}

bool cEpollCSServerTCP::SendAll(cpvd buf, ui32 len)
{
	return true;
}

bool cEpollCSServerTCP::Disconnect(ui32 id)
{
	return true;
}

bool cEpollCSServerTCP::Send(ui32 id, cpvd buf, ui32 len)
{
	if (buf == 0 || len == 0 || len >= 1048576)
		return false;
	i32 sockfd = Link2Sk(id);
	if (sockfd == -1)
		return false;
	stTCPPack pk;
	pk.header.lenData = len;
	pk.data.append((cpstr)&pk.header, sizeof(pk.header));
	pk.data.append((cpstr)buf, len);
	string& dat = m_mapSend[sockfd];
	dat.append(pk.data.c_str(), pk.data.size());
	epoll_event ev;
	ev.data.fd = sockfd;
	ev.events = EPOLLOUT;
	epoll_ctl(m_epfd, EPOLL_CTL_MOD, sockfd, &ev);
	return true;
}

bool cEpollCSServerTCP::GetLinkIpPort(ui32 id, pstr ip, ui16& port)
{
	mapAddress::iterator it = m_mapAddress.find(id);
	if (it == m_mapAddress.end())
		return false;
	strcpy(ip, it->second.c_str());
	port = 0;
	return true;
}

void cEpollCSServerTCP::RunOnce()
{
	EpollRun();
	ui32 hsz = sizeof(stTCPHeader);
	for (aEpEvent::iterator it = m_aEpEvent.begin(); it != m_aEpEvent.end(); it++) {
		const stEpEvent& ee = *it;
		switch(ee.type) {
		case 'A':
			{
				NETTRACE << "accapt a connection from " << ee.buf << LEnd;
				if (m_pCallback)
					m_pCallback->OnConnect(ee.lnk, m_userdata);
				break;
			}
		case 'R':
			{
				NETTRACE << "close a connection " << LEnd;
				if (m_pCallback)
					m_pCallback->OnDisconnect(ee.lnk, m_userdata);
				break;
			}
		case 'D':
			{
				NETTRACE << "recv " << ee.buf.size() << LEnd;
				stTCPPack& pk = m_packs[ee.lnk];
				i8* pData = (i8*)ee.buf.data();
				ui32 nLen = ee.buf.size();
				while (nLen != 0) {
					if (nLen > hsz) {
						stTCPHeader tmp;
						memcpy(&tmp, pData, hsz);
						if (tmp.guid == c_ui64TCPGUID) {
							if (tmp.lenData >= 1048576)
								break;
							pk.clear();
							pk.header = tmp;
							if (nLen > pk.header.lenData) {
								pk.data.append(pData + hsz, pk.header.lenData);
								pData = pData + hsz + pk.header.lenData;
								nLen = nLen - (hsz + pk.header.lenData);
							} else {
								pk.data.append(pData + hsz, nLen - hsz);
								pData = 0;
								nLen = 0;
							}
							if (pk.data.size() < pk.header.lenData) {
								break;
							}
						} else {
							ui32 nLast = pk.header.lenData - pk.data.size();
							if (nLen > nLast) {
								pk.data.append(pData, nLast);
								pData = pData + nLast;
								nLen = nLen - nLast;
							} else {
								pk.data.append(pData, nLen);
								pData = 0;
								nLen = 0;
							}
						}
					} else {
						ui32 nLast = pk.header.lenData - pk.data.size();
						if (nLen > nLast) {
							pk.data.append(pData, nLast);
							pData = pData + nLast;
							nLen = nLen - nLast;
						} else {
							pk.data.append(pData, nLen);
							pData = 0;
							nLen = 0;
						}
					}
					if (pk.data.size() == pk.header.lenData) {
						if (m_pCallback) {
							m_pCallback->OnRecv(ee.lnk, pk.data.data(), pk.data.size(), m_userdata);
						}
						pk.clear();
					} else if (pk.data.size() > pk.header.lenData) {
						//error
						pk.clear();
						LError << "recv buffer error." << LEnd;
						//assert(0);
						break;
					}
				}
			}
			break;
		}
	}
}

void cEpollCSServerTCP::EpollRun()
{
	i32 nfds = epoll_wait(m_epfd, m_events, m_maxConnect, 0);
	for (i32 i = 0; i < nfds; ++i) {
		epoll_event& ee = m_events[i];
		if (ee.data.fd == m_listenfd) {
			socklen_t clilen;
			sockaddr_in clientaddr;
			i32 connfd = accept(m_listenfd, (sockaddr*)&clientaddr, &clilen);
			if (connfd < 0)
				continue;
			m_linknum++;
			if (m_linknum > m_maxConnect) {
				close(connfd);
				continue;
			}
			m_baselinkid++;
			ui32 id = m_baselinkid;
			m_mapSk2Link[connfd] = id;
			m_mapLink2Sk[m_baselinkid] = connfd;
			setnonblocking(connfd);
			
			epoll_event ev;
			ev.data.fd = connfd;
			ev.events = EPOLLIN;
			epoll_ctl(m_epfd, EPOLL_CTL_ADD, connfd, &ev);
			
			string ip = inet_ntoa(clientaddr.sin_addr);
			stEpEvent ep;
			ep.type = 'A';
			ep.lnk = id;
			ep.buf = ip;
			m_aEpEvent.push_back(ep);
			m_mapAddress[id] = ip;
		}
		else if (ee.events & EPOLLIN) {//如果是已经连接的用户，并且收到数据，那么进行读入。
			i32 sockfd = ee.data.fd;
			if (sockfd < 0)
				continue;
			ui32 lnk = Sk2Link(sockfd);
			i32 n = read(sockfd, m_data, BUFF_SIZE);
			if (n > 0) {
				stEpEvent ep;
				ep.type = 'D';
				ep.lnk = lnk;
				ep.buf.append(m_data, n);
				m_aEpEvent.push_back(ep);
				epoll_event ev;
				ev.data.fd = sockfd;
				ev.events = EPOLLOUT;
				epoll_ctl(m_epfd, EPOLL_CTL_MOD, sockfd, &ev);
			} else {
				if (n == -1 && errno == EAGAIN) {
					continue;
				} else {
					stEpEvent ep;
					ep.type = 'R';
					ep.lnk = lnk;
					m_aEpEvent.push_back(ep);
					m_mapAddress.erase(lnk);
					m_mapLink2Sk.erase(lnk);
					m_mapSk2Link.erase(sockfd);
					m_packs.erase(lnk);
					epoll_ctl(m_epfd, EPOLL_CTL_DEL, sockfd, 0);
					close(sockfd);
				}
			}
		} else if (ee.events & EPOLLOUT) {
			i32 sockfd = ee.data.fd;
			if (sockfd < 0)
				continue;
			string& dat = m_mapSend[sockfd];
			while (dat.size() > 0) {
				i32 nwrite = write(sockfd, (i8*)dat.data(), dat.size());
				if (nwrite == -1) {
					if (errno == EAGAIN)
						break;
					else {
						NETTRACE << "write error" << LEnd;
						dat.clear();
						break;
					}
				} else
					dat = dat.substr(nwrite);
			}
			epoll_event ev;
			ev.data.fd = sockfd;
			ev.events = EPOLLIN;
			epoll_ctl(m_epfd, EPOLL_CTL_MOD, sockfd, &ev);
		}
	}
}

ui32 cEpollCSServerTCP::Sk2Link(i32 sk)
{
	mapSk2Link::iterator it = m_mapSk2Link.find(sk);
	if (it != m_mapSk2Link.end())
		return it->second;
	return 0;
}

i32 cEpollCSServerTCP::Link2Sk(ui32 lnk)
{
	mapLink2Sk::iterator it = m_mapLink2Sk.find(lnk);
	if (it != m_mapLink2Sk.end())
		return it->second;
	return -1;
}

}

#endif