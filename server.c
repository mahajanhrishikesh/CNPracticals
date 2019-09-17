//SERVER SIDE
//C program to send a file over a udp socket 
//header includes

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//Constants for the code
#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File not found!!"

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

void Cipher(char ch)
{
	/*
	* Code in this function exclusive ORs the input  
	* character bitwise
	*/
	return ch ^ cipherKey;
}

int main()
{
	/*
	* Driver code holds the main function 
	*/
	int sockfd, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con)
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = INADDR_ANY;
	char net_buf[NET_BUF_SIZE];

	//Creating socket using the socket function
	//socket function returns the file the descriptor
	//REFER CONSTANTS DEFINED ABOVE
	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

	if (sockfd < 0)
		printf("\nFile descriptor was not received\n");
	else
		printf("\nFile descriptor %d received\n", sockfd);

	//Binding the socket to the server address 
	if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0)
		printf("\nSuccessfully binded!\n");
	else
		printf("\n Binding failed!\n");

	while(1) {
		printf("\nWaiting for file name...\n");

		//recieves the file name from the client
		clearBuf(net_buf);
		nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, &addrlen);

		fp= fopen(net_buf, "r");
		printf("\nFile name recieved: %s\n", net_buf);

		if (fp == NULL)
			printf("\nFile open failed!\n");
		else
			printf("\nFile successfully opened");

		while(1){
			//We now transfer the file name 
			if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
				sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
				break;
			}
			sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
			clearBuf(net_buf);
		}
		if (fp != NULL)
			fclose(fp);
	}
	return 0;
}


