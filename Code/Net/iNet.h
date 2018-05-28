

#ifndef cross_net_inet_h
#define cross_net_inet_h

#include "Common/Common.h"
#include <vector>
#include <string>
#include <functional>

namespace cross
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//P2P结构
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class iP2PServer
{
public:
	static iP2PServer* CreateNormalInstance();
	static iP2PServer* CreateProxyInstance();
	virtual void Release() = 0;
	//sBind和sProxy 格式为 "127.0.0.1,1000,127.0.0.2,1001...."
	virtual bool Init(cpstr sBind, cpstr sProxy = 0, cpstr sClientConnect = 0) = 0;
	virtual void RunOnce() = 0;
};

class iP2PClientStream
{
public:
	virtual void Write(cpvd buf, ui32 len) = 0;
	virtual bool Read(pvd buf, ui32 len) = 0;
};
class iP2PClientCallback
{
public:
	virtual void OnRecv(cpbyte, ui32) = 0;
	//事件同步
	virtual void OnRecvAction(ui32 id, cpvd buf, ui32 len) = 0;
	//帧属性同步
	virtual void OnGet(ui32 id, iP2PClientStream* pStream) = 0;
	virtual void OnSet(ui32 id, iP2PClientStream* pStream) = 0;
};
class iP2PClient
{
public:
	static iP2PClient* CreateInstance();
	virtual bool Init() = 0;
	virtual void Release() = 0;
	virtual void Login(ui32 id, cpstr ip, ui16 port) = 0;
	virtual void Logout() = 0;
	virtual void Connect(cpstr ip, ui16 port) = 0;
	virtual void Disconnect() = 0;
	virtual void DisconnectAllPlayer() = 0;
	virtual void RunOnce() = 0;
	virtual void SetCallback(iP2PClientCallback*) = 0;

	virtual void EnterRoom(cpstr sName) = 0;
	virtual void LeaveRoom() = 0;
	virtual bool IsInRoom() const = 0;
	virtual void TraceRoom() = 0;
	virtual bool IsHost() const = 0;
	virtual ui32 GetHostID() const = 0;
	virtual void Action(cpvd buf, ui32 len) = 0;
	virtual i32 GetPing(ui32 id) = 0;
	virtual cpstr GetRoomName() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C/S结构
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class iCSServerCallback
{
public:
	//有客户端接入时
	virtual void OnConnect(ui32 id, i32 userdata) = 0;
	//有客户端断开时
	virtual void OnDisconnect(ui32 id, i32 userdata) = 0;
	//收到消息
	virtual void OnRecv(ui32 id, cpvd buf, ui32 len, i32 userdata) = 0;
	//玩家名
	virtual cpstr GetPlayerName(ui32 id) const {return "";}
};
class iCSServer
{
public:
	static iCSServer* CreateInstance(b8 tcp);
	//sBind 格式为 "127.0.0.1,1000,127.0.0.2,1001...."
	virtual bool Init(cpstr sBind, ui16 nMaxConnect = 0, ui32 nTimeout = 0) = 0;
	virtual void Release() = 0;
	virtual bool Send(ui32, cpvd, ui32) = 0;
	virtual bool SendAll(cpvd, ui32) = 0;
	virtual bool Disconnect(ui32) = 0;
	virtual void DisconnectAll() = 0;
	virtual i32 GetPing(ui32) = 0;
	virtual bool GetLinkIpPort(ui32, pstr, ui16&) = 0;
	virtual void SetCallback(iCSServerCallback*) = 0;
	virtual void RunOnce() = 0;
	virtual void SetUserData(i32) = 0;
	virtual i32 GetUserData() const = 0;
	virtual void CloseBroadcast() = 0;
};

enum eConnectType
{
	CT_OK,
	CT_Error
};
enum eDisconnectType
{
	DT_ClientInitiative,		//客户端主动断开
	DT_ServerInitiative,		//服务器主动断开
	DT_Exception,				//异常断开
};

class iCSClientCallback
{
public:
	virtual void OnConnect(eConnectType ct, i32 userdata) = 0;
	virtual void OnDisconnect(eDisconnectType ct, i32 userdata) = 0;
	virtual void OnRecv(cpvd buf, ui32 len, i32 userdata) = 0;
};
class iCSClient
{
public:
	static iCSClient* CreateInstance(b8 tcp);
	virtual bool Init(ui32 nTimeout = 0, b8 bCheckTime = false) = 0;
	virtual void Release() = 0;
	virtual bool Connect(cpstr ip, ui16 port) = 0;
	virtual bool IsConnected() = 0;
	virtual void AutoConnect() = 0;
	virtual void NotAutoConnect() = 0;
	virtual bool Disconnect() = 0;
	virtual void RunOnce() = 0;
	virtual bool Send(cpvd, ui32) = 0;
	virtual void SetCallback(iCSClientCallback*) = 0;
	virtual i32 GetPing() = 0;
	virtual void SendPackage(ui32 waittime = 0) = 0;
	virtual void SetUserData(i32) = 0;
	virtual i32 GetUserData() const = 0;
};

enum eTCPCallbackType
{
	TCPC_OnConnect,
	TCPC_OnDisconnect,
	TCPC_OnRecv
};
struct stTCPSocketCallbackParam
{
	eTCPCallbackType type;
	ui32 linkid;
	std::string data;
	b8 bConnected;
	stTCPSocketCallbackParam() : type(TCPC_OnConnect), linkid(0), bConnected(false) {}
};
typedef std::function<void(const stTCPSocketCallbackParam&)> funcTCPCallback;
class iTCPSocket
{
public:
	static iTCPSocket* CreateInstance();
	virtual bool Init(ui16 nBindPort, ui16 nMaxIncomingConnect, ui16 nMaxConnect) = 0;
	virtual void Release() = 0;
	virtual void Connect(cpstr ip, ui16 port) = 0;
	virtual bool IsConnected(ui32 linkid = 0) = 0;
	virtual void Disconnect(ui32 linkid = 0) = 0;
	virtual void RunOnce() = 0;
	virtual void Send(cpvd, ui32, ui32 linkid = 0) = 0;
	virtual void SetCallback(funcTCPCallback) = 0;
};

typedef std::function<std::string(const std::string&)> funcUDPCallback;
class iUDPSocket
{
public:
	static iUDPSocket* CreateInstance();
	virtual bool Init(cpstr sBind = nullptr) = 0;
	virtual void Release() = 0;
	virtual void Send(const std::string&, cpstr sAddress) = 0;
	virtual void RunOnce() = 0;
	virtual void SetCallback(funcUDPCallback) = 0;
};

std::string DomainToIP(cpstr sdomain);
void HTTPPostRTVoid(std::string host, i32 port, std::string dir, std::string data);
cpstr GetLocalMac();
void UseNetMapped(bool);
typedef std::vector< std::pair<std::string, ui16> > addrs;
bool StrToAddress(addrs& as, std::string s);
ui64 NetNewGUID();

}

#endif