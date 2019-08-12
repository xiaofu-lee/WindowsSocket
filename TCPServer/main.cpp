#include <stdio.h>
#include <string.h>
#include "SocketServer.h"

unsigned char buffer[1024] = { 0x00 };
int len = 1024;
char ip[15] = { 0x00 };

void pintfBufferInfo(unsigned char* pBuffer, int stop)
{
	for (int i = 0; i < stop; i++)
	{
		printf("%c", pBuffer[i]);
	}
}

void srartServer() {
	SocketServer socketServer(8090);
	//socketServer.setnBlock(true);
	socketServer.Listen();
	Connector* connector = NULL;
	
	do {
		if (!connector) {
			fprintf(stdout, "wait client to connect...\n");
			connector = socketServer.Accept();
			if (!connector) {
				continue;
			}
			connector->setnBlock(true);
			memset(ip, 0x00, 14);
			inet_ntop(AF_INET, &(connector->getSockaddr().sin_addr), ip, 14);
			unsigned int port = ntohs(connector->getSockaddr().sin_port);
			fprintf(stdout, "ip:%s, port:%d connected\n", ip, port);
			connector->sendMsg((unsigned char*) & "hello i am server\n", strlen("hello i am server\n"));
		}

		if (connector->invalid()) {
			delete connector;
			connector = NULL;
			continue;
		}

		fprintf(stdout, "recvMsg...\n");
		int nrecv = connector->recvMsg(buffer, 1024 - 1);
		if (nrecv <= 0) {
			printf("recv msg none or error\n");
			Sleep(1 * 1000);
			continue;
		}

		printf("%s:", ip);
		pintfBufferInfo(buffer, nrecv);
		printf("\n");
		sprintf((char*)buffer, "%s\n", buffer);
		connector->sendMsg(buffer, 1024 - 1);
		memset(buffer, 0x00, 1024 - 1);

	} while (true);
	socketServer.Close();
}

int main()
{
	srartServer();
	return 0;
}