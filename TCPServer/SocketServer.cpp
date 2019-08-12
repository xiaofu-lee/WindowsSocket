#include "SocketServer.h"

#include <assert.h>

SocketServer::SocketServer(unsigned short port) : _port(port), _ip(NULL)
{
	init();
}

SocketServer::SocketServer(char* ip, unsigned short port) : _ip(ip), _port(port)
{
	init();
}

void SocketServer::init()
{
	this->hSocket = this->CreateSocket();
	this->Bind();
}

SocketServer::~SocketServer()
{
	this->Close();
	// 卸载DLL
	int iret = WSACleanup();
	assert(iret == 0);
}

// 创建套接字
SOCKET SocketServer::CreateSocket()
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	int iret = WSAStartup(wVersion, &wsaData);
	assert(iret == 0);
	SOCKET hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(hSocket != INVALID_SOCKET);
	return hSocket;
}

void SocketServer::Bind()
{
	assert(hSocket != INVALID_SOCKET);
	// 初始化地址信息
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET; // 使用IPv4
	sockaddr.sin_port = htons(this->_port); // 端口
	if (this->_ip) {
		//sockaddr.sin_addr.s_addr = inet_addr(this->_ip);
		int iret = inet_pton(AF_INET, this->_ip, &(sockaddr.sin_addr));
	}
	else {
		sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 主机ip
	}
	// 绑定套接字地址
	int iret = bind(hSocket, (struct sockaddr*)(&sockaddr), sizeof(sockaddr));
	assert(iret != SOCKET_ERROR);
	this->_sockaddr = sockaddr;
}

bool SocketServer::setnBlock(bool nblock)
{
	assert(hSocket != INVALID_SOCKET);
	u_long mode = nblock ? 1 : 0;
	int iret = ioctlsocket(this->hSocket, FIONBIO, &mode);
	assert(iret == 0);
	return true;
}

/*
success 0
error SOCKET_ERROR
*/
int SocketServer::getSocketOpt(char* buf, int *len, int level, int optname)
{
	return getsockopt(this->hSocket, level, optname, buf, len);
}

void SocketServer::Listen(int backlog)
{
	assert(hSocket != INVALID_SOCKET);
	int iret = listen(hSocket, backlog);
	assert(iret == 0);
}

Connector* SocketServer::Accept()
{
	assert(this->hSocket != INVALID_SOCKET);
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	int isize = sizeof(sockaddr);
	SOCKET mhcSocket = accept(this->hSocket, (struct sockaddr*) & sockaddr, &isize);
	if (mhcSocket == INVALID_SOCKET) {
		closesocket(mhcSocket);
		return NULL;
	}
	else {
		Connector* connector = new Connector(mhcSocket, sockaddr);
		return connector;
	}
}

void SocketServer::Close()
{
	int iret = closesocket(this->hSocket);
	if (iret == 0) {
		this->hSocket = INVALID_SOCKET;
	}
}

bool SocketServer::invalid()
{
	if (this->hSocket == INVALID_SOCKET) {
		return true;
	}
	else {
		return false;
	}
}
