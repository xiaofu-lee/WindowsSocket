#include "Connector.h"
#include <assert.h>


Connector::Connector(SOCKET hSocket, struct sockaddr_in sockaddr) :hSocket(hSocket), _sockaddr_in(sockaddr)
{
}

Connector::~Connector()
{
}

SOCKET Connector::getSocket()
{
	return this->hSocket;
}

void Connector::setSocket(SOCKET hSocket)
{
	this->hSocket = hSocket;
}

struct sockaddr_in& Connector::getSockaddr()
{
	return this->_sockaddr_in;
}

void Connector::setSockaddr(struct sockaddr_in& sockaddr)
{
	this->_sockaddr_in = sockaddr;
}

bool Connector::setnBlock(bool nblock)
{
	assert(hSocket != INVALID_SOCKET);
	u_long mode = nblock ? 1 : 0;
	int iret = ioctlsocket(this->hSocket, FIONBIO, &mode);
	assert(iret == 0);
	return true;
}

//  setsockopt(), WSAAsyncSelect().
int Connector::recvMsg(unsigned char* byteBuf, int nsize)
{
	assert(byteBuf);
	assert(nsize > 0);
	char* buffer = (char*)byteBuf;
	int nread = recv(this->hSocket, buffer, nsize, 0);
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
			this->Close();
			break;
		}
	}
	return nread;
}

bool Connector::sendMsg(const unsigned char* byteBuf, int nsize)
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
			int nwrite = send(this->hSocket, (buffer + nsend), limit, 0);
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

/*
该函数用于关闭TCP连接，但并不关闭socket句柄。
how值：SD_RECEIVE，SD_SEND，SD_BOTH。
SD_RECEIVE表明关闭接收通道，在该socket上不能再接收数据，
如果当前接收缓存中仍有未取出数据或者以后再有数据到达，
则TCP会向发送端发送RST包，将连接重置。

SD_SEND表明关闭发送通道，TCP会将发送缓存中的数据都发送完毕并在收到所有数据的ACK后向对端发送FIN包，
表明本端没有更多数据发送。这个是一个优雅关闭过程。

SD_BOTH则表示同时关闭接收通道和发送通道。
*/
void Connector::Shutdown(int how)
{
	assert(hSocket != INVALID_SOCKET);
	int iret = shutdown(hSocket, how);
	assert(iret == 0);
}


void Connector::Close()
{
	assert(hSocket != INVALID_SOCKET);
	int iret = closesocket(hSocket);
	if (iret == 0) {
		this->hSocket = INVALID_SOCKET;
		return;
	}
	if (iret == SOCKET_ERROR) {
		iret = WSAGetLastError();
		switch (iret)
		{
		case WSANOTINITIALISED: // 成功地调用WSAStartup()
			break;
		case WSAENETDOWN: // WINDOWS套接口实现检测到网络子系统失效
			break;
		case WSAENOTSOCK: // 描述字不是一个套接口。
			break;
		case WSAEINPROGRESS: // 一个阻塞的WINDOWS套接口调用正在运行中。
			break;
		case WSAEINTR: // 通过一个WSACancelBlockingCall()来取消一个（阻塞的）调用。
			break;
		case WSAEWOULDBLOCK: // 该套接口设置为非阻塞方式且SO_LINGER设置为非零超时间隔。
			break;
		default:
			break;
		}
		this->hSocket = INVALID_SOCKET;
	}
}

bool Connector::invalid()
{
	if (this->hSocket == INVALID_SOCKET) {
		return true;
	}
	else {
		return false;
	}
}