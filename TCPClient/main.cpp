#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "SocketClient.h"

unsigned char buffer[1024] = { 0x00 };
char sendBuffer[1024] = { 0x00 };

int main()
{
	SocketClient socketClient;
	printf("ConnectServer...\n");
	bool success = socketClient.connectServer("127.0.0.1", 8090);
	assert(success);
	socketClient.setnBlock(true);
	printf("ConnectServer success\n");

	fprintf(stdout, "sendMsg...\n");
	bool bsend = socketClient.sendMsg((unsigned char*) & "i am client\n", strlen("i am client\n"));
	if (bsend) {
		fprintf(stdout, "sendMsg success\n");
	}

	do {
		if (socketClient.invalid()) {
			break;
		}

		//fprintf(stdout, "recvMsg...\n");
		//int nrecv = socketClient.recvMsg(buffer, 1024 - 1);
		//if (nrecv <= 0) {
		//	printf("recv msg none or error\n");
		//	Sleep(1 * 1000);
		//	continue;
		//}

		//fprintf(stdout, "------------------------- msg content ------------------------------- \n");
		//for (int i = 0; i < nrecv; i++)
		//{
		//	printf("%c", buffer[i]);
		//}
		///*printf("\n");*/
		//fprintf(stdout, "----------------------------------------------------------------------\n");
		//memset(buffer, 0x00, 1024 - 1);

		fprintf(stdout, "input msg:");
		memset(sendBuffer, 0x00, 1024);
		gets_s(sendBuffer, 1024);

		fprintf(stdout, "sendMsg...\n");
		bool isend = socketClient.sendMsg((unsigned char*)sendBuffer, strlen(sendBuffer));
		if (isend) {
			fprintf(stdout, "sendMsg success\n");
		}
		else {
			fprintf(stdout, "sendMsg error\n");
		}

	} while (true);

	socketClient.Close();

	return 0;
}