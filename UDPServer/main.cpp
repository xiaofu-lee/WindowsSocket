#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SocketServer.h"

char buffer[1024] = {0x00};

int main()
{
	SocketServer socketServer(8090);
	socketServer.setnBlock(true);
	sockaddr_in client_addr;
	int addr_size = sizeof(client_addr);
	while (1) {
		fprintf(stdout, "recv msg...\n");
		memset(buffer, 0x00, 1024);
		int iret = socketServer.recvMsg(&client_addr, &addr_size, (unsigned char*)buffer, 1024 - 1);
		if (iret > 0) {
			printf("[公告]收到来自IP(%s)的信息: %s\n", inet_ntoa(client_addr.sin_addr), buffer);
			socketServer.sendMsg(&client_addr, addr_size, (unsigned char*)buffer, strlen(buffer));
		}
		Sleep(1000);
	}
	
	return 0;
}