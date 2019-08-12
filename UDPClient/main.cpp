#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SocketClient.h"

char buffer[1024] = { 0x00 };

int main()
{
	SocketClient socketClient("127.0.0.1", 8090);
	//socketClient.setnBlock(true);
	sockaddr serv_addr;
	int addr_size = sizeof(serv_addr);
	while (1) {
		fprintf(stdout, "input msg:");
		gets_s(buffer, 1024-1);
		bool flag = socketClient.sendMsg((unsigned char *)buffer, strlen(buffer) + 1);
		if (flag) {
			fprintf(stdout, "send msg success\n");
		}
		fprintf(stdout, "recv msg...\n");
		int iret = socketClient.recvMsg(&serv_addr, &addr_size, (unsigned char*)buffer, 1024 - 1);
		if (iret > 0) {
			for (int i = 0; i < iret; i++)
			{
				printf("%c", buffer[i]);
			}
			printf("\n");
		}
	}

	return 0;
}