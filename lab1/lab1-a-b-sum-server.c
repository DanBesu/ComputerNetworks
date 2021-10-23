// exists on all platforms

#include <stdio.h>

// this section will only be compiled on NON Windows platforms
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#define closesocked close
typedef int SOCKET;

#else
// for windows
#include<WinSock2.h>
// for uint16_t an so
#include<cstdint>
//#pragma comment(lib,"Ws2_32.lib")
#endif

#include<netinet/in.h> 

int main(){

    SOCKET s;
    struct sockaddr_in server, client;
    int c, l, err;

    //initialise the windows Socket Library only when on windows
#ifdef WIN32
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2, 2), &wsadata) < 0){
        printf("Error initializing the Windows Sockets Library");
        return -1;
    }
#endif
    //create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("Eroare la crearea socketului");
        return 1;
    }
    //server
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    //bind
    if(bind(s,(struct sockaddr*) &server, sizeof(server)) < 0){
        printf("Bind error:");
        return 1;
    }

    listen(s, 5);
    //client
    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1){
        uint16_t a, b, suma;
        printf("Sa vina conexiunile!");
        c = accept(s, (struct sockaddr *) &client, &l);
        err = errno;

#ifdef WIN32
    err = WSAGetLastError();
#endif

    if(c < 0){
        printf("accept error %d", err);
        continue;
    }
    // client from: addr:port
    printf("Incomming connected client from: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));    
    // serving the connected client
    int res = recv(c, (char*)&a, sizeof(a), 0);
    if(res != sizeof(a)) // VALIDATION check we got an unsigned short (char sized) value
        printf("Error for operand a\n");
    
    res = recv(c, (char*)&b, sizeof(b), 0);
    if(res != sizeof(b))
        printf("Error for operand b\n");

    // DECODE
    a = ntohs(a);
    b = ntohs(b);
    suma = a + b;
    suma = htons(suma);

    res = send(c, (char*)&suma, sizeof(suma), 0);
    // check if data was sent correctly
    if(res != sizeof(suma))
        printf("Error sending SUMA\n");
    
    closesocked(c);
    }
    // release the windows sockets library

#ifdef WIN32
    WSACleanup();
#endif
}
