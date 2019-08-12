/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "Connector.h"

class SocketServer
{
public:
	SocketServer(unsigned short port);
	SocketServer(char* ip, unsigned short port);
	~SocketServer();

	bool setnBlock(bool nblock);
	void Listen(int backlog = 5);
	int getSocketOpt(char* buf, int *len, int level = SOL_SOCKET, int optname = SO_ACCEPTCONN);
	Connector* Accept();
	void Close();
	bool invalid();

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