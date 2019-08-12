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
	SocketClient();
	~SocketClient();

	bool setnBlock(bool nblock);
	SOCKET getSocket();
	void setSocket(SOCKET hSocket);
	struct sockaddr_in& getSockaddr();
	bool connectServer(const char* ip, unsigned short port, unsigned long long timeout = 3);
	int recvMsg(unsigned char* byteBuf, int nsize);
	bool sendMsg(const unsigned char* byteBuf, int nsize);
	void Shutdown(int how = SD_BOTH);
	void Close();
	bool invalid();

private:
	void init();
	inline SOCKET createSocket();

private:
	SOCKET hSocket;
	struct sockaddr_in _sockaddr_in;
};

#endif