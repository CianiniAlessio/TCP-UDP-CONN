#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#define PORT	 8081
#define MAXLINE  4096

// nc -w 1 -u localhost 8081 < file.txt
	
int main() 
{

	// STANDARD CREATION FOR A SERVER 
	int sockfd;
	char buffer[MAXLINE];

	struct sockaddr_in servaddr, cliaddr;
		
	// creating socket descr
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("ERROR SOCKET");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
		
	
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("ERROR BINDING");
		exit(EXIT_FAILURE);
	}
	
    int len, n;
    
    len = sizeof(cliaddr); 
	
	// LISTENING INFINITELY FROM MESSAGE FROM UDP CONNECTION
    while(true)
    {

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len);

		buffer[n] = '\n';

		// SEND IT TO THE STANDARD OUTPUT SO THROUGH THE PIPELINE THEY'LL BECOME THE INPUT OF COMPONENT B
		std::cout << buffer << std::flush;      			

    }	
            
	return 0;
}
