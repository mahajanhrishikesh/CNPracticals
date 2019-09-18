//CLIENT SIDE
//C Program to recieve file from server 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//Defining the constants used throughout the code
#define IP_ADDRESS "172.16.36.100" 
#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0

void clearBuf(char* b)
{
	/* 
	* Code in this function clears the buffer for the
	* max buffer size provided in the constants above
	*/
	int i;
	for (i = 0; i < NET_BUF_SIZE; i++)
		b[i] = '\0';
}

char Cipher(char ch)
{
	/*
	* Code in this function exclusive ORs the input  
	* character bitwise
	*/
	return ch ^ cipherKey;
}

int recvFile(char* buf, int s)
{
	int i;
	char ch;
	for(i = 0; i < s; i++) {
		ch = buf[i];
		ch = Cipher(ch);
		if (ch == EOF)
			return 1;
		else
			printf("%c", ch);
	}
	return 0;
}

int main()
{
	int sockfd, nBytes;
	struct sockaddr_in addr_con;
	int addr_len = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	char net_buf[NET_BUF_SIZE];
	FILE* fp;

	//Creating socket using the socket function
	//socket function returns the file the descriptor
	//REFER CONSTANTS DEFINED ABOVE
	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

	if (sockfd < 0)
		printf("\nFile descriptor was not received\n");
	else
		printf("\nFile descriptor %d received\n", sockfd);

	while(1) {
		printf("\nPlease enter file name to recieve: \n");
		scanf("%s", net_buf);

		sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addr_len);

		printf("\n------------DATA RECIEVED------------\n");

		while(1) {
			// Receiving the buffer 
			clearBuf(net_buf);

			nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, &addr_len);

			if (recvFile(net_buf, NET_BUF_SIZE)){
				break;
			}
		}
		printf("\n-------------------------------------\n");
	}
	return 0;
}
