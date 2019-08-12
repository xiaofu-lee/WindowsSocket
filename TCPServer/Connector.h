/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib ")

class Connector
{
public:
	Connector(SOCKET hSocket, struct sockaddr_in sockaddr);
	~Connector();
	SOCKET getSocket();
	void setSocket(SOCKET hSocket);
	struct sockaddr_in& getSockaddr();
	void setSockaddr(struct sockaddr_in& sockaddr);
	bool setnBlock(bool nblock);
	int recvMsg(unsigned char* byteBuf, int nsize);
	bool sendMsg(const unsigned char* byteBuf, int nsize);
	void Shutdown(int how = SD_BOTH);
	void Close();
	bool invalid();

private:
	SOCKET hSocket;
	struct sockaddr_in _sockaddr_in;
};

#endif