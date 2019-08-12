/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib ")

class SocketClient
{
public:
	SocketClient(const char* ip, unsigned short port);
	~SocketClient();

	bool setnBlock(bool nblock);
	SOCKET getSocket();
	void setSocket(SOCKET hSocket);
	struct sockaddr_in& getSockaddr();
	int recvMsg(sockaddr *from_sockaddr, int* fromlen, unsigned char* byteBuf, int nsize);
	bool sendMsg(const unsigned char* byteBuf, int nsize);
	void Shutdown(int how = SD_BOTH);
	void Close();
	bool invalid();

private:
	void init(const char* ip, unsigned short port);
	inline SOCKET createSocket();

private:
	SOCKET hSocket;
	struct sockaddr_in _sockaddr_in;
};

#endif