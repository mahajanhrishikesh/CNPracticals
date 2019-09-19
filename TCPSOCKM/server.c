//Server Side code
//Libraries required 
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> //for read()

//Constants required across the program
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

/*
*   Following function is to create a chat in between the 
*   server and the client
*/
void func (int sockfd)
{
    char buff[MAX];
    int n;
    
    for(;;){
        bzero(buff, MAX);
        //reads the message from the client and store it in buffer
        read(sockfd, buff, sizeof(buff));
        //print buffer which contains the client contents
        printf("Client: %s\t To client: ", buff);
        bzero(buff, MAX);   //Clears out the buffer, fills with zeros
        n = 0;
        while ((buff[n++] = getchar())!='\n');
        
        //copy server message from the server 
        write(sockfd, buff, sizeof(buff));
        
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server has left the conversation. \n");
            break;
        }        
    }
}

//Driving main function for the entire program
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, client;
    
    //create a socket using socket funcion
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Socket creation has failed.\n");
        exit(0);
    }
    else{
        printf("Socket has successfully been created!\n");
    }
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    } 
    else
    {
        printf("Socket successfully binded..\n"); 
    }
    
    if ((listen(sockfd, 5)) != 0){
        printf("Listen failed....\n");
        exit(0);
    }
    else{
        printf("Server Listening...\n");
    }
    len = sizeof(client);
    
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&client, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
    {
        printf("server accept the client...\n"); 
    }
    // Function for chatting between client and server 
    func(connfd); 
  
    // After chatting close the socket 
    close(sockfd);  
    
    return 0;
}
