#include "SocketServer.h"

#include <assert.h>

SocketServer::SocketServer(unsigned short port) : _port(port), _ip(NULL)
{
	this->init();
}

SocketServer::SocketServer(char* ip, unsigned short port) : _ip(ip), _port(port)
{
	this->init();
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

void SocketServer::Close()
{
	int iret = closesocket(this->hSocket);
	if (iret == 0) {
		this->hSocket = INVALID_SOCKET;
	}
}


// 创建套接字
SOCKET SocketServer::CreateSocket()
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	int iret = WSAStartup(wVersion, &wsaData);
	assert(iret == 0);

	SOCKET hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	assert(hSocket != INVALID_SOCKET);
	return hSocket;
}

void SocketServer::Bind()
{
	assert(hSocket != INVALID_SOCKET);
	// 初始化地址信息
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr_in));
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

int SocketServer::recvMsg(sockaddr_in* fromSockaddr, int* addrlen, unsigned char* byteBuf, int nsize)
{
	assert(byteBuf);
	assert(nsize > 0);
	char* buffer = (char*)byteBuf;
	int nread = recvfrom(this->hSocket, buffer, nsize, 0, (struct sockaddr*)fromSockaddr, addrlen);
	if (nread == SOCKET_ERROR) {
		int ierr = WSAGetLastError();
		switch (ierr)
		{
		case WSAEWOULDBLOCK: // 接受缓冲区暂无数数A non-blocking socket operation could not be completed immediately.
			nread = 0;
			break;
		case WSAEBADF: // The file handle supplied is not valid.
		case WSAETIMEDOUT: // 连接超时
		case WSAENETDOWN: // 网络中断
		default:
			//this->Close();
			break;
		}
	}
	return nread;
}

bool SocketServer::sendMsg(const sockaddr_in* toSockaddr, int addrlen, const unsigned char* byteBuf, int nsize)
{
	if (!(byteBuf && nsize > 0)) {
		return false;
	}
	else {
		const char* buffer = (const char*)byteBuf;
		int limit = 1024 * 4;
		int nsend = 0;
		while (nsend < nsize) {
			limit = (nsize - nsend) < limit ? (nsize - nsend) : limit;
			int nwrite = sendto(this->hSocket, (buffer + nsend), limit, 0, (struct sockaddr*)toSockaddr, addrlen);
			if (nwrite > 0) {
				nsend += nwrite;
			}
			else if (nwrite == SOCKET_ERROR) {
				int ierr = WSAGetLastError();
				switch (ierr)
				{
				case WSAEBADF: // The file handle supplied is not valid.
					this->Close();
					return false;
				default:
					this->Close();
					return false;
				}
			}
			else {
				return false;
			}
		}
		return true;
	}
}

bool SocketServer::setnBlock(bool nblock)
{
	assert(hSocket != INVALID_SOCKET);
	u_long mode = nblock ? 1 : 0;
	int iret = ioctlsocket(this->hSocket, FIONBIO, &mode);
	assert(iret == 0);
	return true;
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

/*
success 0
error SOCKET_ERROR
*/
int SocketServer::getSocketOpt(char* buf, int* len, int level, int optname)
{
	return getsockopt(this->hSocket, level, optname, buf, len);
}