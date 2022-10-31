#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <sstream>

#define PORT 8080
#define SO 14 // SHIFT OUT 
#define LINES 4096

struct Entry
{
	unsigned char price;
	unsigned char volume;
};


//////////////////////// DECLARATION OF FUNCTIONS ////////////////////////////

void read_from_A(int sock, int separator);

void clientB_read_from_server(int sock, char buffer[], int client_fd);

void checking_price_and_sending_to_server(std::vector<Entry> entries,int sock);

std::vector<Entry> creating_entries(std::vector<unsigned char> entries);

////////////////////////////////////////////////////////////////////////////

int main()
{
	// CREATING CLIENT TCP ( STANDARD WAY )

    int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[LINES] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n ERROR SOCKET \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0)
	{		
		printf("\n ERROR ADDRESS \n");
	}

	while ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))< 0)
	{
		//printf("\n CAN'T CONNECT \n");
	}


	// SETTING WHICH IS THE SEPARATOR, I SET THE LINE FEED AS REQUESTED

	int separator = 10;

	// TWO THREAD, ONE FOR RECEVING FROM COMPONENT A WHICH WILL PRINT ON STDOUT THE MESSAGES FROM THE UDP CONNECTION
	// AND THROUGH THE PIPELINE ( ./A | ./B) THEY'LL BECOME THE INPUT FOR B
    
	std::thread worker1(read_from_A,sock,separator);

	// SECOND THREAD TO LISTEN CONTINUOSLY FROM SERVER TCP PORT 8080 

    std::thread worker2(clientB_read_from_server,sock,buffer,client_fd);

	// THE TWO THREAD ARE MADE IN ORDER TO LET B FREE TO READ BOTH FROM THE SERVER AND FROM A WITHOUT INTERRUPTION    
	
    worker1.join();
    worker2.join();
	
}


// CHECKING FOR PRICE > 80 
void checking_price_and_sending_to_server(std::vector<Entry> entries, int sock)
{

	

    for(Entry& e: entries)
	{
		
		
        if(e.price > 80)
        {          
			unsigned char * c = (unsigned char*)malloc(5 * sizeof(unsigned char));
			//unsigned char * c = new unsigned char[5];
			for(int a = 0;a < 5;a++)
			{
				c[a] = e.price;				
			}
		

			// IF I OUTPUT IN A TEXT FILE LIKE out.txt AND USING THE PLUGIN OF NOTEPAD FOR THE HEX IT'S POSSIBLE TO READ THE DATA, OTHERWISE IN THE 
			// TERMINAL ARE MOSTLY UNREADABLE CHARACTER.

            send(sock, (const void*)c, 5 * sizeof(unsigned char), 0);
			delete[] c;
			
        }
		
		
	}

}


// CREATING A VECTOR OF ENTITIES (P,V)s
std::vector<Entry> creating_entries(std::vector<unsigned char> entries)
{
	std::vector<Entry> pair_entries;
	
	// IF EVEN PAIRS (P,V) : OK
	if(entries.size()%2==0)
	{
		// PUSHING BACK PAIR AFTER PAIR WITH INCREMENT i+=2
		for(int i = 0; i < entries.size(); i+=2)
		{
			pair_entries.push_back({entries[i],entries[i+1]});
		}
	}
	// IF ODD I DON'T CONSIDER THE LAST ONE ( MY CHOICE )
	else
	{
		
		for(int i = 0; i < entries.size() - 1; i+=2)
		{
			pair_entries.push_back({entries[i],entries[i+1]});
		}
	}

	return pair_entries;	
}

// READ FROM STDINPUT FROM A THROUGH THE PIPELINE
void read_from_A(int sock, int separator)
{

    int c;
	std::vector<unsigned char> chars;
	std::vector<Entry> entries;
    

    while (1) {
            c = getchar();

            // IF SEPARATOR OR SHIFT OUT I PUSH BACK IN A VECTOR WHICH I'LL EVALUATE LATER FOR THE PRICE > 80
			
			if(c != separator && c != SO)
			{
				chars.push_back(c);
			}
			// IF == A SEPARATOR O SO THEN I DON'T CONSIDER THAT CHAR AND I SEND THE VECTOR TO CHECK FOR THE PRICES
			else
			{
				
				entries = creating_entries(chars);
                checking_price_and_sending_to_server(entries,sock);

				// CLEAR SO I CAN FILL IT AGAIN
				chars.clear();
			}
    }
}

// USE THE SOCKET TO PRINT ON STDOUTPUT ALL MESSAGES FROM THE SERVER, I SUPPOSED TO SEND SENTENCES INSTEAD OF FILE LIKE FOR THE UDP CONNECTION

void clientB_read_from_server(int sock, char buffer[], int client_fd)
{    
	while(1)
	{
		int valread = read(sock, buffer, LINES);
		std::cout << buffer << std::flush;
		
    }
}
