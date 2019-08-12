/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib ")

class SocketServer
{
public:
	SocketServer(unsigned short port);
	SocketServer(char* ip, unsigned short port);
	~SocketServer();

	bool setnBlock(bool nblock);
	int getSocketOpt(char* buf, int* len, int level = SOL_SOCKET, int optname = SO_ACCEPTCONN);
	void Close();
	bool invalid();
	int recvMsg(sockaddr_in* fromSockaddr, int* addrlen, unsigned char* byteBuf, int nsize);
	bool sendMsg(const sockaddr_in* toSockaddr, int addrlen, const unsigned char* byteBuf, int nsize);

private:
	inline void init();
	inline SOCKET CreateSocket();
	void Bind();

private:
	SOCKET hSocket;
	struct sockaddr_in _sockaddr;
	char* _ip;
	unsigned short _port;
};

#endif